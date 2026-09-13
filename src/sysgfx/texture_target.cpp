/// @file
/// @brief Implements texture_target.hpp.

#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/sub_bitmap.hpp>
#include <tr/sysgfx/texture_target.hpp>
#include <tr/sysgfx/texture_view.hpp>

//

tr::texture_target::texture_target(graphics_context& context) noexcept
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

tr::texture_target::operator texture_view() const noexcept
{
	return m_texture;
}

tr::texture_target::operator tr::render_target() noexcept
{
	TR_ASSERT(complete(), "Tried to create a render target for an incomplete texture.");

	return render_target{m_framebuffer, size()};
}

//

tr::graphics_context& tr::texture_target::context() const noexcept
{
	return m_texture.context();
}

//

bool tr::texture_target::valid() const noexcept
{
	return m_texture.valid();
}

bool tr::texture_target::complete() const noexcept
{
	return m_texture.complete();
}

glm::ivec2 tr::texture_target::size() const noexcept
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

void tr::texture_target::set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept
{
	m_texture.set_filtering(min_filter, mag_filter);
}

void tr::texture_target::set_wrap(wrap wrap) noexcept
{
	m_texture.set_wrap(wrap);
}

void tr::texture_target::set_border_color(rgbaf color) noexcept
{
	m_texture.set_border_color(color);
}

//

void tr::texture_target::clear(rgbaf color) noexcept
{
	m_texture.clear(color);
}

void tr::texture_target::clear_region(rectangle<int> region, rgbaf color) noexcept
{
	m_texture.clear_region(region, color);
}

void tr::texture_target::copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region) noexcept
{
	m_texture.copy_region(tl, src, region);
}

void tr::texture_target::set_region(glm::ivec2 tl, sub_bitmap bitmap) noexcept
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