/// @file
/// @brief Implements shader_pipeline.hpp.

#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/utility/hash_map.hpp"
#include "tr/sysgfx/shader.hpp"

//

#ifdef TR_ENABLE_GL_CHECKS

bool tr::shader_pipeline::vertex_shader_debug_info::valid(graphics_context& context) const
{
	return context.registry.check_shader(tid, gid);
}

bool tr::shader_pipeline::fragment_shader_debug_info::valid(graphics_context& context) const
{
	return context.registry.check_shader(tid, gid);
}

#endif

//

tr::shader_pipeline::shader_pipeline(graphics_context& context, const vertex_shader& vertex_shader, const fragment_shader& fragment_shader)
	: m_ppo{deleter{context}}
{
	const gl_api& gl{context.make_current_and_return_gl_api()};
	gl.create_program_pipelines(1, out_handle(m_ppo));
	set_shaders(vertex_shader, fragment_shader);
}

void tr::shader_pipeline::deleter::operator()(unsigned int id) const
{
	const gl_api& gl{context.make_current_and_return_gl_api()};
	gl.delete_program_pipelines(1, &id);
}

//

tr::graphics_context& tr::shader_pipeline::context() const
{
	TR_ASSERT(valid(), "Tried to get context of a shader pipeline in an invalid state.");

	return m_ppo.get_deleter().context;
}

//

void tr::shader_pipeline::set_shaders(const vertex_shader& vertex_shader, const fragment_shader& fragment_shader)
{
	TR_ASSERT(valid(), "Tried to set shaders to a shader pipeline in an invalid state");
	TR_ASSERT(vertex_shader.valid(), "Tried to set invalid vertex shader to shader pipeline {}.", *this);
	TR_ASSERT(fragment_shader.valid(), "Tried to set invalid fragment shader to pipeline {}.", *this);
	TR_ASSERT(&vertex_shader.context() == &context(),
			  "Tried to set vertex shader '{}' to pipeline '{}' despite them not being on the same graphics context.",
			  vertex_shader.label(), label());
	TR_ASSERT(&fragment_shader.context() == &context(),
			  "Tried to set fragment shader '{}' to pipeline '{}' despite them not being on the same graphics context.",
			  fragment_shader.label(), label());

#ifdef TR_ENABLE_GL_CHECKS
	m_vertex_shader_info = {vertex_shader.gid(), vertex_shader.tid(), vertex_shader.label(), vertex_shader.outputs()};
	m_fragment_shader_info = {fragment_shader.gid(), vertex_shader.tid(), fragment_shader.label(), fragment_shader.inputs()};
	assert_shaders_compatible();
#endif

	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.use_program_stages(m_ppo.get(), GL_VERTEX_SHADER_BIT, vertex_shader.gid());
	gl.use_program_stages(m_ppo.get(), GL_FRAGMENT_SHADER_BIT, fragment_shader.gid());
}

void tr::shader_pipeline::set_vertex_shader(const vertex_shader& vertex_shader)
{
	TR_ASSERT(valid(), "Tried to set a vertex shader to a shader pipeline in an invalid state");
	TR_ASSERT(vertex_shader.valid(), "Tried to set invalid vertex shader to pipeline '{}'.", label());
	TR_ASSERT(&vertex_shader.context() == &context(),
			  "Tried to set vertex shader '{}' to pipeline '{}' despite them not being on the same graphics context.",
			  vertex_shader.label(), label());

#ifdef TR_ENABLE_GL_CHECKS
	m_vertex_shader_info = {vertex_shader.gid(), vertex_shader.tid(), vertex_shader.label(), vertex_shader.outputs()};
	assert_shaders_compatible();
#endif

	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.use_program_stages(m_ppo.get(), GL_VERTEX_SHADER_BIT, vertex_shader.gid());
}

void tr::shader_pipeline::set_fragment_shader(const fragment_shader& fragment_shader)
{
	TR_ASSERT(valid(), "Tried to set a fragment shader to a shader pipeline in an invalid state");
	TR_ASSERT(fragment_shader.valid(), "Tried to set invalid fragment shader to pipeline '{}'.", label());
	TR_ASSERT(&fragment_shader.context() == &context(),
			  "Tried to set fragment shader '{}' to pipeline '{}' despite them not being on the same graphics context.",
			  fragment_shader.label(), label());

#ifdef TR_ENABLE_GL_CHECKS
	m_fragment_shader_info = {fragment_shader.gid(), fragment_shader.tid(), fragment_shader.label(), fragment_shader.inputs()};
	assert_shaders_compatible();
#endif

	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.use_program_stages(m_ppo.get(), GL_FRAGMENT_SHADER_BIT, fragment_shader.gid());
}

//

