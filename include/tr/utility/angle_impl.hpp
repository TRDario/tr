#pragma once
#include "angle.hpp"

constexpr tr::angle::angle(float rads)
	: base{rads}
{
}

constexpr tr::angle& tr::angle::operator+=(const angle& r)
{
	base += r.base;
	return *this;
}

constexpr tr::angle& tr::angle::operator-=(const angle& r)
{
	base -= r.base;
	return *this;
}

template <tr::arithmetic U> constexpr tr::angle& tr::angle::operator*=(const U& r)
{
	base *= r;
	return *this;
}

template <tr::arithmetic U> constexpr tr::angle& tr::angle::operator/=(const U& r)
{
	base /= r;
	return *this;
}

constexpr tr::angle tr::operator+(const angle& l, const angle& r)
{
	return angle{l.base + r.base};
}

constexpr tr::angle tr::operator-(const angle& l, const angle& r)
{
	return angle{l.base - r.base};
}

constexpr tr::angle tr::operator-(const angle& l)
{
	return angle{-l.base};
}

template <tr::arithmetic U> constexpr tr::angle tr::operator*(const angle& l, const U& r)
{
	return angle{l.base * r};
}

template <tr::arithmetic U> constexpr tr::angle tr::operator*(const U& l, const angle& r)
{
	return r + l;
}

template <tr::arithmetic U> constexpr tr::angle tr::operator/(const angle& l, const U& r)
{
	return angle{l.base / r};
}

constexpr float tr::operator/(const angle& l, const angle& r)
{
	return l.base / r.base;
}

constexpr tr::angle tr::operator%(const angle& l, const angle& r)
{
	return angle{std::fmod(l.base, r.base)};
}

template <tr::arithmetic T> constexpr tr::angle tr::rads(T th)
{
	return angle{static_cast<float>(th)};
}

template <tr::arithmetic T> constexpr tr::angle tr::degs(T th)
{
	return angle{std::numbers::pi_v<float> / 180 * static_cast<float>(th)};
}

template <tr::arithmetic T> constexpr tr::angle tr::turns(T th)
{
	return angle{std::numbers::pi_v<float> * 2 * static_cast<float>(th)};
}

template <tr::arithmetic T> constexpr tr::angle tr::asin(T sin)
{
	return angle{std::asin(sin)};
}

template <tr::arithmetic T> constexpr tr::angle tr::acos(T cos)
{
	return angle{std::acos(cos)};
}

template <tr::arithmetic T> constexpr tr::angle tr::atan(T tan)
{
	return angle{std::atan(tan)};
}

template <tr::arithmetic T> constexpr tr::angle tr::atan2(T y, T x)
{
	return angle{std::atan2(y, x)};
}

constexpr float tr::angle::rads() const
{
	return base;
}

constexpr float tr::angle::degs() const
{
	return base * (180 / std::numbers::pi_v<float>);
}

constexpr float tr::angle::turns() const
{
	return base / (2 * std::numbers::pi_v<float>);
}

constexpr float tr::angle::sin() const
{
	return std::sin(base);
}

constexpr float tr::angle::cos() const
{
	return std::cos(base);
}

constexpr float tr::angle::tan() const
{
	return std::tan(base);
}

consteval tr::angle tr::angle_literals::operator""_deg(long double deg)
{
	return degs(static_cast<float>(deg));
}

consteval tr::angle tr::angle_literals::operator""_deg(unsigned long long deg)
{
	return degs(static_cast<float>(deg));
}

consteval tr::angle tr::angle_literals::operator""_rad(long double rad)
{
	return rads(static_cast<float>(rad));
}

consteval tr::angle tr::angle_literals::operator""_rad(unsigned long long rad)
{
	return rads(static_cast<float>(rad));
}

consteval tr::angle tr::angle_literals::operator""_turns(long double tr)
{
	return turns(static_cast<float>(tr));
}

consteval tr::angle tr::angle_literals::operator""_turns(unsigned long long tr)
{
	return turns(static_cast<float>(tr));
}