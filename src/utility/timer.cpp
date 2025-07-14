#include "../../include/tr/utility/timer.hpp"

using namespace std::chrono_literals;

tr::timer::~timer() noexcept
{
	if (_active != nullptr) {
		*_active = false;
		if (_thread.joinable()) {
			_thread.join();
		}
	}
}

tr::timer& tr::timer::operator=(timer&& r) noexcept
{
	std::ignore = timer{std::move(*this)};
	_active = std::move(r._active);
	_thread = std::move(r._thread);
	return *this;
}

bool tr::timer::active() const noexcept
{
	return _active != nullptr;
}

void tr::timer::_thread_fn(bool& active, duration interval, callback cb) noexcept
{
	if (interval == 0s) {
		return;
	}

	try {
		time_point prev{clock::now()};
		duration total_error{};

		std::this_thread::sleep_for(interval);
		while (active) {
			atomic_thread_fence(std::memory_order::relaxed);
			const time_point now{clock::now()};
			atomic_thread_fence(std::memory_order::relaxed);

			const duration last_interval{now - prev};
			total_error += last_interval - interval;

			duration next_interval{interval - total_error};
			while (next_interval <= 0s) {
				cb();
				next_interval += interval;
				total_error -= interval;
			}
			cb();

			prev = now;
			std::this_thread::sleep_until(now + next_interval);
		}
	}
	catch (...) {
		// Exit gracefully if an exception occurs.
		active = false;
		return;
	}
}