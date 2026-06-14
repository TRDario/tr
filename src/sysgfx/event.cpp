///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-constexpr parts of event.hpp.                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/event.hpp"
#include <SDL3/SDL.h>

using namespace std::chrono_literals;

/////////////////////////////////////////////////////////////// EVENT TYPES ///////////////////////////////////////////////////////////////

// Converts SDL keymods to tr keymods.
static constexpr tr::keymod convert_keymods(SDL_Keymod mods)
{
	if (mods & SDL_KMOD_SHIFT) {
		mods |= SDL_KMOD_SHIFT;
	}
	if (mods & SDL_KMOD_CTRL) {
		mods |= SDL_KMOD_CTRL;
	}
	if (mods & SDL_KMOD_ALT) {
		mods |= SDL_KMOD_ALT;
	}
	mods &= (SDL_KMOD_SHIFT | SDL_KMOD_CTRL | SDL_KMOD_ALT);
	return tr::keymod(mods);
}

//

tr::key_down_event::key_down_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).key.windowID)}
{
	const SDL_KeyboardEvent& sdl{((const SDL_Event&)event).key};
	repeat = sdl.repeat;
	scan = scancode(sdl.scancode);
	key = keycode(sdl.key);
	mods = convert_keymods(sdl.mod);
}

tr::key_down_event::operator scan_chord() const
{
	return {mods, scan};
}

tr::key_down_event::operator key_chord() const
{
	return {mods, key};
}

//

tr::key_up_event::key_up_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).key.windowID)}
{
	const SDL_KeyboardEvent& sdl{((const SDL_Event&)event).key};
	scan = scancode(sdl.scancode);
	key = keycode(sdl.key);
	mods = convert_keymods(sdl.mod);
}

//

tr::text_input_event::text_input_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).text.windowID)}
{
	const SDL_TextInputEvent& sdl{((const SDL_Event&)event).text};
	text = sdl.text;
}

//

tr::mouse_motion_event::mouse_motion_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).motion.windowID)}
{
	const SDL_MouseMotionEvent& sdl{((const SDL_Event&)event).motion};
	const glm::vec2 pixel_density{window.pixel_density()};

	buttons = mouse_button(sdl.state);
	pos = glm::vec2{sdl.x, sdl.y} * pixel_density;
	delta = glm::vec2{sdl.xrel, sdl.yrel} * pixel_density;
}

//

tr::mouse_down_event::mouse_down_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).button.windowID)}
{
	const SDL_MouseButtonEvent& sdl{((const SDL_Event&)event).button};
	button = mouse_button(1 << (sdl.button - 1));
	clicks = sdl.clicks;
	pos = glm::vec2{sdl.x, sdl.y} * window.pixel_density();
}

//

tr::mouse_up_event::mouse_up_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).button.windowID)}
{
	const SDL_MouseButtonEvent& sdl{((const SDL_Event&)event).button};
	button = mouse_button(1 << (sdl.button - 1));
	pos = glm::vec2{sdl.x, sdl.y} * window.pixel_density();
}

//

tr::mouse_wheel_event::mouse_wheel_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).wheel.windowID)}
{
	const SDL_MouseWheelEvent& sdl{((const SDL_Event&)event).wheel};
	delta = {sdl.x, sdl.y};
	mouse_pos = glm::vec2{sdl.mouse_x, sdl.mouse_y} * window.pixel_density();
}

//

tr::window_show_event::window_show_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).window.windowID)}
{
}

//

tr::window_hide_event::window_hide_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).window.windowID)}
{
}

//

tr::backbuffer_resize_event::backbuffer_resize_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).window.windowID)}
{
	const SDL_WindowEvent& sdl{((const SDL_Event&)event).window};
	size = {sdl.data1, sdl.data2};
}

//

tr::window_mouse_enter_event::window_mouse_enter_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).window.windowID)}
{
}

//

tr::window_mouse_leave_event::window_mouse_leave_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).window.windowID)}
{
}

//

tr::window_gain_focus_event::window_gain_focus_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).window.windowID)}
{
}

//

tr::window_lose_focus_event::window_lose_focus_event(const event& event)
	: window{SDL_GetWindowFromID(((const SDL_Event&)event).window.windowID)}
{
}

//

tr::u32 tr::event::type() const
{
	return ((const SDL_Event*)m_buffer)->type;
}