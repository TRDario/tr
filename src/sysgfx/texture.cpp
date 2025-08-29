#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"

namespace tr::gfx {
	// Converts a pixel format to a texture format.
	GLenum tex_format(pixel_format format);
	// Converts a pixel format to an OpenGL format.
	GLenum format(pixel_format format);
	// Converts a pixel format to an OpenGL type.
	GLenum type(pixel_format format);
} // namespace tr::gfx

GLenum tr::gfx::tex_format(pixel_format format)
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

GLenum tr::gfx::format(pixel_format format)
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

GLenum tr::gfx::type(pixel_format format)
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
	TR_GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &m_handle);
}

tr::gfx::texture::texture(unsigned int handle, glm::ivec2 size)
	: m_handle{handle}, m_size{size}
{
}

tr::gfx::texture::texture(glm::ivec2 size, bool mipmapped, pixel_format format)
	: texture{}
{
	reallocate(size, mipmapped, format);
}

tr::gfx::texture::texture(const sub_bitmap& bitmap, bool mipmapped, std::optional<pixel_format> format)
	: texture{bitmap.size(), mipmapped, format.value_or(bitmap.format())}
{
	set_region({}, bitmap);
}

tr::gfx::texture::texture(texture&& r) noexcept
	: m_handle{r.m_handle}, m_size{r.m_size}, m_refs{std::move(r.m_refs)}
{
	r.m_handle = 0;
	for (texture_ref& ref : m_refs) {
		ref.m_ref = *this;
	}
}

tr::gfx::texture::~texture()
{
	TR_GL_CALL(glDeleteTextures, 1, &m_handle);
	for (texture_ref& ref : m_refs) {
		ref = std::nullopt;
	}
}

tr::gfx::texture& tr::gfx::texture::operator=(texture&& r) noexcept
{
	TR_GL_CALL(glDeleteTextures, 1, &m_handle);
	for (texture_ref& ref : m_refs) {
		ref = std::nullopt;
	}

	m_handle = r.m_handle;
	m_size = r.m_size;
	m_refs = std::move(r.m_refs);
	r.m_handle = 0;
	return *this;
}

tr::gfx::texture tr::gfx::texture::reallocate(glm::ivec2 size, bool mipmapped, pixel_format format)
{
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to allocate a texture with an invalid size of {}x{}", size.x, size.y);

	GLuint old_handle{m_handle};
	glm::ivec2 old_size{m_size};

	if (m_size != glm::ivec2{0, 0}) {
		GLint min_filter;
		GLint mag_filter;
		GLint wrap;
		rgbaf border_color;
		char label_buffer[128];
		GLsizei label_length;
		TR_GL_CALL(glGetTextureParameteriv, m_handle, GL_TEXTURE_MIN_FILTER, &min_filter);
		TR_GL_CALL(glGetTextureParameteriv, m_handle, GL_TEXTURE_MAG_FILTER, &mag_filter);
		TR_GL_CALL(glGetTextureParameteriv, m_handle, GL_TEXTURE_WRAP_S, &wrap);
		TR_GL_CALL(glGetTextureParameterfv, m_handle, GL_TEXTURE_BORDER_COLOR, &border_color.r);
		TR_GL_CALL(glGetObjectLabel, GL_TEXTURE, m_handle, 128, &label_length, label_buffer);

		TR_GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &m_handle);
		TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_MIN_FILTER, min_filter);
		TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_MAG_FILTER, mag_filter);
		TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_WRAP_S, wrap);
		TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_WRAP_T, wrap);
		TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_WRAP_R, wrap);
		if (label_length > 0) {
			TR_GL_CALL(glObjectLabel, GL_TEXTURE, m_handle, label_length, label_buffer);
		}
	}
	else if (m_handle == 0) {
		TR_GL_CALL(glCreateTextures, GL_TEXTURE_2D, 1, &m_handle);
	}
	else {
		old_handle = 0;
		old_size = {};
	}

	TR_GL_CALL(glTextureStorage2D, m_handle, mipmapped ? floor_cast<GLsizei>(std::log2(std::max(size.x, size.y)) + 1) : 1,
			   tex_format(format), size.x, size.y);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"texture allocation"};
	}
	m_size = size;

	for (int i = 0; i < 80; ++i) {
		if (texture_unit_textures[i] == *this) {
			TR_GL_CALL(glBindTextures, i, 1, &m_handle);
		}
	}

	return texture{old_handle, old_size};
}

