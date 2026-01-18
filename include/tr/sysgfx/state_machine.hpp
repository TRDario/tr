///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a state machine class and related functionality.                                                                             //
//                                                                                                                                       //
// The state machine works with the polymorphic tr::state. States inherited from tr::state may overload the                              //
// .handle_event(const tr::sys::event& event) method used to handle incoming events, the .tick() method used to do fixed 'tick' updates, //
// the .update() method used to do delta-time updates, and the .draw() method used to draw the state.                                    //
// .handle_event(), .tick(), and .update() all return tr::next_state, which is a sum type containing either a state, tr::keep_state, or  //
// tr::drop_state. A tr::next_state can be constructed with tr::make_next_state:                                                         //
//     - struct my_state : tr::state { next_state tick() override { return tr::make_next_state<my_other_state>(); } };                   //
//       -> provides a tick function that returns the next state                                                                         //
// If any of the above functions returns tr::keep_state, the state machine keeps that state.                                             //
// If any of the above functions returns tr::drop_state, the state machine drops that state and becomes empty.                           //
// Otherwise, the state machine replaces the current state with the returned one.                                                        //
//                                                                                                                                       //
// The state machine is constructed empty; a state can be emplaced into it with the .emplace<T>(args...) method, and can also be cleared //
// with the .clear() method. Whether a state machine is empty can be queried with the .empty() method. The current state can be accessed //
// using the .get<T>() method:                                                                                                           //
//     - tr::state_machine state_machine -> creates an empty state machine                                                               //
//     - state_machine.empty() -> true                                                                                                   //
//     - state_machine.emplace<my_state>() -> state_machine now holds a state of type 'my_state'                                         //
//     - state_machine.empty() -> false                                                                                                  //
//     - state_machine.get<my_state>() -> gets the state                                                                                 //
//     - state_machine.clear() -> clears the state machine, state_machine is now empty again                                             //
//                                                                                                                                       //
// The state machine provides the methods .handle_event(), .tick(), .update(), and .draw() that call the corresponding method in the     //
// contained state, if one is present. .tick(), .update(), and .draw() are benchmarked internally, the results of which can be queried:  //
//     - state_machine.handle_event(event) -> calls .handle_event() on the contained state, or does nothing if empty                     //
//     - state_machine.tick() -> calls .tick() on the contained state and measures the taken time, or does nothing if empty              //
//     - state_machine.tick_benchmark() -> gets access to the tick benchmark                                                             //
//     - state_machine.update(10ms) -> calls .update() on the contained state and measures the taken time, or does nothing if empty      //
//     - state_machine.update_benchmark() -> gets access to the update benchmark                                                         //
//     - state_machine.draw() -> calls .draw() on the contained state and measures the taken time, or does nothing if empty              //
//     - state_machine.draw_benchmark() -> gets access to the draw benchmark                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/benchmark.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

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
	return (const T&)*m_current_state;
}

template <std::derived_from<tr::state> T, class... Args>
	requires(std::constructible_from<T, Args...>)
void tr::state_machine::emplace(Args&&... args)
{
	m_current_state = std::make_unique<T>(std::forward<Args>(args)...);
}

template <std::derived_from<tr::state> T> T& tr::state_machine::get()
{
	return (T&)*m_current_state;
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