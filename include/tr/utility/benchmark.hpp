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
		benchmark() = default;

		// Starts a new measurement.
		void start();
		// Stops a measurement.
		void stop();
		// Clears all previous and ongoing measurements from the queue.
		void clear();

		// Gets the duration of the latest measurement.
		duration latest() const;
		// Gets the duration of the shortest available measurement.
		duration min() const;
		// Gets the duration of the longest available measurement.
		duration max() const;
		// Gets the average duration of the available measurements.
		duration avg() const;
		// Gets the average number of measurements per second.
		double fps() const;
		// Gets the available measurements.
		const deque& measurements() const;

	  private:
		// The measurement deque.
		deque _durations;
		// The start point of the latest started (but not ended) measurement.
		time_point _start;
	};
} // namespace tr
