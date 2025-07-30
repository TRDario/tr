#pragma once
#include "../utility/chrono.hpp"
#include "display.hpp"
#include "event_types.hpp"

namespace tr {
	class timer;

	// Creates a timer that sends tick events.
	timer create_tick_timer(float frequency, std::uint32_t id);
	// Creates a timer that sends draw events.
	timer create_draw_timer(float frequency = refresh_rate());

	// Global event queue.
	namespace event_queue {
		// Polls for an event, returning it from the event queue if it exists.
		std::optional<event> poll();
		// Gets an event from the event queue, or waits until one appears.
		event wait();
		// Gets an event from the event queue, waiting until one appears or until a certain amount of time has passed.
		std::optional<event> wait(imsecs timeout);
		// Handles all available events in a loop.
		template <std::invocable<tr::event> Fn> void handle(const Fn& fn);

		// Enables sending text input events.
		void enable_text_input_events();
		// Disables sending text input events.
		void disable_text_input_events();

		// Pushes an event to the queue.
		void push(const event& event);
		// Pushes an event to the queue.
		void push(event&& event);
	}; // namespace event_queue
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::invocable<tr::event> Fn> void tr::event_queue::handle(const Fn& fn)
{
	for (std::optional<event> event = wait(imsecs{1}); event.has_value(); event = poll()) {
		fn(*event);
	}
}
