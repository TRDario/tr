///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr parts of event.hpp.                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "event.hpp"

////////////////////////////////////////////////////////////////// EVENT //////////////////////////////////////////////////////////////////

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
	default:
		return visitor(unknown_event{});
	}
}

template <class... Fs>
	requires(tr::sys::event_visitor<tr::match<Fs...>>)
decltype(auto) tr::sys::event::operator|(match<Fs...>&& match) const
{
	return visit(std::move(match));
}

template <class State, class... Fs>
	requires(tr::sys::event_visitor<tr::stateful_match<State, Fs...>>)
decltype(auto) tr::sys::event::operator|(stateful_match<State, Fs...>&& match) const
{
	return visit(std::move(match));
}