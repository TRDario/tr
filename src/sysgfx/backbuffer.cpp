#include "../../include/tr/sysgfx/backbuffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include <SDL3/SDL.h>

glm::ivec2 tr::backbuffer::size() noexcept
{
	TR_ASSERT(_window != nullptr, "Tried to get the size of the backbuffer before opening the window.");

	glm::ivec2 size;
	SDL_GetWindowSizeInPixels(_window, &size.x, &size.y);
	return size;
}

tr::render_target tr::backbuffer::render_target() noexcept
{
	return tr::render_target{0, {{}, size()}};
}

tr::render_target tr::backbuffer::region_render_target(const irect2& rect) noexcept
{
	TR_ASSERT(irect2{size()}.contains(rect.tl + rect.size),
			  "Tried to create render target for out-of-bounds region from ({}, {}) to ({}, {}) in a backbuffer with size {}x{}.",
			  rect.tl.x, rect.tl.y, rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, size().x, size().y);

	return tr::render_target{0, rect};
}

void tr::backbuffer::clear(const tr::rgbaf& color) noexcept
{
	gfx_context::set_render_target(render_target());
	TR_GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
	TR_GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
}

void tr::backbuffer::clear(const tr::rgbaf& color, double depth, int stencil) noexcept
{
	gfx_context::set_render_target(render_target());
	TR_GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
	TR_GL_CALL(glClearDepth, depth);
	TR_GL_CALL(glClearStencil, stencil);
	TR_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::backbuffer::clear_region(const tr::irect2& rect, const tr::rgbaf& color) noexcept
{
	gfx_context::set_render_target(region_render_target(rect));
	TR_GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
	TR_GL_CALL(glClear, GL_COLOR_BUFFER_BIT);
}

void tr::backbuffer::clear_region(const tr::irect2& rect, const tr::rgbaf& color, double depth, int stencil) noexcept
{
	gfx_context::set_render_target(region_render_target(rect));
	TR_GL_CALL(glClearColor, color.r, color.g, color.b, color.a);
	TR_GL_CALL(glClearDepth, depth);
	TR_GL_CALL(glClearStencil, stencil);
	TR_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::backbuffer::flip() noexcept
{
	SDL_GL_SwapWindow(_window);
}