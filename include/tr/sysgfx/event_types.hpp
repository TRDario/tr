/// @file
/// @brief Provides event types.

#pragma once
#include <tr/sysgfx/keyboard.hpp>
#include <tr/sysgfx/mouse.hpp>
#include <tr/sysgfx/window_view.hpp>
#include <tr/utility/pack_traits.hpp>

union SDL_Event;
namespace tr
{
	class event_view;
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

		/// @name Constructors and destructors
		/// @{

		/// Converts a generic event into a key down event.
		/// @param event Event to convert.
		[[nodiscard]] explicit key_down_event(event_view event) noexcept;

		/// @}
		/// @name Chords
		/// @{

		/// Gets the pressed scan chord.
		/// @return Pressed scan chord.
		[[nodiscard]] scan_chord scan_chord() const noexcept;

		/// Gets the pressed key chord.
		/// @return Pressed key chord.
		[[nodiscard]] key_chord key_chord() const noexcept;

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
		[[nodiscard]] explicit key_up_event(event_view event) noexcept;
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
		[[nodiscard]] explicit text_input_event(event_view event) noexcept;
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
		[[nodiscard]] explicit mouse_motion_event(event_view event) noexcept;
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
		[[nodiscard]] explicit mouse_down_event(event_view event) noexcept;
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
		[[nodiscard]] explicit mouse_up_event(event_view event) noexcept;
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
		[[nodiscard]] explicit mouse_wheel_event(event_view event) noexcept;
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
		[[nodiscard]] explicit window_show_event(event_view event) noexcept;
	};

	/// Event emitted when a window is hidden.
	struct window_hide_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window hiding event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_hide_event(event_view event) noexcept;
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
		[[nodiscard]] explicit backbuffer_resize_event(event_view event) noexcept;
	};

	/// Event emitted when the mouse enters a window.
	struct window_mouse_enter_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Converts a generic event into a window mouse entering event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_mouse_enter_event(event_view event) noexcept;
	};

	/// Event emitted when the mouse leaves a window.
	struct window_mouse_leave_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window mouse leaving event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_mouse_leave_event(event_view event) noexcept;
	};

	/// Event emitted when a window gains focus.
	struct window_gain_focus_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window focus gaining event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_gain_focus_event(event_view event) noexcept;
	};

	/// Event emitted when a window loses focus.
	struct window_lose_focus_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window focus losing event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_lose_focus_event(event_view event) noexcept;
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
	template <typename Target>
	concept event_type = one_of<Target, quit_event, window_show_event, window_hide_event, backbuffer_resize_event, window_gain_focus_event,
								window_lose_focus_event, window_mouse_enter_event, window_mouse_leave_event, key_down_event, key_up_event,
								text_input_event, mouse_motion_event, mouse_down_event, mouse_up_event, mouse_wheel_event>;
} // namespace tr