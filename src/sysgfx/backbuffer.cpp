///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements backbuffer.hpp.                                                                                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/backbuffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

//////////////////////////////////////////////////////////////// BACKBUFFER ///////////////////////////////////////////////////////////////

glm::ivec2 tr::gfx::backbuffer_size()
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to get the size of the backbuffer before opening the window.");

	glm::ivec2 size;
	SDL_GetWindowSizeInPixels(sys::g_window.ptr(), &size.x, &size.y);
	return size;
}

tr::gfx::render_target tr::gfx::backbuffer_render_target()
{
	const glm::ivec2 size{backbuffer_size()};
	return render_target{0, {{}, size}, {{}, size}};
}

void tr::gfx::clear_backbuffer(const tr::rgbaf& color)
{
	set_render_target(backbuffer_render_target());
	TR_GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
	TR_GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
}

void tr::gfx::clear_backbuffer(const tr::rgbaf& color, double depth, int stencil)
{
	set_render_target(backbuffer_render_target());
	TR_GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
	TR_GL_CALL(glClearDepth, depth);
	TR_GL_CALL(glClearStencil, stencil);
	TR_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::gfx::clear_backbuffer_region(const tr::irect2& rect, const tr::rgbaf& color)
{
	set_render_target(backbuffer_render_target().cropped(rect));
	TR_GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
	TR_GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
}

void tr::gfx::clear_backbuffer_region(const tr::irect2& rect, const tr::rgbaf& color, double depth, int stencil)
{
	set_render_target(backbuffer_render_target().cropped(rect));
	TR_GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
	TR_GL_CALL(glClearDepth, depth);
	TR_GL_CALL(glClearStencil, stencil);
	TR_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::gfx::flip_backbuffer()
{
	SDL_GL_SwapWindow(sys::g_window.ptr());
}