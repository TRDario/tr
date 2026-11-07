/// @file
/// @brief Provides miscellaneous chrono utilities.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// @ingroup wrappers
	/// @defgroup chrono Chrono
	/// @brief Contains miscellaneous chrono-related utilities.

	/// @ingroup chrono
	/// @brief Standard duration datatype.
	using duration = std::chrono::nanoseconds;

	inline namespace aliases
	{
		/// @ingroup chrono
		/// @brief Namespace containing common chrono aliases.
		inline namespace chrono_aliases
		{
			/// @brief Duration in seconds.
			/// @tparam Rep Representation type of the duration.
			template <typename Rep>
			using secs = std::chrono::duration<Rep>;

			/// @brief Duration in seconds with a float representation.
			using fsecs = secs<float>;

			/// @brief Duration in seconds with a double representation.
			using dsecs = secs<double>;

			/// @brief Duration in seconds with an integer representation.
			using isecs = std::chrono::seconds;

			//

			/// @brief Duration in milliseconds.
			/// @tparam Rep Representation type of the duration.
			template <typename Rep>
			using msecs = std::chrono::duration<Rep, std::milli>;

			/// @brief Duration in milliseconds with a float representation.
			using fmsecs = msecs<float>;

			/// @brief Duration in milliseconds with a double representation.
			using dmsecs = msecs<double>;

			/// @brief Duration in milliseconds with an integer representation.
			using imsecs = std::chrono::milliseconds;

			//

			/// @brief Duration in microseconds.
			/// @tparam Rep Representation type of the duration.
			template <typename Rep>
			using usecs = std::chrono::duration<Rep, std::micro>;

			/// @brief Duration in microseconds with a float representation.
			using fusecs = usecs<float>;

			/// @brief Duration in microseconds with a double representation.
			using dusecs = usecs<double>;

			/// @brief Duration in microseconds with an integer representation.
			using iusecs = std::chrono::microseconds;

			//

			/// @brief Duration in nanoseconds.
			/// @tparam Rep Representation type of the duration.
			template <typename Rep>
			using nsecs = std::chrono::duration<Rep, std::nano>;

			/// @brief Duration in nanoseconds with a float representation.
			using fnsecs = nsecs<float>;

			/// @brief Duration in nanoseconds with a double representation.
			using dnsecs = nsecs<double>;

			/// @brief Duration in nanoseconds with an integer representation.
			using insecs = std::chrono::nanoseconds;
		} // namespace chrono_aliases
	} // namespace aliases

	//

	inline namespace literals
	{
		/// @ingroup chrono
		/// @brief Namespace containing additional chrono literals.
		inline namespace chrono_literals
		{
			/// @brief Returns a duration derived from a frequency in hertz.
			/// @param hz Frequency in hertz.
			/// @return Duration `1s / hz`.
			[[nodiscard]] consteval duration operator""_hz(unsigned long long hz) noexcept
			{
				return duration{isecs{1}} / static_cast<long long>(hz);
			}

			/// @brief Returns a duration derived from a frequency in hertz.
			/// @param hz Frequency in hertz.
			/// @return Duration `1s / hz`.
			[[nodiscard]] consteval duration operator""_hz(long double hz) noexcept
			{
				return std::chrono::duration_cast<duration>(duration{isecs{1}} / hz);
			}
		} // namespace chrono_literals
	} // namespace literals

	//

	/// @ingroup chrono
	/// @brief Returns the ratio between two durations.
	/// @tparam Rep1 Representation type of the first duration.
	/// @tparam Rep2 Representation type of the second duration.
	/// @tparam Period1 Period of the first duration.
	/// @tparam Period2 Period of the second duration.
	/// @param a, b Durations to get the ratio between.
	/// @return `a/b`.
	template <typename Rep1, typename Rep2, typename Period1, typename Period2>
	[[nodiscard]] constexpr float ratio(std::chrono::duration<Rep1, Period1> a, std::chrono::duration<Rep2, Period2> b) noexcept
	{
		return float(dnsecs{a} / dnsecs{b});
	}

	//

	/// @ingroup chrono
	/// @brief Converts time since epoch into calendar time expressed in local time and returns it.
	/// @details This function is a cross-platform thread-safe replacement of `std::localtime()`.
	/// @param time Time since epoch.
	/// @return Local time information.
	[[nodiscard]] std::tm localtime(std::time_t time) noexcept;
} // namespace tr