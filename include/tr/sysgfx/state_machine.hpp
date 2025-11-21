#pragma once
#include "../utility/benchmark.hpp"

namespace tr {
	namespace sys {
		class event;
	}

	inline constexpr struct drop_state_t {
	} DROP_STATE; // Special return value signalling the state machine to drop the current state.
	inline constexpr struct keep_state_t {
	} KEEP_STATE; // Special return value signalling the state machine to keep the current state.
	// Alias template for the return type of state functions.
	template <class... States> using next_state = std::variant<drop_state_t, keep_state_t, States...>;

	// State machine manager class.
	// States may define any of handle_event(), tick(), update() or draw(), which will then be called by the state machine when needed.
	// Defaults are provided in case any of these are not provided.
	template <class... States> class state_machine {
	  public:
		// The type that states should return from handle_event, tick and update.
		using next_state = tr::next_state<States...>;

		// Constructs an empty state machine.
		state_machine() = default;

		// Checks whether the state machine is in an empty states.
		bool empty() const;
		// Checks whether the state machine is in a particular state.
		template <one_of<States...> T> bool is() const;
		// Gets a reference to the current state.
		template <one_of<States...> T> const T& as() const;
		// Visits the state machine.
		template <class Visitor> auto visit(Visitor&& visitor) const;

		// Gets the tick benchmark.
		const tr::benchmark& tick_benchmark() const;
		// Gets the update benchmark.
		const tr::benchmark& update_benchmark() const;
		// Gets the draw benchmark.
		const tr::benchmark& draw_benchmark() const;

		// Clears the state machine.
		void clear();
		// Emplaces a state.
		template <class T, class... Args>
			requires(std::constructible_from<T, Args...>)
		void emplace(Args&&... args);
		// Gets a reference to the current state.
		template <one_of<States...> T> T& as();
		// Visits the state machine.
		template <class Visitor> auto visit(Visitor&& visitor);

		// Handles an event.
		void handle_event(const tr::sys::event& event);
		// Does a fixed 'tick' update on the state.
		void tick();
		// Does a delta-time update on the state.
		template <class R, class P> void update(std::chrono::duration<R, P> delta);
		// Draws the state.
		void draw();

	  private:
		// The base state variant.
		std::variant<std::monostate, States...> m_current_state;
		// Benchmark measuring the tick times.
		benchmark m_tick_benchmark;
		// Benchmark measuring the update times.
		benchmark m_update_benchmark;
		// Benchmark measuring the drawing times.
		benchmark m_draw_benchmark;

		struct next_state_assigner;
		struct event_handler;
		struct tick_handler;
		struct update_handler;
		struct draw_handler;
	};
} // namespace tr

#include "state_machine_impl.hpp" // IWYU pragma: keep