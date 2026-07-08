///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements window.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/window.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/utility/enum.hpp"
#include <SDL3/SDL.h>

/////////////////////////////////////////////////////////// WINDOW OPENING ERROR //////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////// WINDOW ERROR //////////////////////////////////////////////////////////////

tr::window_error::window_error(std::string&& description)
	: m_description{description}
	, m_details{SDL_GetError()}
{
}

std::string_view tr::window_error::name() const
{
	return "Window error";
}

std::string_view tr::window_error::description() const
{
	return m_description;
}

std::string_view tr::window_error::details() const
{
	return m_details;
}

/////////////////////////////////////////////////////////////// WINDOW VIEW ///////////////////////////////////////////////////////////////

//

tr::zstring_view tr::window_view::title() const
{
	return SDL_GetWindowTitle(m_ptr);
}

void tr::window_view::set_title(zstring_view new_title) const
{
	if (!SDL_SetWindowTitle(m_ptr, new_title.c_str())) {
		throw window_error{"Failed to rename window from '{}' to '{}'", title(), new_title};
	}
}

//

void tr::window_view::set_icon(const bitmap& bitmap) const
{
	if (!SDL_SetWindowIcon(m_ptr, bitmap.m_ptr.get())) {
		throw window_error{"Failed to set icon of window '{}'", title()};
	}
}

void tr::window_view::set_icon(const bitmap_view& view) const
{
	if (!SDL_SetWindowIcon(m_ptr, view.m_ptr.get())) {
		throw window_error{"Failed to set icon of window '{}'", title()};
	}
}

//

glm::ivec2 tr::window_view::size() const
{
	glm::ivec2 size{};
	return SDL_GetWindowSizeInPixels(m_ptr, &size.x, &size.y) ? size : throw window_error{"Failed to get size of window '{}'", title()};
}

float tr::window_view::pixel_density() const
{
	const float density{SDL_GetWindowPixelDensity(m_ptr)};
	return density != 0.0f ? density : throw window_error{"Failed to get pixel density of window '{}'", title()};
}

void tr::window_view::set_size(glm::ivec2 size) const
{
	const glm::ivec2 real_size{glm::vec2{size} / pixel_density()};
	if (!SDL_SetWindowSize(m_ptr, real_size.x, real_size.y) ||
		!SDL_SetWindowPosition(m_ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED)) {
		throw window_error{"Failed to set size of window '{}'", title()};
	}
}

//

bool tr::window_view::fullscreen() const
{
	return SDL_GetWindowFlags(m_ptr) & SDL_WINDOW_FULLSCREEN;
}

void tr::window_view::set_fullscreen(bool fullscreen) const
{
	if (!SDL_SetWindowFullscreen(m_ptr, fullscreen)) {
		throw window_error{"Failed to set window '{}' {} fullscreen", title(), fullscreen ? "to" : "from"};
	}
}

//

void tr::window_view::show() const
{
	if (!SDL_ShowWindow(m_ptr)) {
		throw window_error{"Failed to show window '{}'.", title()};
	}
}

void tr::window_view::hide() const
{
	if (!SDL_ShowWindow(m_ptr)) {
		throw window_error{"Failed to hide window '{}'.", title()};
	}
}

//

bool tr::window_view::maximized() const
{
	return SDL_GetWindowFlags(m_ptr) & SDL_WINDOW_MAXIMIZED;
}

bool tr::window_view::minimized() const
{
	return SDL_GetWindowFlags(m_ptr) & SDL_WINDOW_MINIMIZED;
}

bool tr::window_view::has_focus() const
{
	return SDL_GetWindowFlags(m_ptr) & SDL_WINDOW_INPUT_FOCUS;
}

void tr::window_view::raise() const
{
	if (!SDL_RaiseWindow(m_ptr)) {
		throw window_error{"Failed to raise window '{}'.", title()};
	}
}

//

void tr::window_view::enable_text_input() const
{
	SDL_StartTextInput(m_ptr);
}

void tr::window_view::disable_text_input() const
{
	SDL_StopTextInput(m_ptr);
}

//

void tr::window_view::set_vsync(vsync vsync) const
{
	if (!SDL_GL_SetSwapInterval(to_underlying(vsync))) {
		if (vsync == vsync::adaptive) {
			set_vsync(vsync::enabled);
		}
		else {
			throw window_error{"Failed to set V-sync for window '{}'.", title()};
		}
	}
}

//

void tr::window_view::flip_backbuffer() const
{
	SDL_GL_SwapWindow(m_ptr);
}

////////////////////////////////////////////////////////////////// WINDOW /////////////////////////////////////////////////////////////////

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

void tr::window::deleter::operator()(SDL_Window* window) const
{
	SDL_DestroyWindow(window);
}

//

tr::zstring_view tr::window::title() const
{
	return SDL_GetWindowTitle(m_ptr.get());
}

void tr::window::set_title(zstring_view title)
{
	window_view{*this}.set_title(title);
}

//

void tr::window::set_icon(const bitmap& bitmap)
{
	window_view{*this}.set_icon(bitmap);
}

void tr::window::set_icon(const bitmap_view& view)
{
	window_view{*this}.set_icon(view);
}

//

glm::ivec2 tr::window::size() const
{
	glm::ivec2 size{};
	return SDL_GetWindowSizeInPixels(m_ptr.get(), &size.x, &size.y) ? size
																	: throw window_error{"Failed to get size of window '{}'", title()};
}

float tr::window::pixel_density() const
{
	const float density{SDL_GetWindowPixelDensity(m_ptr.get())};
	return density != 0.0f ? density : throw window_error{"Failed to get pixel density of window '{}'", title()};
}

void tr::window::set_size(glm::ivec2 size)
{
	window_view{*this}.set_size(size);
}

//

bool tr::window::fullscreen() const
{
	return SDL_GetWindowFlags(m_ptr.get()) & SDL_WINDOW_FULLSCREEN;
}

void tr::window::set_fullscreen(bool fullscreen)
{
	window_view{*this}.set_fullscreen(fullscreen);
}

//

void tr::window::show()
{
	window_view{*this}.show();
}

void tr::window::hide()
{
	window_view{*this}.hide();
}

//

bool tr::window::maximized() const
{
	return SDL_GetWindowFlags(m_ptr.get()) & SDL_WINDOW_MAXIMIZED;
}

bool tr::window::minimized() const
{
	return SDL_GetWindowFlags(m_ptr.get()) & SDL_WINDOW_MINIMIZED;
}

bool tr::window::has_focus() const
{
	return SDL_GetWindowFlags(m_ptr.get()) & SDL_WINDOW_INPUT_FOCUS;
}

void tr::window::raise()
{
	window_view{*this}.raise();
}

//

void tr::window::enable_text_input()
{
	SDL_StartTextInput(m_ptr.get());
}

void tr::window::disable_text_input()
{
	SDL_StopTextInput(m_ptr.get());
}

//

void tr::window::set_vsync(vsync vsync)
{
	window_view{*this}.set_vsync(vsync);
}

//

void tr::window::set_mouse_mode(mouse_mode mode)
{
	if (!SDL_SetWindowRelativeMouseMode(m_ptr.get(), to_underlying(mode))) {
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
	SDL_GL_SwapWindow(m_ptr.get());
}