#include "../../include/tr/sysgfx/render_texture.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

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
	gl.set_framebuffer_texture(m_fbo, GL_COLOR_ATTACHMENT0, m_handle, 0);
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

//

tr::render_texture& tr::render_texture::operator=(render_texture&& r) noexcept
{
	static_cast<texture&>(*this) = std::move(r);
	m_fbo = std::exchange(r.m_fbo, 0);
	return *this;
}

//

tr::render_texture::operator texture_ref() const
{
	return static_cast<const texture&>(*this);
}

//

tr::render_texture::operator tr::render_target() const
{
	return render_target();
}

tr::render_target tr::render_texture::render_target() const
{
	TR_ASSERT(!empty(), "Tried to create a render target for an empty texture.");

	return tr::render_target{m_fbo, m_size};
}

//

tr::texture tr::render_texture::reallocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	const graphics_context::functions& gl{m_context.make_current_and_return_functions()};

	texture old_data{texture::reallocate(size, mipmaps, format)};
	if (m_fbo == 0) {
		gl.create_framebuffers(1, &m_fbo);
	}
	gl.set_framebuffer_texture(m_fbo, GL_COLOR_ATTACHMENT0, m_handle, 0);
	return old_data;
}