bool tr::gfx::texture::empty() const
{
	return m_handle == 0;
}

const glm::ivec2& tr::gfx::texture::size() const
{
	return m_size;
}

void tr::gfx::texture::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	TR_ASSERT(!empty(), "Tried to set filter on an empty texture.");

	TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_MIN_FILTER, GLint(min_filter));
	TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_MAG_FILTER, GLint(mag_filter));
}

void tr::gfx::texture::set_wrap(wrap wrap)
{
	TR_ASSERT(!empty(), "Tried to set wrap on an empty texture.");

	TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_WRAP_S, GLint(wrap));
	TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_WRAP_T, GLint(wrap));
	TR_GL_CALL(glTextureParameteri, m_handle, GL_TEXTURE_WRAP_R, GLint(wrap));
}

void tr::gfx::texture::set_border_color(rgbaf color)
{
	TR_ASSERT(!empty(), "Tried to set border color on an empty texture.");

	TR_GL_CALL(glTextureParameterfv, m_handle, GL_TEXTURE_BORDER_COLOR, &color.r);
}

void tr::gfx::texture::clear(const rgbaf& color)
{
	TR_ASSERT(!empty(), "Tried to clear an empty texture.");

	TR_GL_CALL(glClearTexImage, m_handle, 0, GL_RGBA, GL_FLOAT, &color);
}

void tr::gfx::texture::clear_region(const irect2& rect, const rgbaf& color)
{
	TR_ASSERT(!empty(), "Tried to clear a region of an empty texture.");

	TR_GL_CALL(glClearTexSubImage, m_handle, 0, rect.tl.x, rect.tl.y, 0, rect.size.x, rect.size.y, 1, GL_RGBA, GL_FLOAT, &color);
}

void tr::gfx::texture::copy_region(glm::ivec2 tl, const texture& src, const irect2& rect)
{
	TR_ASSERT(!empty(), "Tried to copy to an empty texture.");

	TR_GL_CALL(glCopyImageSubData, src.m_handle, GL_TEXTURE_2D, 0, rect.tl.x, rect.tl.y, 0, m_handle, GL_TEXTURE_2D, 0, tl.x, tl.y, 0,
			   rect.size.x, rect.size.y, 1);
}

void tr::gfx::texture::set_region(glm::ivec2 tl, const sub_bitmap& bitmap)
{
	TR_ASSERT(!empty(), "Tried to set a region of an empty texture.");

	TR_ASSERT(irect2{size()}.contains(tl + bitmap.size()),
			  "Tried to set out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size {}x{}.", tl.x, tl.y,
			  tl.x + bitmap.size().x, tl.y + bitmap.size().y, m_size.x, m_size.y);

	TR_GL_CALL(glPixelStorei, GL_UNPACK_ROW_LENGTH, bitmap.pitch() / pixel_bytes(bitmap.format()));
	TR_GL_CALL(glTextureSubImage2D, m_handle, 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, format(bitmap.format()),
			   type(bitmap.format()), bitmap.data());
	TR_GL_CALL(glGenerateTextureMipmap, m_handle);
}

void tr::gfx::texture::set_label(std::string_view label)
{
	if (!empty()) {
		TR_GL_CALL(glObjectLabel, GL_TEXTURE, m_handle, GLsizei(label.size()), label.data());
	}
}

/////////////////////////////////////////////////////////////// TEXTURE REF ///////////////////////////////////////////////////////////////

tr::gfx::texture_ref::texture_ref(const texture& tex)
	: m_ref{tex}
{
	tex.m_refs.emplace_back(*this);
}

tr::gfx::texture_ref::texture_ref(const texture_ref& r)
	: m_ref{r.m_ref}
{
	if (!empty()) {
		m_ref->m_refs.emplace_back(*this);
	}
}

