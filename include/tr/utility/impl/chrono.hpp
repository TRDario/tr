///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr parts of chrono.hpp.                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../chrono.hpp"

///////////////////////////////////////////////////////////////// LITERALS ////////////////////////////////////////////////////////////////

consteval tr::duration tr::operator""_hz(unsigned long long hz)
{
	return duration{isecs{1}} / (long long)hz;
}

consteval tr::duration tr::operator""_hz(long double hz)
{
	return std::chrono::duration_cast<duration>(duration{isecs{1}} / hz);
}

////////////////////////////////////////////////////////////////// RATIO //////////////////////////////////////////////////////////////////

template <typename Rep1, typename Rep2, typename Period1, typename Period2>
constexpr float tr::ratio(std::chrono::duration<Rep1, Period1> a, std::chrono::duration<Rep2, Period2> b)
{
	return float(dnsecs{a} / dnsecs{b});
}