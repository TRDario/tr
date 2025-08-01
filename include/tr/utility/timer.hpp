#pragma once
#include "chrono.hpp"

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

		static void timer_loop(bool& active, duration interval, callback cb);
	};
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class R, class P, class C>
tr::timer::timer(const std::chrono::duration<R, P>& interval, C&& cb)
	: m_active{std::make_unique<bool>(true)}
	, m_thread{timer_loop, std::ref(*m_active), std::chrono::duration_cast<duration>(interval), callback{std::forward<C>(cb)}}
{
}