/// @file
/// @brief Provides event types and related utlities.

#pragma once
#include "keyboard.hpp"
#include "mouse.hpp"
#include "window_view.hpp"

namespace tr
{
	class event;
}

//

namespace tr
{
	/// Event emitted when a key is pressed.
	struct key_down_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Whether the event is a repeat.
		bool repeat;

		/// Physical keycode of the pressed key.
		scancode scan;

		/// Virtual keycode of the pressed key.
		keycode key;

		/// Bitmask of held modifiers when the key was pressed.
		keymod mods;

		/// @name Constructors
		/// @{

		/// Converts a generic event into a key down event.
		/// @param event Event to convert.
		explicit key_down_event(const event& event);

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets the pressed scan chord.
		/// @return Pressed scan chord.
		operator scan_chord() const;

		/// Gets the pressed key chord.
		/// @return Pressed key chord.
		operator key_chord() const;

		/// @}
	};

	/// Event emitted when a key is release.
	struct key_up_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Physical keycode of the released key.
		scancode scan;

		/// Virtual keycode of the released key.
		keycode key;

		/// Bitmask of held modifiers when the key was released.
		keymod mods;

		//

		/// Converts a generic event into a key up event.
		/// @param event Event to convert.
		explicit key_up_event(const event& event);
	};

	/// Event emitted when text is inputted.
	struct text_input_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// View over the inputted text string.
		std::string_view text;

		//

		/// Converts a generic event into a text input event.
		/// @param event Event to convert.
		explicit text_input_event(const event& event);
	};

	//

	/// Event emitted when the mouse is moved.
	struct mouse_motion_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Mask of the held mouse buttons.
		mouse_button buttons;

		/// Position of the mouse.
		glm::vec2 pos;

		/// Change in mouse position since the last event of this type.
		glm::vec2 delta;

		//

		/// Converts a generic event into a mouse motion event.
		/// @param event Event to convert.
		explicit mouse_motion_event(const event& event);
	};

	/// Event emitted when a mouse button is pressed.
	struct mouse_down_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Pressed mouse button.
		mouse_button button;

		/// Number of consecutive clicks.
		u8 clicks;

		/// Position of the mouse.
		glm::vec2 pos;

		//

		/// Converts a generic event into a mouse down event.
		/// @param event Event to convert.
		explicit mouse_down_event(const event& event);
	};

	/// Event emitted when a mouse button is released.
	struct mouse_up_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Released mouse button.
		mouse_button button;

		/// Position of the mouse.
		glm::vec2 pos;

		//

		/// Converts a generic event into a mouse up event.
		/// @param event Event to convert.
		explicit mouse_up_event(const event& event);
	};

	/// Event emitted when the mouse wheel is moved.
	struct mouse_wheel_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Change in wheel value.
		glm::vec2 delta;

		/// Position of the mouse.
		glm::vec2 mouse_pos;

		//

		/// Converts a generic event into a mouse wheel event.
		/// @param event Event to convert.
		explicit mouse_wheel_event(const event& event);
	};

	//

	/// Event emitted when a window is shown.
	struct window_show_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window showing event.
		/// @param event Event to convert.
		explicit window_show_event(const event& event);
	};

	/// Event emitted when a window is hidden.
	struct window_hide_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window hiding event.
		/// @param event Event to convert.
		explicit window_hide_event(const event& event);
	};

	/// Event emitted when the window backbuffer changes size.
	struct backbuffer_resize_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// New size of the backbuffer.
		glm::ivec2 size;

		//

		/// Converts a generic event into a window backbuffer resizing event.
		/// @param event Event to convert.
		explicit backbuffer_resize_event(const event& event);
	};

	/// Event emitted when the mouse enters a window.
	struct window_mouse_enter_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Converts a generic event into a window mouse entering event.
		/// @param event Event to convert.
		explicit window_mouse_enter_event(const event& event);
	};

	/// Event emitted when the mouse leaves a window.
	struct window_mouse_leave_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window mouse leaving event.
		/// @param event Event to convert.
		explicit window_mouse_leave_event(const event& event);
	};

	/// Event emitted when a window gains focus.
	struct window_gain_focus_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window focus gaining event.
		/// @param event Event to convert.
		explicit window_gain_focus_event(const event& event);
	};

	/// Event emitted when a window loses focus.
	struct window_lose_focus_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window focus losing event.
		/// @param event Event to convert.
		explicit window_lose_focus_event(const event& event);
	};

	//

	/// Event emitted for unrecognized event types.
	struct unknown_event
	{
	};

	/// Event emitted when the application wants to quit.
	struct quit_event
	{
	};

	//

	/// Valid event type.
	template <typename T>
	concept event_type = one_of<T, quit_event, window_show_event, window_hide_event, backbuffer_resize_event, window_gain_focus_event,
								window_lose_focus_event, window_mouse_enter_event, window_mouse_leave_event, key_down_event, key_up_event,
								text_input_event, mouse_motion_event, mouse_down_event, mouse_up_event, mouse_wheel_event>;

	/// Valid event visitor type.
	/// @details Event visitors must be callable with all event types, and all overloads must return the same type.
	template <typename T>
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

	//

	/// Unified event type.
	class event
	{
	  public:
		/// @name Alternatives
		/// @{

		/// Checks whether the event is of a certain type.
		/// @tparam T Event type.
		/// @return Whether the event holds an event of type `T`.
		template <event_type T>
		bool is() const;

		/// Converts the event into a sub-type.
		/// @tparam T Event type.
		/// @pre The event must hold an event of type `T`.
		/// @return Held event subtype.
		template <event_type T>
		T as() const;

		/// @}
		/// @name Visiting
		/// @{

		/// Visits the event.
		/// @tparam Visitor Event visitor type.
		/// @param visitor Event visitor.
		/// @return Result returned by the visitor.
		template <event_visitor Visitor>
		auto visit(Visitor&& visitor) const;

		/// @}

	  private:
		/// Storage for SDL_Event.
		alignas(8) std::byte m_buffer[128];

		//

		/// Private default constructor.
		event() = default;

		//

		/// Gets the event subtype.
		/// @return SDL event subtype ID.
		u32 type() const;
	};
} // namespace tr

#include "impl/event.hpp" // IWYU pragma: export