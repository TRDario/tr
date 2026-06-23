///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements texture.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

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
	, m_refs{std::move(r.m_refs)}
{
	for (texture_ref& ref : m_refs) {
		ref.m_ref = *this;
	}
}

tr::texture::~texture()
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.delete_textures(1, &m_handle);
	for (texture_ref& ref : m_refs) {
		ref.m_ref = std::nullopt;
	}
}

tr::texture& tr::texture::operator=(texture&& r) noexcept
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.delete_textures(1, &m_handle);
	for (texture_ref& ref : m_refs) {
		ref.m_ref = std::nullopt;
	}

	m_handle = std::exchange(r.m_handle, 0);
	m_size = r.m_size;
	m_refs = std::move(r.m_refs);
	return *this;
}

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
		gl.texture_parameter_i(new_handle, GL_TEXTURE_MIN_FILTER, min_filter);

		int mag_filter;
		gl.get_texture_parameter_iv(m_handle, GL_TEXTURE_MAG_FILTER, &mag_filter);
		gl.texture_parameter_i(new_handle, GL_TEXTURE_MAG_FILTER, mag_filter);

		int wrap;
		gl.get_texture_parameter_iv(m_handle, GL_TEXTURE_WRAP_S, &wrap);
		gl.texture_parameter_i(new_handle, GL_TEXTURE_WRAP_S, wrap);
		gl.texture_parameter_i(new_handle, GL_TEXTURE_WRAP_T, wrap);
		gl.texture_parameter_i(new_handle, GL_TEXTURE_WRAP_R, wrap);

		rgbaf border_color;
		gl.get_texture_parameter_fv(m_handle, GL_TEXTURE_BORDER_COLOR, &border_color.r);
		gl.texture_parameter_fv(new_handle, GL_TEXTURE_BORDER_COLOR, &border_color.r);

#ifdef TR_ENABLE_ASSERTS
		m_context.move_label(GL_TEXTURE, m_handle, new_handle);
#endif
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
	gl.texture_storage_2d(m_handle, levels, gl_tex_format(format), size.x, size.y);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"texture allocation"};
	}
	m_size = size;

	m_context.rebind_texture_units(*this);

	return texture{m_context, old_handle, old_size};
}

bool tr::texture::empty() const
{
	return m_handle == 0;
}

glm::ivec2 tr::texture::size() const
{
	return m_size;
}

void tr::texture::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	TR_ASSERT(!empty(), "Tried to set filter on an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.texture_parameter_i(m_handle, GL_TEXTURE_MIN_FILTER, int(min_filter));
	gl.texture_parameter_i(m_handle, GL_TEXTURE_MAG_FILTER, int(mag_filter));
}

