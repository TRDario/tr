/// @file
/// @brief Implements angle.hpp.

#pragma once
#include "../angle.hpp"
#include "../math.hpp"

//

constexpr tr::angle::angle(float rads)
	: m_rads{rads}
{
}

template <tr::arithmetic Number>
constexpr tr::angle tr::rads(Number th)
{
	return angle{static_cast<float>(th)};
}

template <tr::arithmetic Number>
constexpr tr::angle tr::degs(Number th)
{
	return rads(std::numbers::pi_v<float> / 180 * static_cast<float>(th));
}

template <tr::arithmetic Number>
constexpr tr::angle tr::turns(Number th)
{
	return rads(std::numbers::pi_v<float> * 2 * static_cast<float>(th));
}

template <tr::arithmetic Number>
inline tr::angle tr::asin(Number sin)
{
	return rads(std::asin(sin));
}

template <tr::arithmetic Number>
inline tr::angle tr::acos(Number cos)
{
	return rads(std::acos(cos));
}

template <tr::arithmetic Number>
inline tr::angle tr::atan(Number tan)
{
	return rads(std::atan(tan));
}

template <tr::arithmetic Number>
inline tr::angle tr::atan2(Number y, Number x)
{
	return rads(std::atan2(y, x));
}

template <tr::arithmetic Number>
inline tr::angle tr::atan2(glm::tvec2<Number> p)
{
	return tr::atan2(p.y, p.x);
}

//

constexpr tr::angle tr::abs(tr::angle th)
{
	return th >= tr::angle{} ? th : -th;
}

template <std::floating_point Ratio>
constexpr tr::angle tr::lerp(angle x, angle y, Ratio a)
{
	x = eucmod(x, turns(1.0f));
	y = eucmod(y, turns(1.0f));
	if (abs(y - x) >= turns(0.5f)) {
		(x > y ? x : y) -= turns(1.0f);
	}
	return x + (y - x) * a;
}

//

consteval tr::angle tr::literals::angle_literals::operator""_deg(long double deg)
{
	return degs(deg);
}

consteval tr::angle tr::literals::angle_literals::operator""_deg(unsigned long long deg)
{
	return degs(deg);
}

consteval tr::angle tr::literals::angle_literals::operator""_rad(long double rad)
{
	return rads(rad);
}

consteval tr::angle tr::literals::angle_literals::operator""_rad(unsigned long long rad)
{
	return rads(rad);
}

consteval tr::angle tr::literals::angle_literals::operator""_tr(long double tr)
{
	return turns(tr);
}

consteval tr::angle tr::literals::angle_literals::operator""_tr(unsigned long long tr)
{
	return turns(tr);
}

//

constexpr tr::angle& tr::angle::operator+=(angle r)
{
	m_rads += r.m_rads;
	return *this;
}

constexpr tr::angle& tr::angle::operator-=(angle r)
{
	m_rads -= r.m_rads;
	return *this;
}

template <tr::arithmetic Factor>
constexpr tr::angle& tr::angle::operator*=(Factor r)
{
	m_rads *= r;
	return *this;
}

template <tr::arithmetic Factor>
constexpr tr::angle& tr::angle::operator/=(Factor r)
{
	m_rads /= r;
	return *this;
}

constexpr tr::angle tr::operator+(angle lhs, angle rhs)
{
	return angle{lhs.m_rads + rhs.m_rads};
}

constexpr tr::angle tr::operator-(angle lhs, angle rhs)
{
	return angle{lhs.m_rads - rhs.m_rads};
}

constexpr tr::angle tr::operator-(angle lhs)
{
	return angle{-lhs.m_rads};
}

template <tr::arithmetic Factor>
constexpr tr::angle tr::operator*(angle lhs, Factor rhs)
{
	return angle{lhs.m_rads * rhs};
}

template <tr::arithmetic Factor>
constexpr tr::angle tr::operator*(Factor lhs, angle rhs)
{
	return rhs + lhs;
}

template <tr::arithmetic Factor>
constexpr tr::angle tr::operator/(angle lhs, Factor rhs)
{
	return angle{lhs.m_rads / rhs};
}

constexpr float tr::operator/(angle lhs, angle rhs)
{
	return lhs.m_rads / rhs.m_rads;
}

constexpr tr::angle tr::operator%(angle lhs, angle rhs)
{
	return angle{std::fmod(lhs.m_rads, rhs.m_rads)};
}

//

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

//

namespace std
{
	/// Angle formatter.
	template <>
	class formatter<tr::angle> : public formatter<float>, public formatter<const char*>
	{
	  public:
		/// Parses the format specification.
		template <typename ParseContext>
		constexpr auto parse(ParseContext& ctx)
		{
			auto ctx_it{ctx.begin()};
			if (ctx_it == ctx.end() || (*ctx_it != 'r' && *ctx_it != 'd' && *ctx_it != 't')) {
				throw std::format_error{"One of {r, d, t} must start an angle formatting specification."};
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

		/// Formats an angle.
		template <typename FormatContext>
		constexpr auto format(const tr::angle& p, FormatContext& ctx) const
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

	  private:
		/// Units the formatted angle can be displayed in.
		enum class unit
		{
			/// Display the angle in radians.
			radians,
			/// Display the angle in degrees.
			degrees,
			/// Display the angle in turns.
			turns
		};

		//

		/// The unit to use for the formatted angle.
		unit m_unit;
	};
} // namespace std