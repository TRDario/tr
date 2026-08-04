/// @file
/// @brief Provides a periodic callback timer class.

#pragma once
#include "chrono.hpp"
#include "concepts.hpp"
#include "specialization_of.hpp"

//

namespace tr
{
	/// Periodic timer class running a callback at a fixed interval.
	/// @note The accuracy of the timer on Windows may be limited to ~10ms if the sysgfx module is not available.
	class timer
	{
	  public:
		/// Callback function signature expected by the timer.
		using callback = std::function<void()>;

		/// @name Constructors
		/// @{

		/// Constructs an inactive timer.
		timer() = default;

		/// Constructs an active timer.
		/// @warning If the callback throws an exception, the timer will silently go inactive.
		/// @tparam Rep Duration representation.
		/// @tparam Period Duration period.
		/// @tparam Callback Base callback object type.
		/// @param interval Interval at which the callback is called.
		/// @param cb Callback object.
		template <arithmetic Rep, specialization_of_v<std::ratio> Period, std::convertible_to<std::function<void()>> Callback>
		timer(const std::chrono::duration<Rep, Period>& interval, Callback&& cb);

		/// @}
		/// @name Status
		/// @{

		/// Reports whether the timer is active.
		/// @return `true` if the timer is active, `false` otherwise.
		bool active() const;

		/// @}

	  private:
		/// Timer thread.
		std::jthread m_thread;

		//

		/// Loop thread used by timer threads.
		/// @param stoken Thread stop token.
		/// @param interval Timer interval.
		/// @param cb Callback object.
		static void timer_loop(std::stop_token stoken, duration interval, callback cb);
	};
} // namespace tr

#include "impl/timer.hpp" // IWYU pragma: export