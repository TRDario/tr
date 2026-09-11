/// @file
/// @brief Implements angle.hpp.

#pragma once
#include "../angle.hpp"
#include "../math.hpp"

//

constexpr tr::angle::angle(float rads) noexcept
	: m_rads{rads}
{
}

template <tr::arithmetic Number>
constexpr tr::angle tr::rads(Number th) noexcept
{
	return angle{static_cast<float>(th)};
}

template <tr::arithmetic Number>
constexpr tr::angle tr::degs(Number th) noexcept
{
	return rads(std::numbers::pi_v<float> / 180 * static_cast<float>(th));
}

template <tr::arithmetic Number>
constexpr tr::angle tr::turns(Number th) noexcept
{
	return rads(std::numbers::pi_v<float> * 2 * static_cast<float>(th));
}

template <tr::arithmetic Number>
tr::angle tr::asin(Number sin) noexcept
{
	return rads(std::asin(sin));
}

template <tr::arithmetic Number>
tr::angle tr::acos(Number cos) noexcept
{
	return rads(std::acos(cos));
}

template <tr::arithmetic Number>
tr::angle tr::atan(Number tan) noexcept
{
	return rads(std::atan(tan));
}

template <tr::arithmetic Number>
tr::angle tr::atan2(Number y, Number x) noexcept
{
	return rads(std::atan2(y, x));
}

template <tr::arithmetic Number>
tr::angle tr::atan2(glm::tvec2<Number> p) noexcept
{
	return tr::atan2(p.y, p.x);
}

//

constexpr tr::angle tr::abs(tr::angle th) noexcept
{
	return th >= tr::angle{} ? th : -th;
}

template <std::floating_point Ratio>
constexpr tr::angle tr::lerp(angle x, angle y, Ratio a) noexcept
{
	x = eucmod(x, turns(1.0f));
	y = eucmod(y, turns(1.0f));
	if (abs(y - x) >= turns(0.5f)) {
		(x > y ? x : y) -= turns(1.0f);
	}
	return x + (y - x) * a;
}

//

consteval tr::angle tr::literals::angle_literals::operator""_deg(long double deg) noexcept
{
	return degs(deg);
}

consteval tr::angle tr::literals::angle_literals::operator""_deg(unsigned long long deg) noexcept
{
	return degs(deg);
}

consteval tr::angle tr::literals::angle_literals::operator""_rad(long double rad) noexcept
{
	return rads(rad);
}

consteval tr::angle tr::literals::angle_literals::operator""_rad(unsigned long long rad) noexcept
{
	return rads(rad);
}

consteval tr::angle tr::literals::angle_literals::operator""_tr(long double tr) noexcept
{
	return turns(tr);
}

consteval tr::angle tr::literals::angle_literals::operator""_tr(unsigned long long tr) noexcept
{
	return turns(tr);
}

//

constexpr tr::angle& tr::angle::operator+=(angle r) noexcept
{
	m_rads += r.m_rads;
	return *this;
}

constexpr tr::angle& tr::angle::operator-=(angle r) noexcept
{
	m_rads -= r.m_rads;
	return *this;
}

template <tr::arithmetic Factor>
constexpr tr::angle& tr::angle::operator*=(Factor r) noexcept
{
	m_rads *= r;
	return *this;
}

template <tr::arithmetic Factor>
constexpr tr::angle& tr::angle::operator/=(Factor r) noexcept
{
	m_rads /= r;
	return *this;
}

[[nodiscard]] constexpr tr::angle tr::operator+(angle lhs, angle rhs) noexcept
{
	return angle{lhs.m_rads + rhs.m_rads};
}

[[nodiscard]] constexpr tr::angle tr::operator-(angle lhs, angle rhs) noexcept
{
	return angle{lhs.m_rads - rhs.m_rads};
}

[[nodiscard]] constexpr tr::angle tr::operator-(angle lhs) noexcept
{
	return angle{-lhs.m_rads};
}

template <tr::arithmetic Factor>
[[nodiscard]] constexpr tr::angle tr::operator*(angle lhs, Factor rhs) noexcept
{
	return angle{lhs.m_rads * rhs};
}

template <tr::arithmetic Factor>
[[nodiscard]] constexpr tr::angle tr::operator*(Factor lhs, angle rhs) noexcept
{
	return rhs + lhs;
}

template <tr::arithmetic Factor>
[[nodiscard]] constexpr tr::angle tr::operator/(angle lhs, Factor rhs) noexcept
{
	return angle{lhs.m_rads / rhs};
}

[[nodiscard]] constexpr float tr::operator/(angle lhs, angle rhs) noexcept
{
	return lhs.m_rads / rhs.m_rads;
}

[[nodiscard]] constexpr tr::angle tr::operator%(angle lhs, angle rhs) noexcept
{
	return angle{std::fmod(lhs.m_rads, rhs.m_rads)};
}

//

constexpr float tr::angle::rads() const noexcept
{
	return m_rads;
}

constexpr float tr::angle::degs() const noexcept
{
	return m_rads * (180 / std::numbers::pi_v<float>);
}

constexpr float tr::angle::turns() const noexcept
{
	return m_rads / (2 * std::numbers::pi_v<float>);
}

inline float tr::angle::sin() const noexcept
{
	return std::sin(m_rads);
}

inline float tr::angle::cos() const noexcept
{
	return std::cos(m_rads);
}

inline float tr::angle::tan() const noexcept
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