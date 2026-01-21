///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-constexpr parts of event.hpp.                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/event.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/window.hpp"
#include <SDL3/SDL.h>

using namespace std::chrono_literals;

/////////////////////////////////////////////////////////////// EVENT TYPES ///////////////////////////////////////////////////////////////

namespace tr::sys {
	// Converts SDL keymods to tr keymods.
	static constexpr keymod convert_keymods(SDL_Keymod mods);
} // namespace tr::sys

constexpr tr::sys::keymod tr::sys::convert_keymods(SDL_Keymod mods)
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
	return keymod(mods);
}

tr::sys::key_down_event::key_down_event(const event& event)
{
	const SDL_KeyboardEvent& sdl{((const SDL_Event&)event).key};
	repeat = sdl.repeat;
	scan = scancode(sdl.scancode);
	key = keycode(sdl.key);
	mods = convert_keymods(sdl.mod);
}

tr::sys::key_down_event::operator scan_chord() const
{
	return {mods, scan};
}

tr::sys::key_down_event::operator key_chord() const
{
	return {mods, key};
}

tr::sys::key_up_event::key_up_event(const event& event)
{
	const SDL_KeyboardEvent& sdl{((const SDL_Event&)event).key};
	scan = scancode(sdl.scancode);
	key = keycode(sdl.key);
	mods = convert_keymods(sdl.mod);
}

tr::sys::text_input_event::text_input_event(const event& event)
{
	const SDL_TextInputEvent& sdl{((const SDL_Event&)event).text};
	text = sdl.text;
}

tr::sys::mouse_motion_event::mouse_motion_event(const event& event)
{
	const SDL_MouseMotionEvent& sdl{((const SDL_Event&)event).motion};
	const glm::vec2 pixel_density{window_pixel_density()};

	buttons = mouse_button(sdl.state);
	pos = glm::vec2{sdl.x, sdl.y} * pixel_density;
	delta = glm::vec2{sdl.xrel, sdl.yrel} * pixel_density;
}

tr::sys::mouse_down_event::mouse_down_event(const event& event)
{
	const SDL_MouseButtonEvent& sdl{((const SDL_Event&)event).button};
	button = mouse_button(1 << (sdl.button - 1));
	clicks = sdl.clicks;
	pos = glm::vec2{sdl.x, sdl.y} * window_pixel_density();
}

tr::sys::mouse_up_event::mouse_up_event(const event& event)
{
	const SDL_MouseButtonEvent& sdl{((const SDL_Event&)event).button};
	button = mouse_button(1 << (sdl.button - 1));
	pos = glm::vec2{sdl.x, sdl.y} * window_pixel_density();
}

tr::sys::mouse_wheel_event::mouse_wheel_event(const event& event)
{
	const SDL_MouseWheelEvent& sdl{((const SDL_Event&)event).wheel};
	delta = {sdl.x, sdl.y};
	mouse_pos = glm::vec2{sdl.mouse_x, sdl.mouse_y} * window_pixel_density();
}

tr::sys::backbuffer_resize_event::backbuffer_resize_event(const event& event)
{
	const SDL_WindowEvent& sdl{((const SDL_Event&)event).window};
	size = {sdl.data1, sdl.data2};
}

tr::u32 tr::sys::event::type() const
{
	return ((const SDL_Event*)m_buffer)->type;
}

/////////////////////////////////////////////////////////////// EVENT STATE ///////////////////////////////////////////////////////////////

void tr::sys::enable_text_input_events()
{
	TR_ASSERT(g_window.is_open(), "Tried to enable text input event sending before opening the window.");

	SDL_StartTextInput(g_window.ptr());
}

void tr::sys::disable_text_input_events()
{
	TR_ASSERT(g_window.is_open(), "Tried to disable text input event sending before opening the window.");

	SDL_StopTextInput(g_window.ptr());
}