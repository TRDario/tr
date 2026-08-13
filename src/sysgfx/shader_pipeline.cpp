/// @file
/// @brief Implements shader_pipeline.hpp.

#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/utility/hash_map.hpp"
#include "tr/sysgfx/shader.hpp"

//

namespace tr
{
	namespace
	{
		/// Checks shaders set to a shader pipeline for compatibility.
		/// @param vertex_shader Optional vertex shader to check.
		/// @param fragment_shader Optional fragment shader to check.
		void check_shader_pipeline_shaders(opt_ref<const vertex_shader> vertex_shader, opt_ref<const fragment_shader> fragment_shader)
		{
			if (!(vertex_shader.has_ref() && !fragment_shader.has_ref())) {
				return;
			}

			// clang-format off
			TR_ASSERT(
				vertex_shader->outputs().size() == fragment_shader->inputs().size(),
				"Mismatched shader inputs/outputs (vertex shader '{}' has {} outputs, fragment shader '{}' has {} inputs).",
				vertex_shader->label(), vertex_shader->outputs().size(), fragment_shader->label(), fragment_shader->inputs().size()
			);

			for (const auto& [location, info] : vertex_shader->outputs()) {
				TR_ASSERT(
					fragment_shader->inputs().contains(location),
					"Mismatched shader inputs/outputs (vertex shader '{}' has output '{}' at location {} that was not found in fragment shader '{}''s inputs).",
					vertex_shader->label(), info, location, fragment_shader->label()
				);

				const glsl_variable& frag_info{get(fragment_shader->inputs(), location)};
				TR_ASSERT(
					frag_info.type == info.type && frag_info.array_size == info.array_size,
					"Mismatched shader inputs/outputs (vertex shader '{}' has output '{}' at location {}, but the input '{}' at the same location in fragment shader '{}' is not compatible with it).",
					vertex_shader->label(), info, location, get(fragment_shader->inputs(), location), fragment_shader->label()
				);
			}
			// clang-format on
		}
	} // namespace
} // namespace tr

//

tr::shader_pipeline::shader_pipeline(graphics_context& context, const vertex_shader& vshader, const fragment_shader& fshader)
	: m_ppo{{context}}
{
	const gl_api& gl{context.make_current_and_return_gl_api()};
	gl.create_program_pipelines(1, out_handle(m_ppo));
	set_shaders(vshader, fshader);
}

void tr::shader_pipeline::deleter::operator()(unsigned int id) const
{
	const gl_api& gl{context.make_current_and_return_gl_api()};

	gl.delete_program_pipelines(1, &id);
}

//

tr::graphics_context& tr::shader_pipeline::context() const
{
	return m_ppo.get_deleter().context;
}

//

void tr::shader_pipeline::set_shaders(const vertex_shader& vertex_shader, const fragment_shader& fragment_shader)
{
	check_shader_pipeline_shaders(vertex_shader, fragment_shader);
	m_vertex_shader = vertex_shader;
	m_fragment_shader = fragment_shader;

	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.use_program_stages(m_ppo.get(), GL_VERTEX_SHADER_BIT, vertex_shader.id());
	gl.use_program_stages(m_ppo.get(), GL_FRAGMENT_SHADER_BIT, fragment_shader.id());
}

void tr::shader_pipeline::set_vertex_shader(const vertex_shader& vertex_shader)
{
	check_shader_pipeline_shaders(vertex_shader, m_fragment_shader);
	m_vertex_shader = vertex_shader;

	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.use_program_stages(m_ppo.get(), GL_VERTEX_SHADER_BIT, vertex_shader.id());
}

void tr::shader_pipeline::set_fragment_shader(const fragment_shader& fragment_shader)
{
	check_shader_pipeline_shaders(m_vertex_shader, fragment_shader);
	m_fragment_shader = fragment_shader;

	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.use_program_stages(m_ppo.get(), GL_FRAGMENT_SHADER_BIT, fragment_shader.id());
}

//

void tr::shader_pipeline::set_label(std::string_view label)
{
	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.set_object_label(GL_PROGRAM_PIPELINE, m_ppo.get(), label.size(), label.data());
}

std::string tr::shader_pipeline::label() const
{
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

unsigned int tr::shader_pipeline::id() const
{
	return m_ppo.get();
}

//

#ifdef TR_ENABLE_GL_CHECKS
bool tr::shader_pipeline::complete() const
{
	return m_vertex_shader.has_ref() && m_fragment_shader.has_ref();
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
	return m_vertex_shader;
}

const tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader() const
{
	return m_vertex_shader;
}

tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader()
{
	return m_fragment_shader;
}

const tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader() const
{
	return m_fragment_shader;
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

unsigned int tr::owning_shader_pipeline::id() const
{
	return m_base.id();
}