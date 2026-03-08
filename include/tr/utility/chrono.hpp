///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous chrono-related functionality.                                                                                  //
//                                                                                                                                       //
// The alias tr::duration serves as the standard duration datatype in tr.                                                                //
//                                                                                                                                       //
// Shortened alises for various duration types are provided in tr::chrono_aliases, as well as tr::aliases.                               //
//     - tr::secs<T>, tr::fsecs, tr::dsecs, tr::isecs                                                                                    //
//     - tr::msecs<T>, tr::fmsecs, tr::dmsecs, tr::imsecs                                                                                //
//     - tr::usecs<T>, tr::fusecs, tr::dusecs, tr::iusecs                                                                                //
//     - tr::nsecs<T>, tr::fnsecs, tr::dnsecs, tr::insecs                                                                                //
//                                                                                                                                       //
// The literal _hz is provided as a way to construct durations from a frequency:                                                         //
//     - 100_hz -> 10ms                                                                                                                  //
//     - 0.5_hz -> 2s                                                                                                                    //
//                                                                                                                                       //
// tr::ratio is provided as way to get a ratio between any two durations without having to think about casting:                          //
//     - tr::ratio(10ms, 30ms) -> 1.0f / 3                                                                                               //
//                                                                                                                                       //
// tr::localtime is provided as a cross-platform and thread-safe implementation of localtime.                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Alias for the standard duration datatype.
	using duration = std::chrono::nanoseconds;

	inline namespace aliases {
		// Common chrono aliases.
		inline namespace chrono_aliases {
			// Duration in seconds.
			template <typename Rep> using secs = std::chrono::duration<Rep>;
			// Duration in seconds.
			using fsecs = secs<float>;
			// Duration in seconds.
			using dsecs = secs<double>;
			// Duration in seconds.
			using isecs = std::chrono::seconds;

			// Duration in milliseconds.
			template <typename Rep> using msecs = std::chrono::duration<Rep, std::milli>;
			// Duration in milliseconds.
			using fmsecs = msecs<float>;
			// Duration in milliseconds.
			using dmsecs = msecs<double>;
			// Duration in milliseconds.
			using imsecs = std::chrono::milliseconds;

			// Duration in microseconds.
			template <typename Rep> using usecs = std::chrono::duration<Rep, std::micro>;
			// Duration in microseconds.
			using fusecs = usecs<float>;
			// Duration in microseconds.
			using dusecs = usecs<double>;
			// Duration in microseconds.
			using iusecs = std::chrono::microseconds;

			// Duration in nanoseconds.
			template <typename Rep> using nsecs = std::chrono::duration<Rep, std::nano>;
			// Duration in nanoseconds.
			using fnsecs = nsecs<float>;
			// Duration in nanoseconds.
			using dnsecs = nsecs<double>;
			// Duration in nanoseconds.
			using insecs = std::chrono::nanoseconds;
		} // namespace chrono_aliases
	} // namespace aliases

	inline namespace literals {
		inline namespace chrono_literals {
			// Creates a duration from a value in hertz.
			consteval duration operator""_hz(unsigned long long hz);
			// Creates a duration from a value in hertz.
			consteval duration operator""_hz(long double hz);
		} // namespace chrono_literals
	} // namespace literals

	// Computes the ratio between two durations.
	template <typename Rep1, typename Rep2, typename Period1, typename Period2>
	constexpr float ratio(std::chrono::duration<Rep1, Period1> a, std::chrono::duration<Rep2, Period2> b);

	// Cross-platform, thread-safe localtime.
	std::tm localtime(std::time_t time);
} // namespace tr

#include "impl/chrono.hpp"