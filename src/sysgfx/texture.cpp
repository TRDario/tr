///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements texture.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/texture_ref.hpp"
#include "../../include/tr/utility/enum.hpp"

///////////////////////////////////////////////////////////// HELPER FUNCTIONS ////////////////////////////////////////////////////////////

// Converts a pixel format to an OpenGL texture format.
static unsigned int gl_tex_format(tr::pixel_format format)
{
	switch (format) {
	case tr::pixel_format::r8:
		return GL_R8;
	case tr::pixel_format::rgb_p332:
		return GL_R3_G3_B2;
	case tr::pixel_format::xrgb_p4444:
	case tr::pixel_format::xbgr_p4444:
		return GL_RGB4;
	case tr::pixel_format::xrgb_p1555:
	case tr::pixel_format::xbgr_p1555:
		return GL_RGB5;
	case tr::pixel_format::argb_p4444:
	case tr::pixel_format::rgba_p4444:
	case tr::pixel_format::abgr_p4444:
	case tr::pixel_format::bgra_p4444:
		return GL_RGBA4;
	case tr::pixel_format::argb_p1555:
	case tr::pixel_format::rgba_p5551:
	case tr::pixel_format::abgr_p1555:
	case tr::pixel_format::bgra_p5551:
		return GL_RGB5_A1;
	case tr::pixel_format::rgb_p565:
	case tr::pixel_format::bgr_p565:
		return GL_RGB565;
	case tr::pixel_format::rgb24:
	case tr::pixel_format::bgr24:
	case tr::pixel_format::bgrx32:
	case tr::pixel_format::xbgr32:
	case tr::pixel_format::rgbx32:
	case tr::pixel_format::xrgb32:
		return GL_RGB8;
	case tr::pixel_format::bgra32:
	case tr::pixel_format::abgr32:
	case tr::pixel_format::argb32:
	case tr::pixel_format::rgba32:
		return GL_RGBA8;
	default:
		TR_UNREACHABLE;
	}
}

// Converts a pixel format to an OpenGL format.
static unsigned int gl_format(tr::pixel_format format)
{
	switch (format) {
	case tr::pixel_format::r8:
		return GL_RED;
	case tr::pixel_format::rgb_p332:
	case tr::pixel_format::xbgr_p4444:
	case tr::pixel_format::xbgr_p1555:
	case tr::pixel_format::rgb_p565:
	case tr::pixel_format::rgb24:
	case tr::pixel_format::xbgr32:
	case tr::pixel_format::rgbx32:
		return GL_RGB;
	case tr::pixel_format::xrgb_p4444:
	case tr::pixel_format::xrgb_p1555:
	case tr::pixel_format::bgr_p565:
	case tr::pixel_format::bgr24:
	case tr::pixel_format::bgrx32:
	case tr::pixel_format::xrgb32:
		return GL_BGR;
	case tr::pixel_format::rgba_p4444:
	case tr::pixel_format::abgr_p4444:
	case tr::pixel_format::rgba_p5551:
	case tr::pixel_format::abgr_p1555:
	case tr::pixel_format::abgr32:
	case tr::pixel_format::rgba32:
		return GL_RGBA;
	case tr::pixel_format::argb_p4444:
	case tr::pixel_format::bgra_p4444:
	case tr::pixel_format::argb_p1555:
	case tr::pixel_format::bgra_p5551:
	case tr::pixel_format::bgra32:
	case tr::pixel_format::argb32:
		return GL_BGRA;
	default:
		TR_UNREACHABLE;
	}
}

