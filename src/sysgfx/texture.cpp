#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"

namespace tr {
	// Converts a pixel format to a texture format.
	GLenum tex_format(pixel_format format);
	// Converts a pixel format to an OpenGL format.
	GLenum format(pixel_format format);
	// Converts a pixel format to an OpenGL type.
	GLenum type(pixel_format format);

	// List of known textures.
	std::unordered_set<unsigned int> textures;
} // namespace tr

GLenum tr::tex_format(pixel_format format)
{
	switch (format) {
	case pixel_format::R8:
		return GL_R8;
	case pixel_format::RGB_P332:
		return GL_R3_G3_B2;
	case pixel_format::XRGB_P4444:
	case pixel_format::XBGR_P4444:
		return GL_RGB4;
	case pixel_format::XRGB_P1555:
	case pixel_format::XBGR_P1555:
		return GL_RGB5;
	case pixel_format::ARGB_P4444:
	case pixel_format::RGBA_P4444:
	case pixel_format::ABGR_P4444:
	case pixel_format::BGRA_P4444:
		return GL_RGBA4;
	case pixel_format::ARGB_P1555:
	case pixel_format::RGBA_P5551:
	case pixel_format::ABGR_P1555:
	case pixel_format::BGRA_P5551:
		return GL_RGB5_A1;
	case pixel_format::RGB_P565:
	case pixel_format::BGR_P565:
		return GL_RGB565;
	case pixel_format::RGB24:
	case pixel_format::BGR24:
	case pixel_format::BGRX32:
	case pixel_format::XBGR32:
	case pixel_format::RGBX32:
	case pixel_format::XRGB32:
		return GL_RGB8;
	case pixel_format::BGRA32:
	case pixel_format::ABGR32:
	case pixel_format::ARGB32:
	case pixel_format::RGBA32:
		return GL_RGBA8;
	default:
		return 0;
	}
}

GLenum tr::format(pixel_format format)
{
	switch (format) {
	case pixel_format::R8:
		return GL_RED;
	case pixel_format::RGB_P332:
	case pixel_format::XBGR_P4444:
	case pixel_format::XBGR_P1555:
	case pixel_format::RGB_P565:
	case pixel_format::RGB24:
	case pixel_format::XBGR32:
	case pixel_format::RGBX32:
		return GL_RGB;
	case pixel_format::XRGB_P4444:
	case pixel_format::XRGB_P1555:
	case pixel_format::BGR_P565:
	case pixel_format::BGR24:
	case pixel_format::BGRX32:
	case pixel_format::XRGB32:
		return GL_BGR;
	case pixel_format::RGBA_P4444:
	case pixel_format::ABGR_P4444:
	case pixel_format::RGBA_P5551:
	case pixel_format::ABGR_P1555:
	case pixel_format::ABGR32:
	case pixel_format::RGBA32:
		return GL_RGBA;
	case pixel_format::ARGB_P4444:
	case pixel_format::BGRA_P4444:
	case pixel_format::ARGB_P1555:
	case pixel_format::BGRA_P5551:
	case pixel_format::BGRA32:
	case pixel_format::ARGB32:
		return GL_BGRA;
	default:
		return 0;
	}
}

GLenum tr::type(pixel_format format)
{
	switch (format) {
	case pixel_format::R8:
	case pixel_format::RGB24:
	case pixel_format::RGBA32:
	case pixel_format::BGR24:
	case pixel_format::BGRA32:
		return GL_UNSIGNED_BYTE;
	case pixel_format::RGB_P332:
		return GL_UNSIGNED_BYTE_3_3_2;
	case pixel_format::RGBA_P4444:
	case pixel_format::BGRA_P4444:
		return GL_UNSIGNED_SHORT_4_4_4_4;
	case pixel_format::XRGB_P4444:
	case pixel_format::XBGR_P4444:
	case pixel_format::ARGB_P4444:
	case pixel_format::ABGR_P4444:
		return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	case pixel_format::RGBA_P5551:
	case pixel_format::BGRA_P5551:
		return GL_UNSIGNED_SHORT_5_5_5_1;
	case pixel_format::XRGB_P1555:
	case pixel_format::XBGR_P1555:
	case pixel_format::ARGB_P1555:
	case pixel_format::ABGR_P1555:
		return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case pixel_format::RGB_P565:
	case pixel_format::BGR_P565:
		return GL_UNSIGNED_SHORT_5_6_5;
	case pixel_format::RGBX32:
	case pixel_format::BGRX32:
		return GL_UNSIGNED_INT_8_8_8_8_REV;
	case pixel_format::XRGB32:
	case pixel_format::XBGR32:
	case pixel_format::ARGB32:
	case pixel_format::ABGR32:
		return GL_UNSIGNED_INT_8_8_8_8;
	default:
		return 0;
	}
}

///////////////////////////////////////////////////////////////// TEXTURE /////////////////////////////////////////////////////////////////

tr::gfx::texture::texture()
	: m_size{0, 0}
{
	GLuint temp;
	TR_GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &temp);
	textures.insert(temp);
	m_id.reset(temp);
}

void tr::gfx::texture::allocate(glm::ivec2 size, bool mipmapped, pixel_format format)
{
	TR_GL_CALL(glTextureStorage2D, m_id.get(), mipmapped ? static_cast<GLsizei>(std::floor(std::log2(std::max(size.x, size.y))) + 1) : 1,
			   tex_format(format), size.x, size.y);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"texture allocation"};
	}
	m_size = size;
}

