/// @file
/// @brief Implements window.hpp.

#include "../../include/tr/sysgfx/window.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/window_view.hpp"
#include <SDL3/SDL.h>

//

tr::window_open_error::window_open_error()
	: m_description{SDL_GetError()}
{
}

std::string_view tr::window_open_error::name() const
{
	return "Window opening error";
}

std::string_view tr::window_open_error::description() const
{
	return m_description;
}

std::string_view tr::window_open_error::details() const
{
	return {};
}

//

tr::window::window(zstring_view title, window_parameters parameters)
{
	SDL_WindowFlags window_flags{SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY};
	if (parameters.fullscreen) {
		window_flags |= SDL_WINDOW_FULLSCREEN;
	}
	if (parameters.size == tr::maximized) {
		window_flags |= SDL_WINDOW_MAXIMIZED;
		parameters.size = parameters.min_size != not_resizable ? parameters.min_size : glm::ivec2{640, 480};
	}
	if (parameters.min_size != not_resizable) {
		window_flags |= SDL_WINDOW_RESIZABLE;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
						SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | (parameters.debug_graphics_context * SDL_GL_CONTEXT_DEBUG_FLAG));
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, parameters.enable_depth_stencil ? 24 : 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, parameters.enable_depth_stencil ? 8 : 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, parameters.multisamples != 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, parameters.multisamples);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

	m_ptr.reset(SDL_CreateWindow(title.c_str(), parameters.size.x, parameters.size.y, window_flags));
	if (m_ptr == nullptr) {
		throw window_open_error{};
	}
	if (parameters.min_size != not_resizable) {
		SDL_SetWindowMinimumSize(m_ptr.get(), parameters.min_size.x, parameters.min_size.y);
	}
}

void tr::window::deleter::operator()(SDL_Window* window)
{
	SDL_DestroyWindow(window);
}

//

tr::window::operator window_view()
{
	return view();
}

tr::window_view tr::window::view()
{
	return window_view{m_ptr.get()};
}

tr::window_view tr::window::view() const
{
	return window_view{m_ptr.get()};
}

//

tr::zstring_view tr::window::title() const
{
	return view().title();
}

void tr::window::set_title(zstring_view title)
{
	view().set_title(title);
}

//

void tr::window::set_icon(const bitmap& bitmap)
{
	view().set_icon(bitmap);
}

void tr::window::set_icon(const bitmap_view& bitmap)
{
	view().set_icon(bitmap);
}

//

glm::ivec2 tr::window::size() const
{
	return view().size();
}

float tr::window::pixel_density() const
{
	return view().pixel_density();
}

void tr::window::set_size(glm::ivec2 size)
{
	view().set_size(size);
}

//

bool tr::window::fullscreen() const
{
	return view().fullscreen();
}

void tr::window::set_fullscreen(bool fullscreen)
{
	view().set_fullscreen(fullscreen);
}

//

void tr::window::show()
{
	view().show();
}

void tr::window::hide()
{
	view().hide();
}

//

bool tr::window::maximized() const
{
	return view().maximized();
}

bool tr::window::minimized() const
{
	return view().minimized();
}

bool tr::window::has_focus() const
{
	return view().has_focus();
}

void tr::window::raise()
{
	view().raise();
}

//

void tr::window::enable_text_input()
{
	view().enable_text_input();
}

void tr::window::disable_text_input()
{
	view().disable_text_input();
}

//

void tr::window::set_vsync(vsync vsync)
{
	view().set_vsync(vsync);
}

//

void tr::window::set_mouse_mode(mouse_mode mode)
{
	if (!SDL_SetWindowRelativeMouseMode(m_ptr.get(), std::to_underlying(mode))) {
		throw window_error("Failed to set mouse mode on window '{}'.", title());
	}

	// Workaround for an SDL bug where the cursor does not properly disappear sometimes in relative mode on Windows.
#ifdef _WIN32
	switch (mode) {
	case mouse_mode::absolute:
		m_cursor_reset_timer.reset();
		break;
	case mouse_mode::relative:
		if (!m_cursor_reset_timer.has_value()) {
			m_cursor_reset_timer.emplace(std::chrono::seconds{1}, [] {
				float x, y;
				SDL_GetMouseState(&x, &y);
				SDL_SetWindowRelativeMouseMode(m_ptr.get(), false);
				const bool cursor_visible{SDL_CursorVisible()};
				if (cursor_visible) {
					SDL_HideCursor();
				}
				SDL_WarpMouseInWindow(m_ptr.get(), x, y);
				if (cursor_visible) {
					SDL_ShowCursor();
				}
				SDL_SetWindowRelativeMouseMode(m_ptr.get(), true);
			});
		}
		break;
	}
#endif
}

//

void tr::window::flip_backbuffer()
{
	view().flip_backbuffer();
}