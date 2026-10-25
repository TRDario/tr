/// @file
/// @brief Provides `tr::event_view`.

#pragma once
#include <tr/sysgfx/event_types.hpp>
#include <tr/utility/ref.hpp>

union SDL_Event;

//

namespace tr
{
	/// Valid event view visitor type.
	/// @details Event visitors must be callable with all event types, and all overloads must return the same type.
	template <typename Target>
	concept event_view_visitor =
		std::invocable<Target, quit_event> && std::invocable<Target, window_show_event> && std::invocable<Target, window_hide_event> &&
		std::invocable<Target, backbuffer_resize_event> && std::invocable<Target, window_gain_focus_event> &&
		std::invocable<Target, window_lose_focus_event> && std::invocable<Target, window_mouse_enter_event> &&
		std::invocable<Target, window_mouse_leave_event> && std::invocable<Target, key_down_event> &&
		std::invocable<Target, key_up_event> && std::invocable<Target, text_input_event> && std::invocable<Target, mouse_motion_event> &&
		std::invocable<Target, mouse_down_event> && std::invocable<Target, mouse_up_event> && std::invocable<Target, mouse_wheel_event> &&
		std::invocable<Target, unknown_event> &&
		all_same<std::invoke_result_t<Target, quit_event>, std::invoke_result_t<Target, window_show_event>,
				 std::invoke_result_t<Target, window_hide_event>, std::invoke_result_t<Target, backbuffer_resize_event>,
				 std::invoke_result_t<Target, window_gain_focus_event>, std::invoke_result_t<Target, window_lose_focus_event>,
				 std::invoke_result_t<Target, window_mouse_enter_event>, std::invoke_result_t<Target, window_mouse_leave_event>,
				 std::invoke_result_t<Target, key_down_event>, std::invoke_result_t<Target, key_up_event>,
				 std::invoke_result_t<Target, text_input_event>, std::invoke_result_t<Target, mouse_motion_event>,
				 std::invoke_result_t<Target, mouse_down_event>, std::invoke_result_t<Target, mouse_up_event>,
				 std::invoke_result_t<Target, mouse_wheel_event>, std::invoke_result_t<Target, unknown_event>>;

	/// Valid event view visitor type that doesn't throw.
	template <typename Target>
	concept nothrow_event_view_visitor = event_view_visitor<Target> && noexcept(std::declval<Target>(std::declval<quit_event>())) &&
										 noexcept(std::declval<Target>(std::declval<window_show_event>())) &&
										 noexcept(std::declval<Target>(std::declval<window_hide_event>())) &&
										 noexcept(std::declval<Target>(std::declval<backbuffer_resize_event>())) &&
										 noexcept(std::declval<Target>(std::declval<window_gain_focus_event>())) &&
										 noexcept(std::declval<Target>(std::declval<window_lose_focus_event>())) &&
										 noexcept(std::declval<Target>(std::declval<window_mouse_enter_event>())) &&
										 noexcept(std::declval<Target>(std::declval<window_mouse_leave_event>())) &&
										 noexcept(std::declval<Target>(std::declval<key_down_event>())) &&
										 noexcept(std::declval<Target>(std::declval<key_up_event>())) &&
										 noexcept(std::declval<Target>(std::declval<text_input_event>())) &&
										 noexcept(std::declval<Target>(std::declval<mouse_motion_event>())) &&
										 noexcept(std::declval<Target>(std::declval<mouse_down_event>())) &&
										 noexcept(std::declval<Target>(std::declval<mouse_up_event>())) &&
										 noexcept(std::declval<Target>(std::declval<mouse_wheel_event>())) &&
										 noexcept(std::declval<Target>(std::declval<unknown_event>()));

	//

	/// View to a generic event.
	/// @details
	/// `tr::event_view` represents a view to a generic system event. It is designed to be cheap to pass around and copy.
	class event_view
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// @cond sdl_interop

		/// Wraps an SDL event.
		/// @param event Reference to an event to wrap.
		[[nodiscard]] event_view(const SDL_Event& event) noexcept;

		/// @endcond

