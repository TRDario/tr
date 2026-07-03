///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of timer.hpp.                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../timer.hpp"

////////////////////////////////////////////////////////////////// TIMER //////////////////////////////////////////////////////////////////

template <tr::arithmetic Rep, tr::specialization_of_v<std::ratio> Period, std::convertible_to<std::function<void()>> Callback>
tr::timer::timer(const std::chrono::duration<Rep, Period>& interval, Callback&& cb)
	: m_thread{timer_loop, std::chrono::duration_cast<duration>(interval), callback{std::forward<Callback>(cb)}}
{
}