void tr::texture::set_wrap(wrap wrap)
{
	TR_ASSERT(!empty(), "Tried to set wrap on an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.texture_parameter_i(m_handle, GL_TEXTURE_WRAP_S, int(wrap));
	gl.texture_parameter_i(m_handle, GL_TEXTURE_WRAP_T, int(wrap));
	gl.texture_parameter_i(m_handle, GL_TEXTURE_WRAP_R, int(wrap));
}

void tr::texture::set_border_color(rgbaf color)
{
	TR_ASSERT(!empty(), "Tried to set border color on an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.texture_parameter_fv(m_handle, GL_TEXTURE_BORDER_COLOR, &color.r);
}

void tr::texture::clear(const rgbaf& color)
{
	TR_ASSERT(!empty(), "Tried to clear an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.clear_tex_image(m_handle, 0, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::clear_region(const irect2& rect, const rgbaf& color)
{
	TR_ASSERT(!empty(), "Tried to clear a region of an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.clear_tex_sub_image(m_handle, 0, rect.tl.x, rect.tl.y, 0, rect.size.x, rect.size.y, 1, GL_RGBA, GL_FLOAT, &color);
}

void tr::texture::copy_region(glm::ivec2 tl, const texture& src, const irect2& rect)
{
	TR_ASSERT(!empty(), "Tried to copy to an empty texture.");

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.copy_image_sub_data(src.m_handle, GL_TEXTURE_2D, 0, rect.tl.x, rect.tl.y, 0, m_handle, GL_TEXTURE_2D, 0, tl.x, tl.y, 0, rect.size.x,
						   rect.size.y, 1);
}

void tr::texture::set_region(glm::ivec2 tl, const sub_bitmap& bitmap)
{
	TR_ASSERT(!empty(), "Tried to set a region of an empty texture.");

	TR_ASSERT(irect2{size()}.contains(tl + bitmap.size()),
			  "Tried to set out-of-bounds region from ({}, {}) to ({}, {}) in a texture with size {}x{}.", tl.x, tl.y,
			  tl.x + bitmap.size().x, tl.y + bitmap.size().y, m_size.x, m_size.y);

	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.pixel_store_i(GL_UNPACK_ALIGNMENT, 1);
	gl.pixel_store_i(GL_UNPACK_ROW_LENGTH, bitmap.pitch() / pixel_bytes(bitmap.format()));
	gl.texture_sub_image_2d(m_handle, 0, tl.x, tl.y, bitmap.size().x, bitmap.size().y, gl_format(bitmap.format()), gl_type(bitmap.format()),
							bitmap.data());
	gl.generate_texture_mipmap(m_handle);
}

#ifdef TR_ENABLE_ASSERTS
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
		gl.object_label(GL_TEXTURE, m_handle, label.size(), label.data());
	}
}
#endif

/////////////////////////////////////////////////////////////// TEXTURE REF ///////////////////////////////////////////////////////////////

tr::texture_ref::texture_ref(const texture& tex)
	: m_ref{tex}
{
	tex.m_refs.emplace_back(*this);
}

tr::texture_ref::texture_ref(const texture_ref& r)
	: m_ref{r.m_ref}
{
	if (!empty()) {
		m_ref->m_refs.emplace_back(*this);
	}
}

tr::texture_ref::texture_ref(texture_ref&& r) noexcept
	: m_ref{std::exchange(r.m_ref, std::nullopt)}
{
	if (!empty()) {
		std::ranges::replace(m_ref->m_refs, tr::ref{r}, tr::ref{*this});
	}
}

tr::texture_ref::~texture_ref()
{
	if (!empty()) {
		unstable_erase(m_ref->m_refs, std::ranges::find(m_ref->m_refs, *this));
	}
}

tr::texture_ref& tr::texture_ref::operator=(const texture& tex)
{
	if (!empty()) {
		unstable_erase(m_ref->m_refs, std::ranges::find(m_ref->m_refs, *this));
	}
	m_ref = tex;
	tex.m_refs.emplace_back(*this);
	return *this;
}

tr::texture_ref& tr::texture_ref::operator=(const texture_ref& r)
{
	if (!empty()) {
		unstable_erase(m_ref->m_refs, std::ranges::find(m_ref->m_refs, *this));
	}
	m_ref = r.m_ref;
	if (!empty()) {
		m_ref->m_refs.emplace_back(*this);
	}
	return *this;
}

tr::texture_ref& tr::texture_ref::operator=(texture_ref&& r) noexcept
{
	if (!empty()) {
		unstable_erase(m_ref->m_refs, std::ranges::find(m_ref->m_refs, *this));
	}
	m_ref = std::exchange(r.m_ref, std::nullopt);
	if (!empty()) {
		std::ranges::replace(m_ref->m_refs, tr::ref{r}, tr::ref{*this});
	}
	return *this;
}

bool tr::texture_ref::empty() const
{
	return !m_ref.has_ref();
}

glm::ivec2 tr::texture_ref::size() const
{
	TR_ASSERT(!empty(), "Tried to get size of empty texture reference.");

	return m_ref->size();
}

///////////////////////////////////////////////////////////// RENDER TEXTURE //////////////////////////////////////////////////////////////

tr::render_texture::render_texture(graphics_context& context)
	: texture{context}
{
}

tr::render_texture::render_texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps, pixel_format format)
	: texture{context, size, mipmaps, format}
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	gl.create_framebuffers(1, &m_fbo);
	gl.named_framebuffer_texture(m_fbo, GL_COLOR_ATTACHMENT0, m_handle, 0);
}

tr::render_texture::render_texture(graphics_context& context, const sub_bitmap& bitmap, mipmaps mipmaps, std::optional<pixel_format> format)
	: render_texture{context, bitmap.size(), mipmaps, format.value_or(bitmap.format())}
{
	set_region({}, bitmap);
}

tr::render_texture::render_texture(render_texture&& r) noexcept
	: texture{std::move(r)}
	, m_fbo{r.m_fbo}
{
	r.m_fbo = 0;
}

tr::render_texture::~render_texture()
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	if (!empty() && m_context.is_fbo_of_render_target(m_fbo)) {
		m_context.clear_render_target();
	}
	gl.delete_framebuffers(1, &m_fbo);
}

tr::render_texture& tr::render_texture::operator=(render_texture&& r) noexcept
{
	*(texture*)this = std::move(r);
	m_fbo = std::exchange(r.m_fbo, 0);
	return *this;
}

tr::texture tr::render_texture::reallocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	texture old_data{texture::reallocate(size, mipmaps, format)};
	if (m_fbo == 0) {
		gl.create_framebuffers(1, &m_fbo);
	}
	gl.named_framebuffer_texture(m_fbo, GL_COLOR_ATTACHMENT0, m_handle, 0);
	return old_data;
}

tr::render_texture::operator tr::render_target() const
{
	return render_target();
}

tr::render_target tr::render_texture::render_target() const
{
	TR_ASSERT(!empty(), "Tried to create a render target for an empty texture.");

	return tr::render_target{m_fbo, m_size};
}