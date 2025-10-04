#pragma once
#include "../utility/handle.hpp"

namespace tr::gfx {
	class gpu_benchmark {
	  public:
		// Shorthand for the deque type used by the benchmark.
		using deque = std::deque<std::pair<time_point, duration>>;

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
		// Gets the average number of measurements per second.
		double fps() const;
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