///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a basic benchmark class.                                                                                                     //
//                                                                                                                                       //
// The benchmark is used by surrounding the region of code that needs ot be timed with calls to the .start() and .stop() methods. Other  //
// than that, the benchmark can be cleared of measurements with the .clear() method:                                                     //
//     - tr::benchmark benchmark{} -> creates an empty benchmark                                                                         //
//     - benchmark.start();                                                                                                              //
//       do_things();                                                                                                                    //
//       benchmark.stop();                                                                                                               //
//       -> measures the time of do_things                                                                                               //
//     - benchmark.clear() -> clears the benchmark measurement deque                                                                     //
//                                                                                                                                       //
// The latest, fastest, average, and slowest time can be obtained using the appropriate method, as well as a computed "FPS" value.       //
// The measurement deque can also be directly accessed, containing starting points of the measurements, as well as their durations:      //
//     - gpu_benchmark.latest() -> gets the latest measurement                                                                           //
//     - gpu_benchmark.min() -> gets the shortest measurement time                                                                       //
//     - gpu_benchmark.avg() -> gets the average of recent measurement times                                                             //
//     - gpu_benchmark.max() -> gets the longest measurement time                                                                        //
//     - gpu_benchmark.fps() -> gets the average number of recent measurements in a second                                               //
//     - gpu_benchmark.measurements() -> gets the deque holding recent measurements                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "chrono.hpp"
#include <chrono>
#include <deque>

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Basic benchmarking class.
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
		// The maximum allowed age of a measurement before it is thrown out.
		static constexpr tr::dsecs max_measurement_age{2.5};

		// The measurement deque.
		std::deque<measurement> m_measurements;
		// The start point of the latest started (but not ended) measurement.
		std::chrono::steady_clock::time_point m_start{not_started};
	};
} // namespace tr
