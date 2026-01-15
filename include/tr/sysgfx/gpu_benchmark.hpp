///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a GPU benchmark class.                                                                                                       //
//                                                                                                                                       //
// The GPU benchmark mostly has the same usage pattern as the regular benchmark, with .start() and .stop() methods delineating the       //
// benchmarked region of code, however it has an additional .fetch() method that much be called to make the measurement accessable to    //
// the CPU and add it to the measurement deque. Other than that, the benchmark can be cleared much like tr::benchmark:                   //
//     - tr::gfx::gpu_benchmark gpu_benchmark{} -> creates an empty benchmark                                                            //
//     - gpu_benchmark.start();                                                                                                          //
//       draw_things();                                                                                                                  //
//       gpu_benchmark.stop();                                                                                                           //
//       ...                                                                                                                             //
//       tr::gfx::flip_backbuffer();                                                                                                     //
//       gpu_benchmark.fetch();                                                                                                          //
//       -> measures the GPU time of draw_things                                                                                         //
//     - gpu_benchmark.clear() -> clears the benchmark measurement deque                                                                 //
//                                                                                                                                       //
// Again, must like the regular benchmark, the latest, fastest, average, and slowest time can be obtained using the appropriate method,  //
// though the GPU benchmark doesn't have an equivalent to tr::benchmark::fps. The measurement deque also doesn't contain starting time   //
// points, unlike the regular benchmark:                                                                                                 //
//     - gpu_benchmark.latest() -> gets the latest measurement                                                                           //
//     - gpu_benchmark.min() -> gets the shortest measurement time                                                                       //
//     - gpu_benchmark.avg() -> gets the average of recent measurement times                                                             //
//     - gpu_benchmark.max() -> gets the longest measurement time                                                                        //
//     - gpu_benchmark.measurements() -> gets the deque holding recent measurement times                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/handle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// GPU benchmark.
	class gpu_benchmark {
	  public:
		// Shorthand for the deque type used by the benchmark.
		using deque = std::deque<duration>;

		// Constructs an empty GPU benchmark.
		gpu_benchmark();

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
		const deque& measurements() const;

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// OpenGL query object handle.
		tr::handle<unsigned int, 0, deleter> m_qo;
		// The measurement deque.
		deque m_durations;
	};
} // namespace tr::gfx