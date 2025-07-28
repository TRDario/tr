#include "../../include/tr/utility/benchmark.hpp"
#include "../../include/tr/utility/macro.hpp"

using namespace std::chrono_literals;

void tr::benchmark::start()
{
	TR_ASSERT(start_time == time_point{}, "Tried to start a benchmark measurement before stopping the previous one.");
	atomic_thread_fence(std::memory_order::relaxed);
	start_time = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

void tr::benchmark::stop()
{
	TR_ASSERT(start_time != time_point{}, "Tried to stop a benchmark measurement before starting one.");

	try {
		atomic_thread_fence(std::memory_order::relaxed);
		const time_point now{clock::now()};
		duration_queue.emplace_back(start_time, now - start_time);
		const auto it{std::ranges::upper_bound(duration_queue, now - 2.5s, std::less{}, &deque::value_type::first)};
		if (it != duration_queue.end()) {
			duration_queue.erase(duration_queue.begin(), it);
		}
		start_time = time_point{};
		atomic_thread_fence(std::memory_order::relaxed);
	}
	catch (std::exception&) {
		start_time = time_point{};
	}
}

void tr::benchmark::clear()
{
	start_time = time_point{};
	duration_queue.clear();
}

tr::duration tr::benchmark::latest() const
{
	return !duration_queue.empty() ? duration_queue.back().second : duration{0};
}

tr::duration tr::benchmark::min() const
{
	return !duration_queue.empty() ? *std::ranges::min_element(std::views::values(duration_queue)) : duration{0};
}

tr::duration tr::benchmark::max() const
{
	return !duration_queue.empty() ? *std::ranges::max_element(std::views::values(duration_queue)) : duration{0};
}

tr::duration tr::benchmark::avg() const
{
	if (duration_queue.empty()) {
		return duration{0};
	}
	else {
		const auto durations{std::views::values(duration_queue)};
		return std::accumulate(durations.begin(), durations.end(), duration{0}) / duration_queue.size();
	}
}

double tr::benchmark::fps() const
{
	return !duration_queue.empty() ? duration_queue.size() / 2.5 : 0;
}

const tr::benchmark::deque& tr::benchmark::measurements() const
{
	return duration_queue;
}
