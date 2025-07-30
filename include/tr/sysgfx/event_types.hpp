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
		tick_event(std::uint32_t id);
		// Converts a generic event into a tick event.
		tick_event(const event& event);

		// Converts the event into a generic event.
		operator event() const;
	};

	// Event emitted when the application wants to redraw.
	struct draw_event {
		// The event ID of draw events.
		static inline constexpr std::uint32_t ID{0x8001};

		// Converts the event into a generic event.
		operator event() const;
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
		custom_event_base(std::uint32_t type);
		// Constructs a simple event base.
		custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint);
		// Constructs an event base.
		custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint, std::any&& any1, std::any&& any2);
		// Constructs an event base.
		template <class T1, class T2> custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint, T1&& any1, T2&& any2);
		// Converts an event into an event base.
		custom_event_base(const event& event);
		// Converts an event into an event base.
		custom_event_base(event&& event);

		// Converts an event base into an event.
		operator event() const&;
		// Converts an event base into an event.
		operator event() &&;
	};

	// Unified event type.
	class event {
	  public:
		event() = default;
		// Copy-constructs an event.
		event(const event& r);
		// Move-constructs an event.
		event(event&& r) noexcept;
		// Destroys the event, deallocating any dynamically allocated resources it owned.
		~event();

		// Copy-assigns an event.
		event& operator=(const event& r);
		// Move-assigns an event.
		event& operator=(event&& r) noexcept;

		// Gets the type ID of the event.
		std::uint32_t type() const;

	  private:
		// Storage for SDL_Event.
		alignas(8) std::byte buffer[128];
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class T1, class T2>
tr::custom_event_base::custom_event_base(std::uint32_t type, std::uint32_t uint, std::int32_t sint, T1&& any1, T2&& any2)
	: custom_event_base{type, uint, sint, std::any{std::forward<T1>(any1)}, std::any{std::forward<T2>(any2)}}
{
}