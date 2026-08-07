/// @file
/// @brief Provides a graphics benchmark class.

#pragma once
#include "../utility/chrono.hpp"
#include "../utility/handle.hpp"

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
	/// Graphics benchmark.
	/// @details
	/// The graphics benchmark mostly has the same usage pattern as the regular benchmark, with .start() and .stop() methods delineating the
	/// benchmarked region of code, however it has an additional .fetch() method that much be called to make the measurement accessable to
	/// the CPU and add it to the measurement deque.
	class graphics_benchmark
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an empty graphics benchmark.
		/// @param context Graphics context to create the benchmark on.
		graphics_benchmark(graphics_context& context);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the benchmark is on.
		/// @return Reference to the graphics context the benchmark is on.
		graphics_context& context() const;

		/// @}
		/// @name Controls
		/// @{

		/// Starts a new measurement.
		void start();

		/// Stops a measurement.
		void stop();

		/// Fetches the latest measurement from the GPU. This blocks execution until the GPU is done.
		void fetch();

		/// Clears all previous and ongoing measurements from the queue.
		void clear();

		/// @}
		/// @name Results
		/// @{

		/// Gets the duration of the latest measurement.
		/// @return Duration of the latest measurement.
		duration latest() const;

		/// Gets the duration of the shortest available measurement.
		/// @return Duration of the shortest available measurement.
		duration min() const;

		/// Gets the duration of the longest available measurement.
		/// @return Duration of the longest available measurement.
		duration max() const;

		/// Gets the average duration of the available measurements.
		/// @return Average duration of the available measurements.
		duration avg() const;

		/// Gets the available measurements.
		/// @return Reference to the queue of available measurements.
		const std::deque<duration>& measurements() const;

		/// @}

	  private:
		/// Benchmark deleter.
		struct deleter
		{
			/// Reference to the graphics context the benchmark is on.
			graphics_context& context;

			//

			/// Destroys a benchmark.
			/// @param id OpenGL query object ID.
			void operator()(unsigned int id) const;
		};

		//

		/// OpenGL query object handle.
		tr::handle<unsigned int, 0, deleter> m_qo;

		/// Benchmark measurement deque.
		std::deque<duration> m_durations;
	};
} // namespace tr