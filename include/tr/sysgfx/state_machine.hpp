/// @file
/// @brief Provides `tr::state`, `tr::state_machine` and related functionality.

#pragma once
#include "../utility/benchmark.hpp"

namespace tr
{
	class event;
	struct state;
} // namespace tr

//

namespace tr
{
	/// Tag struct indicating that the current state should be kept.
	struct keep_state_t
	{
	};

	/// Tag struct indicating that the current state should be dropped.
	struct drop_state_t
	{
	};

	/// Shorthand for the return type of most `tr::state` functions: owning pointer to the next state, `tr::keep_state()`, or
	/// `tr::drop_state()`.
	using next_state = std::variant<keep_state_t, drop_state_t, std::unique_ptr<state>>;

	/// State interface used by `tr::state_machine`.
	struct state
	{
		/// @name Constructors
		/// @{

		/// Virtual destructor.
		virtual ~state() noexcept = default;

		/// @}
		/// @name Interface
		/// @{

		/// Handles an event.
		/// @param event Event to handle.
		/// @return Pointer to the next state, or one of the tag structs.
		virtual next_state handle_event(const event& event);

		/// Updates the state.
		/// @param delta Time since the last update.
		/// @return Pointer to the next state, or one of the tag structs.
		virtual next_state update(duration delta);

		/// Draws the state.
		virtual void draw();

		/// @}
	};

	/// @name State machine
	/// @{

	/// Returns a sentinel indicating that the current state should be kept.
	/// @return Sentinel indicating that the current state should be kept
	consteval next_state keep_state();

	/// Returns a sentinel indicating that the current state should be dropped.
	/// @return Sentinel indicating that the current state should be dropped.
	consteval next_state drop_state();

	/// Convenience function for constructing a next state.
	/// @tparam State State type to construct.
	/// @tparam Args Types of the arguments passed to the state constructor.
	/// @param args Arguments passed to the state constructor.
	/// @return Owning pointer to the next state.
	template <std::derived_from<state> State, typename... Args>
		requires(std::constructible_from<State, Args...>)
	next_state make_next_state(Args&&... args);

	/// @}

	/// State machine manager.
	/// @details
	/// Instances of `tr::state_machine` work with the polymorphic `tr::state` interface. States inherited from `tr::state` may overload the
	/// `handle_event()` method used to handle incoming events, the `update()` method used to update the state, and the `draw()` method used
	/// to draw the state.
	///
	/// `handle_event()` and `update()` return `tr::next_state`, which is a sum type containing either a state, `tr::keep_state()`, or
	/// `tr::drop_state()`. If `tr::keep_state()` is returned, the state machine keeps the current state, and if `tr::drop_state()` is
	/// returned, the state machine drops the current state and becomes empty.
	///
	/// `update()` and `draw()` are benchmarked internally and their benchmarks are gettable.
	///
	/// `tr::state_machine` instances are movable, but not copyable. A moved-from instance of `tr::state_machine` reverts to being empty.
	class state_machine
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an empty state machine.
		state_machine() = default;

		/// @}
		/// @name Status
		/// @{

		/// Checks whether the state machine is in an empty state.
		/// @return `true` if the state machine does not contain a state, `false` otherwise.
		bool empty() const;

		/// Gets access to the current state.
		/// @tparam State Type of the current state.
		/// @pre A state must be contained in the state machine.
		/// @return Reference to the current state.
		template <std::derived_from<state> State>
		const State& get() const;

		/// Gets the update benchmark.
		/// @return Reference to the update benchmark.
		const benchmark& update_benchmark() const;

		/// Gets the draw benchmark.
		/// @return Reference to the draw benchmark.
		const benchmark& draw_benchmark() const;

		/// @}
		/// @name State manipulation
		/// @{

		/// Clears the state machine.
		void clear();

		/// Emplaces a state.
		/// @tparam State State type to construct.
		/// @tparam Args Types of the arguments passed to the state constructor.
		/// @param args Arguments passed to the state constructor.
		template <std::derived_from<state> State, typename... Args>
			requires(std::constructible_from<State, Args...>)
		void emplace(Args&&... args);

		/// Gets access to the current state.
		/// @tparam State Type of the current state.
		/// @pre A state must be contained in the state machine.
		/// @return Reference to the current state.
		template <std::derived_from<state> State>
		State& get();

		/// @}
		/// @name State management
		/// @{

		/// Handles an event.
		/// @param event Event to handle.
		void handle_event(const event& event);

		/// Updates the current state.
		/// @tparam Rep Duration representation type.
		/// @tparam Period Duration period type.
		/// @param delta Time since the last update.
		template <typename Rep, typename Period>
		void update(std::chrono::duration<Rep, Period> delta);

		/// Draws the current state.
		void draw();

		/// @}

	  private:
		/// Currently held state.
		std::unique_ptr<state> m_current_state;

		/// Benchmark measuring the update times.
		benchmark m_update_benchmark;

		/// Benchmark measuring the drawing times.
		benchmark m_draw_benchmark;

		//

		/// Keeps the current state.
		void handle_next_state(keep_state_t);

		/// Drops the current state.
		void handle_next_state(drop_state_t);

		/// Assigns a new current state.
		/// @param next Next held state.
		void handle_next_state(std::unique_ptr<state>&& next);
	};
} // namespace tr

#include "impl/state_machine.hpp" // IWYU pragma: export