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
// tr::localtime is provided as a cross-platform and thread-safe implementation of localtime.                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

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

	// Cross-platform, thread-safe localtime.
	std::tm localtime(std::time_t time);
} // namespace tr
