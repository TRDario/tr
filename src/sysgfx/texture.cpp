/// @file
/// @brief Implements texture.hpp.

#include "internal/opengl_definitions.hpp"
#include "internal/opengl_texture_format.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/sub_bitmap.hpp>
#include <tr/sysgfx/texture.hpp>
#include <tr/sysgfx/texture_view.hpp>
#include <tr/utility/exception.hpp>
#include <tr/utility/out_handle.hpp>

//

tr::texture::texture(graphics_context& context) noexcept
	: m_handle{deleter{context}}
	, m_size{0, 0}
{
	context.gl().create_textures(GL_TEXTURE_2D, 1, out_handle(m_handle));
}

tr::texture::texture(graphics_context& context, unsigned int handle, glm::ivec2 size) noexcept
	: m_handle{handle, deleter{context}, maybe_empty}
	, m_size{size}
{
}

tr::texture::texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps, pixel_format format)
	: texture{context}
{
	allocate(size, mipmaps, format);
}

tr::texture::texture(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps, std::optional<pixel_format> format)
	: texture{context, bitmap.size(), mipmaps, format.value_or(bitmap.format())}
{
	set_region({0, 0}, bitmap);
}

void tr::texture::deleter::operator()(unsigned int texture) const noexcept
{
	context->gl().delete_textures(1, &texture);
}

//

tr::texture tr::texture::allocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to allocate a texture with an invalid size of {}x{}.", size.x, size.y);

	graphics_context& context{this->context()};
	const internal::opengl& gl{context.gl()};

	unsigned int old_handle;
	glm::ivec2 old_size;
	if (complete()) {
		old_handle = m_handle.get();
		old_size = m_size;

		unsigned int new_handle;
		gl.create_textures(GL_TEXTURE_2D, 1, &new_handle);

		int min_filter;
		gl.get_texture_parameter_iv(old_handle, GL_TEXTURE_MIN_FILTER, &min_filter);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_MIN_FILTER, min_filter);

		int mag_filter;
		gl.get_texture_parameter_iv(old_handle, GL_TEXTURE_MAG_FILTER, &mag_filter);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_MAG_FILTER, mag_filter);

		int wrap;
		gl.get_texture_parameter_iv(old_handle, GL_TEXTURE_WRAP_S, &wrap);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_WRAP_S, wrap);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_WRAP_T, wrap);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_WRAP_R, wrap);

		rgbaf border_color;
		gl.get_texture_parameter_fv(old_handle, GL_TEXTURE_BORDER_COLOR, &border_color.r);
		gl.set_texture_parameter_fv(new_handle, GL_TEXTURE_BORDER_COLOR, &border_color.r);

		context.move_label(GL_TEXTURE, old_handle, new_handle);
		m_handle.reset(new_handle);
	}
	else {
		if (!m_handle.has_value()) {
			gl.create_textures(GL_TEXTURE_2D, 1, out_handle(m_handle));
		}
		old_handle = 0;
		old_size = {};
	}

	const int levels{mipmaps == mipmaps::enabled ? floor_cast<int>(std::log2(std::max(size.x, size.y)) + 1) : 1};
	gl.allocate_2d_texture_storage(m_handle.get(), levels, internal::opengl_texture_format(format), size.x, size.y);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"texture allocation"};
	}
	m_size = size;

	return texture{context, old_handle, old_size};
}

//

tr::texture::operator texture_view() const noexcept
{
	return view();
}

tr::texture_view tr::texture::view() const noexcept
{
	TR_ASSERT(valid(), "Tried to create a view over a texture in an invalid state.");

	return texture_view{m_handle.get()};
}

//

tr::graphics_context& tr::texture::context() const noexcept
{
	TR_ASSERT(valid(), "Tried to get context of a texture in an invalid state.");

	return m_handle.get_deleter().context;
}

//

bool tr::texture::valid() const noexcept
{
	return m_handle.has_value();
}

