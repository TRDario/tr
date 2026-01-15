#pragma once
#include "chrono.hpp"
#include <chrono>
#include <deque>

namespace tr {
	// Minimal benchmarking class.
	class benchmark {
	  public:
		// Measurement produced by the benchmark.
		struct measurement {
			// The starting time point of the measurement.
			std::chrono::steady_clock::time_point start;
			// The duration of the measurement.
			duration duration;
		};

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
		const std::deque<measurement>& measurements() const;

	  private:
		// Sentinel starting point for a measurement that hasn't been started.
		static constexpr std::chrono::steady_clock::time_point not_started{};

		// The measurement deque.
		std::deque<measurement> m_measurements;
		// The start point of the latest started (but not ended) measurement.
		std::chrono::steady_clock::time_point m_start{not_started};
	};
} // namespace tr
