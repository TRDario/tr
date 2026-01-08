#include "../../include/tr/utility/timer.hpp"

// Use SDL_DelayPrecise on Windows if available.
#if defined(WIN32) && defined(TR_HAS_SYSGFX)
#include <SDL3/SDL.h>
#endif

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
		std::chrono::steady_clock::time_point prev{std::chrono::steady_clock::now()};
		duration total_error{};

		std::this_thread::sleep_for(interval);
		while (active) {
			atomic_thread_fence(std::memory_order::relaxed);
			const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};
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
#if defined(WIN32) && defined(TR_HAS_SYSGFX)
			SDL_DelayPrecise(tr::insecs{next_interval}.count());
#else
			std::this_thread::sleep_until(now + next_interval);
#endif
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