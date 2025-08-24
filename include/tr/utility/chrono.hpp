#pragma once
#include "common.hpp"

namespace tr {
	using clock = std::chrono::steady_clock;
	using time_point = clock::time_point;
	using duration = clock::duration;

	template <class T> using secs = std::chrono::duration<T>;
	using fsecs = secs<float>;
	using dsecs = secs<double>;
	using isecs = std::chrono::seconds;

	template <class T> using msecs = std::chrono::duration<T, std::milli>;
	using fmsecs = msecs<float>;
	using dmsecs = msecs<double>;
	using imsecs = std::chrono::milliseconds;

	template <class T> using usecs = std::chrono::duration<T, std::micro>;
	using fusecs = usecs<float>;
	using dusecs = usecs<double>;
	using iusecs = std::chrono::microseconds;

	template <class T> using nsecs = std::chrono::duration<T, std::nano>;
	using fnsecs = nsecs<float>;
	using dnsecs = nsecs<double>;
	using insecs = std::chrono::nanoseconds;
} // namespace tr
