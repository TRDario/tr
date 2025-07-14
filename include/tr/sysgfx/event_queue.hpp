#pragma once
#include "display.hpp"
#include "event_types.hpp"

namespace tr {
	// Creates a timer that sends tick events.
	timer create_tick_timer(float frequency, std::uint32_t id) noexcept;
	// Creates a timer that sends draw events.
	timer create_draw_timer(float frequency = refresh_rate()) noexcept;

	// Global event queue.
	struct event_queue {
		// Polls for an event, returning it from the event queue if it exists.
		static std::optional<event> poll() noexcept;

		// Gets an event from the event queue, or waits until one appears.
		static event wait() noexcept;
		// Gets an event from the event queue, waiting until one appears or until a certain amount of time has passed.
		static std::optional<event> wait(imsecs timeout) noexcept;
		// Handles all available events in a loop.
		template <std::invocable<tr::event> Fn>
		static void handle(const Fn& fn) noexcept(noexcept(std::declval<Fn>()(std::declval<event>())));

		// Sets whether text input events should be sent to the event queue.
		static void send_text_input_events(bool arg) noexcept;

		// Pushes an event to the queue.
		static void push(const event& event) noexcept;
		// Pushes an event to the queue.
		static void push(event&& event) noexcept;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::invocable<tr::event> Fn>
void tr::event_queue::handle(const Fn& fn) noexcept(noexcept(std::declval<Fn>()(std::declval<event>())))
{
	for (std::optional<event> event = wait(imsecs{1}); event.has_value(); event = poll()) {
		fn(*event);
	}
}
