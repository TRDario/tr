/// @file
/// @brief Implements timer.hpp.

#include <tr/utility/defer.hpp>
#include <tr/utility/timer.hpp>

// Use SDL_DelayPrecise on Windows if available.
#if defined(WIN32) && defined(TR_HAS_SYSGFX)
#include <SDL3/SDL.h>
#endif

using namespace std::chrono_literals;

//

bool tr::timer::active() const
{
	return m_active != nullptr && m_active->load(std::memory_order_acquire);
}

//

void tr::timer::timer_loop(std::stop_token stoken, duration interval, callback cb, std::shared_ptr<std::atomic_bool> active) noexcept
{
	TR_DEFER(active->store(false, std::memory_order_release));

	if (interval <= 0s) {
		return;
	}

	try {
		std::chrono::steady_clock::time_point prev{std::chrono::steady_clock::now()};
		duration total_error{};

		std::this_thread::sleep_for(interval);
		while (!stoken.stop_requested()) {
			const std::chrono::steady_clock::time_point now{std::chrono::steady_clock::now()};

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
	}
}