tr::gfx::texture_ref::texture_ref(texture_ref&& r) noexcept
	: m_ref{r.m_ref}
{
	if (!empty()) {
		std::ranges::replace(m_ref->m_refs, tr::ref{r}, tr::ref{*this});
	}
	r.m_ref = std::nullopt;
}

tr::gfx::texture_ref::~texture_ref()
{
	if (!empty()) {
		m_ref->m_refs.erase(std::ranges::find(m_ref->m_refs, *this));
	}
}

tr::gfx::texture_ref& tr::gfx::texture_ref::operator=(const texture& tex)
{
	if (!empty()) {
		m_ref->m_refs.erase(std::ranges::find(m_ref->m_refs, *this));
	}
	m_ref = tex;
	tex.m_refs.emplace_back(*this);
	return *this;
}

tr::gfx::texture_ref& tr::gfx::texture_ref::operator=(const texture_ref& r)
{
	if (!empty()) {
		m_ref->m_refs.erase(std::ranges::find(m_ref->m_refs, *this));
	}
	m_ref = r.m_ref;
	if (!empty()) {
		m_ref->m_refs.emplace_back(*this);
	}
	return *this;
}

tr::gfx::texture_ref& tr::gfx::texture_ref::operator=(texture_ref&& r) noexcept
{
	if (!empty()) {
		m_ref->m_refs.erase(std::ranges::find(m_ref->m_refs, *this));
	}
	m_ref = r.m_ref;
	if (!empty()) {
		std::ranges::replace(m_ref->m_refs, tr::ref{r}, tr::ref{*this});
	}
	r.m_ref = std::nullopt;
	return *this;
}

bool tr::gfx::texture_ref::empty() const
{
	return !m_ref.has_ref();
}

///////////////////////////////////////////////////////////// RENDER TEXTURE //////////////////////////////////////////////////////////////

tr::gfx::render_texture::render_texture(glm::ivec2 size, bool mipmapped, pixel_format format)
	: texture{size, mipmapped, format}
{
	unsigned int temp;
	TR_GL_CALL(glCreateFramebuffers, 1, &temp);
	TR_GL_CALL(glNamedFramebufferTexture, temp, GL_COLOR_ATTACHMENT0, m_handle, 0);
	m_fbo.reset(temp);
}

tr::gfx::render_texture::render_texture(const sub_bitmap& bitmap, bool mipmapped, std::optional<pixel_format> format)
	: render_texture{bitmap.size(), mipmapped, format.value_or(bitmap.format())}
{
	set_region({}, bitmap);
}

tr::gfx::render_texture::~render_texture()
{
	if (!empty() && current_render_target.has_value() && current_render_target->m_fbo == m_fbo.get()) {
		current_render_target.reset();
	}
}

void tr::gfx::render_texture::fbo_deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteFramebuffers, 1, &id);
}

tr::gfx::texture tr::gfx::render_texture::reallocate(glm::ivec2 size, bool mipmapped, pixel_format format)
{
	texture old_data{texture::reallocate(size, mipmapped, format)};
	if (!m_fbo.has_value()) {
		unsigned int temp;
		TR_GL_CALL(glCreateFramebuffers, 1, &temp);
		m_fbo.reset(temp);
	}
	TR_GL_CALL(glNamedFramebufferTexture, m_fbo.get(), GL_COLOR_ATTACHMENT0, m_handle, 0);
	return old_data;
}

tr::gfx::render_texture::operator tr::gfx::render_target() const
{
	return render_target();
}

tr::gfx::render_target tr::gfx::render_texture::render_target() const
{
	TR_ASSERT(!empty(), "Tried to create a render target for an empty texture.");

	return gfx::render_target{m_fbo.get(), {{}, m_size}};
}

tr::gfx::render_target tr::gfx::render_texture::region_render_target(const irect2& rect) const
{
	TR_ASSERT(!empty(), "Tried to create a region render target for an empty texture.");
	TR_ASSERT(irect2{m_size}.contains(rect.tl + rect.size),
			  "Tried to create render target for out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size "
			  "%dx%d.",
			  rect.tl.x, rect.tl.y, rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, m_size.x, m_size.y);

	return gfx::render_target{m_fbo.get(), rect};
}