/// @file
/// @brief Provides miscellaneous chrono utilities.

#pragma once
#include "common.hpp"

//

namespace tr
{
	/// Standard duration datatype.
	using duration = std::chrono::nanoseconds;

	inline namespace aliases
	{
		/// Namespace containing common chrono aliases.
		inline namespace chrono_aliases
		{
			/// Duration in seconds.
			/// @tparam Rep Representation type of the duration.
			template <typename Rep>
			using secs = std::chrono::duration<Rep>;

			/// Duration in seconds with a float representation.
			using fsecs = secs<float>;

			/// Duration in seconds with a double representation.
			using dsecs = secs<double>;

			/// Duration in seconds with an integer representation.
			using isecs = std::chrono::seconds;

			//

			/// Duration in milliseconds.
			/// @tparam Rep Representation type of the duration.
			template <typename Rep>
			using msecs = std::chrono::duration<Rep, std::milli>;

			/// Duration in milliseconds with a float representation.
			using fmsecs = msecs<float>;

			/// Duration in milliseconds with a double representation.
			using dmsecs = msecs<double>;

			/// Duration in milliseconds with an integer representation.
			using imsecs = std::chrono::milliseconds;

			//

			/// Duration in microseconds.
			/// @tparam Rep Representation type of the duration.
			template <typename Rep>
			using usecs = std::chrono::duration<Rep, std::micro>;

			/// Duration in microseconds with a float representation.
			using fusecs = usecs<float>;

			/// Duration in microseconds with a double representation.
			using dusecs = usecs<double>;

			/// Duration in microseconds with an integer representation.
			using iusecs = std::chrono::microseconds;

			//

			/// Duration in nanoseconds.
			/// @tparam Rep Representation type of the duration.
			template <typename Rep>
			using nsecs = std::chrono::duration<Rep, std::nano>;

			/// Duration in nanoseconds with a float representation.
			using fnsecs = nsecs<float>;

			/// Duration in nanoseconds with a double representation.
			using dnsecs = nsecs<double>;

			/// Duration in nanoseconds with an integer representation.
			using insecs = std::chrono::nanoseconds;
		} // namespace chrono_aliases
	} // namespace aliases

	//

	inline namespace literals
	{
		/// Namespace containing additional chrono literals.
		inline namespace chrono_literals
		{
			/// @name Literals
			/// @{

			/// Creates a duration from a frequency in hertz.
			/// @param hz Frequency in hertz.
			/// @return Duration `1s / hz`.
			consteval duration operator""_hz(unsigned long long hz);

			/// Creates a duration from a frequency in hertz.
			/// @param hz Frequency in hertz.
			/// @return Duration `1s / hz`.
			consteval duration operator""_hz(long double hz);

			/// @}
		} // namespace chrono_literals
	} // namespace literals

	/// @name Chrono
	/// @{

	/// Computes the ratio between two durations.
	/// @tparam Rep1 Representation type of the first duration.
	/// @tparam Rep2 Representation type of the second duration.
	/// @tparam Period1 Period of the first duration.
	/// @tparam Period2 Period of the second duration.
	/// @param a, b Durations to get the ratio between.
	/// @return `a/b`.
	template <typename Rep1, typename Rep2, typename Period1, typename Period2>
	constexpr float ratio(std::chrono::duration<Rep1, Period1> a, std::chrono::duration<Rep2, Period2> b);

	//

	/// Cross-platform, thread-safe localtime.
	/// @param time Timestamp value.
	/// @return Local time information.
	std::tm localtime(std::time_t time);

	/// @}
} // namespace tr

#include "impl/chrono.hpp" // IWYU pragma: export