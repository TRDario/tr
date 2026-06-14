///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a graphics benchmark class.                                                                                                  //
//                                                                                                                                       //
// The graphics benchmark mostly has the same usage pattern as the regular benchmark, with .start() and .stop() methods delineating the  //
// benchmarked region of code, however it has an additional .fetch() method that much be called to make the measurement accessable to    //
// the CPU and add it to the measurement deque. Other than that, the benchmark can be cleared much like tr::benchmark:                   //
//     - tr::graphics_benchmark benchmark{context} -> creates an empty benchmark                                                         //
//     - benchmark.start();                                                                                                              //
//       draw_things();                                                                                                                  //
//       benchmark.stop();                                                                                                               //
//       ...                                                                                                                             //
//       window.flip_backbuffer();                                                                                                       //
//       benchmark.fetch();                                                                                                              //
//       -> measures the GPU time of draw_things                                                                                         //
//     - benchmark.clear() -> clears the benchmark measurement deque                                                                     //
//                                                                                                                                       //
// Again, must like the regular benchmark, the latest, fastest, average, and slowest time can be obtained using the appropriate method,  //
// though the graphics benchmark doesn't have an equivalent to tr::benchmark::fps. The measurement deque also doesn't contain starting   //
// time points, unlike the regular benchmark:                                                                                            //
//     - benchmark.latest() -> gets the latest measurement                                                                               //
//     - benchmark.min() -> gets the shortest measurement time                                                                           //
//     - benchmark.avg() -> gets the average of recent measurement times                                                                 //
//     - benchmark.max() -> gets the longest measurement time                                                                            //
//     - benchmark.measurements() -> gets the deque holding recent measurement times                                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/chrono.hpp"
#include "../utility/handle.hpp"

namespace tr {
	class graphics_context;
}

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Graphics benchmark.
	class graphics_benchmark {
	  public:
		// Constructs an empty graphics benchmark.
		graphics_benchmark(graphics_context& context);

		// Gets a reference to the graphics context the benchmark is on.
		graphics_context& context() const;

		// Starts a new measurement.
		void start();
		// Stops a measurement.
		void stop();
		// Fetches the latest measurement from the GPU. This blocks execution until the GPU is done.
		void fetch();
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
		// Gets the available measurements.
		const std::deque<duration>& measurements() const;

	  private:
		struct deleter {
			// Reference to the graphics context the benchmark is on.
			graphics_context& context;

			void operator()(unsigned int id) const;
		};

		// OpenGL query object handle.
		tr::handle<unsigned int, 0, deleter> m_qo;
		// The measurement deque.
		std::deque<duration> m_durations;
	};
} // namespace tr