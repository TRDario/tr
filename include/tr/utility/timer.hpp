#pragma once
#include "chrono.hpp"

namespace tr {
	// Minimal callback timer class running a callback at a fixed interval.
	class timer {
	  public:
		// The callback function signature expected by the timer.
		using callback = std::function<void()>;

		// Constructs an inactive timer.
		timer() noexcept = default;
		// Constructs an active timer.
		template <class R, class P, class C> timer(const std::chrono::duration<R, P>& interval, C&& cb) noexcept;
		// Moves a timer.
		timer(timer&& r) noexcept = default;
		// Stops the timer.
		~timer() noexcept;

		// Moves a timer.
		timer& operator=(timer&& r) noexcept;

		// Reports whether the timer is active.
		bool active() const noexcept;

	  private:
		// Dynamically allocated so the thread isn't interrupted when moving.
		std::unique_ptr<bool> _active;
		// The timer thread.
		std::thread _thread;

		static void _thread_fn(bool& active, duration interval, callback cb) noexcept;
	};
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class R, class P, class C>
tr::timer::timer(const std::chrono::duration<R, P>& interval, C&& cb) noexcept
	: _active{std::make_unique<bool>(true)}
	, _thread{_thread_fn, std::ref(*_active), std::chrono::duration_cast<duration>(interval), callback{std::forward<C>(cb)}}
{
}