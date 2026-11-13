/// @file
/// @brief Provides event types.

#pragma once
#include <tr/sysgfx/keyboard.hpp>
#include <tr/sysgfx/mouse.hpp>
#include <tr/sysgfx/window_view.hpp>
#include <tr/utility/pack_traits.hpp>

namespace tr
{
	class event_view;
}

//

namespace tr
{
	/// @brief Event emitted when a key is pressed.
	struct key_down_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @brief Whether the event is a repeat.
		bool repeat;

		/// @brief Physical keycode of the pressed key.
		scancode scan;

		/// @brief Virtual keycode of the pressed key.
		keycode key;

		/// @brief Bitmask of held modifiers when the key was pressed.
		keymod mods;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a key down event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit key_down_event(event_view event) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the pressed scan chord.
		/// @return Pressed scan chord.
		[[nodiscard]] scan_chord pressed_scan_chord() const noexcept;

		/// @brief Returns the pressed key chord.
		/// @return Pressed key chord.
		[[nodiscard]] key_chord pressed_key_chord() const noexcept;

		/// @}
	};

	/// @brief Event emitted when a key is released.
	struct key_up_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @brief Physical keycode of the released key.
		scancode scan;

		/// @brief Virtual keycode of the released key.
		keycode key;

		/// @brief Bitmask of held modifiers when the key was released.
		keymod mods;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a key up event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit key_up_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when text is inputted.
	struct text_input_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @brief View over the inputted text string.
		std::string_view text;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a text input event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit text_input_event(event_view event) noexcept;

		/// @}
	};

	//

	/// @brief Event emitted when the mouse is moved.
	struct mouse_motion_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @brief Mask of the held mouse buttons.
		mouse_button buttons;

		/// @brief Position of the mouse.
		glm::vec2 pos;

		/// @brief Change in mouse position since the last event of this type.
		glm::vec2 delta;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a mouse motion event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit mouse_motion_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when a mouse button is pressed.
	struct mouse_down_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @brief Pressed mouse button.
		mouse_button button;

		/// @brief Number of consecutive clicks.
		u8 clicks;

		/// @brief Position of the mouse.
		glm::vec2 pos;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a mouse down event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit mouse_down_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when a mouse button is released.
	struct mouse_up_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @brief Released mouse button.
		mouse_button button;

		/// @brief Position of the mouse.
		glm::vec2 pos;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a mouse up event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit mouse_up_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when the mouse wheel is moved.
	struct mouse_wheel_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @brief Change in wheel value.
		glm::vec2 delta;

		/// @brief Position of the mouse.
		glm::vec2 mouse_pos;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a mouse wheel event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit mouse_wheel_event(event_view event) noexcept;

		/// @}
	};

	//

	/// @brief Event emitted when a window is shown.
	struct window_show_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a window showing event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit window_show_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when a window is hidden.
	struct window_hide_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a window hiding event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit window_hide_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when the window backbuffer changes size.
	struct backbuffer_resize_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @brief New size of the backbuffer.
		glm::ivec2 size;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a window backbuffer resizing event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit backbuffer_resize_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when the mouse enters a window.
	struct window_mouse_enter_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a window mouse entering event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit window_mouse_enter_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when the mouse leaves a window.
	struct window_mouse_leave_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a window mouse leaving event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit window_mouse_leave_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when a window gains focus.
	struct window_gain_focus_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a window focus gaining event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit window_gain_focus_event(event_view event) noexcept;

		/// @}
	};

	/// @brief Event emitted when a window loses focus.
	struct window_lose_focus_event
	{
		/// @brief View to the window associated with the event.
		window_view window;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a window focus losing event from an event view.
		/// @param event Event view to construct from.
		[[nodiscard]] explicit window_lose_focus_event(event_view event) noexcept;

		/// @}
	};

	//

	/// @brief Event emitted for unrecognized event types.
	struct unknown_event
	{
	};

	/// @brief Event emitted when the application wants to quit.
	struct quit_event
	{
	};

	//

	/// @brief Concept checking for an event type.
	/// @tparam Target Type to check for being an event type.
	template <typename Target>
	concept event_type = one_of<Target, quit_event, window_show_event, window_hide_event, backbuffer_resize_event, window_gain_focus_event,
								window_lose_focus_event, window_mouse_enter_event, window_mouse_leave_event, key_down_event, key_up_event,
								text_input_event, mouse_motion_event, mouse_down_event, mouse_up_event, mouse_wheel_event>;
} // namespace tr