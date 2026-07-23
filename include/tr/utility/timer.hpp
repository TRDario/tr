///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a callback timer class.                                                                                                      //
//                                                                                                                                       //
// Some actions may need to be performed regularly at a fixed interval. tr::timer provides a simple interface for such cases:            //
// it is constructed with a set interval and callback (function taking and returning void), which then run until the timer is destroyed: //
//     - tr::timer my_timer{1.0s, my_func} -> calls my_func every second until my_timer is destroyed.                                    //
//                                                                                                                                       //
// A timer which was default-constructed or moved from, or one whose callback threw an unhandled exception, exists in an inactive state; //
// whether a timer is active can be queried with the .active() method:                                                                   //
//     - my_timer.active() -> true                                                                                                       //
//                                                                                                                                       //
// NOTE: The accuracy of the timer on Windows may be limited to ~10ms if the sysgfx module is not available.                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "chrono.hpp"
#include "concepts.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Minimal callback timer class running a callback at a fixed interval.
	class timer {
	  public:
		// The callback function signature expected by the timer.
		using callback = std::function<void()>;

		// Constructs an inactive timer.
		timer() = default;
		// Constructs an active timer.
		template <arithmetic Rep, specialization_of_v<std::ratio> Period, std::convertible_to<std::function<void()>> Callback>
		timer(const std::chrono::duration<Rep, Period>& interval, Callback&& cb);

		// Reports whether the timer is active.
		bool active() const;

	  private:
		// The timer thread.
		std::jthread m_thread;

		// Loop thread used by timer threads.
		static void timer_loop(std::stop_token stoken, duration interval, callback cb);
	};
} // namespace tr

#include "impl/timer.hpp" // IWYU pragma: export