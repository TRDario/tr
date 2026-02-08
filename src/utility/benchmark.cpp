///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements benchmark.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/benchmark.hpp"
#include "../../include/tr/utility/macro.hpp"
#include "../../include/tr/utility/ranges.hpp"

using namespace std::chrono_literals;

//////////////////////////////////////////////////////////////// BENCHMARK ////////////////////////////////////////////////////////////////

void tr::benchmark::start()
{
	TR_ASSERT(m_start == not_started, "Tried to start a benchmark measurement before stopping the previous one.");

	atomic_thread_fence(std::memory_order::relaxed);
	m_start = std::chrono::steady_clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

void tr::benchmark::stop()
{
	TR_ASSERT(m_start != not_started, "Tried to stop a benchmark measurement before starting one.");

	try {
		atomic_thread_fence(std::memory_order::relaxed);
		const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
		m_measurements.emplace_back(m_start, now - m_start);
		const auto erase_end_it{std::ranges::upper_bound(m_measurements, now - max_measurement_age, std::less{}, &measurement::start)};
		if (erase_end_it != m_measurements.end()) {
			m_measurements.erase(m_measurements.begin(), erase_end_it);
		}
		m_start = not_started;
		atomic_thread_fence(std::memory_order::relaxed);
	}
	catch (std::exception&) {
		m_start = not_started;
	}
}

void tr::benchmark::clear()
{
	m_start = not_started;
	m_measurements.clear();
}

tr::duration tr::benchmark::latest() const
{
	return !m_measurements.empty() ? m_measurements.back().duration : duration{0};
}

tr::duration tr::benchmark::min() const
{
	const auto durations{project(m_measurements, &measurement::duration)};
	return !m_measurements.empty() ? *std::ranges::min_element(durations) : duration{0};
}

tr::duration tr::benchmark::max() const
{
	const auto durations{project(m_measurements, &measurement::duration)};
	return !m_measurements.empty() ? *std::ranges::max_element(durations) : duration{0};
}

tr::duration tr::benchmark::avg() const
{
	if (m_measurements.empty()) {
		return duration{0};
	}
	else {
		const auto durations{project(m_measurements, &measurement::duration)};
		return std::accumulate(durations.begin(), durations.end(), duration{0}) / m_measurements.size();
	}
}

double tr::benchmark::fps() const
{
	return !m_measurements.empty() ? m_measurements.size() / (max_measurement_age / 1.0s) : 0;
}

const std::deque<tr::benchmark::measurement>& tr::benchmark::measurements() const
{
	return m_measurements;
}
