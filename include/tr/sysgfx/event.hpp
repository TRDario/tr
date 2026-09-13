/// @file
/// @brief Provides event types and related utlities.

#pragma once
#include <tr/sysgfx/keyboard.hpp>
#include <tr/sysgfx/mouse.hpp>
#include <tr/sysgfx/window_view.hpp>

union SDL_Event;
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
		[[nodiscard]] explicit key_down_event(const event& event) noexcept;

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
		[[nodiscard]] explicit key_up_event(const event& event) noexcept;
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
		[[nodiscard]] explicit text_input_event(const event& event) noexcept;
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
		[[nodiscard]] explicit mouse_motion_event(const event& event) noexcept;
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
		[[nodiscard]] explicit mouse_down_event(const event& event) noexcept;
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
		[[nodiscard]] explicit mouse_up_event(const event& event) noexcept;
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
		[[nodiscard]] explicit mouse_wheel_event(const event& event) noexcept;
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
		[[nodiscard]] explicit window_show_event(const event& event) noexcept;
	};

	/// Event emitted when a window is hidden.
	struct window_hide_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window hiding event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_hide_event(const event& event) noexcept;
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
		[[nodiscard]] explicit backbuffer_resize_event(const event& event) noexcept;
	};

	/// Event emitted when the mouse enters a window.
	struct window_mouse_enter_event
	{
		/// View to the window associated with the event.
		window_view window;

		/// Converts a generic event into a window mouse entering event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_mouse_enter_event(const event& event) noexcept;
	};

	/// Event emitted when the mouse leaves a window.
	struct window_mouse_leave_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window mouse leaving event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_mouse_leave_event(const event& event) noexcept;
	};

	/// Event emitted when a window gains focus.
	struct window_gain_focus_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window focus gaining event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_gain_focus_event(const event& event) noexcept;
	};

	/// Event emitted when a window loses focus.
	struct window_lose_focus_event
	{
		/// View to the window associated with the event.
		window_view window;

		//

		/// Converts a generic event into a window focus losing event.
		/// @param event Event to convert.
		[[nodiscard]] explicit window_lose_focus_event(const event& event) noexcept;
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
	concept event_visitor = std::invocable<T, quit_event> && std::invocable<T, window_show_event> && std::invocable<T, window_hide_event> &&
							std::invocable<T, backbuffer_resize_event> && std::invocable<T, window_gain_focus_event> &&
							std::invocable<T, window_lose_focus_event> && std::invocable<T, window_mouse_enter_event> &&
							std::invocable<T, window_mouse_leave_event> && std::invocable<T, key_down_event> &&
							std::invocable<T, key_up_event> && std::invocable<T, text_input_event> &&
							std::invocable<T, mouse_motion_event> && std::invocable<T, mouse_down_event> &&
							std::invocable<T, mouse_up_event> && std::invocable<T, mouse_wheel_event> && std::invocable<T, unknown_event> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, window_show_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, window_hide_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, backbuffer_resize_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, window_gain_focus_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, window_lose_focus_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, window_mouse_enter_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, window_mouse_leave_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, key_down_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, key_up_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, text_input_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, mouse_motion_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, mouse_down_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, mouse_up_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, mouse_wheel_event>> &&
							std::same_as<std::invoke_result_t<T, quit_event>, std::invoke_result_t<T, unknown_event>>;

	/// Valid event visitor type that doesn't throw.
	template <typename T>
	concept nothrow_event_visitor = event_visitor<T> && noexcept(std::declval<T>(std::declval<quit_event>())) &&
									noexcept(std::declval<T>(std::declval<window_show_event>())) &&
									noexcept(std::declval<T>(std::declval<window_hide_event>())) &&
									noexcept(std::declval<T>(std::declval<backbuffer_resize_event>())) &&
									noexcept(std::declval<T>(std::declval<window_gain_focus_event>())) &&
									noexcept(std::declval<T>(std::declval<window_lose_focus_event>())) &&
									noexcept(std::declval<T>(std::declval<window_mouse_enter_event>())) &&
									noexcept(std::declval<T>(std::declval<window_mouse_leave_event>())) &&
									noexcept(std::declval<T>(std::declval<key_down_event>())) &&
									noexcept(std::declval<T>(std::declval<key_up_event>())) &&
									noexcept(std::declval<T>(std::declval<text_input_event>())) &&
									noexcept(std::declval<T>(std::declval<mouse_motion_event>())) &&
									noexcept(std::declval<T>(std::declval<mouse_down_event>())) &&
									noexcept(std::declval<T>(std::declval<mouse_up_event>())) &&
									noexcept(std::declval<T>(std::declval<mouse_wheel_event>())) &&
									noexcept(std::declval<T>(std::declval<unknown_event>()));

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
		[[nodiscard]] bool is() const noexcept
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

		/// Converts the event into a sub-type.
		/// @tparam T Event type.
		/// @pre The event must hold an event of type `T`.
		/// @return Held event subtype.
		template <event_type T>
		[[nodiscard]] T as() const noexcept
		{
			TR_ASSERT(is<T>(), "Tried to convert event to a sub-type it is not.");

			if constexpr (std::same_as<T, quit_event>) {
				return quit_event{};
			}
			else if constexpr (std::same_as<T, window_show_event>) {
				return window_show_event{*this};
			}
			else if constexpr (std::same_as<T, window_hide_event>) {
				return window_hide_event{*this};
			}
			else if constexpr (std::same_as<T, backbuffer_resize_event>) {
				return backbuffer_resize_event{*this};
			}
			else if constexpr (std::same_as<T, window_mouse_enter_event>) {
				return window_mouse_enter_event{*this};
			}
			else if constexpr (std::same_as<T, window_mouse_leave_event>) {
				return window_mouse_leave_event{*this};
			}
			else if constexpr (std::same_as<T, window_gain_focus_event>) {
				return window_gain_focus_event{*this};
			}
			else if constexpr (std::same_as<T, window_lose_focus_event>) {
				return window_lose_focus_event{*this};
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

		/// @}
		/// @name Visiting
		/// @{

		/// Visits the event.
		/// @tparam Visitor Event visitor type.
		/// @param visitor Event visitor.
		/// @return Result returned by the visitor.
		template <event_visitor Visitor>
		[[nodiscard]] auto visit(Visitor&& visitor) const noexcept(nothrow_event_visitor<Visitor>)
		{
			switch (type()) {
			case 0x100:
				return visitor(quit_event{});
			case 0x202:
				return visitor(window_show_event{*this});
			case 0x203:
				return visitor(window_hide_event{*this});
			case 0x207:
				return visitor(backbuffer_resize_event{*this});
			case 0x20C:
				return visitor(window_mouse_enter_event{*this});
			case 0x20D:
				return visitor(window_mouse_leave_event{*this});
			case 0x20E:
				return visitor(window_gain_focus_event{*this});
			case 0x20F:
				return visitor(window_lose_focus_event{*this});
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

		/// @}
		/// @cond sdl_interop
		/// @name SDL interoperability
		/// @{

		/// Unwraps the SDL event.
		/// @return Reference to the unwrapped SDL event.
		[[nodiscard]] const SDL_Event& unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Storage for SDL_Event.
		alignas(8) std::byte m_buffer[128];

		//

		/// Private default constructor.
		[[nodiscard]] event() noexcept = default;

		//

		/// Gets the event subtype.
		/// @return SDL event subtype ID.
		[[nodiscard]] u32 type() const noexcept;
	};
} // namespace tr