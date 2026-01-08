#include "../../include/tr/utility/stopwatch.hpp"
#include <atomic>
#include <chrono>

tr::stopwatch::stopwatch()
{
	atomic_thread_fence(std::memory_order::relaxed);
	m_start = std::chrono::steady_clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

tr::duration tr::stopwatch::elapsed() const
{
	atomic_thread_fence(std::memory_order::relaxed);
	const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
	atomic_thread_fence(std::memory_order::relaxed);
	return now - m_start;
}

tr::duration tr::stopwatch::lap()
{
	const std::chrono::steady_clock::time_point start = m_start;
	atomic_thread_fence(std::memory_order::relaxed);
	m_start = std::chrono::steady_clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
	return m_start - start;
}