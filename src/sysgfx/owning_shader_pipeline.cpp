/// @file
/// @brief Implements owning_shader_pipeline.hpp.

#include <tr/sysgfx/owning_shader_pipeline.hpp>

//

tr::owning_shader_pipeline::owning_shader_pipeline(graphics_context& context, tr::vertex_shader&& vertex_shader,
												   tr::fragment_shader&& fragment_shader, std::string_view label) noexcept
	: m_vertex_shader{std::move(vertex_shader)}
	, m_fragment_shader{std::move(fragment_shader)}
	, m_shader_pipeline{context, m_vertex_shader, m_fragment_shader, label}
{
}

//

void tr::owning_shader_pipeline::set_label(std::string_view label) noexcept
{
	m_shader_pipeline.set_label(label);
}

//

tr::owning_shader_pipeline::operator bool() const noexcept
{
	return static_cast<bool>(m_shader_pipeline);
}

bool tr::owning_shader_pipeline::valid() const noexcept
{
	return m_shader_pipeline.valid();
}

tr::graphics_context& tr::owning_shader_pipeline::context() const noexcept
{
	return m_shader_pipeline.context();
}

tr::owning_shader_pipeline::operator const tr::shader_pipeline&() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the shader pipeline of an owning shader pipeline in an invalid state.");

	return m_shader_pipeline;
}

tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader() noexcept
{
	TR_ASSERT(valid(), "Tried to get the vertex shader of an owning shader pipeline in an invalid state.");

	return m_vertex_shader;
}

const tr::vertex_shader& tr::owning_shader_pipeline::vertex_shader() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the vertex shader of an owning shader pipeline in an invalid state.");

	return m_vertex_shader;
}

tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader() noexcept
{
	TR_ASSERT(valid(), "Tried to get the fragment shader of an owning shader pipeline in an invalid state.");

	return m_fragment_shader;
}

const tr::fragment_shader& tr::owning_shader_pipeline::fragment_shader() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the fragment shader of an owning shader pipeline in an invalid state.");

	return m_fragment_shader;
}

std::string tr::owning_shader_pipeline::label() const
{
	return m_shader_pipeline.label();
}