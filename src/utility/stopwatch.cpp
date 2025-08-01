#include "../../include/tr/utility/stopwatch.hpp"
#include <atomic>

tr::stopwatch::stopwatch()
{
	atomic_thread_fence(std::memory_order::relaxed);
	m_start = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

tr::duration tr::stopwatch::elapsed() const
{
	atomic_thread_fence(std::memory_order::relaxed);
	const time_point now{clock::now()};
	atomic_thread_fence(std::memory_order::relaxed);
	return now - m_start;
}

tr::duration tr::stopwatch::lap()
{
	const time_point start = m_start;
	atomic_thread_fence(std::memory_order::relaxed);
	m_start = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
	return m_start - start;
}