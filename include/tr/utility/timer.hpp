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
		template <class R, class P, class C> timer(const std::chrono::duration<R, P>& interval, C&& cb);
		// Moves a timer.
		timer(timer&& r) noexcept = default;
		// Stops the timer.
		~timer();

		// Moves a timer.
		timer& operator=(timer&& r) noexcept;

		// Reports whether the timer is active.
		bool active() const;

	  private:
		// Dynamically allocated so the thread isn't interrupted when moving.
		std::unique_ptr<bool> m_active;
		// The timer thread.
		std::thread m_thread;

		// Loop thread used by timer threads.
		static void timer_loop(bool& active, duration interval, callback cb);

		// Clears the timer state.
		void clear();
	};
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class R, class P, class C>
tr::timer::timer(const std::chrono::duration<R, P>& interval, C&& cb)
	: m_active{std::make_unique<bool>(true)}
	, m_thread{timer_loop, std::ref(*m_active), std::chrono::duration_cast<duration>(interval), callback{std::forward<C>(cb)}}
{
}