// Converts a pixel format to an OpenGL type.
static unsigned int gl_type(tr::pixel_format format)
{
	switch (format) {
	case tr::pixel_format::r8:
	case tr::pixel_format::rgb24:
	case tr::pixel_format::rgba32:
	case tr::pixel_format::bgr24:
	case tr::pixel_format::bgra32:
		return GL_UNSIGNED_BYTE;
	case tr::pixel_format::rgb_p332:
		return GL_UNSIGNED_BYTE_3_3_2;
	case tr::pixel_format::rgba_p4444:
	case tr::pixel_format::bgra_p4444:
		return GL_UNSIGNED_SHORT_4_4_4_4;
	case tr::pixel_format::xrgb_p4444:
	case tr::pixel_format::xbgr_p4444:
	case tr::pixel_format::argb_p4444:
	case tr::pixel_format::abgr_p4444:
		return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	case tr::pixel_format::rgba_p5551:
	case tr::pixel_format::bgra_p5551:
		return GL_UNSIGNED_SHORT_5_5_5_1;
	case tr::pixel_format::xrgb_p1555:
	case tr::pixel_format::xbgr_p1555:
	case tr::pixel_format::argb_p1555:
	case tr::pixel_format::abgr_p1555:
		return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case tr::pixel_format::rgb_p565:
	case tr::pixel_format::bgr_p565:
		return GL_UNSIGNED_SHORT_5_6_5;
	case tr::pixel_format::rgbx32:
	case tr::pixel_format::bgrx32:
		return GL_UNSIGNED_INT_8_8_8_8_REV;
	case tr::pixel_format::xrgb32:
	case tr::pixel_format::xbgr32:
	case tr::pixel_format::argb32:
	case tr::pixel_format::abgr32:
		return GL_UNSIGNED_INT_8_8_8_8;
	default:
		TR_UNREACHABLE;
	}
}

///////////////////////////////////////////////////////////////// TEXTURE /////////////////////////////////////////////////////////////////

tr::texture::texture(graphics_context& context)
	: m_context{context}
	, m_size{0, 0}
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.create_textures(GL_TEXTURE_2D, 1, &m_handle);
}

tr::texture::texture(graphics_context& context, unsigned int handle, glm::ivec2 size)
	: m_context{context}
	, m_handle{handle}
	, m_size{size}
{
}

tr::texture::texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps, pixel_format format)
	: texture{context}
{
	reallocate(size, mipmaps, format);
}

tr::texture::texture(graphics_context& context, const sub_bitmap& bitmap, mipmaps mipmaps, std::optional<pixel_format> format)
	: texture{context, bitmap.size(), mipmaps, format.value_or(bitmap.format())}
{
	set_region({}, bitmap);
}

tr::texture::texture(texture&& r) noexcept
	: m_context{r.m_context}
	, m_handle{std::exchange(r.m_handle, 0)}
	, m_size{r.m_size}
	, m_references{std::move(r.m_references)}
{
	for (texture_ref& ref : m_references) {
		ref.rebind(*this);
	}
}

tr::texture::~texture()
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.delete_textures(1, &m_handle);
	for (texture_ref& ref : m_references) {
		ref.unbind();
	}
}

//

tr::texture& tr::texture::operator=(texture&& r) noexcept
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.delete_textures(1, &m_handle);
	for (texture_ref& ref : m_references) {
		ref.unbind();
	}

	m_handle = std::exchange(r.m_handle, 0);
	m_size = r.m_size;
	m_references = std::move(r.m_references);
	return *this;
}

//

tr::texture tr::texture::reallocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to allocate a texture with an invalid size of {}x{}", size.x, size.y);

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	unsigned int old_handle{m_handle};
	glm::ivec2 old_size{m_size};

	if (m_size != glm::ivec2{0, 0}) {
		unsigned int new_handle;
		gl.create_textures(GL_TEXTURE_2D, 1, &new_handle);

		int min_filter;
		gl.get_texture_parameter_iv(m_handle, GL_TEXTURE_MIN_FILTER, &min_filter);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_MIN_FILTER, min_filter);

		int mag_filter;
		gl.get_texture_parameter_iv(m_handle, GL_TEXTURE_MAG_FILTER, &mag_filter);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_MAG_FILTER, mag_filter);

		int wrap;
		gl.get_texture_parameter_iv(m_handle, GL_TEXTURE_WRAP_S, &wrap);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_WRAP_S, wrap);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_WRAP_T, wrap);
		gl.set_texture_parameter_i(new_handle, GL_TEXTURE_WRAP_R, wrap);

		rgbaf border_color;
		gl.get_texture_parameter_fv(m_handle, GL_TEXTURE_BORDER_COLOR, &border_color.r);
		gl.set_texture_parameter_fv(new_handle, GL_TEXTURE_BORDER_COLOR, &border_color.r);

		m_context.move_label(GL_TEXTURE, m_handle, new_handle);
		m_handle = new_handle;
	}
	else if (m_handle == 0) {
		gl.create_textures(GL_TEXTURE_2D, 1, &m_handle);
	}
	else {
		old_handle = 0;
		old_size = {};
	}

	const int levels{mipmaps == mipmaps::enabled ? floor_cast<int>(std::log2(std::max(size.x, size.y)) + 1) : 1};
	gl.allocate_2d_texture_storage(m_handle, levels, gl_tex_format(format), size.x, size.y);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"texture allocation"};
	}
	m_size = size;

	m_context.rebind_texture_units(*this);

	return texture{m_context, old_handle, old_size};
}

