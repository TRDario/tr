#include "../../include/tr/utility/benchmark.hpp"
#include "../../include/tr/utility/macro.hpp"

using namespace std::chrono_literals;

void tr::benchmark::start()
{
	TR_ASSERT(m_start == time_point{}, "Tried to start a benchmark measurement before stopping the previous one.");
	atomic_thread_fence(std::memory_order::relaxed);
	m_start = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

void tr::benchmark::stop()
{
	TR_ASSERT(m_start != time_point{}, "Tried to stop a benchmark measurement before starting one.");

	try {
		atomic_thread_fence(std::memory_order::relaxed);
		const time_point now{clock::now()};
		m_durations.emplace_back(m_start, now - m_start);
		const auto it{std::ranges::upper_bound(m_durations, now - 2.5s, std::less{}, &deque::value_type::first)};
		if (it != m_durations.end()) {
			m_durations.erase(m_durations.begin(), it);
		}
		m_start = time_point{};
		atomic_thread_fence(std::memory_order::relaxed);
	}
	catch (std::exception&) {
		m_start = time_point{};
	}
}

void tr::benchmark::clear()
{
	m_start = time_point{};
	m_durations.clear();
}

tr::duration tr::benchmark::latest() const
{
	return !m_durations.empty() ? m_durations.back().second : duration{0};
}

tr::duration tr::benchmark::min() const
{
	return !m_durations.empty() ? *std::ranges::min_element(std::views::values(m_durations)) : duration{0};
}

tr::duration tr::benchmark::max() const
{
	return !m_durations.empty() ? *std::ranges::max_element(std::views::values(m_durations)) : duration{0};
}

tr::duration tr::benchmark::avg() const
{
	if (m_durations.empty()) {
		return duration{0};
	}
	else {
		const auto durations{std::views::values(m_durations)};
		return std::accumulate(durations.begin(), durations.end(), duration{0}) / m_durations.size();
	}
}

double tr::benchmark::fps() const
{
	return !m_durations.empty() ? m_durations.size() / 2.5 : 0;
}

const tr::benchmark::deque& tr::benchmark::measurements() const
{
	return m_durations;
}
