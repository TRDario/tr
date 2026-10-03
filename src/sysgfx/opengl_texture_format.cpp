/// @file
/// @brief Implements internal/texture_format.hpp.

#include "internal/opengl_texture_format.hpp"
#include "internal/opengl_definitions.hpp"
#include <tr/utility/macro.hpp>

//

unsigned int tr::internal::opengl_texture_format(pixel_format format) noexcept
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

unsigned int tr::internal::opengl_texture_format_layout(pixel_format format) noexcept
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

unsigned int tr::internal::opengl_texture_format_type(pixel_format format) noexcept
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

//

unsigned int tr::internal::opengl_image_format(pixel_format format) noexcept
{
	switch (format) {
	case pixel_format::r8:
		return GL_R8;
	case pixel_format::rgb_p332:
	case pixel_format::xrgb_p4444:
	case pixel_format::xbgr_p4444:
	case pixel_format::rgb24:
	case pixel_format::bgr24:
	case pixel_format::bgrx32:
	case pixel_format::xbgr32:
	case pixel_format::rgbx32:
	case pixel_format::xrgb32:
		return GL_RGB8;
	case pixel_format::rgba_p4444:
	case pixel_format::bgra_p4444:
	case pixel_format::argb_p4444:
	case pixel_format::abgr_p4444:
	case pixel_format::rgba_p5551:
	case pixel_format::bgra_p5551:
	case pixel_format::xrgb_p1555:
	case pixel_format::xbgr_p1555:
	case pixel_format::argb_p1555:
	case pixel_format::abgr_p1555:
	case pixel_format::rgb_p565:
	case pixel_format::bgr_p565:
	case pixel_format::bgra32:
	case pixel_format::abgr32:
	case pixel_format::argb32:
	case pixel_format::rgba32:
		return GL_RGBA8;
	default:
		TR_UNREACHABLE;
	}
}