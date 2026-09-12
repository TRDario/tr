/// @file
/// @brief Implements stopwatch.hpp.

#include <chrono>
#include <tr/utility/stopwatch.hpp>

//

tr::stopwatch::stopwatch() noexcept
	: m_start{std::chrono::steady_clock::now()}
{
}

tr::duration tr::stopwatch::elapsed() const noexcept
{
	const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
	return now - m_start;
}

tr::duration tr::stopwatch::lap() noexcept
{
	const std::chrono::steady_clock::time_point start = m_start;
	m_start = std::chrono::steady_clock::now();
	return m_start - start;
}