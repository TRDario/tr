/// @file
/// @brief Implements post_processing_buffer.cpp.

#include "../../include/tr/sysgfx/post_processing_buffer.hpp"
#include "../../include/tr/sysgfx/render_target.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"

namespace tr
{
	namespace
	{
/// Post processing vertex shader source code.
#include <generated/post_processing_vert.hpp>

		/// Post processing buffer vertices.
		constexpr std::array<glm::vec2, 4> post_processing_vertices{{{-1, -1}, {-1, 1}, {1, 1}, {1, -1}}};
	} // namespace
} // namespace tr

//

tr::post_processing_buffer::post_processing_buffer(graphics_context& context)
	: m_textures{tr::texture{context}, tr::texture{context}}
	, m_framebuffer{context}
	, m_vertex_shader{context, post_processing_vert}
	, m_shader_pipeline{context}
	, m_vertex_format{context, as_vertex_bindings<vertex_binding_tag<glm::vec2>>}
	, m_vertex_buffer{context, post_processing_vertices}
{
	m_framebuffer.attach(framebuffer::attachment::color0, m_textures[m_source_index]);
	m_shader_pipeline.set_vertex_shader(m_vertex_shader);
}

tr::post_processing_buffer::post_processing_buffer(graphics_context& context, glm::ivec2 size, mipmaps mipmaps, pixel_format format)
	: m_textures{tr::texture{context, size, mipmaps, format}, tr::texture{context, size, mipmaps, format}}
	, m_framebuffer{context}
	, m_vertex_shader{context, post_processing_vert}
	, m_shader_pipeline{context}
	, m_vertex_format{context, as_vertex_bindings<vertex_binding_tag<glm::vec2>>}
	, m_vertex_buffer{context, post_processing_vertices}
{
	m_framebuffer.attach(framebuffer::attachment::color0, m_textures[m_source_index]);
	m_shader_pipeline.set_vertex_shader(m_vertex_shader);
}

//

tr::graphics_context& tr::post_processing_buffer::context() const
{
	return m_framebuffer.context();
}

//

bool tr::post_processing_buffer::complete() const
{
	return m_textures[0].complete();
}

glm::ivec2 tr::post_processing_buffer::size() const
{
	return m_textures[0].size();
}

//

void tr::post_processing_buffer::allocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	for (tr::texture& texture : m_textures) {
		texture.allocate(size, mipmaps, format);
	}
	m_framebuffer.attach(framebuffer::attachment::color0, m_textures[m_source_index]);
}

//

tr::post_processing_buffer::operator render_target() const
{
	return target();
}

tr::render_target tr::post_processing_buffer::target() const
{
	TR_ASSERT(complete(), "Tried to get render target of incomplete post-processing buffer");

	return render_target{m_framebuffer, size()};
}

//

tr::post_processing_buffer::operator texture_view() const
{
	return texture();
}

tr::texture_view tr::post_processing_buffer::texture() const
{
	TR_ASSERT(complete(), "Tried to get texture of incomplete post-processing buffer");

	return m_textures[m_source_index];
}

//

void tr::post_processing_buffer::clear()
{
	TR_ASSERT(complete(), "Tried to clear incomplete post-processing buffer");

	m_textures[m_source_index].clear({});
}

void tr::post_processing_buffer::apply(fragment_shader& fragment_shader)
{
	TR_ASSERT(complete(), "Tried to apply shader on an incomplete post-processing buffer");

	graphics_context& context{this->context()};

	m_framebuffer.attach(framebuffer::attachment::color0, m_textures[(m_source_index + 1) % 2]);
	fragment_shader.set_uniform(0, m_textures[m_source_index]);
	fragment_shader.set_uniform(1, glm::vec2{size()});
	m_shader_pipeline.set_shaders(m_vertex_shader, fragment_shader);
	m_source_index = (m_source_index + 1) % 2;

	context.set_render_target(render_target{m_framebuffer, size()});
	if (context.should_setup_renderer(m_renderer_id)) {
		context.set_shader_pipeline(m_shader_pipeline);
		context.set_vertex_format(m_vertex_format);
		context.set_vertex_buffer(m_vertex_buffer, 0, 0);
	}
	context.draw(primitive::tri_fan, 0, 4);
}