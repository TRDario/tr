#include "../../include/tr/utility/timer.hpp"

using namespace std::chrono_literals;

tr::timer::~timer()
{
	clear();
}

tr::timer& tr::timer::operator=(timer&& r) noexcept
{
	clear();
	m_active = std::move(r.m_active);
	m_thread = std::move(r.m_thread);
	return *this;
}

bool tr::timer::active() const
{
	return m_active != nullptr;
}

void tr::timer::timer_loop(bool& active, duration interval, callback cb)
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

void tr::timer::clear()
{
	if (m_active != nullptr) {
		*m_active = false;
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}
}