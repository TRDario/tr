#include "../../include/tr/utility/stopwatch.hpp"
#include <atomic>

tr::stopwatch::stopwatch()
{
	atomic_thread_fence(std::memory_order::relaxed);
	start_time = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

tr::duration tr::stopwatch::elapsed() const
{
	atomic_thread_fence(std::memory_order::relaxed);
	const time_point now{clock::now()};
	atomic_thread_fence(std::memory_order::relaxed);
	return now - start_time;
}

tr::duration tr::stopwatch::lap()
{
	const time_point start = start_time;
	atomic_thread_fence(std::memory_order::relaxed);
	start_time = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
	return start_time - start;
}