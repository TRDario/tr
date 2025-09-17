#pragma once
#include "../utility/common.hpp"
#include "keyboard_events.hpp"
#include "mouse_events.hpp"
#include "window_events.hpp"

namespace tr::sys {
	class event;

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

	// Unified event type.
	class event {
	  public:
		// Gets the type ID of the event.
		u32 type() const;

		template <class Visitor> void visit(Visitor&& visitor) const;

	  private:
		// Storage for SDL_Event.
		alignas(8) std::byte m_buffer[128];

		event() = default;
	};
} // namespace tr::sys

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class Visitor> void tr::sys::event::visit(Visitor&& visitor) const
{
	switch (type()) {
	case 0x100:
		if constexpr (std::is_invocable_v<Visitor, quit_event>) {
			visitor(quit_event{});
		}
		break;
	case 0x202:
		if constexpr (std::is_invocable_v<Visitor, window_show_event>) {
			visitor(window_show_event{});
		}
		break;
	case 0x203:
		if constexpr (std::is_invocable_v<Visitor, window_hide_event>) {
			visitor(window_hide_event{});
		}
		break;
	case 0x207:
		if constexpr (std::is_invocable_v<Visitor, backbuffer_resize_event>) {
			visitor(backbuffer_resize_event{*this});
		}
		break;
	case 0x20C:
		if constexpr (std::is_invocable_v<Visitor, window_mouse_enter_event>) {
			visitor(window_mouse_enter_event{});
		}
		break;
	case 0x20D:
		if constexpr (std::is_invocable_v<Visitor, window_mouse_leave_event>) {
			visitor(window_mouse_leave_event{});
		}
		break;
	case 0x20E:
		if constexpr (std::is_invocable_v<Visitor, window_gain_focus_event>) {
			visitor(window_gain_focus_event{});
		}
		break;
	case 0x20F:
		if constexpr (std::is_invocable_v<Visitor, window_lose_focus_event>) {
			visitor(window_lose_focus_event{});
		}
		break;
	case 0x300:
		if constexpr (std::is_invocable_v<Visitor, key_down_event>) {
			visitor(key_down_event{*this});
		}
		break;
	case 0x301:
		if constexpr (std::is_invocable_v<Visitor, key_up_event>) {
			visitor(key_up_event{*this});
		}
		break;
	case 0x303:
		if constexpr (std::is_invocable_v<Visitor, text_input_event>) {
			visitor(text_input_event{*this});
		}
		break;
	case 0x400:
		if constexpr (std::is_invocable_v<Visitor, mouse_motion_event>) {
			visitor(mouse_motion_event{*this});
		}
		break;
	case 0x401:
		if constexpr (std::is_invocable_v<Visitor, mouse_down_event>) {
			visitor(mouse_down_event{*this});
		}
		break;
	case 0x402:
		if constexpr (std::is_invocable_v<Visitor, mouse_up_event>) {
			visitor(mouse_up_event{*this});
		}
		break;
	case 0x403:
		if constexpr (std::is_invocable_v<Visitor, mouse_wheel_event>) {
			visitor(mouse_wheel_event{*this});
		}
		break;
	case 0x8000:
		if constexpr (std::is_invocable_v<Visitor, tick_event>) {
			visitor(tick_event{*this});
		}
		break;
	case 0x8001:
		if constexpr (std::is_invocable_v<Visitor, draw_event>) {
			visitor(draw_event{});
		}
		break;
	}
}