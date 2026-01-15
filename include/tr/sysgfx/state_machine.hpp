#pragma once
#include "../utility/benchmark.hpp"

namespace tr {
	namespace sys {
		class event;
	}
	struct state;

	// Shorthand for the return type of most state functions: the pointer to the next state, keep_state, or drop_state.
	using next_state = std::optional<std::unique_ptr<state>>;

	// The base state type.
	struct state {
		// Virtual destructor.
		virtual ~state() noexcept = default;

		// Handles an event.
		virtual next_state handle_event(const tr::sys::event& event);
		// Does a fixed 'tick' update on the state.
		virtual next_state tick();
		// Does a delta-time update on the state.
		virtual next_state update(tr::duration delta);
		// Draws the state.
		virtual void draw();
	};
	// Sentinel indicating that the current state should be kept.
	inline constexpr std::nullopt_t keep_state{std::nullopt};
	// Sentinel indicating that the current state should be dropped.
	inline constexpr std::nullptr_t drop_state{};

	// Convenience function for constructing a next state.
	template <std::derived_from<state> T, class... Args>
		requires(std::constructible_from<T, Args...>)
	next_state make_next_state(Args&&... args);

	// State machine manager class.
	class state_machine {
	  public:
		// Constructs an empty state machine.
		state_machine() = default;

		// Checks whether the state machine is in an empty states.
		bool empty() const;
		// Gets access to the current state.
		template <std::derived_from<state> T> const T& get() const;
		// Gets the tick benchmark.
		const tr::benchmark& tick_benchmark() const;
		// Gets the update benchmark.
		const tr::benchmark& update_benchmark() const;
		// Gets the draw benchmark.
		const tr::benchmark& draw_benchmark() const;

		// Clears the state machine.
		void clear();
		// Emplaces a state.
		template <std::derived_from<state> T, class... Args>
			requires(std::constructible_from<T, Args...>)
		void emplace(Args&&... args);
		// Gets access to the current state.
		template <std::derived_from<state> T> T& get();

		// Handles an event.
		void handle_event(const tr::sys::event& event);
		// Does a fixed 'tick' update on the state.
		void tick();
		// Does a delta-time update on the state.
		template <class R, class P> void update(std::chrono::duration<R, P> delta);
		// Draws the state.
		void draw();

	  private:
		// The currently held state.
		std::unique_ptr<state> m_current_state;
		// Benchmark measuring the tick times.
		benchmark m_tick_benchmark;
		// Benchmark measuring the update times.
		benchmark m_update_benchmark;
		// Benchmark measuring the drawing times.
		benchmark m_draw_benchmark;
	};
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <std::derived_from<tr::state> T, class... Args>
	requires(std::constructible_from<T, Args...>)
tr::next_state tr::make_next_state(Args&&... args)
{
	return std::optional{next_state(std::make_unique<T>(std::forward<Args>(args)...))};
}

template <std::derived_from<tr::state> T> const T& tr::state_machine::get() const
{
	return (const T&)(*m_current_state);
}

template <std::derived_from<tr::state> T, class... Args>
	requires(std::constructible_from<T, Args...>)
void tr::state_machine::emplace(Args&&... args)
{
	m_current_state = std::make_unique<T>(std::forward<Args>(args)...);
}

template <std::derived_from<tr::state> T> T& tr::state_machine::get()
{
	return (T&)(*m_current_state);
}

template <class R, class P> void tr::state_machine::update(std::chrono::duration<R, P> delta)
{
	if (m_current_state != nullptr) {
		m_update_benchmark.start();
		next_state next{m_current_state->update(std::chrono::duration_cast<duration>(delta))};
		m_update_benchmark.stop();
		if (next != keep_state) {
			m_current_state = *std::move(next);
		}
	}
}