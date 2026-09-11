/// @file
/// @brief Provides `tr::benchmark`.

#pragma once
#include "chrono.hpp"

//

namespace tr
{
	/// Basic benchmarking class.
	class benchmark
	{
	  public:
		/// Measurement produced by the benchmark.
		struct measurement
		{
			// Starting time point of the measurement.
			std::chrono::steady_clock::time_point start;

			// Duration of the measurement.
			duration duration;
		};

		/// @name Constructors
		/// @{

		/// Constructs an empty benchmark.
		[[nodiscard]] benchmark() noexcept = default;

		/// @}
		/// @name Controls
		/// @{

		/// Starts a new measurement.
		void start() noexcept;

		/// Stops a measurement.
		void stop();

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

		/// Gets the average number of measurements per second.
		/// @return Average number of measurements per second.
		[[nodiscard]] double fps() const noexcept;

		/// Gets the available measurements.
		/// @return Reference to the queue of available measurements.
		[[nodiscard]] const std::deque<measurement>& measurements() const noexcept;

	  private:
		/// Sentinel starting point for a measurement that hasn't been started.
		static constexpr std::chrono::steady_clock::time_point not_started{};

		/// Maximum allowed age of a measurement before it is thrown out.
		static constexpr tr::dsecs max_measurement_age{2.5};

		//

		/// Benchmark measurement deque.
		std::deque<measurement> m_measurements;

		/// Starting time point of the latest started (but not ended) measurement.
		std::chrono::steady_clock::time_point m_start{not_started};
	};
} // namespace tr
