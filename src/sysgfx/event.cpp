#include "../../include/tr/sysgfx/event_queue.hpp"
#include "../../include/tr/sysgfx/event_types.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/keyboard_events.hpp"
#include "../../include/tr/sysgfx/mouse_events.hpp"
#include "../../include/tr/sysgfx/window.hpp"
#include "../../include/tr/sysgfx/window_events.hpp"
#include "../../include/tr/utility/timer.hpp"
#include <SDL3/SDL.h>

using namespace std::chrono_literals;

namespace tr::sys {
	// Converts SDL keymods to tr keymods.
	constexpr keymod convert_keymods(SDL_Keymod mods);
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

bool tr::sys::operator==(const key_down_event& event, const key_chord& chord)
{
	return key_chord{event.mods, event.key} == chord;
}

bool tr::sys::operator==(const key_down_event& event, const scan_chord& chord)
{
	return scan_chord{event.mods, event.scan} == chord;
}

tr::sys::key_up_event::key_up_event(const event& event)
{
	const SDL_KeyboardEvent& sdl{((const SDL_Event&)event).key};
	scan = scancode(sdl.scancode);
	key = keycode(sdl.key);
	mods = convert_keymods(sdl.mod);
}

bool tr::sys::operator==(const key_up_event& event, const key_chord& chord)
{
	return key_chord{event.mods, event.key} == chord;
}

bool tr::sys::operator==(const key_up_event& event, const scan_chord& chord)
{
	return scan_chord{event.mods, event.scan} == chord;
}

tr::sys::text_input_event::text_input_event(const event& event)
{
	const SDL_TextInputEvent& sdl{((const SDL_Event&)event).text};
	text = sdl.text;
}

tr::sys::mouse_motion_event::mouse_motion_event(const event& event)
{
	const SDL_MouseMotionEvent& sdl{((const SDL_Event&)event).motion};
	buttons = mouse_button(sdl.state);
	pos = glm::vec2{sdl.x, sdl.y} * window_pixel_density();
	delta = glm::vec2{sdl.xrel, sdl.yrel} * window_pixel_density();
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

tr::sys::tick_event::tick_event(const event& event)
	: id{((const SDL_Event&)event).user.code}
{
}

tr::u32 tr::sys::event::type() const
{
	return ((const SDL_Event*)m_buffer)->type;
}

tr::timer tr::sys::create_tick_timer(float frequency, int id)
{
	return timer{
		1.0s / frequency,
		[=] {
			SDL_Event event;
			event.type = 0x8000;
			event.user.code = id;
			SDL_PushEvent(&event);
		},
	};
}

tr::timer tr::sys::create_draw_timer(float frequency)
{
	return timer{
		1.0s / frequency,
		[] {
			SDL_Event event{.type = 0x8001};
			SDL_PushEvent(&event);
		},
	};
}

void tr::sys::enable_text_input_events()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to enable text input event sending before opening the window.");

	SDL_StartTextInput(sdl_window);
}

void tr::sys::disable_text_input_events()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to disable text input event sending before opening the window.");

	SDL_StopTextInput(sdl_window);
}