///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of timer.hpp.                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../timer.hpp"

////////////////////////////////////////////////////////////////// TIMER //////////////////////////////////////////////////////////////////

template <typename R, typename P, typename C>
tr::timer::timer(const std::chrono::duration<R, P>& interval, C&& cb)
	: m_active{std::make_unique<bool>(true)}
	, m_thread{timer_loop, std::ref(*m_active), std::chrono::duration_cast<duration>(interval), callback{std::forward<C>(cb)}}
{
}