#pragma once
#include "../utility/benchmark.hpp"

namespace tr {
	class event;

	// ID of a state with no special given ID.
	constexpr std::uint32_t NO_ID{0};

	// Abstract game state interface.
	struct state {
		virtual ~state() = default;

		// Gets the type of the state.
		virtual std::uint32_t type() const noexcept;
		// Handles an event.
		// Returning nullptr mains the current state, drop_state will drop the current state, any other pointer will replace the state.
		virtual std::unique_ptr<state> handle_event(const event& event) = 0;
		// Updates the state.
		// Returning nullptr mains the current state, drop_state will drop the current state, any other pointer will replace the state.
		virtual std::unique_ptr<state> update(duration delta) = 0;
		// Draws the state to the screen.
		virtual void draw() = 0;
	};

	// Sentinel state thats tells the state manager to drop the current state when returned from a state operation.
	struct drop_state : state {
		std::uint32_t type() const noexcept override;
		std::unique_ptr<state> handle_event(const event&) noexcept override;
		std::unique_ptr<state> update(duration) noexcept override;
		void draw() noexcept override;
	};

	// Game state manager.
	class state_manager {
	  public:
		// The currently held state.
		std::unique_ptr<state> state;

		// Constructs a state manager.
		state_manager(std::unique_ptr<tr::state>&& state = nullptr) noexcept;

		// Gets the benchmark that measures update time.
		const benchmark& update_benchmark() const noexcept;
		// Gets the benchmark that measures drawing time.
		const benchmark& draw_benchmark() const noexcept;

		// Passes an event to the held state.
		void handle_event(const event& event);
		// Updates the state.
		void update(duration delta);
		// Updates the state.
		template <class R, class P> void update(std::chrono::duration<R, P> delta);
		// Draws the state.
		void draw();

	  private:
		// Benchmark measuring the update times.
		benchmark _update_benchmark;
		// Benchmark measuring the drawing times.
		benchmark _draw_benchmark;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class R, class P> void tr::state_manager::update(std::chrono::duration<R, P> delta)
{
	update(duration_cast<duration>(delta));
}