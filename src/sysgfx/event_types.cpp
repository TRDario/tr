/// @file
/// @brief Implements event_types.hpp.

#include "internal/convert_sdl_keymods.hpp"
#include <SDL3/SDL.h>
#include <tr/sysgfx/event_types.hpp>
#include <tr/sysgfx/event_view.hpp>

//

tr::key_down_event::key_down_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().key.windowID)}
{
	const SDL_KeyboardEvent& sdl{event.sdl_event().key};
	repeat = sdl.repeat;
	scan = static_cast<scancode>(sdl.scancode);
	key = static_cast<keycode>(sdl.key);
	mods = internal::convert_sdl_keymods(sdl.mod);
}

tr::scan_chord tr::key_down_event::pressed_scan_chord() const noexcept
{
	return tr::scan_chord{mods, scan};
}

tr::key_chord tr::key_down_event::pressed_key_chord() const noexcept
{
	return tr::key_chord{mods, key};
}

//

tr::key_up_event::key_up_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().key.windowID)}
{
	const SDL_KeyboardEvent& sdl{event.sdl_event().key};
	scan = static_cast<scancode>(sdl.scancode);
	key = static_cast<keycode>(sdl.key);
	mods = internal::convert_sdl_keymods(sdl.mod);
}

//

tr::text_input_event::text_input_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().text.windowID)}
{
	const SDL_TextInputEvent& sdl{event.sdl_event().text};
	text = sdl.text;
}

//

tr::mouse_motion_event::mouse_motion_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().motion.windowID)}
{
	const SDL_MouseMotionEvent& sdl{event.sdl_event().motion};
	const glm::vec2 pixel_density{window.pixel_density()};

	buttons = static_cast<mouse_button>(sdl.state);
	pos = glm::vec2{sdl.x, sdl.y} * pixel_density;
	delta = glm::vec2{sdl.xrel, sdl.yrel} * pixel_density;
}

//

tr::mouse_down_event::mouse_down_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().button.windowID)}
{
	const SDL_MouseButtonEvent& sdl{event.sdl_event().button};
	button = static_cast<mouse_button>(1 << (sdl.button - 1));
	clicks = sdl.clicks;
	pos = glm::vec2{sdl.x, sdl.y} * window.pixel_density();
}

//

tr::mouse_up_event::mouse_up_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().button.windowID)}
{
	const SDL_MouseButtonEvent& sdl{event.sdl_event().button};
	button = static_cast<mouse_button>(1 << (sdl.button - 1));
	pos = glm::vec2{sdl.x, sdl.y} * window.pixel_density();
}

//

tr::mouse_wheel_event::mouse_wheel_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().wheel.windowID)}
{
	const SDL_MouseWheelEvent& sdl{event.sdl_event().wheel};
	delta = {sdl.x, sdl.y};
	mouse_pos = glm::vec2{sdl.mouse_x, sdl.mouse_y} * window.pixel_density();
}

//

tr::window_show_event::window_show_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().window.windowID)}
{
}

//

tr::window_hide_event::window_hide_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().window.windowID)}
{
}

//

tr::backbuffer_resize_event::backbuffer_resize_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().window.windowID)}
{
	const SDL_WindowEvent& sdl{event.sdl_event().window};
	size = glm::ivec2{sdl.data1, sdl.data2};
}

//

tr::window_mouse_enter_event::window_mouse_enter_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().window.windowID)}
{
}

//

tr::window_mouse_leave_event::window_mouse_leave_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().window.windowID)}
{
}

//

tr::window_gain_focus_event::window_gain_focus_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().window.windowID)}
{
}

//

tr::window_lose_focus_event::window_lose_focus_event(event_view event) noexcept
	: window{SDL_GetWindowFromID(event.sdl_event().window.windowID)}
{
}