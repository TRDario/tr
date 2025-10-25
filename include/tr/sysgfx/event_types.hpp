#pragma once
#include "../utility/variant.hpp"
#include "keyboard_events.hpp"
#include "mouse_events.hpp"
#include "window_events.hpp"

namespace tr::sys {
	class event;

	// Event emitted for unrecognized event types.
	struct unknown_event {};
	// Event emitted when the application wants to quit.
	struct quit_event {};
	// The event type emitted by ticker timers.
	struct tick_event {
		// The user-assigned type ID attached to the ticker emitting the event.
		int id;

		// Converts a generic event into a tick event.
		explicit tick_event(const event& event);
	};
	// Event emitted when the application wants to redraw.
	struct draw_event {};

	// An event visitor must be callable with all event types, and all overloads must return the same type.
	template <class T>
	concept event_visitor =
		std::invocable<T, quit_event> && std::invocable<T, window_show_event> && std::invocable<T, window_hide_event> &&
		std::invocable<T, backbuffer_resize_event> && std::invocable<T, window_gain_focus_event> &&
		std::invocable<T, window_lose_focus_event> && std::invocable<T, window_mouse_enter_event> &&
		std::invocable<T, window_mouse_leave_event> && std::invocable<T, key_down_event> && std::invocable<T, key_up_event> &&
		std::invocable<T, text_input_event> && std::invocable<T, mouse_motion_event> && std::invocable<T, mouse_down_event> &&
		std::invocable<T, mouse_up_event> && std::invocable<T, mouse_wheel_event> && std::invocable<T, tick_event> &&
		std::invocable<T, draw_event> && requires(T visitor) {
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<window_show_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<window_hide_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<backbuffer_resize_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<window_gain_focus_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<window_lose_focus_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<window_mouse_enter_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<window_mouse_leave_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<key_down_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<key_up_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<text_input_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<mouse_motion_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<mouse_down_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<mouse_up_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<mouse_wheel_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<tick_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<draw_event>()))>;
			requires std::same_as<decltype(visitor(quit_event{})), decltype(visitor(std::declval<unknown_event>()))>;
		};

	// Unified event type.
	class event {
	  public:
		// Gets the type ID of the event.
		u32 type() const;

		template <event_visitor Visitor> auto visit(Visitor&& visitor) const;
		template <class... Fs>
			requires(event_visitor<match<Fs...>>)
		auto operator|(match<Fs...>&& match) const;

	  private:
		// Storage for SDL_Event.
		alignas(8) std::byte m_buffer[128];

		event() = default;
	};
} // namespace tr::sys

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::sys::event_visitor Visitor> auto tr::sys::event::visit(Visitor&& visitor) const
{
	switch (type()) {
	case 0x100:
		return visitor(quit_event{});
	case 0x202:
		return visitor(window_show_event{});
	case 0x203:
		return visitor(window_hide_event{});
	case 0x207:
		return visitor(backbuffer_resize_event{*this});
	case 0x20C:
		return visitor(window_mouse_enter_event{});
	case 0x20D:
		return visitor(window_mouse_leave_event{});
	case 0x20E:
		return visitor(window_gain_focus_event{});
	case 0x20F:
		return visitor(window_lose_focus_event{});
	case 0x300:
		return visitor(key_down_event{*this});
	case 0x301:
		return visitor(key_up_event{*this});
	case 0x303:
		return visitor(text_input_event{*this});
	case 0x400:
		return visitor(mouse_motion_event{*this});
	case 0x401:
		return visitor(mouse_down_event{*this});
	case 0x402:
		return visitor(mouse_up_event{*this});
	case 0x403:
		return visitor(mouse_wheel_event{*this});
	case 0x8000:
		return visitor(tick_event{*this});
	case 0x8001:
		return visitor(draw_event{});
	default:
		return visitor(unknown_event{});
	}
}

template <class... Fs>
	requires(tr::sys::event_visitor<tr::match<Fs...>>)
auto tr::sys::event::operator|(match<Fs...>&& match) const
{
	return visit(std::move(match));
}