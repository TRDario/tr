#pragma once
#include "../utility/common.hpp"

namespace tr {
	class event;

	// ID of the first user defined event.
	inline constexpr std::uint32_t USER_EVENT_START{0x9000};

	// Event emitted when the application wants to quit.
	struct quit_event {
		// The event ID of quit events.
		static inline constexpr std::uint32_t ID{0x100};
	};

	// The event type emitted by ticker timers.
	struct tick_event {
		// The event ID of tick events.
		static inline constexpr std::uint32_t ID{0x8000};

		// The user-assigned type ID attached to the ticker emitting the event.
		std::uint32_t id;

		// Constructs a tick event.
		tick_event(std::uint32_t id) noexcept;
		// Converts a generic event into a tick event.
		tick_event(const event& event) noexcept;

		// Converts the event into a generic event.
		operator event() const noexcept;
	};

	// Event emitted when the application wants to redraw.
	struct draw_event {
		// The event ID of draw events.
		static inline constexpr std::uint32_t ID{0x8001};

		// Converts the event into a generic event.
		operator event() const noexcept;
	};

	// Intermediate interface between custom event types and tr::event.
	struct custom_event_base {
		// The type of the event.
		std::uint32_t type;
		// An unsigned integer value.
		std::uint32_t uint;
		// A signed integer value.
		std::int32_t sint;
		// A generic value.
		std::any any1;
		// A generic value.
		std::any any2;

		// Constructs a simple event base.
		custom_event_base(std::uint32_t type) noexcept;
		// Constructs a simple event base.
		custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint) noexcept;
		// Constructs an event base.
		custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint, std::any&& any1, std::any&& any2) noexcept;
		// Constructs an event base.
		template <class T1, class T2> custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint, T1&& any1, T2&& any2);
		// Converts an event into an event base.
		custom_event_base(const event& event);
		// Converts an event into an event base.
		custom_event_base(event&& event) noexcept;

		// Converts an event base into an event.
		operator event() const&;
		// Converts an event base into an event.
		operator event() && noexcept;
	};

	// Unified event type.
	class event {
	  public:
		// Copy-constructs an event.
		event(const event& r);
		// Move-constructs an event.
		event(event&& r) noexcept;
		// Destroys the event, deallocating any dynamically allocated resources it owned.
		~event() noexcept;

		// Copy-assigns an event.
		event& operator=(const event& r);
		// Move-assigns an event.
		event& operator=(event&& r) noexcept;

		// Gets the type ID of the event.
		std::uint32_t type() const noexcept;

	  private:
		// Storage for SDL_Event.
		alignas(8) std::byte _impl[128];

		event() noexcept = default;

		friend struct custom_event_base;
		friend struct event_queue;

		friend struct key_down_event;
		friend struct key_up_event;
		friend struct text_input_event;
		friend struct mouse_motion_event;
		friend struct mouse_down_event;
		friend struct mouse_up_event;
		friend struct mouse_wheel_event;
		friend struct backbuffer_resize_event;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class T1, class T2>
tr::custom_event_base::custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint, T1&& any1, T2&& any2)
	: custom_event_base{type, uint, sint, std::any{std::forward<T1>(any1)}, std::any{std::forward<T2>(any2)}}
{
}