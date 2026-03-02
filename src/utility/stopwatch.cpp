///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements stopwatch.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/stopwatch.hpp"
#include <chrono>

//////////////////////////////////////////////////////////////// STOPWATCH ////////////////////////////////////////////////////////////////

tr::stopwatch::stopwatch()
	: m_start{std::chrono::steady_clock::now()}
{
}

tr::duration tr::stopwatch::elapsed() const
{
	const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
	return now - m_start;
}

tr::duration tr::stopwatch::lap()
{
	const std::chrono::steady_clock::time_point start = m_start;
	m_start = std::chrono::steady_clock::now();
	return m_start - start;
}