tr::gfx::texture::texture(glm::ivec2 size, bool mipmapped, pixel_format format)
	: texture{}
{
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to allocate a texture with an invalid size of {}x{}", size.x, size.y);

	m_size = size;
	allocate(size, mipmapped, format);
}

tr::gfx::texture::texture(const sub_bitmap& bitmap, bool mipmapped, std::optional<pixel_format> format)
	: texture{bitmap.size(), mipmapped, format.value_or(bitmap.format())}
{
	set_region({}, bitmap);
}

void tr::gfx::texture::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteTextures, 1, &id);
	textures.erase(id);
}

tr::gfx::render_texture::~render_texture()
{
	if (m_fbo.has_value() && current_render_target.has_value() && current_render_target->fbo == m_fbo.get()) {
		current_render_target.reset();
	}
}

const glm::ivec2& tr::gfx::texture::size() const
{
	return m_size;
}

void tr::gfx::texture::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	TR_GL_CALL(glTextureParameteri, m_id.get(), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
	TR_GL_CALL(glTextureParameteri, m_id.get(), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
}

void tr::gfx::texture::set_wrap(wrap wrap)
{
	TR_GL_CALL(glTextureParameteri, m_id.get(), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
	TR_GL_CALL(glTextureParameteri, m_id.get(), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
	TR_GL_CALL(glTextureParameteri, m_id.get(), GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap));
}

void tr::gfx::texture::set_swizzle(swizzle r, swizzle g, swizzle b, swizzle a)
{
	std::array<int, 4> glSwizzle{static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(a)};
	TR_GL_CALL(glTextureParameteriv, m_id.get(), GL_TEXTURE_SWIZZLE_RGBA, glSwizzle.data());
}

void tr::gfx::texture::set_border_color(rgbaf color)
{
	TR_GL_CALL(glTextureParameterfv, m_id.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
}

void tr::gfx::texture::clear(const rgbaf& color)
{
	TR_GL_CALL(glClearTexImage, m_id.get(), 0, GL_RGBA, GL_FLOAT, &color);
}

void tr::gfx::texture::clear_region(const irect2& rect, const rgbaf& color)
{
	TR_GL_CALL(glClearTexSubImage, m_id.get(), 0, rect.tl.x, rect.tl.y, 0, rect.size.x, rect.size.y, 1, GL_RGBA, GL_FLOAT, &color);
}

void tr::gfx::texture::copy_region(glm::ivec2 tl, const texture& src, const irect2& rect)
{
	TR_GL_CALL(glCopyImageSubData, src.m_id.get(), GL_TEXTURE_2D, 0, rect.tl.x, rect.tl.y, 0, m_id.get(), GL_TEXTURE_2D, 0, tl.x, tl.y, 0,
			   rect.size.x, rect.size.y, 1);
}

void tr::gfx::texture::set_region(glm::ivec2 tl, const sub_bitmap& bitmap)
{
	TR_ASSERT(irect2{size()}.contains(tl + bitmap.size()),
			  "Tried to set out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size {}x{}.", tl.x, tl.y,
			  tl.x + bitmap.size().x, tl.y + bitmap.size().y, m_size.x, m_size.y);

	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / pixel_bytes(bitmap.format()));
	TR_GL_CALL(glTextureSubImage2D, m_id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format(bitmap.format()),
			   type(bitmap.format()), bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, m_id.get());
}

void tr::gfx::texture::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_TEXTURE, m_id.get(), static_cast<GLsizei>(label.size()), label.data());
}

/////////////////////////////////////////////////////////////// TEXTURE REF ///////////////////////////////////////////////////////////////

tr::gfx::texture_ref::texture_ref(const texture& texture)
	: m_id{texture.m_id.get()}
{
}

bool tr::gfx::texture_ref::valid() const
{
	return textures.contains(m_id);
}

///////////////////////////////////////////////////////////// RENDER TEXTURE //////////////////////////////////////////////////////////////

tr::gfx::render_texture::render_texture(glm::ivec2 size, bool mipmapped, pixel_format format)
	: texture{size, mipmapped, format}
{
	unsigned int temp;
	TR_GL_CALL(glCreateFramebuffers, 1, &temp);
	TR_GL_CALL(glNamedFramebufferTexture, temp, GL_COLOR_ATTACHMENT0, m_id.get(), 0);
	m_fbo.reset(temp);
}

tr::gfx::render_texture::render_texture(const sub_bitmap& bitmap, bool mipmapped, std::optional<pixel_format> format)
	: render_texture{bitmap.size(), mipmapped, format.value_or(bitmap.format())}
{
	set_region({}, bitmap);
}

void tr::gfx::render_texture::fbo_deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteFramebuffers, 1, &id);
}

tr::gfx::render_texture::operator tr::gfx::render_target() const
{
	return render_target();
}

tr::gfx::render_target tr::gfx::render_texture::render_target() const
{
	return gfx::render_target{m_fbo.get(), {{}, m_size}};
}

tr::gfx::render_target tr::gfx::render_texture::region_render_target(const irect2& rect) const
{
	TR_ASSERT(irect2{m_size}.contains(rect.tl + rect.size),
			  "Tried to create render target for out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size "
			  "%dx%d.",
			  rect.tl.x, rect.tl.y, rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, m_size.x, m_size.y);

	return gfx::render_target{m_fbo.get(), rect};
}