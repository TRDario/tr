#pragma once
#include "angle.hpp"

template <std::floating_point T>
constexpr tr::angle<T>::angle(T rads) noexcept
	: _rads{rads}
{
}

template <std::floating_point T>
template <std::floating_point U>
constexpr tr::angle<T>::angle(angle<U> th) noexcept
	: _rads{static_cast<T>(th.rads())}
{
}

template <std::floating_point T> template <std::floating_point U> constexpr auto tr::angle<T>::operator<=>(const angle<U>& r) const noexcept
{
	return _rads <=> r.rads();
}

template <std::floating_point T> template <std::floating_point U> constexpr bool tr::angle<T>::operator==(const angle<U>& r) const noexcept
{
	return _rads == r.rads();
}

template <std::floating_point T>
template <std::floating_point U>
constexpr tr::angle<T>& tr::angle<T>::operator+=(const angle<U>& r) noexcept
{
	_rads += r.rads();
	return *this;
}

template <std::floating_point T>
template <std::floating_point U>
constexpr tr::angle<T>& tr::angle<T>::operator-=(const angle<U>& r) noexcept
{
	_rads -= r.rads();
	return *this;
}

template <std::floating_point T> template <tr::arithmetic U> constexpr tr::angle<T>& tr::angle<T>::operator*=(const U& r) noexcept
{
	_rads *= r;
	return *this;
}

template <std::floating_point T> template <tr::arithmetic U> constexpr tr::angle<T>& tr::angle<T>::operator/=(const U& r) noexcept
{
	_rads /= r;
	return *this;
}

template <std::floating_point T> constexpr tr::angle<T> tr::angle<T>::operator-() const noexcept
{
	return angle<T>{-_rads};
}

template <std::floating_point T> template <std::floating_point U> constexpr auto tr::angle<T>::operator+(const angle<U>& r) const noexcept
{
	return tr::rads(_rads + r.rads());
}

template <std::floating_point T> template <std::floating_point U> constexpr auto tr::angle<T>::operator-(const angle<U>& r) const noexcept
{
	return tr::rads(_rads - r.rads());
}

template <std::floating_point T> template <tr::arithmetic U> constexpr auto tr::angle<T>::operator*(const U& r) const noexcept
{
	return tr::rads(_rads * r);
}

template <std::floating_point T> template <tr::arithmetic U> constexpr auto tr::angle<T>::operator/(const U& r) const noexcept
{
	return tr::rads(_rads / r);
}

template <std::floating_point T> template <std::floating_point U> constexpr auto tr::angle<T>::operator/(const angle<U>& r) const noexcept
{
	return _rads / r.rads();
}

template <std::floating_point T> template <std::floating_point U> constexpr auto tr::angle<T>::operator%(const angle<U>& r) const noexcept
{
	return tr::rads(std::fmod(_rads, r.rads()));
}

template <tr::arithmetic T> constexpr auto tr::rads(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return angle<T>{th};
	}
	else {
		return dangle{static_cast<double>(th)};
	}
}

template <tr::arithmetic T> constexpr auto tr::degs(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return angle<T>{std::numbers::pi_v<T> / 180 * th};
	}
	else {
		return dangle{std::numbers::pi / 180 * th};
	}
}

template <tr::arithmetic T> constexpr auto tr::turns(T th) noexcept
{
	if constexpr (std::floating_point<T>) {
		return angle<T>{std::numbers::pi_v<T> * 2 * th};
	}
	else {
		return dangle{std::numbers::pi * 2 * th};
	}
}

template <tr::arithmetic T> constexpr auto tr::asin(T sin) noexcept
{
	return angle{std::asin(sin)};
}

template <tr::arithmetic T> constexpr auto tr::acos(T cos) noexcept
{
	return angle{std::acos(cos)};
}

template <tr::arithmetic T> constexpr auto tr::atan(T tan) noexcept
{
	return angle{std::atan(tan)};
}

template <tr::arithmetic T> constexpr auto tr::atan2(T y, T x) noexcept
{
	return angle{std::atan2(y, x)};
}

template <std::floating_point T> constexpr T tr::angle<T>::rads() const noexcept
{
	return _rads;
}

template <std::floating_point T> constexpr T tr::angle<T>::degs() const noexcept
{
	return _rads * (180 / std::numbers::pi_v<T>);
}

template <std::floating_point T> constexpr T tr::angle<T>::turns() const noexcept
{
	return _rads / (2 * std::numbers::pi_v<T>);
}

template <std::floating_point T> constexpr T tr::angle<T>::sin() const noexcept
{
	return std::sin(_rads);
}

template <std::floating_point T> constexpr T tr::angle<T>::cos() const noexcept
{
	return std::cos(_rads);
}

template <std::floating_point T> constexpr T tr::angle<T>::tan() const noexcept
{
	return std::tan(_rads);
}

consteval tr::fangle tr::angle_literals::operator""_degf(long double deg) noexcept
{
	return degs(static_cast<float>(deg));
}

consteval tr::fangle tr::angle_literals::operator""_degf(unsigned long long deg) noexcept
{
	return degs(static_cast<float>(deg));
}

consteval tr::dangle tr::angle_literals::operator""_deg(long double deg) noexcept
{
	return degs(static_cast<double>(deg));
}

consteval tr::dangle tr::angle_literals::operator""_deg(unsigned long long deg) noexcept
{
	return degs(static_cast<double>(deg));
}

consteval tr::langle tr::angle_literals::operator""_degl(long double deg) noexcept
{
	return degs(deg);
}

consteval tr::langle tr::angle_literals::operator""_degl(unsigned long long deg) noexcept
{
	return degs(static_cast<long double>(deg));
}

consteval tr::fangle tr::angle_literals::operator""_radf(long double rad) noexcept
{
	return rads(static_cast<float>(rad));
}

consteval tr::fangle tr::angle_literals::operator""_radf(unsigned long long rad) noexcept
{
	return rads(static_cast<float>(rad));
}

consteval tr::dangle tr::angle_literals::operator""_rad(long double rad) noexcept
{
	return rads(static_cast<double>(rad));
}

consteval tr::dangle tr::angle_literals::operator""_rad(unsigned long long rad) noexcept
{
	return rads(rad);
}

consteval tr::langle tr::angle_literals::operator""_radl(long double rad) noexcept
{
	return rads(rad);
}

consteval tr::langle tr::angle_literals::operator""_radl(unsigned long long rad) noexcept
{
	return rads(static_cast<long double>(rad));
}

consteval tr::fangle tr::angle_literals::operator""_turnsf(long double tr) noexcept
{
	return turns(static_cast<float>(tr));
}

consteval tr::fangle tr::angle_literals::operator""_turnsf(unsigned long long tr) noexcept
{
	return turns(static_cast<float>(tr));
}

consteval tr::dangle tr::angle_literals::operator""_turns(long double tr) noexcept
{
	return turns(static_cast<double>(tr));
}

consteval tr::dangle tr::angle_literals::operator""_turns(unsigned long long tr) noexcept
{
	return turns(static_cast<double>(tr));
}

consteval tr::langle tr::angle_literals::operator""_turnsl(long double tr) noexcept
{
	return turns(tr);
}

consteval tr::langle tr::angle_literals::operator""_turnsl(unsigned long long tr) noexcept
{
	return turns(static_cast<long double>(tr));
}