//

bool tr::texture::empty() const
{
	return m_handle == 0;
}

glm::ivec2 tr::texture::size() const
{
	return m_size;
}

//

void tr::texture::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	TR_ASSERT(!empty(), "Tried to set filter on an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.set_texture_parameter_i(m_handle, GL_TEXTURE_MIN_FILTER, to_underlying(min_filter));
	gl.set_texture_parameter_i(m_handle, GL_TEXTURE_MAG_FILTER, to_underlying(mag_filter));
}

void tr::texture::set_wrap(wrap wrap)
{
	TR_ASSERT(!empty(), "Tried to set wrap on an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.set_texture_parameter_i(m_handle, GL_TEXTURE_WRAP_S, to_underlying(wrap));
	gl.set_texture_parameter_i(m_handle, GL_TEXTURE_WRAP_T, to_underlying(wrap));
	gl.set_texture_parameter_i(m_handle, GL_TEXTURE_WRAP_R, to_underlying(wrap));
}

void tr::texture::set_border_color(rgbaf color)
{
	TR_ASSERT(!empty(), "Tried to set border color on an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.set_texture_parameter_fv(m_handle, GL_TEXTURE_BORDER_COLOR, &color.r);
}

//

void tr::texture::clear(const rgbaf& color)
{
	TR_ASSERT(!empty(), "Tried to clear an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.clear_texture_image(m_handle, 0, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::clear_region(const rectangle<int>& region, const rgbaf& color)
{
	TR_ASSERT(!empty(), "Tried to clear a region of an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.clear_texture_sub_image(m_handle, 0, region.tl.x, region.tl.y, 0, region.size.x, region.size.y, 1, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::copy_region(glm::ivec2 tl, const texture& src, const rectangle<int>& region)
{
	TR_ASSERT(!empty(), "Tried to copy to an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.copy_image_sub_data(src.m_handle, GL_TEXTURE_2D, 0, region.tl.x, region.tl.y, 0, m_handle, GL_TEXTURE_2D, 0, tl.x, tl.y, 0,
						   region.size.x, region.size.y, 1);
}

void tr::texture::set_region(glm::ivec2 tl, const sub_bitmap& bitmap)
{
	TR_ASSERT(!empty(), "Tried to set a region of an empty texture.");

	TR_ASSERT(rectangle<int>{size()}.contains(tl + bitmap.size()),
			  "Tried to set out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size {}x{}.", tl.x, tl.y,
			  tl.x + bitmap.size().x, tl.y + bitmap.size().y, m_size.x, m_size.y);

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.set_pixel_store_i(GL_UNPACK_ALIGNMENT, 1);
	gl.set_pixel_store_i(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / pixel_bytes(bitmap.format()));
	gl.set_2d_texture_sub_image(m_handle, 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, gl_format(bitmap.format()),
								gl_type(bitmap.format()), bitmap.data());
	gl.generate_texture_mipmap(m_handle);
}

//

std::string tr::texture::label() const
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	int label_length;
	gl.get_object_label(GL_TEXTURE, m_handle, 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_TEXTURE, m_handle, label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

void tr::texture::set_label(std::string_view label)
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	if (!empty()) {
		gl.set_object_label(GL_TEXTURE, m_handle, label.size(), label.data());
	}
}