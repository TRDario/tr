///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements render_texture.hpp.                                                                                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/render_texture.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"

///////////////////////////////////////////////////////////// RENDER TEXTURE //////////////////////////////////////////////////////////////

tr::render_texture::render_texture(graphics_context& context)
	: m_texture{context}
	, m_framebuffer{context}
{
}

tr::render_texture::render_texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps, pixel_format format)
	: m_texture{context, size, mipmaps, format}
	, m_framebuffer{context}
{
	m_framebuffer.attach(framebuffer::attachment::color0, m_texture);
}

tr::render_texture::render_texture(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps, std::optional<pixel_format> format)
	: m_texture{context, bitmap, mipmaps, format}
	, m_framebuffer{context}
{
	m_framebuffer.attach(framebuffer::attachment::color0, m_texture);
}

//

tr::render_texture::operator texture_view() const
{
	return m_texture.view();
}

tr::texture_view tr::render_texture::view() const
{
	return m_texture.view();
}

//

tr::render_texture::operator tr::render_target() const
{
	return render_target();
}

tr::render_target tr::render_texture::render_target() const
{
	TR_ASSERT(complete(), "Tried to create a render target for an incomplete texture.");

	return m_framebuffer.render_target(size());
}

//

tr::graphics_context& tr::render_texture::context() const
{
	return m_texture.context();
}

//

bool tr::render_texture::complete() const
{
	return m_texture.complete();
}

glm::ivec2 tr::render_texture::size() const
{
	return m_texture.size();
}

//

tr::texture tr::render_texture::allocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	tr::texture old_texture{m_texture.allocate(size, mipmaps, format)};
	m_framebuffer.attach(framebuffer::attachment::color0, m_texture);
	return old_texture;
}

//

void tr::render_texture::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	m_texture.set_filtering(min_filter, mag_filter);
}

void tr::render_texture::set_wrap(wrap wrap)
{
	m_texture.set_wrap(wrap);
}

void tr::render_texture::set_border_color(rgbaf color)
{
	m_texture.set_border_color(color);
}

//

void tr::render_texture::clear(rgbaf color)
{
	m_texture.clear(color);
}

void tr::render_texture::clear_region(rectangle<int> region, rgbaf color)
{
	m_texture.clear_region(region, color);
}

void tr::render_texture::copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region)
{
	m_texture.copy_region(tl, src, region);
}

void tr::render_texture::set_region(glm::ivec2 tl, sub_bitmap bitmap)
{
	m_texture.set_region(tl, bitmap);
}

//

std::string tr::render_texture::label() const
{
	return m_texture.label();
}

void tr::render_texture::set_label(std::string_view label)
{
	m_texture.set_label(label);
}