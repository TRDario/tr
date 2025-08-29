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

namespace tr::system {
	// Converts SDL keymods to tr keymods.
	constexpr keymod convert_keymods(SDL_Keymod mods);
} // namespace tr::system

constexpr tr::system::keymod tr::system::convert_keymods(SDL_Keymod mods)
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

tr::system::key_down_event::key_down_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to key_down_event.");

	const SDL_KeyboardEvent& sdl{((const SDL_Event&)event).key};
	repeat = sdl.repeat;
	scan = scancode::enum_t(sdl.scancode);
	key = keycode::enum_t(sdl.key);
	mods = convert_keymods(sdl.mod);
}

bool tr::system::operator==(const key_down_event& event, const key_chord& chord)
{
	return key_chord{event.key, event.mods} == chord;
}

bool tr::system::operator==(const key_down_event& event, const scan_chord& chord)
{
	return scan_chord{event.scan, event.mods} == chord;
}

bool tr::system::operator==(const key_chord& chord, const key_down_event& event)
{
	return key_chord{event.key, event.mods} == chord;
}

bool tr::system::operator==(const scan_chord& chord, const key_down_event& event)
{
	return scan_chord{event.scan, event.mods} == chord;
}

tr::system::key_up_event::key_up_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to key_up_event.");

	const SDL_KeyboardEvent& sdl{((const SDL_Event&)event).key};
	scan = scancode::enum_t(sdl.scancode);
	key = keycode::enum_t(sdl.key);
	mods = convert_keymods(sdl.mod);
}

tr::system::text_input_event::text_input_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to text_input_event.");

	const SDL_TextInputEvent& sdl{((const SDL_Event&)event).text};
	text = sdl.text;
}

tr::system::mouse_motion_event::mouse_motion_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to mouse_motion_event.");

	const SDL_MouseMotionEvent& sdl{((const SDL_Event&)event).motion};
	buttons = mouse_button(sdl.state);
	pos = glm::vec2{sdl.x, sdl.y} * window_pixel_density();
	delta = glm::vec2{sdl.xrel, sdl.yrel} * window_pixel_density();
}

tr::system::mouse_down_event::mouse_down_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to mouse_down_event.");

	const SDL_MouseButtonEvent& sdl{((const SDL_Event&)event).button};
	button = mouse_button(1 << (sdl.button - 1));
	clicks = sdl.clicks;
	pos = glm::vec2{sdl.x, sdl.y} * window_pixel_density();
}

tr::system::mouse_up_event::mouse_up_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to mouse_up_event.");

	const SDL_MouseButtonEvent& sdl{((const SDL_Event&)event).button};
	button = mouse_button(1 << (sdl.button - 1));
	pos = glm::vec2{sdl.x, sdl.y} * window_pixel_density();
}

tr::system::mouse_wheel_event::mouse_wheel_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to mouse_wheel_event.");

	const SDL_MouseWheelEvent& sdl{((const SDL_Event&)event).wheel};
	delta = {sdl.x, sdl.y};
	mouse_pos = glm::vec2{sdl.mouse_x, sdl.mouse_y} * window_pixel_density();
}

tr::system::backbuffer_resize_event::backbuffer_resize_event(const event& event)
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to backbuffer_resize_event.");

	const SDL_WindowEvent& sdl{((const SDL_Event&)event).window};
	size = {sdl.data1, sdl.data2};
}

tr::system::tick_event::tick_event(const event& event)
	: id{((const SDL_Event&)event).user.code}
{
	TR_ASSERT(event.type() == ID, "Tried to cast an unrelated event to tick_event.");
}

tr::u32 tr::system::event::type() const
{
	return ((const SDL_Event*)m_buffer)->type;
}

tr::timer tr::system::create_tick_timer(float frequency, int id)
{
	return timer{
		1.0s / frequency,
		[=] {
			SDL_Event event;
			event.type = tick_event::ID;
			event.user.code = id;
			SDL_PushEvent(&event);
		},
	};
}

tr::timer tr::system::create_draw_timer(float frequency)
{
	return timer{
		1.0s / frequency,
		[] {
			SDL_Event event{.type = draw_event::ID};
			SDL_PushEvent(&event);
		},
	};
}

std::optional<tr::system::event> tr::system::poll_event()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to poll for events before opening the window.");

	SDL_Event sdl;
	return SDL_PollEvent(&sdl) ? std::optional<system::event>{std::bit_cast<event>(sdl)} : std::nullopt;
}

tr::system::event tr::system::wait_for_event()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to wait for events before opening the window.");

	SDL_Event sdl;
	SDL_WaitEvent(&sdl);
	return std::bit_cast<event>(sdl);
}

std::optional<tr::system::event> tr::system::wait_for_event(imsecs timeout)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to wait for events before opening the window.");

	SDL_Event sdl;
	if (SDL_WaitEventTimeout(&sdl, i32(timeout.count()))) {
		return std::bit_cast<event>(sdl);
	}
	else {
		return std::nullopt;
	}
}

void tr::system::enable_text_input_events()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to enable text input event sending before opening the window.");

	SDL_StartTextInput(sdl_window);
}

void tr::system::disable_text_input_events()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to disable text input event sending before opening the window.");

	SDL_StopTextInput(sdl_window);
}