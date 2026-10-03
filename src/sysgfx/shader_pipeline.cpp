/// @file
/// @brief Implements shader_pipeline.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/shader.hpp>
#include <tr/sysgfx/shader_pipeline.hpp>
#include <tr/utility/hash_map.hpp>
#include <tr/utility/out_handle.hpp>

//

tr::shader_pipeline::shader_pipeline(graphics_context& context) noexcept
	: m_handle{deleter{context}}
{
	context.gl().create_program_pipelines(1, out_handle(m_handle));
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context.registered_shader_pipelines().emplace(id());
#endif
}

tr::shader_pipeline::shader_pipeline(graphics_context& context, const vertex_shader& vertex_shader,
									 const fragment_shader& fragment_shader) noexcept
	: shader_pipeline{context}
{
	set_shaders(vertex_shader, fragment_shader);
}

void tr::shader_pipeline::deleter::operator()(unsigned int ppo) const noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context->registered_shader_pipelines().erase(id);
#endif
	context->gl().delete_program_pipelines(1, &ppo);
}

//

tr::graphics_context& tr::shader_pipeline::context() const noexcept
{
	TR_ASSERT(valid(), "Tried to get context of a shader pipeline in an invalid state.");

	return m_handle.get_deleter().context;
}

//

void tr::shader_pipeline::set_shaders(const vertex_shader& vertex_shader, const fragment_shader& fragment_shader) noexcept
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
	TR_ASSERT(vertex_shader.outputs().size() == fragment_shader.inputs().size(),
			  "Tried to set mismatched shaders to shader pipeline {}:\n"
			  "Vertex shader '{}' has {} outputs, fragment shader '{}' has {} inputs.",
			  *this, vertex_shader.label(), vertex_shader.outputs().size(), fragment_shader.label(), fragment_shader.inputs().size());
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	for (const auto& [location, info] : vertex_shader.outputs()) {
		TR_ASSERT(fragment_shader.inputs().contains(location),
				  "Tried to set mismatched shaders to shader pipeline {}:\n"
				  "Vertex shader '{}' has output '{}' at location {} that was not found in fragment shader '{}''s inputs.",
				  *this, vertex_shader.label(), info, location, fragment_shader.label());

		const internal::glsl_variable& frag_info{get(fragment_shader.inputs(), location)};
		TR_ASSERT(frag_info.type == info.type && frag_info.array_size == info.array_size,
				  "Tried to set mismatched shaders to shader pipeline {}:\n"
				  "Vertex shader '{}' has output '{}' at location {}, but the input '{}' at the same location in fragment shader '{}' is "
				  "not compatible with it.",
				  *this, vertex_shader.label(), info, location, get(fragment_shader.inputs(), location), fragment_shader.label());
	}
#endif

	const internal::opengl& gl{context().gl()};
	if (internal::graphics_object_id id{vertex_shader.id()}; m_vertex_shader_id != id) {
		TR_LOG_TRACE("gfx", "Setting vertex shader {} on shader pipeline {}.", vertex_shader, *this);
		gl.use_program_stages(unwrap(), GL_VERTEX_SHADER_BIT, vertex_shader.unwrap());
		m_vertex_shader_id = id;
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		m_vertex_shader_outputs = vertex_shader.outputs();
#endif
	}
	if (internal::graphics_object_id id{fragment_shader.id()}; m_fragment_shader_id != id) {
		TR_LOG_TRACE("gfx", "Setting fragment shader {} on shader pipeline {}.", vertex_shader, *this);
		gl.use_program_stages(unwrap(), GL_FRAGMENT_SHADER_BIT, fragment_shader.unwrap());
		m_fragment_shader_id = id;
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		m_fragment_shader_inputs = fragment_shader.inputs();
#endif
	}
}

void tr::shader_pipeline::set_vertex_shader(const vertex_shader& vertex_shader) noexcept
{
	TR_ASSERT(valid(), "Tried to set a vertex shader to a shader pipeline in an invalid state");
	TR_ASSERT(vertex_shader.valid(), "Tried to set invalid vertex shader to pipeline '{}'.", label());
	TR_ASSERT(&vertex_shader.context() == &context(),
			  "Tried to set vertex shader '{}' to pipeline '{}' despite them not being on the same graphics context.",
			  vertex_shader.label(), label());
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	if (m_fragment_shader_id != internal::graphics_object_id::invalid) {
		TR_ASSERT(vertex_shader.outputs().size() == m_fragment_shader_inputs.size(),
				  "Tried to set mismatched shaders to shader pipeline {}:\n"
				  "Vertex shader '{}' has {} outputs, set fragment shader has {} inputs.",
				  *this, vertex_shader.label(), vertex_shader.outputs().size(), m_fragment_shader_inputs.size());
		for (const auto& [location, info] : vertex_shader.outputs()) {
			TR_ASSERT(m_fragment_shader_inputs.contains(location),
					  "Tried to set mismatched shaders to shader pipeline {}:\n"
					  "Vertex shader '{}' has output '{}' at location {} that was not found in the set fragment shader's inputs.",
					  *this, vertex_shader.label(), info, location);

			const internal::glsl_variable& frag_info{get(m_fragment_shader_inputs, location)};
			TR_ASSERT(
				frag_info.type == info.type && frag_info.array_size == info.array_size,
				"Tried to set mismatched shaders to shader pipeline {}:\n"
				"Vertex shader '{}' has output '{}' at location {}, but the input '{}' at the same location in the set fragment shader is "
				"not compatible with it.",
				*this, vertex_shader.label(), info, location, get(m_fragment_shader_inputs, location));
		}
	}
#endif

	if (internal::graphics_object_id id{vertex_shader.id()}; m_vertex_shader_id != id) {
		TR_LOG_TRACE("gfx", "Setting vertex shader {} on shader pipeline {}.", vertex_shader, *this);
		context().gl().use_program_stages(unwrap(), GL_VERTEX_SHADER_BIT, vertex_shader.unwrap());
		m_vertex_shader_id = id;
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		m_vertex_shader_outputs = vertex_shader.outputs();
#endif
	}
}

