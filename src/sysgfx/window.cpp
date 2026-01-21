///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements window.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/window.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/display.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/main.hpp"
#include <SDL3/SDL.h>

////////////////////////////////////////////////////////////////// WINDOW /////////////////////////////////////////////////////////////////

void tr::sys::open_window(cstring_view title, glm::ivec2 size, glm::ivec2 min_size, const gfx::properties& gfx_properties)
{
	TR_ASSERT(!g_window.is_open(), "Tried to reopen window without closing it first.");

	SDL_WindowFlags flags{SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY};
	if (size == maximized) {
		flags |= SDL_WINDOW_MAXIMIZED;
		size = min_size != not_resizable ? min_size : glm::ivec2{640, 480};
	}
	if (min_size != not_resizable) {
		flags |= SDL_WINDOW_RESIZABLE;
	}

	if (!g_window.open(title, size, flags, min_size, gfx_properties)) {
		throw init_error{"Failed to open window."};
	}
}

void tr::sys::open_fullscreen_window(cstring_view title, glm::ivec2 min_size, const gfx::properties& gfx_properties)
{
	TR_ASSERT(!g_window.is_open(), "Tried to reopen window without closing it first.");

	SDL_WindowFlags flags{SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_FULLSCREEN};
	const glm::ivec2 size{min_size != not_resizable ? min_size : display_size()};

	if (!g_window.open(title, size, flags, min_size, gfx_properties)) {
		throw init_error{"Failed to open fullscreen window."};
	}
}

void tr::sys::close_window()
{
	g_window.close();
}

tr::cstring_view tr::sys::window_title()
{
	TR_ASSERT(g_window.is_open(), "Tried to get window title before opening it.");

	return SDL_GetWindowTitle(g_window.ptr());
}

void tr::sys::set_window_title(cstring_view title)
{
	TR_ASSERT(g_window.is_open(), "Tried to set window title before opening it.");

	if (!SDL_SetWindowTitle(g_window.ptr(), title)) {
		TR_LOG_SDL_ERROR("Failed to set window title to '{}'.", title);
	};
}

void tr::sys::set_window_icon(const bitmap& bitmap)
{
	TR_ASSERT(g_window.is_open(), "Tried to set window icon before opening it.");

	if (!SDL_SetWindowIcon(g_window.ptr(), bitmap.m_ptr.get())) {
		TR_LOG_SDL_ERROR("Failed to set window icon.");
	}
}

void tr::sys::set_window_icon(const bitmap_view& view)
{
	TR_ASSERT(g_window.is_open(), "Tried to set window icon before opening it.");

	if (!SDL_SetWindowIcon(g_window.ptr(), view.m_ptr.get())) {
		TR_LOG_SDL_ERROR("Failed to set window icon.");
	}
}

glm::ivec2 tr::sys::window_size()
{
	TR_ASSERT(g_window.is_open(), "Tried to get window size before opening it.");

	glm::ivec2 size{};
	if (!SDL_GetWindowSizeInPixels(g_window.ptr(), &size.x, &size.y)) {
		TR_LOG_SDL_ERROR("Failed to get window size.");
	}
	return size;
}

float tr::sys::window_pixel_density()
{
	TR_ASSERT(g_window.is_open(), "Tried to get window pixel density before opening it.");

	const float density{SDL_GetWindowPixelDensity(g_window.ptr())};
	if (density == 0.0f) {
		TR_LOG_SDL_ERROR("Failed to get window pixel density.");
		return 1.0f;
	}
	return density;
}

void tr::sys::set_window_size(glm::ivec2 size)
{
	TR_ASSERT(g_window.is_open(), "Tried to set window size before opening it.");
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to set window size to an invalid value of {}x{}.", size.x, size.y);

	const glm::ivec2 real_size{glm::vec2{size} / window_pixel_density()};
	if (!SDL_SetWindowSize(g_window.ptr(), real_size.x, real_size.y) ||
		!SDL_SetWindowPosition(g_window.ptr(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED)) {
		TR_LOG_SDL_ERROR("Failed to set window size.");
	}
}

bool tr::sys::window_fullscreen()
{
	TR_ASSERT(g_window.is_open(), "Tried to get window fullscreen status before opening it.");

	return SDL_GetWindowFlags(g_window.ptr()) & SDL_WINDOW_FULLSCREEN;
}

void tr::sys::set_window_fullscreen(bool fullscreen)
{
	TR_ASSERT(g_window.is_open(), "Tried to set window to fullscreen before opening it.");

	if (!SDL_SetWindowFullscreen(g_window.ptr(), fullscreen)) {
		TR_LOG_SDL_ERROR("Failed to set window {} fullscreen.", fullscreen ? "to" : "from");
	}
}

void tr::sys::show_window()
{
	TR_ASSERT(g_window.is_open(), "Tried to show window before opening it.");

	if (!SDL_ShowWindow(g_window.ptr())) {
		TR_LOG_SDL_ERROR("Failed to show window.");
	}
}

void tr::sys::hide_window()
{
	TR_ASSERT(g_window.is_open(), "Tried to hide window before opening it.");

	if (!SDL_HideWindow(g_window.ptr())) {
		TR_LOG_SDL_ERROR("Failed to hide window.");
	}
}

bool tr::sys::window_has_focus()
{
	return SDL_GetWindowFlags(g_window.ptr()) & SDL_WINDOW_INPUT_FOCUS;
}

bool tr::sys::window_maximized()
{
	return SDL_GetWindowFlags(g_window.ptr()) & SDL_WINDOW_MAXIMIZED;
}

bool tr::sys::window_minimized()
{
	return SDL_GetWindowFlags(g_window.ptr()) & SDL_WINDOW_MINIMIZED;
}

void tr::sys::raise_window()
{
	TR_ASSERT(g_window.is_open(), "Tried to raise window before opening it.");

	if (!SDL_RaiseWindow(g_window.ptr())) {
		TR_LOG_SDL_ERROR("Failed to raise window.");
	}
}

void tr::sys::set_window_vsync(vsync vsync)
{
	TR_ASSERT(g_window.is_open(), "Tried to set window V-sync before opening it.");

	if (!SDL_GL_SetSwapInterval(int(vsync))) {
		if (vsync == vsync::adaptive) {
			TR_LOG(log, tr::severity::warning, "Failed to set V-sync to adaptive, falling back to regular.");
			TR_LOG_CONTINUE(log, "{}", SDL_GetError());
			set_window_vsync(vsync::enabled);
		}
		else {
			TR_LOG_SDL_ERROR("Failed to set V-sync.");
		}
	}
}