bool tr::texture::complete() const noexcept
{
	TR_ASSERT(valid(), "Tried to check completeness of a texture in an invalid state.");

	return m_size.x > 0;
}

glm::ivec2 tr::texture::size() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the size of a texture in an invalid state.");

	return m_size;
}

//

void tr::texture::set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept
{
	TR_ASSERT(valid(), "Tried to set filtering of a texture in an invalid state.");

	const internal::opengl& gl{context().gl()};
	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_MIN_FILTER, std::to_underlying(min_filter));
	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_MAG_FILTER, std::to_underlying(mag_filter));
}

void tr::texture::set_wrap(wrap wrap) noexcept
{

	TR_ASSERT(valid(), "Tried to set wrapping of a texture in an invalid state.");

	const internal::opengl& gl{context().gl()};
	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_WRAP_S, std::to_underlying(wrap));
	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_WRAP_T, std::to_underlying(wrap));
	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_WRAP_R, std::to_underlying(wrap));
}

void tr::texture::set_border_color(rgbaf color) noexcept
{
	TR_ASSERT(valid(), "Tried to set border color of a texture in an invalid state.");

	context().gl().set_texture_parameter_fv(m_handle.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
}

//

void tr::texture::clear(rgbaf color) noexcept
{
	TR_ASSERT(valid(), "Tried to clear a texture in an invalid state.");

	context().gl().clear_texture_image(m_handle.get(), 0, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::clear_region(rectangle<int> region, rgbaf color) noexcept
{
	TR_ASSERT(valid(), "Tried to clear a region of a texture in an invalid state.");

	context().gl().clear_texture_sub_image(m_handle.get(), 0, region.tl.x, region.tl.y, 0, region.size.x, region.size.y, 1, GL_RGBA,
										   GL_FLOAT, &color);
}

void tr::texture::copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region) noexcept
{
	TR_ASSERT(valid(), "Tried to copy to a region of a texture in an invalid state.");
	TR_ASSERT(!src.empty(), "Tried to copy a region from an empty texture view.");

	context().gl().copy_image_sub_data(src.unwrap(), GL_TEXTURE_2D, 0, region.tl.x, region.tl.y, 0, m_handle.get(), GL_TEXTURE_2D, 0, tl.x,
									   tl.y, 0, region.size.x, region.size.y, 1);
}

void tr::texture::set_region(glm::ivec2 tl, sub_bitmap bitmap) noexcept
{
	TR_ASSERT(valid(), "Tried to set a region of a texture in an invalid state.");
	TR_ASSERT(rectangle<int>{size()}.contains(tl + bitmap.size()),
			  "Tried to set out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size {}x{}.", tl.x, tl.y,
			  tl.x + bitmap.size().x, tl.y + bitmap.size().y, m_size.x, m_size.y);

	const internal::opengl& gl{context().gl()};
	gl.set_pixel_store_i(GL_UNPACK_ALIGNMENT, 1);
	gl.set_pixel_store_i(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / pixel_bytes(bitmap.format()));
	gl.set_2d_texture_sub_image(m_handle.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y,
								internal::opengl_texture_format_layout(bitmap.format()),
								internal::opengl_texture_format_type(bitmap.format()), bitmap.data());
	gl.generate_texture_mipmap(m_handle.get());
}

//

std::string tr::texture::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a texture in an invalid state.");

	const internal::opengl& gl{context().gl()};
	int label_length;
	gl.get_object_label(GL_TEXTURE, m_handle.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_TEXTURE, m_handle.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

void tr::texture::set_label(std::string_view label) noexcept
{
	TR_ASSERT(valid(), "Tried to set the label of a texture in an invalid state.");

	context().gl().set_object_label(GL_TEXTURE, m_handle.get(), label.size(), label.data());
}

//

unsigned int tr::texture::unwrap() const noexcept
{
	return m_handle.get();
}