		/// Event views are trivially copyable.
		[[nodiscard]] event_view(const event_view& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Event views are trivially copyable.
		[[nodiscard]] event_view& operator=(const event_view& rhs) noexcept = default;

		/// @}
		/// @name Alternatives
		/// @{

		/// Checks whether the event is of a certain type.
		/// @tparam Event Event type.
		/// @return Whether the event holds an event of type `Event`.
		template <event_type Event>
		[[nodiscard]] bool is() const noexcept
		{
			if constexpr (std::same_as<Event, quit_event>) {
				return type() == 0x100;
			}
			else if constexpr (std::same_as<Event, window_show_event>) {
				return type() == 0x202;
			}
			else if constexpr (std::same_as<Event, window_hide_event>) {
				return type() == 0x203;
			}
			else if constexpr (std::same_as<Event, backbuffer_resize_event>) {
				return type() == 0x207;
			}
			else if constexpr (std::same_as<Event, window_mouse_enter_event>) {
				return type() == 0x20C;
			}
			else if constexpr (std::same_as<Event, window_mouse_leave_event>) {
				return type() == 0x20D;
			}
			else if constexpr (std::same_as<Event, window_gain_focus_event>) {
				return type() == 0x20E;
			}
			else if constexpr (std::same_as<Event, window_lose_focus_event>) {
				return type() == 0x20F;
			}
			else if constexpr (std::same_as<Event, key_down_event>) {
				return type() == 0x300;
			}
			else if constexpr (std::same_as<Event, key_up_event>) {
				return type() == 0x301;
			}
			else if constexpr (std::same_as<Event, text_input_event>) {
				return type() == 0x303;
			}
			else if constexpr (std::same_as<Event, mouse_motion_event>) {
				return type() == 0x400;
			}
			else if constexpr (std::same_as<Event, mouse_down_event>) {
				return type() == 0x401;
			}
			else if constexpr (std::same_as<Event, mouse_up_event>) {
				return type() == 0x402;
			}
			else if constexpr (std::same_as<Event, mouse_wheel_event>) {
				return type() == 0x403;
			}
		}

		/// Converts the event into a sub-type.
		/// @tparam Event Event type.
		/// @pre The event must hold an event of type `Event`.
		/// @return Held event subtype.
		template <event_type Event>
		[[nodiscard]] Event as() const noexcept
		{
			TR_ASSERT(is<Event>(), "Tried to convert event to a sub-type it is not.");

			if constexpr (std::same_as<Event, quit_event>) {
				return quit_event{};
			}
			else if constexpr (std::same_as<Event, window_show_event>) {
				return window_show_event{*this};
			}
			else if constexpr (std::same_as<Event, window_hide_event>) {
				return window_hide_event{*this};
			}
			else if constexpr (std::same_as<Event, backbuffer_resize_event>) {
				return backbuffer_resize_event{*this};
			}
			else if constexpr (std::same_as<Event, window_mouse_enter_event>) {
				return window_mouse_enter_event{*this};
			}
			else if constexpr (std::same_as<Event, window_mouse_leave_event>) {
				return window_mouse_leave_event{*this};
			}
			else if constexpr (std::same_as<Event, window_gain_focus_event>) {
				return window_gain_focus_event{*this};
			}
			else if constexpr (std::same_as<Event, window_lose_focus_event>) {
				return window_lose_focus_event{*this};
			}
			else if constexpr (std::same_as<Event, key_down_event>) {
				return key_down_event{*this};
			}
			else if constexpr (std::same_as<Event, key_up_event>) {
				return key_up_event{*this};
			}
			else if constexpr (std::same_as<Event, text_input_event>) {
				return text_input_event{*this};
			}
			else if constexpr (std::same_as<Event, mouse_motion_event>) {
				return mouse_motion_event{*this};
			}
			else if constexpr (std::same_as<Event, mouse_down_event>) {
				return mouse_down_event{*this};
			}
			else if constexpr (std::same_as<Event, mouse_up_event>) {
				return mouse_up_event{*this};
			}
			else if constexpr (std::same_as<Event, mouse_wheel_event>) {
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
		template <event_view_visitor Visitor>
		[[nodiscard]] auto visit(Visitor&& visitor) const noexcept(nothrow_event_view_visitor<Visitor>)
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
		/// Reference to an SDL event.
		tr::ref<const SDL_Event> m_ref;

		//

		/// Gets the event subtype.
		/// @return SDL event subtype ID.
		[[nodiscard]] u32 type() const noexcept;
	};
} // namespace tr