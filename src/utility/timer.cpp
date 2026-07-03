///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of timer.hpp.                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/timer.hpp"

// Use SDL_DelayPrecise on Windows if available.
#if defined(WIN32) && defined(TR_HAS_SYSGFX)
#include <SDL3/SDL.h>
#endif

using namespace std::chrono_literals;

////////////////////////////////////////////////////////////////// TIMER //////////////////////////////////////////////////////////////////

bool tr::timer::active() const
{
	return m_thread.joinable();
}

//

void tr::timer::timer_loop(std::stop_token stoken, duration interval, callback cb)
{
	using clock = std::chrono::steady_clock;

	if (interval == 0s) {
		return;
	}

	try {
		clock::time_point prev{clock::now()};
		duration total_error{};

		std::this_thread::sleep_for(interval);
		while (!stoken.stop_requested()) {
			const clock::time_point now{clock::now()};

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