///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements angle.hpp.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../angle.hpp"

//////////////////////////////////////////////////////// CONSTRUCTORS AND LITERALS ////////////////////////////////////////////////////////

constexpr tr::angle::angle(float rads)
	: m_rads{rads}
{
}

template <tr::arithmetic Number> constexpr tr::angle tr::rads(Number th)
{
	return angle{float(th)};
}

template <tr::arithmetic Number> constexpr tr::angle tr::degs(Number th)
{
	return rads(std::numbers::pi_v<float> / 180 * float(th));
}

template <tr::arithmetic Number> constexpr tr::angle tr::turns(Number th)
{
	return rads(std::numbers::pi_v<float> * 2 * float(th));
}

template <tr::arithmetic Number> inline tr::angle tr::asin(Number sin)
{
	return rads(std::asin(sin));
}

template <tr::arithmetic Number> inline tr::angle tr::acos(Number cos)
{
	return rads(std::acos(cos));
}

template <tr::arithmetic Number> inline tr::angle tr::atan(Number tan)
{
	return rads(std::atan(tan));
}

template <tr::arithmetic Number> inline tr::angle tr::atan2(Number y, Number x)
{
	return rads(std::atan2(y, x));
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

//////////////////////////////////////////////////////////////// OPERATORS ////////////////////////////////////////////////////////////////

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

template <tr::arithmetic Factor> constexpr tr::angle& tr::angle::operator*=(const Factor& r)
{
	m_rads *= r;
	return *this;
}

template <tr::arithmetic Factor> constexpr tr::angle& tr::angle::operator/=(const Factor& r)
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

template <tr::arithmetic Factor> constexpr tr::angle tr::operator*(const angle& l, const Factor& r)
{
	return angle{l.m_rads * r};
}

template <tr::arithmetic Factor> constexpr tr::angle tr::operator*(const Factor& l, const angle& r)
{
	return r + l;
}

template <tr::arithmetic Factor> constexpr tr::angle tr::operator/(const angle& l, const Factor& r)
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

///////////////////////////////////////////////////// UNIT AND TRIGONOMETRIC FUNCTIONS ////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////// FORMATTING ///////////////////////////////////////////////////////////////

template <typename ParseContext> constexpr auto TR_FMT::formatter<tr::angle>::parse(ParseContext& ctx)
{
	auto ctx_it = ctx.begin();
	if (ctx_it == ctx.end() || (*ctx_it != 'r' && *ctx_it != 'd' && *ctx_it != 't')) {
		throw TR_FMT::format_error{"One of {r, d, t} must start an angle formatting specification."};
	}

	switch (*ctx_it) {
	case 'r':
		m_unit = unit::radians;
		break;
	case 'd':
		m_unit = unit::degrees;
		break;
	case 't':
		m_unit = unit::turns;
		break;
	}

	ctx.advance_to(ctx_it + 1);
	return formatter<float>::parse(ctx);
}

template <typename FormatContext> constexpr auto TR_FMT::formatter<tr::angle>::format(const tr::angle& p, FormatContext& ctx) const
{
	switch (m_unit) {
	case unit::radians:
		ctx.advance_to(formatter<float>::format(p.rads(), ctx));
		ctx.advance_to(formatter<const char*>::format("rad", ctx));
		break;
	case unit::degrees:
		ctx.advance_to(formatter<float>::format(p.degs(), ctx));
		ctx.advance_to(formatter<const char*>::format("deg", ctx));
		break;
	case unit::turns:
		ctx.advance_to(formatter<float>::format(p.turns(), ctx));
		ctx.advance_to(formatter<const char*>::format("tr", ctx));
		break;
	}
	return ctx.out();
}