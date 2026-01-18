///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides event types and related functionality.                                                                                       //
//                                                                                                                                       //
// Events are handled in the handle_event(tr::sys::event& event) main function. tr::event is an opaque sum type that can be converted    //
// into one of its possible subtypes. This can be done using using a match statement, a visitor using the .visit() method, or by using   //
// the .is<T>() and .as<T>() methods to check for and convert to a specific type:                                                        //
//     - event | tr::match {                                                                                                             //
//           [] (tr::sys::key_down_event evt) { tr::println("Pressed {}", evt.span); },                                                  //
//           [] (tr::sys::key_up_event evt) { tr::println("Released {}", evt.span); },                                                   //
//           tr::ignore_other_cases                                                                                                      //
//       }                                                                                                                               //
//     - event.visit(tr::match {                                                                                                         //
//           [] (tr::sys::key_down_event evt) { tr::println("Pressed {}", evt.span); },                                                  //
//           [] (tr::sys::key_up_event evt) { tr::println("Released {}", evt.span); },                                                   //
//           tr::ignore_other_cases                                                                                                      //
//       })                                                                                                                              //
//     - if (event.is<tr::sys::key_down_event>()) {                                                                                      //
//           tr::println("Pressed {}", event.as<tr::sys::key_down_event>().span);                                                        //
//       }                                                                                                                               //
//       else if (event.is<tr::sys::key_up_event>()) {                                                                                   //
//           tr::println("Released {}", event.as<tr::sys::key_up_event>().span);                                                         //
//       }                                                                                                                               //
//                                                                                                                                       //
// Key-down events are convertible into both chord types:                                                                                //
//     - tr::sys::key_down_event{event} == "Ctrl+K"_sc -> returns whether Ctrl+K is pressed                                              //
//     - tr::sys::key_down_event{event} == "Ctrl+Ć"_kc -> returns whether Ctrl+Ć is pressed                                              //
//                                                                                                                                       //
// Text inputs are disabled by default and must be enabled and disabled manually:                                                        //
//     - tr::sys::enable_text_input_events() -> tr::sys::text_input_event can now be sent                                                //
//     - tr::sys::disable_text_input_events() -> tr::sys::text_input_event will no longer be sent                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/variant.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"

namespace tr::sys {
	class event;
}

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::sys {
	// Event emitted when a key is pressed.
	struct key_down_event {
		// Whether the event is a repeat.
		bool repeat;
		// The physical keycode of the pressed key.
		scancode scan;
		// The virtual keycode of the pressed key.
		keycode key;
		// A bitmask of held modifiers when the key was pressed.
		keymod mods;

		// Converts a generic event into a key down event.
		explicit key_down_event(const event& event);

		// Gets the pressed scan chord.
		operator scan_chord() const;
		// Gets the pressed key chord.
		operator key_chord() const;
	};
	// Event emitted when a key is release.
	struct key_up_event {
		// The physical keycode of the released key.
		scancode scan;
		// The virtual keycode of the released key.
		keycode key;
		// A bitmask of held modifiers when the key was released.
		keymod mods;

		// Converts a generic event into a key up event.
		explicit key_up_event(const event& event);
	};
	// Event emitted when text is inputted.
	struct text_input_event {
		// A view over the inputted text string.
		std::string_view text;

		// Converts a generic event into a text input event.
		explicit text_input_event(const event& event);
	};

	// Event emitted when the mouse is moved.
	struct mouse_motion_event {
		// A mask of the held mouse buttons.
		mouse_button buttons;
		// The position of the mouse.
		glm::vec2 pos;
		// The change in mouse position since the last event of this type.
		glm::vec2 delta;

		// Converts a generic event into a mouse motion event.
		explicit mouse_motion_event(const event& event);
	};
	// Event emitted when a mouse button is pressed.
	struct mouse_down_event {
		// The pressed mouse button.
		mouse_button button;
		// The number of consecutive clicks.
		u8 clicks;
		// The position of the mouse.
		glm::vec2 pos;

		// Converts a generic event into a mouse down event.
		explicit mouse_down_event(const event& event);
	};
	// Event emitted when a mouse button is released.
	struct mouse_up_event {
		// The released mouse button.
		mouse_button button;
		// The position of the mouse.
		glm::vec2 pos;

		// Converts a generic event into a mouse up event.
		explicit mouse_up_event(const event& event);
	};
	// Event emitted when the mouse wheel is moved.
	struct mouse_wheel_event {
		// The change in wheel value.
		glm::vec2 delta;
		// The position of the mouse.
		glm::vec2 mouse_pos;

		// Converts a generic event into a mouse wheel event.
		explicit mouse_wheel_event(const event& event);
	};

	// Event emitted when a window is shown.
	struct window_show_event {};
	// Event emitted when a window is hidden.
	struct window_hide_event {};
	// Event emitted when the window backbuffer changes size.
	struct backbuffer_resize_event {
		// The new size of the backbuffer.
		glm::ivec2 size;

		// Converts a generic event into a window backbuffer resizing event.
		explicit backbuffer_resize_event(const event& event);
	};
	// Event emitted when the mouse enters a window.
	struct window_mouse_enter_event {};
	// Event emitted when the mouse leaves a window.
	struct window_mouse_leave_event {};
	// Event emitted when a window gains focus.
	struct window_gain_focus_event {};
	// Event emitted when a window loses focus.
	struct window_lose_focus_event {};

	// Event emitted for unrecognized event types.
	struct unknown_event {};
	// Event emitted when the application wants to quit.
	struct quit_event {};

	// Concept denoting the list of valid event types.
	template <class T>
	concept event_type = one_of<T, quit_event, window_show_event, window_hide_event, backbuffer_resize_event, window_gain_focus_event,
								window_lose_focus_event, window_mouse_enter_event, window_mouse_leave_event, key_down_event, key_up_event,
								text_input_event, mouse_motion_event, mouse_down_event, mouse_up_event, mouse_wheel_event>;

	// An event visitor must be callable with all event types, and all overloads must return the same type.
	template <class T>
	concept event_visitor =
		std::invocable<T, quit_event> && std::invocable<T, window_show_event> && std::invocable<T, window_hide_event> &&
		std::invocable<T, backbuffer_resize_event> && std::invocable<T, window_gain_focus_event> &&
		std::invocable<T, window_lose_focus_event> && std::invocable<T, window_mouse_enter_event> &&
		std::invocable<T, window_mouse_leave_event> && std::invocable<T, key_down_event> && std::invocable<T, key_up_event> &&
		std::invocable<T, text_input_event> && std::invocable<T, mouse_motion_event> && std::invocable<T, mouse_down_event> &&
		std::invocable<T, mouse_up_event> && std::invocable<T, mouse_wheel_event> && std::invocable<T, unknown_event> &&
		requires(T visitor) {
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
		decltype(auto) operator|(match<Fs...>&& match) const;
		// Visits the event with a stateful match helper.
		template <class State, class... Fs>
			requires(event_visitor<stateful_match<State, Fs...>>)
		decltype(auto) operator|(stateful_match<State, Fs...>&& match) const;

	  private:
		// Storage for SDL_Event.
		alignas(8) std::byte m_buffer[128];

		event() = default;

		// Gets the event subtype.
		u32 type() const;
	};

	// Enables sending text input events.
	void enable_text_input_events();
	// Disables sending text input events.
	void disable_text_input_events();
} // namespace tr::sys

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

#include "event_impl.hpp" // IWYU pragma: export