void tr::shader_pipeline::set_fragment_shader(const fragment_shader& fragment_shader) noexcept
{
	TR_ASSERT(valid(), "Tried to set a fragment shader to a shader pipeline in an invalid state");
	TR_ASSERT(fragment_shader.valid(), "Tried to set invalid fragment shader to pipeline '{}'.", label());
	TR_ASSERT(&fragment_shader.context() == &context(),
			  "Tried to set fragment shader '{}' to pipeline '{}' despite them not being on the same graphics context.",
			  fragment_shader.label(), label());
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	if (m_vertex_shader_id != internal::graphics_object_id::invalid) {
		TR_ASSERT(m_vertex_shader_outputs.size() == fragment_shader.inputs().size(),
				  "Tried to set mismatched shaders to shader pipeline {}:\n"
				  "Set vertex shader has {} outputs, fragment shader '{}' has {} inputs.",
				  *this, m_vertex_shader_outputs.size(), fragment_shader.label(), fragment_shader.inputs().size());
		for (const auto& [location, info] : m_vertex_shader_outputs) {
			TR_ASSERT(fragment_shader.inputs().contains(location),
					  "Tried to set mismatched shaders to shader pipeline {}:\n"
					  "Set vertex shader has output '{}' at location {} that was not found in fragment shader '{}''s inputs.",
					  *this, info, location, fragment_shader.label());

			const internal::glsl_variable& frag_info{get(fragment_shader.inputs(), location)};
			TR_ASSERT(
				frag_info.type == info.type && frag_info.array_size == info.array_size,
				"Tried to set mismatched shaders to shader pipeline {}:\n"
				"Set vertex shader has output '{}' at location {}, but the input '{}' at the same location in fragment shader '{}' is "
				"not compatible with it.",
				*this, info, location, get(fragment_shader.inputs(), location), fragment_shader.label());
		}
	}
#endif

	if (internal::graphics_object_id id{fragment_shader.id()}; m_fragment_shader_id != id) {
		TR_LOG_TRACE("gfx", "Setting fragment shader {} on shader pipeline {}.", fragment_shader, *this);
		context().gl().use_program_stages(unwrap(), GL_FRAGMENT_SHADER_BIT, fragment_shader.unwrap());
		m_fragment_shader_id = id;
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		m_fragment_shader_inputs = fragment_shader.inputs();
#endif
	}
}

//

void tr::shader_pipeline::set_label(std::string_view label) noexcept
{
	TR_ASSERT(valid(), "Tried to set the label of a shader pipeline in an invalid state");

	context().gl().set_object_label(GL_PROGRAM_PIPELINE, unwrap(), label.size(), label.data());
}

std::string tr::shader_pipeline::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a shader pipeline in an invalid state");

	const internal::opengl& gl{context().gl()};

	int label_length;
	gl.get_object_label(GL_PROGRAM_PIPELINE, unwrap(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_PROGRAM_PIPELINE, unwrap(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

//

bool tr::shader_pipeline::valid() const noexcept
{
	return m_handle.has_value();
}

//

unsigned int tr::shader_pipeline::unwrap() const noexcept
{
	return m_handle.get();
}

tr::internal::graphics_object_id tr::shader_pipeline::id() const noexcept
{
	return m_handle.get_deleter().id;
}

tr::internal::graphics_object_id tr::shader_pipeline::vertex_shader_id() const noexcept
{
	return m_vertex_shader_id;
}

tr::internal::graphics_object_id tr::shader_pipeline::fragment_shader_id() const noexcept
{
	return m_fragment_shader_id;
}

#ifdef TR_ENABLE_CHECKED_GRAPHICS
const boost::unordered_flat_map<unsigned int, tr::internal::glsl_variable>& tr::shader_pipeline::vertex_shader_outputs() const noexcept
{
	return m_vertex_shader_outputs;
}

const boost::unordered_flat_map<unsigned int, tr::internal::glsl_variable>& tr::shader_pipeline::fragment_shader_inputs() const noexcept
{
	return m_fragment_shader_inputs;
}
#endif