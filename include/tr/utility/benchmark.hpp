#pragma once
#include "chrono.hpp"
#include <deque>

namespace tr {
	// Minimal benchmarking class.
	class benchmark {
	  public:
		// Shorthand for the deque type used by the benchmark.
		using deque = std::deque<std::pair<time_point, duration>>;

		// Constructs an empty benchmark.
		benchmark() noexcept = default;

		// Starts a new measurement.
		void start() noexcept;
		// Stops a measurement.
		void stop() noexcept;
		// Clears all previous and ongoing measurements from the queue.
		void clear() noexcept;

		// Gets the duration of the latest measurement.
		duration latest() const noexcept;
		// Gets the duration of the shortest available measurement.
		duration min() const noexcept;
		// Gets the duration of the longest available measurement.
		duration max() const noexcept;
		// Gets the average duration of the available measurements.
		duration avg() const noexcept;
		// Gets the average number of measurements per second.
		double fps() const noexcept;
		// Gets the available measurements.
		const deque& measurements() const noexcept;

	  private:
		// The measurement deque.
		deque _durations;
		// The start point of the latest started (but not ended) measurement.
		time_point _start;
	};
} // namespace tr
