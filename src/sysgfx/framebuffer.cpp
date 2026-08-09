/// @file
/// @brief Implements framebuffer.hpp.

#include "../../include/tr/sysgfx/framebuffer.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/render_target.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"

//

void tr::framebuffer::create_handle()
{
	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.create_framebuffers(1, out_handle(m_handle));
}

tr::framebuffer::framebuffer(graphics_context& context)
	: m_handle{{context}}
{
	create_handle();
}

void tr::framebuffer::deleter::operator()(unsigned int fbo) const
{
	const gl_api& gl{context.make_current_and_return_gl_api()};
	gl.delete_framebuffers(1, &fbo);
}

//

tr::graphics_context& tr::framebuffer::context() const
{
	return m_handle.get_deleter().context;
}

//

tr::render_target tr::framebuffer::render_target(glm::ivec2 size) const
{
	return tr::render_target{m_handle.get(), size};
}

//

void tr::framebuffer::attach(attachment attachment, texture_view texture)
{
	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.set_framebuffer_texture(m_handle.get(), std::to_underlying(attachment), texture.m_id, 0);
}

void tr::framebuffer::detach(attachment attachment)
{
	const gl_api& gl{context().make_current_and_return_gl_api()};
	gl.set_framebuffer_texture(m_handle.get(), std::to_underlying(attachment), 0, 0);
}