#include <SDL3/SDL.h>
#include <tr/sysgfx/bitmap.hpp>
#include <tr/sysgfx/bitmap_view.hpp>
#include <tr/sysgfx/window_view.hpp>

//

tr::window_error::window_error(std::string&& description) noexcept
	: m_description{description}
	, m_details{SDL_GetError()}
{
}

std::string_view tr::window_error::name() const noexcept
{
	return "Window error";
}

std::string_view tr::window_error::description() const noexcept
{
	return m_description;
}

std::string_view tr::window_error::details() const noexcept
{
	return m_details;
}

//

tr::window_view::window_view(SDL_Window* ptr) noexcept
	: m_ptr{ptr}
{
	TR_ASSERT(ptr != nullptr, "Tried to create a view to a window in an invalid state.");
}

//

tr::zstring_view tr::window_view::title() const noexcept
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
	if (!SDL_SetWindowIcon(m_ptr, bitmap.unwrap())) {
		throw window_error{"Failed to set icon of window '{}'", title()};
	}
}

void tr::window_view::set_icon(const bitmap_view& bitmap) const
{
	if (!SDL_SetWindowIcon(m_ptr, bitmap.unwrap())) {
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

bool tr::window_view::fullscreen() const noexcept
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

bool tr::window_view::maximized() const noexcept
{
	return SDL_GetWindowFlags(m_ptr) & SDL_WINDOW_MAXIMIZED;
}

bool tr::window_view::minimized() const noexcept
{
	return SDL_GetWindowFlags(m_ptr) & SDL_WINDOW_MINIMIZED;
}

bool tr::window_view::has_focus() const noexcept
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

void tr::window_view::set_vsync(vsync vsync) const
{
	if (!SDL_GL_SetSwapInterval(std::to_underlying(vsync))) {
		if (vsync == vsync::adaptive) {
			set_vsync(vsync::enabled);
		}
		else {
			throw window_error{"Failed to set V-sync for window '{}'.", title()};
		}
	}
}

//

void tr::window_view::enable_text_input() const noexcept
{
	SDL_StartTextInput(m_ptr);
}

void tr::window_view::disable_text_input() const noexcept
{
	SDL_StopTextInput(m_ptr);
}

//

void tr::window_view::flip_backbuffer() const noexcept
{
	SDL_GL_SwapWindow(m_ptr);
}

//

SDL_Window* tr::window_view::unwrap() const noexcept
{
	return m_ptr;
}