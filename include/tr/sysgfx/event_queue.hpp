#pragma once
#include "../utility/chrono.hpp"
#include "display.hpp"
#include "event_types.hpp"

namespace tr {
	class timer;
}

namespace tr::system {
	// Creates a timer that sends tick events.
	timer create_tick_timer(float frequency, int id = 0);
	// Creates a timer that sends draw events.
	timer create_draw_timer(float frequency = refresh_rate());

	// Polls for an event, returning it from the event queue if it exists.
	std::optional<event> poll_event();
	// Gets an event from the event queue, or waits until one appears.
	event wait_for_event();
	// Gets an event from the event queue, waiting until one appears or until a certain amount of time has passed.
	std::optional<event> wait_for_event(imsecs timeout);
	// Handles all available events in a loop.
	template <std::invocable<tr::system::event> Fn> void handle_events(const Fn& fn);

	// Enables sending text input events.
	void enable_text_input_events();
	// Disables sending text input events.
	void disable_text_input_events();
} // namespace tr::system

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::invocable<tr::system::event> Fn> void tr::system::handle_events(const Fn& fn)
{
	for (std::optional<event> event = wait_for_event(imsecs{1}); event.has_value(); event = poll_event()) {
		fn(*event);
	}
}
