/// @file
/// @brief Implements texture.hpp.

#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"

//

namespace tr
{
	namespace
	{
		/// Converts a pixel format to an OpenGL texture format.
		/// @param format Pixel format type.
		/// @return Equivalent OpenGL texture format.
		unsigned int gl_tex_format(pixel_format format)
		{
			switch (format) {
			case pixel_format::r8:
				return GL_R8;
			case pixel_format::rgb_p332:
				return GL_R3_G3_B2;
			case pixel_format::xrgb_p4444:
			case pixel_format::xbgr_p4444:
				return GL_RGB4;
			case pixel_format::xrgb_p1555:
			case pixel_format::xbgr_p1555:
				return GL_RGB5;
			case pixel_format::argb_p4444:
			case pixel_format::rgba_p4444:
			case pixel_format::abgr_p4444:
			case pixel_format::bgra_p4444:
				return GL_RGBA4;
			case pixel_format::argb_p1555:
			case pixel_format::rgba_p5551:
			case pixel_format::abgr_p1555:
			case pixel_format::bgra_p5551:
				return GL_RGB5_A1;
			case pixel_format::rgb_p565:
			case pixel_format::bgr_p565:
				return GL_RGB565;
			case pixel_format::rgb24:
			case pixel_format::bgr24:
			case pixel_format::bgrx32:
			case pixel_format::xbgr32:
			case pixel_format::rgbx32:
			case pixel_format::xrgb32:
				return GL_RGB8;
			case pixel_format::bgra32:
			case pixel_format::abgr32:
			case pixel_format::argb32:
			case pixel_format::rgba32:
				return GL_RGBA8;
			default:
				TR_UNREACHABLE;
			}
		}

		/// Converts a pixel format to an OpenGL format.
		/// @param format Pixel format type.
		/// @return Equivalent OpenGL format.
		unsigned int gl_format(pixel_format format)
		{
			switch (format) {
			case pixel_format::r8:
				return GL_RED;
			case pixel_format::rgb_p332:
			case pixel_format::xbgr_p4444:
			case pixel_format::xbgr_p1555:
			case pixel_format::rgb_p565:
			case pixel_format::rgb24:
			case pixel_format::xbgr32:
			case pixel_format::rgbx32:
				return GL_RGB;
			case pixel_format::xrgb_p4444:
			case pixel_format::xrgb_p1555:
			case pixel_format::bgr_p565:
			case pixel_format::bgr24:
			case pixel_format::bgrx32:
			case pixel_format::xrgb32:
				return GL_BGR;
			case pixel_format::rgba_p4444:
			case pixel_format::abgr_p4444:
			case pixel_format::rgba_p5551:
			case pixel_format::abgr_p1555:
			case pixel_format::abgr32:
			case pixel_format::rgba32:
				return GL_RGBA;
			case pixel_format::argb_p4444:
			case pixel_format::bgra_p4444:
			case pixel_format::argb_p1555:
			case pixel_format::bgra_p5551:
			case pixel_format::bgra32:
			case pixel_format::argb32:
				return GL_BGRA;
			default:
				TR_UNREACHABLE;
			}
		}

		/// Converts a pixel format to an OpenGL type.
		/// @param format Pixel format type.
		/// @return Equivalent OpenGL type.
		unsigned int gl_type(pixel_format format)
		{
			switch (format) {
			case pixel_format::r8:
			case pixel_format::rgb24:
			case pixel_format::rgba32:
			case pixel_format::bgr24:
			case pixel_format::bgra32:
				return GL_UNSIGNED_BYTE;
			case pixel_format::rgb_p332:
				return GL_UNSIGNED_BYTE_3_3_2;
			case pixel_format::rgba_p4444:
			case pixel_format::bgra_p4444:
				return GL_UNSIGNED_SHORT_4_4_4_4;
			case pixel_format::xrgb_p4444:
			case pixel_format::xbgr_p4444:
			case pixel_format::argb_p4444:
			case pixel_format::abgr_p4444:
				return GL_UNSIGNED_SHORT_4_4_4_4_REV;
			case pixel_format::rgba_p5551:
			case pixel_format::bgra_p5551:
				return GL_UNSIGNED_SHORT_5_5_5_1;
			case pixel_format::xrgb_p1555:
			case pixel_format::xbgr_p1555:
			case pixel_format::argb_p1555:
			case pixel_format::abgr_p1555:
				return GL_UNSIGNED_SHORT_1_5_5_5_REV;
			case pixel_format::rgb_p565:
			case pixel_format::bgr_p565:
				return GL_UNSIGNED_SHORT_5_6_5;
			case pixel_format::rgbx32:
			case pixel_format::bgrx32:
				return GL_UNSIGNED_INT_8_8_8_8_REV;
			case pixel_format::xrgb32:
			case pixel_format::xbgr32:
			case pixel_format::argb32:
			case pixel_format::abgr32:
				return GL_UNSIGNED_INT_8_8_8_8;
			default:
				TR_UNREACHABLE;
			}
		}
	} // namespace
} // namespace tr

