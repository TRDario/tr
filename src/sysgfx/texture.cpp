#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"

namespace tr {
	// Converts a pixel format to a texture format.
	GLenum _tex_format(pixel_format format) noexcept;
	// Converts a pixel format to an OpenGL format.
	GLenum _format(pixel_format format) noexcept;
	// Converts a pixel format to an OpenGL type.
	GLenum _type(pixel_format format) noexcept;

	// List of known textures.
	std::unordered_set<unsigned int> _textures;
} // namespace tr

GLenum tr::_tex_format(pixel_format format) noexcept
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

GLenum tr::_format(pixel_format format) noexcept
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

GLenum tr::_type(pixel_format format) noexcept
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

tr::texture::texture() noexcept
	: _size{0, 0}
{
	try {
		GLuint id;
		TR_GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &id);
		_textures.insert(id);
		_id.reset(id);
	}
	catch (std::bad_alloc&) {
		TR_TERMINATE("Out of memory", "Exception occurred while creating a texture.");
	}
}

void tr::texture::_allocate(glm::ivec2 size, bool mipmapped, pixel_format format) noexcept
{
	TR_GL_CALL(glTextureStorage2D, _id.get(), mipmapped ? static_cast<GLsizei>(std::floor(std::log2(std::max(size.x, size.y))) + 1) : 1,
			   _tex_format(format), size.x, size.y);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		TR_TERMINATE("Out of video memory", "Exception occurred while allocating a texture.");
	}
	_size = size;
}

tr::texture::texture(glm::ivec2 size, bool mipmapped, pixel_format format) noexcept
	: texture{}
{
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to allocate a texture with an invalid size of {}x{}", size.x, size.y);

	_size = size;
	_allocate(size, mipmapped, format);
}

tr::texture::texture(const sub_bitmap& bitmap, bool mipmapped, std::optional<pixel_format> format) noexcept
	: texture{bitmap.size(), mipmapped, format.value_or(bitmap.format())}
{
	set_region({}, bitmap);
}

void tr::texture::deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteTextures, 1, &id);
	_textures.erase(id);
}

tr::render_texture::~render_texture() noexcept
{
	if (_fbo.has_value() && _render_target.has_value() && _render_target->_fbo == _fbo.get()) {
		_render_target.reset();
	}
}

const glm::ivec2& tr::texture::size() const noexcept
{
	return _size;
}

void tr::texture::set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(min_filter));
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(mag_filter));
}

void tr::texture::set_wrap(wrap wrap) noexcept
{
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap));
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap));
	TR_GL_CALL(glTextureParameteri, _id.get(), GL_TEXTURE_WRAP_R, static_cast<GLint>(wrap));
}

void tr::texture::set_swizzle(swizzle r, swizzle g, swizzle b, swizzle a) noexcept
{
	std::array<int, 4> glSwizzle{static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), static_cast<int>(a)};
	TR_GL_CALL(glTextureParameteriv, _id.get(), GL_TEXTURE_SWIZZLE_RGBA, glSwizzle.data());
}

void tr::texture::set_border_color(rgbaf color) noexcept
{
	TR_GL_CALL(glTextureParameterfv, _id.get(), GL_TEXTURE_BORDER_COLOR, &color.r);
}

void tr::texture::clear(const rgbaf& color) noexcept
{
	TR_GL_CALL(glClearTexImage, _id.get(), 0, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::clear_region(const irect2& rect, const rgbaf& color) noexcept
{
	TR_GL_CALL(glClearTexSubImage, _id.get(), 0, rect.tl.x, rect.tl.y, 0, rect.size.x, rect.size.y, 1, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::copy_region(glm::ivec2 tl, const texture& src, const irect2& rect) noexcept
{
	TR_GL_CALL(glCopyImageSubData, src._id.get(), GL_TEXTURE_2D, 0, rect.tl.x, rect.tl.y, 0, _id.get(), GL_TEXTURE_2D, 0, tl.x, tl.y, 0,
			   rect.size.x, rect.size.y, 1);
}

void tr::texture::set_region(glm::ivec2 tl, const sub_bitmap& bitmap) noexcept
{
	TR_ASSERT(irect2{size()}.contains(tl + bitmap.size()),
			  "Tried to set out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size {}x{}.", tl.x, tl.y,
			  tl.x + bitmap.size().x, tl.y + bitmap.size().y, _size.x, _size.y);

	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / pixel_bytes(bitmap.format()));
	TR_GL_CALL(glTextureSubImage2D, _id.get(), 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, _format(bitmap.format()),
			   _type(bitmap.format()), bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, _id.get());
}

void tr::texture::set_label(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_TEXTURE, _id.get(), static_cast<GLsizei>(label.size()), label.data());
}

/////////////////////////////////////////////////////////////// TEXTURE REF ///////////////////////////////////////////////////////////////

tr::texture_ref::texture_ref(const texture& texture) noexcept
	: _id{texture._id.get()}
{
}

bool tr::texture_ref::valid() const noexcept
{
	return _textures.contains(_id);
}

///////////////////////////////////////////////////////////// RENDER TEXTURE //////////////////////////////////////////////////////////////

tr::render_texture::render_texture(glm::ivec2 size, bool mipmapped, pixel_format format) noexcept
	: texture{size, mipmapped, format}
{
	unsigned int fbo;
	TR_GL_CALL(glCreateFramebuffers, 1, &fbo);
	TR_GL_CALL(glNamedFramebufferTexture, fbo, GL_COLOR_ATTACHMENT0, _id.get(), 0);
	_fbo.reset(fbo);
}

tr::render_texture::render_texture(const sub_bitmap& bitmap, bool mipmapped, std::optional<pixel_format> format) noexcept
	: render_texture{bitmap.size(), mipmapped, format.value_or(bitmap.format())}
{
	set_region({}, bitmap);
}

void tr::render_texture::fbo_deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteFramebuffers, 1, &id);
}

tr::render_texture::operator tr::render_target() const noexcept
{
	return render_target();
}

tr::render_target tr::render_texture::render_target() const noexcept
{
	return tr::render_target{_fbo.get(), {{}, _size}};
}

tr::render_target tr::render_texture::region_render_target(const irect2& rect) const noexcept
{
	TR_ASSERT(irect2{_size}.contains(rect.tl + rect.size),
			  "Tried to create render target for out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size "
			  "%dx%d.",
			  rect.tl.x, rect.tl.y, rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, _size.x, _size.y);

	return tr::render_target{_fbo.get(), rect};
}