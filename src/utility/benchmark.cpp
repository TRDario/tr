#include "../../include/tr/utility/benchmark.hpp"
#include "../../include/tr/utility/macro.hpp"

using namespace std::chrono_literals;

void tr::benchmark::start() noexcept
{
	TR_ASSERT(_start == time_point{}, "Tried to start a benchmark measurement before stopping the previous one.");
	atomic_thread_fence(std::memory_order::relaxed);
	_start = clock::now();
	atomic_thread_fence(std::memory_order::relaxed);
}

void tr::benchmark::stop() noexcept
{
	TR_ASSERT(_start != time_point{}, "Tried to stop a benchmark measurement before starting one.");

	try {
		atomic_thread_fence(std::memory_order::relaxed);
		const time_point now{clock::now()};
		_durations.emplace_back(_start, now - _start);
		const auto it{std::ranges::upper_bound(_durations, now - 2.5s, std::less{}, &deque::value_type::first)};
		if (it != _durations.end()) {
			_durations.erase(_durations.begin(), it);
		}
		_start = time_point{};
		atomic_thread_fence(std::memory_order::relaxed);
	}
	catch (std::exception&) {
		_start = time_point{};
	}
}

void tr::benchmark::clear() noexcept
{
	_start = time_point{};
	_durations.clear();
}

tr::duration tr::benchmark::latest() const noexcept
{
	return !_durations.empty() ? _durations.back().second : duration{0};
}

tr::duration tr::benchmark::min() const noexcept
{
	return !_durations.empty() ? *std::ranges::min_element(std::views::values(_durations)) : duration{0};
}

tr::duration tr::benchmark::max() const noexcept
{
	return !_durations.empty() ? *std::ranges::max_element(std::views::values(_durations)) : duration{0};
}

tr::duration tr::benchmark::avg() const noexcept
{
	if (_durations.empty()) {
		return duration{0};
	}
	else {
		const auto durations{std::views::values(_durations)};
		return std::accumulate(durations.begin(), durations.end(), duration{0}) / _durations.size();
	}
}

double tr::benchmark::fps() const noexcept
{
	return !_durations.empty() ? _durations.size() / 2.5 : 0;
}

const tr::benchmark::deque& tr::benchmark::measurements() const noexcept
{
	return _durations;
}