//

void tr::texture::create_handle() const
{
	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.create_textures(GL_TEXTURE_2D, 1, out_handle(m_handle));
}

tr::texture::texture(graphics_context& context)
	: m_handle{{context}}
	, m_size{0, 0}
{
	create_handle();
}

tr::texture::texture(graphics_context& context, unsigned int handle, glm::ivec2 size)
	: m_handle{handle, {context}}
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
	set_region({}, bitmap);
}

void tr::texture::deleter::operator()(unsigned int texture) const
{
	const gl_api& gl{context.make_current_and_return_gl_api()};
	gl.delete_textures(1, &texture);
}

//

tr::texture tr::texture::allocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to allocate a texture with an invalid size of {}x{}.", size.x, size.y);

	graphics_context& context{this->context()};
	const gl_api& gl{context.make_current_and_return_gl_api()};

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
	gl.allocate_2d_texture_storage(m_handle.get(), levels, gl_tex_format(format), size.x, size.y);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"texture allocation"};
	}
	m_size = size;

	return texture{context, old_handle, old_size};
}

//

tr::texture::operator texture_view() const
{
	return view();
}

tr::texture_view tr::texture::view() const
{
	if (!m_handle.has_value()) {
		create_handle();
	}
	return m_handle.get();
}

//

tr::graphics_context& tr::texture::context() const
{
	return m_handle.get_deleter().context;
}

//

bool tr::texture::complete() const
{
	return m_size.x > 0;
}

glm::ivec2 tr::texture::size() const
{
	return m_size;
}

//

void tr::texture::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	if (!m_handle.has_value()) {
		create_handle();
	}

	const gl_api& gl{context().make_current_and_return_gl_api()};

	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_MIN_FILTER, std::to_underlying(min_filter));
	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_MAG_FILTER, std::to_underlying(mag_filter));
}

void tr::texture::set_wrap(wrap wrap)
{
	if (!m_handle.has_value()) {
		create_handle();
	}

	const gl_api& gl{context().make_current_and_return_gl_api()};

	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_WRAP_S, std::to_underlying(wrap));
	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_WRAP_T, std::to_underlying(wrap));
	gl.set_texture_parameter_i(m_handle.get(), GL_TEXTURE_WRAP_R, std::to_underlying(wrap));
}

void tr::texture::set_border_color(rgbaf color)
{
	if (!m_handle.has_value()) {
		create_handle();
	}

	const gl_api& gl{context().make_current_and_return_gl_api()};

	gl.set_texture_parameter_fv(m_handle.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
}

//

void tr::texture::clear(rgbaf color)
{
	if (!m_handle.has_value()) {
		create_handle();
	}

	const gl_api& gl{context().make_current_and_return_gl_api()};

	gl.clear_texture_image(m_handle.get(), 0, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::clear_region(rectangle<int> region, rgbaf color)
{
	if (!m_handle.has_value()) {
		create_handle();
	}

	const gl_api& gl{context().make_current_and_return_gl_api()};

	gl.clear_texture_sub_image(m_handle.get(), 0, region.tl.x, region.tl.y, 0, region.size.x, region.size.y, 1, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region)
{
	if (!m_handle.has_value()) {
		create_handle();
	}

	const gl_api& gl{context().make_current_and_return_gl_api()};

	gl.copy_image_sub_data(src.m_id, GL_TEXTURE_2D, 0, region.tl.x, region.tl.y, 0, m_handle.get(), GL_TEXTURE_2D, 0, tl.x, tl.y, 0,
						   region.size.x, region.size.y, 1);
}

void tr::texture::set_region(glm::ivec2 tl, sub_bitmap bitmap)
{
	if (!m_handle.has_value()) {
		create_handle();
	}

	TR_ASSERT(rectangle<int>{size()}.contains(tl + bitmap.size()),
			  "Tried to set out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size {}x{}.", tl.x, tl.y,
			  tl.x + bitmap.size().x, tl.y + bitmap.size().y, m_size.x, m_size.y);

	const gl_api& gl{context().make_current_and_return_gl_api()};

	gl.set_pixel_store_i(GL_UNPACK_ALIGNMENT, 1);
	gl.set_pixel_store_i(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / pixel_bytes(bitmap.format()));
	gl.set_2d_texture_sub_image(m_handle.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, gl_format(bitmap.format()),
								gl_type(bitmap.format()), bitmap.data());
	gl.generate_texture_mipmap(m_handle.get());
}

//

std::string tr::texture::label() const
{
	if (!m_handle.has_value()) {
		return "<unnamed>";
	}

	const gl_api& gl{context().make_current_and_return_gl_api()};

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

void tr::texture::set_label(std::string_view label)
{
	if (!m_handle.has_value()) {
		create_handle();
	}

	const gl_api& gl{context().make_current_and_return_gl_api()};

	gl.set_object_label(GL_TEXTURE, m_handle.get(), label.size(), label.data());
}