#include "../../include/tr/utility/stopwatch.hpp"
#include <atomic>

tr::stopwatch::stopwatch() noexcept
{
	atomic_thread_fence(std::memory_order::relaxed);
	_start = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

tr::duration tr::stopwatch::elapsed() const noexcept
{
	atomic_thread_fence(std::memory_order::relaxed);
	const time_point now{clock::now()};
	atomic_thread_fence(std::memory_order::relaxed);
	return now - _start;
}

tr::duration tr::stopwatch::lap() noexcept
{
	const time_point start = _start;
	atomic_thread_fence(std::memory_order::relaxed);
	_start = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
	return _start - start;
}