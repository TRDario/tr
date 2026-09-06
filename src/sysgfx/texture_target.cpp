/// @file
/// @brief Implements texture_target.hpp.

#include "../../include/tr/sysgfx/texture_target.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"

//

tr::texture_target::texture_target(graphics_context& context)
	: m_texture{context}
	, m_framebuffer{context}
{
}

tr::texture_target::texture_target(graphics_context& context, glm::ivec2 size, mipmaps mipmaps, pixel_format format)
	: m_texture{context, size, mipmaps, format}
	, m_framebuffer{context}
{
	m_framebuffer.attach(framebuffer::attachment::color0, m_texture);
}

tr::texture_target::texture_target(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps, std::optional<pixel_format> format)
	: m_texture{context, bitmap, mipmaps, format}
	, m_framebuffer{context}
{
	m_framebuffer.attach(framebuffer::attachment::color0, m_texture);
}

//

tr::texture_target::operator texture_view() const
{
	return m_texture;
}

tr::texture_target::operator tr::render_target()
{
	TR_ASSERT(complete(), "Tried to create a render target for an incomplete texture.");

	return render_target{m_framebuffer, size()};
}

//

tr::graphics_context& tr::texture_target::context() const
{
	return m_texture.context();
}

//

bool tr::texture_target::valid() const
{
	return m_texture.valid();
}

bool tr::texture_target::complete() const
{
	return m_texture.complete();
}

glm::ivec2 tr::texture_target::size() const
{
	return m_texture.size();
}

//

tr::texture tr::texture_target::allocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	texture old_texture{m_texture.allocate(size, mipmaps, format)};
	m_framebuffer.attach(framebuffer::attachment::color0, m_texture);
	return old_texture;
}

//

void tr::texture_target::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	m_texture.set_filtering(min_filter, mag_filter);
}

void tr::texture_target::set_wrap(wrap wrap)
{
	m_texture.set_wrap(wrap);
}

void tr::texture_target::set_border_color(rgbaf color)
{
	m_texture.set_border_color(color);
}

//

void tr::texture_target::clear(rgbaf color)
{
	m_texture.clear(color);
}

void tr::texture_target::clear_region(rectangle<int> region, rgbaf color)
{
	m_texture.clear_region(region, color);
}

void tr::texture_target::copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region)
{
	m_texture.copy_region(tl, src, region);
}

void tr::texture_target::set_region(glm::ivec2 tl, sub_bitmap bitmap)
{
	m_texture.set_region(tl, bitmap);
}

//

std::string tr::texture_target::label() const
{
	std::string label{m_texture.label()};
	if (label.ends_with("- Texture")) {
		label.resize(label.size() - 9);
	}
	return label;
}

void tr::texture_target::set_label(std::string_view label)
{
	m_texture.set_label(std::format("{} - Texture", label));
	m_framebuffer.set_label(std::format("{} - Framebuffer", label));
}