void tr::shader_pipeline::set_label(std::string_view label)
{
	TR_ASSERT(valid(), "Tried to set the label of a shader pipeline in an invalid state");

	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.set_object_label(GL_PROGRAM_PIPELINE, m_ppo.get(), label.size(), label.data());
}

std::string tr::shader_pipeline::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a shader pipeline in an invalid state");

	const gl_api& gl{context().make_current_and_return_gl_api()};

	int label_length;
	gl.get_object_label(GL_PROGRAM_PIPELINE, m_ppo.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_PROGRAM_PIPELINE, m_ppo.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

//

bool tr::shader_pipeline::valid() const
{
	return m_ppo.has_value();
}

unsigned int tr::shader_pipeline::gid() const
{
	return m_ppo.get();
}

//

#ifdef TR_ENABLE_GL_CHECKS
void tr::shader_pipeline::assert_settable(graphics_context& context) const
{
	TR_ASSERT(valid(), "Tried to set a shader pipeline in an invalid state to a context.");
	TR_ASSERT(&context == &this->context(), "Tried to set shader pipeline {} to a context it is not associated with.", *this);
	TR_ASSERT(m_vertex_shader_info.valid(context), "Tried to set shader pipeline {} with invalid set vertex shader '{}'.", *this,
			  m_vertex_shader_info.label);
	TR_ASSERT(m_fragment_shader_info.valid(context), "Tried to set shader pipeline {} with invalid set fragment shader '{}'.", *this,
			  m_fragment_shader_info.label);
}

//

void tr::shader_pipeline::assert_shaders_compatible() const
{
	graphics_context& context{this->context()};
	if (!(m_vertex_shader_info.valid(context) && !m_fragment_shader_info.valid(context))) {
		return;
	}

	TR_ASSERT(m_vertex_shader_info.outputs.size() == m_fragment_shader_info.inputs.size(),
			  "Tried to set mismatched shaders to shader pipeline {}:\n"
			  "Vertex shader '{}' has {} outputs, fragment shader '{}' has {} inputs.",
			  *this, m_vertex_shader_info.label, m_vertex_shader_info.outputs.size(), m_fragment_shader_info.label,
			  m_fragment_shader_info.inputs.size());

	for (const auto& [location, info] : m_vertex_shader_info.outputs) {
		TR_ASSERT(m_fragment_shader_info.inputs.contains(location),
				  "Tried to set mismatched shaders to shader pipeline {}:\n"
				  "Vertex shader '{}' has output '{}' at location {} that was not found in fragment shader '{}''s inputs.",
				  *this, m_vertex_shader_info.label, info, location, m_fragment_shader_info.label);

		const glsl_variable& frag_info{get(m_fragment_shader_info.inputs, location)};
		TR_ASSERT(frag_info.type == info.type && frag_info.array_size == info.array_size,
				  "Tried to set mismatched shaders to shader pipeline {}:\n"
				  "Vertex shader '{}' has output '{}' at location {}, but the input '{}' at the same location in fragment shader '{}' is "
				  "not compatible with it.",
				  *this, m_vertex_shader_info.label, info, location, get(m_fragment_shader_info.inputs, location),
				  m_fragment_shader_info.label);
	}
}
#endif

//

tr::owning_shader_pipeline::owning_shader_pipeline(graphics_context& context, tr::vertex_shader&& vertex_shader,
												   tr::fragment_shader&& fragment_shader)
	: m_vertex_shader{std::move(vertex_shader)}
	, m_fragment_shader{std::move(fragment_shader)}
	, m_base{context, m_vertex_shader, m_fragment_shader}
{
}

//

tr::owning_shader_pipeline::operator const tr::shader_pipeline&() const
{
	return m_base;
}

//

tr::graphics_context& tr::owning_shader_pipeline::context() const
{
	return m_base.context();
}

//

tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader()
{
	TR_ASSERT(valid(), "Tried to get vertex shader of an owning shader pipeline in an invalid state.");

	return m_vertex_shader;
}

const tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader() const
{
	TR_ASSERT(valid(), "Tried to get vertex shader of an owning shader pipeline in an invalid state.");

	return m_vertex_shader;
}

tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader()
{
	TR_ASSERT(valid(), "Tried to get fragment shader of an owning shader pipeline in an invalid state.");

	return m_fragment_shader;
}

const tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader() const
{
	TR_ASSERT(valid(), "Tried to get fragment shader of an owning shader pipeline in an invalid state.");

	return m_fragment_shader;
}

//

bool tr::owning_shader_pipeline::valid() const
{
	return m_base.valid();
}

//

void tr::owning_shader_pipeline::set_label(std::string_view label)
{
	m_base.set_label(label);
}

std::string tr::owning_shader_pipeline::label() const
{
	return m_base.label();
}

//

unsigned int tr::owning_shader_pipeline::gid() const
{
	return m_base.gid();
}