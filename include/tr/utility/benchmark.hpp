/// @file
/// @brief Provides `tr::benchmark`.

#pragma once
#include <tr/utility/chrono.hpp>

//

namespace tr
{
	/// @ingroup utility
	/// @brief Benchmark object recording and aggregating execution time measurements for a region of code delineated with `start()` and
	/// `stop()`.
	/// @details
	/// `tr::benchmark` is intended to be used to benchmark sections of code that execute in the order of nanoseconds - milliseconds that
	/// are executed repeatedly, possibly many times a second. Measurements are stored for up to 2 seconds and then discarded, which may
	/// interfere with longer or more infrequent measurements.
	class benchmark
	{
	  public:
		/// @brief Measurement produced by the benchmark.
		struct measurement
		{
			/// @brief Starting time point of the measurement.
			std::chrono::steady_clock::time_point start;

			/// @brief Duration of the measurement.
			duration duration;
		};

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty benchmark.
		[[nodiscard]] benchmark() noexcept = default;

		/// @}
		/// @name Measurement
		/// @{

		/// @brief Starts a new measurement.
		void start() noexcept;

		/// @brief Stops a measurement.
		void stop() noexcept;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Clears all previous and ongoing measurements from the queue.
		void clear() noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the duration of the latest measurement.
		/// @return Duration of the latest measurement, or 0s if there are no available measurements.
		[[nodiscard]] duration latest() const noexcept;

		/// @brief Returns the duration of the shortest available measurement.
		/// @return Duration of the shortest available measurement, or 0s if there are no available measurements.
		[[nodiscard]] duration min() const noexcept;

		/// @brief Returns the duration of the longest available measurement.
		/// @return Duration of the longest available measurement, or 0s if there are no available measurements.
		[[nodiscard]] duration max() const noexcept;

		/// @brief Returns the average duration of the available measurements.
		/// @return Average duration of the available measurements, or 0s if there are no available measurements.
		[[nodiscard]] duration avg() const noexcept;

		/// @brief Returns the average number of measurements per second.
		/// @return Average number of measurements per second, or 0 if there are no available measurements.
		[[nodiscard]] double fps() const noexcept;

		/// @brief Returns the available measurements.
		/// @return Reference to the queue of available measurements.
		[[nodiscard]] const std::deque<measurement>& measurements() const noexcept;

	  private:
		/// @brief Sentinel starting point for a measurement that hasn't been started.
		static constexpr std::chrono::steady_clock::time_point not_started{};

		/// @brief Maximum allowed age of a measurement before it is thrown out.
		static constexpr tr::dsecs max_measurement_age{2.5};

		//

		/// @brief Benchmark measurement deque.
		std::deque<measurement> m_measurements;

		/// @brief Starting time point of the latest started (but not ended) measurement.
		std::chrono::steady_clock::time_point m_start{not_started};
	};
} // namespace tr
