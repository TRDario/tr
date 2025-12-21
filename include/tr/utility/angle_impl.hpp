#pragma once
#include "angle.hpp"

constexpr tr::angle::angle(float rads)
	: m_rads{rads}
{
}

constexpr tr::angle& tr::angle::operator+=(const angle& r)
{
	m_rads += r.m_rads;
	return *this;
}

constexpr tr::angle& tr::angle::operator-=(const angle& r)
{
	m_rads -= r.m_rads;
	return *this;
}

template <tr::arithmetic U> constexpr tr::angle& tr::angle::operator*=(const U& r)
{
	m_rads *= r;
	return *this;
}

template <tr::arithmetic U> constexpr tr::angle& tr::angle::operator/=(const U& r)
{
	m_rads /= r;
	return *this;
}

constexpr tr::angle tr::operator+(const angle& l, const angle& r)
{
	return angle{l.m_rads + r.m_rads};
}

constexpr tr::angle tr::operator-(const angle& l, const angle& r)
{
	return angle{l.m_rads - r.m_rads};
}

constexpr tr::angle tr::operator-(const angle& l)
{
	return angle{-l.m_rads};
}

template <tr::arithmetic U> constexpr tr::angle tr::operator*(const angle& l, const U& r)
{
	return angle{l.m_rads * r};
}

template <tr::arithmetic U> constexpr tr::angle tr::operator*(const U& l, const angle& r)
{
	return r + l;
}

template <tr::arithmetic U> constexpr tr::angle tr::operator/(const angle& l, const U& r)
{
	return angle{l.m_rads / r};
}

constexpr float tr::operator/(const angle& l, const angle& r)
{
	return l.m_rads / r.m_rads;
}

constexpr tr::angle tr::operator%(const angle& l, const angle& r)
{
	return angle{std::fmod(l.m_rads, r.m_rads)};
}

template <tr::arithmetic T> constexpr tr::angle tr::rads(T th)
{
	return angle{float(th)};
}

template <tr::arithmetic T> constexpr tr::angle tr::degs(T th)
{
	return angle{std::numbers::pi_v<float> / 180 * float(th)};
}

template <tr::arithmetic T> constexpr tr::angle tr::turns(T th)
{
	return angle{std::numbers::pi_v<float> * 2 * float(th)};
}

template <tr::arithmetic T> inline tr::angle tr::asin(T sin)
{
	return angle{std::asin(sin)};
}

template <tr::arithmetic T> inline tr::angle tr::acos(T cos)
{
	return angle{std::acos(cos)};
}

template <tr::arithmetic T> inline tr::angle tr::atan(T tan)
{
	return angle{std::atan(tan)};
}

template <tr::arithmetic T> inline tr::angle tr::atan2(T y, T x)
{
	return angle{std::atan2(y, x)};
}

constexpr float tr::angle::rads() const
{
	return m_rads;
}

constexpr float tr::angle::degs() const
{
	return m_rads * (180 / std::numbers::pi_v<float>);
}

constexpr float tr::angle::turns() const
{
	return m_rads / (2 * std::numbers::pi_v<float>);
}

inline float tr::angle::sin() const
{
	return std::sin(m_rads);
}

inline float tr::angle::cos() const
{
	return std::cos(m_rads);
}

inline float tr::angle::tan() const
{
	return std::tan(m_rads);
}

consteval tr::angle tr::literals::angle_literals::operator""_deg(long double deg)
{
	return degs(float(deg));
}

consteval tr::angle tr::literals::angle_literals::operator""_deg(unsigned long long deg)
{
	return degs(float(deg));
}

consteval tr::angle tr::literals::angle_literals::operator""_rad(long double rad)
{
	return rads(float(rad));
}

consteval tr::angle tr::literals::angle_literals::operator""_rad(unsigned long long rad)
{
	return rads(float(rad));
}

consteval tr::angle tr::literals::angle_literals::operator""_turns(long double tr)
{
	return turns(float(tr));
}

consteval tr::angle tr::literals::angle_literals::operator""_turns(unsigned long long tr)
{
	return turns(float(tr));
}