/// @file
/// @brief Provides a graphics benchmark class.

#pragma once
#include "../utility/chrono.hpp"
#include "../utility/handle.hpp"
#include "../utility/ref.hpp"

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
		[[nodiscard]] explicit graphics_benchmark(graphics_context& context) noexcept;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the benchmark is on.
		/// @return Reference to the graphics context the benchmark is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name Controls
		/// @{

		/// Starts a new measurement.
		void start() noexcept;

		/// Stops a measurement.
		void stop() noexcept;

		/// Fetches the latest measurement from the GPU. This blocks execution until the GPU is done.
		void fetch();

		/// Clears all previous and ongoing measurements from the queue.
		void clear() noexcept;

		/// @}
		/// @name Results
		/// @{

		/// Gets the duration of the latest measurement.
		/// @return Duration of the latest measurement.
		[[nodiscard]] duration latest() const noexcept;

		/// Gets the duration of the shortest available measurement.
		/// @return Duration of the shortest available measurement.
		[[nodiscard]] duration min() const noexcept;

		/// Gets the duration of the longest available measurement.
		/// @return Duration of the longest available measurement.
		[[nodiscard]] duration max() const noexcept;

		/// Gets the average duration of the available measurements.
		/// @return Average duration of the available measurements.
		[[nodiscard]] duration avg() const noexcept;

		/// Gets the available measurements.
		/// @return Reference to the queue of available measurements.
		[[nodiscard]] const std::deque<duration>& measurements() const noexcept;

		/// @}

	  private:
		/// Benchmark deleter.
		struct deleter
		{
			/// Reference to the graphics context the benchmark is on.
			ref<graphics_context> context;

			//

			/// Destroys a benchmark.
			/// @param id OpenGL query object ID.
			void operator()(unsigned int id) const noexcept;
		};

		//

		/// OpenGL query object handle.
		tr::handle<unsigned int, 0, deleter> m_qo;

		/// Benchmark measurement deque.
		std::deque<duration> m_durations;
	};
} // namespace tr