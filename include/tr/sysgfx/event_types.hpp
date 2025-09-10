#pragma once
#include "../utility/common.hpp"

namespace tr::sys {
	class event;

	// Event emitted when the application wants to quit.
	struct quit_event {
		// The event ID of quit events.
		static inline constexpr u32 ID{0x100};
	};

	// The event type emitted by ticker timers.
	struct tick_event {
		// The event ID of tick events.
		static inline constexpr u32 ID{0x8000};

		// The user-assigned type ID attached to the ticker emitting the event.
		int id;

		// Converts a generic event into a tick event.
		tick_event(const event& event);
	};

	// Event emitted when the application wants to redraw.
	struct draw_event {
		// The event ID of draw events.
		static inline constexpr u32 ID{0x8001};
	};

	// Unified event type.
	class event {
	  public:
		// Gets the type ID of the event.
		u32 type() const;

	  private:
		// Storage for SDL_Event.
		alignas(8) std::byte m_buffer[128];

		event() = default;
	};
} // namespace tr::sys