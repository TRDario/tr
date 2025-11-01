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

	// Concept denoting the list of valid event types.
	template <class T>
	concept event_type =
		one_of<T, quit_event, window_show_event, window_hide_event, backbuffer_resize_event, window_gain_focus_event,
			   window_lose_focus_event, window_mouse_enter_event, window_mouse_leave_event, key_down_event, key_up_event, text_input_event,
			   mouse_motion_event, mouse_down_event, mouse_up_event, mouse_wheel_event, tick_event, draw_event>;

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
		// Checks whether the event is of a certain type.
		template <event_type T> bool is() const;
		// Converts the event into a sub-type.
		template <event_type T> T as() const;

		// Visits the event.
		template <event_visitor Visitor> auto visit(Visitor&& visitor) const;
		// Visits the event with a match helper.
		template <class... Fs>
			requires(event_visitor<match<Fs...>>)
		auto operator|(match<Fs...>&& match) const;

	  private:
		// Storage for SDL_Event.
		alignas(8) std::byte m_buffer[128];

		event() = default;

		u32 type() const;
	};
} // namespace tr::sys

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::sys::event_type T> bool tr::sys::event::is() const
{
	if constexpr (std::same_as<T, quit_event>) {
		return type() == 0x100;
	}
	else if constexpr (std::same_as<T, window_show_event>) {
		return type() == 0x202;
	}
	else if constexpr (std::same_as<T, window_hide_event>) {
		return type() == 0x203;
	}
	else if constexpr (std::same_as<T, backbuffer_resize_event>) {
		return type() == 0x207;
	}
	else if constexpr (std::same_as<T, window_mouse_enter_event>) {
		return type() == 0x20C;
	}
	else if constexpr (std::same_as<T, window_mouse_leave_event>) {
		return type() == 0x20D;
	}
	else if constexpr (std::same_as<T, window_gain_focus_event>) {
		return type() == 0x20E;
	}
	else if constexpr (std::same_as<T, window_lose_focus_event>) {
		return type() == 0x20F;
	}
	else if constexpr (std::same_as<T, key_down_event>) {
		return type() == 0x300;
	}
	else if constexpr (std::same_as<T, key_up_event>) {
		return type() == 0x301;
	}
	else if constexpr (std::same_as<T, text_input_event>) {
		return type() == 0x303;
	}
	else if constexpr (std::same_as<T, mouse_motion_event>) {
		return type() == 0x400;
	}
	else if constexpr (std::same_as<T, mouse_down_event>) {
		return type() == 0x401;
	}
	else if constexpr (std::same_as<T, mouse_up_event>) {
		return type() == 0x402;
	}
	else if constexpr (std::same_as<T, mouse_wheel_event>) {
		return type() == 0x403;
	}
	else if constexpr (std::same_as<T, tick_event>) {
		return type() == 0x8000;
	}
	else if constexpr (std::same_as<T, draw_event>) {
		return type() == 0x8001;
	}
}

template <tr::sys::event_type T> T tr::sys::event::as() const
{
	TR_ASSERT(is<T>(), "Tried to convert event to a sub-type it is not.");

	if constexpr (std::same_as<T, quit_event>) {
		return quit_event{};
	}
	else if constexpr (std::same_as<T, window_show_event>) {
		return window_show_event{};
	}
	else if constexpr (std::same_as<T, window_hide_event>) {
		return window_hide_event{};
	}
	else if constexpr (std::same_as<T, backbuffer_resize_event>) {
		return backbuffer_resize_event{*this};
	}
	else if constexpr (std::same_as<T, window_mouse_enter_event>) {
		return window_mouse_enter_event{};
	}
	else if constexpr (std::same_as<T, window_mouse_leave_event>) {
		return window_mouse_leave_event{};
	}
	else if constexpr (std::same_as<T, window_gain_focus_event>) {
		return window_gain_focus_event{};
	}
	else if constexpr (std::same_as<T, window_lose_focus_event>) {
		return window_lose_focus_event{};
	}
	else if constexpr (std::same_as<T, key_down_event>) {
		return key_down_event{*this};
	}
	else if constexpr (std::same_as<T, key_up_event>) {
		return key_up_event{*this};
	}
	else if constexpr (std::same_as<T, text_input_event>) {
		return text_input_event{*this};
	}
	else if constexpr (std::same_as<T, mouse_motion_event>) {
		return mouse_motion_event{*this};
	}
	else if constexpr (std::same_as<T, mouse_down_event>) {
		return mouse_down_event{*this};
	}
	else if constexpr (std::same_as<T, mouse_up_event>) {
		return mouse_up_event{*this};
	}
	else if constexpr (std::same_as<T, mouse_wheel_event>) {
		return mouse_wheel_event{*this};
	}
	else if constexpr (std::same_as<T, tick_event>) {
		return tick_event{*this};
	}
	else if constexpr (std::same_as<T, draw_event>) {
		return draw_event{};
	}
}

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