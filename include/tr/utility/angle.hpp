#pragma once
#include "concepts.hpp"

namespace tr {
	// Angular value type.
	class angle {
	  public:
		constexpr angle() = default;
		// Constructs an angle from a numeric value in radians.
		constexpr explicit angle(float rads);

		constexpr std::partial_ordering operator<=>(const angle& r) const = default;
		constexpr bool operator==(const angle& r) const = default;

		constexpr angle& operator+=(const angle& r);
		constexpr angle& operator-=(const angle& r);
		template <arithmetic U> constexpr angle& operator*=(const U& r);
		template <arithmetic U> constexpr angle& operator/=(const U& r);

		friend constexpr angle operator+(const angle& l, const angle& r);
		friend constexpr angle operator-(const angle& l, const angle& r);
		friend constexpr angle operator-(const angle& l);
		template <arithmetic U> friend constexpr angle operator*(const angle& l, const U& r);
		template <arithmetic U> friend constexpr angle operator*(const U& l, const angle& r);
		template <arithmetic U> friend constexpr angle operator/(const angle& l, const U& r);
		friend constexpr float operator/(const angle& l, const angle& r);
		friend constexpr angle operator%(const angle& l, const angle& r);

		// Converts the angle value into a numeric radian value.
		constexpr float rads() const;
		// Converts the angle value into a numeric degree value.
		constexpr float degs() const;
		// Converts the angle value into a numeric turn value.
		constexpr float turns() const;

		// Computes the sine of the angle.
		constexpr float sin() const;
		// Computes the cosine of the angle.
		constexpr float cos() const;
		// Computes the tangent of the angle.
		constexpr float tan() const;

	  private:
		// The underlying value in radians.
		float base;
	};

	// Converts a numeric value in radians into an angle value.
	template <arithmetic T> constexpr angle rads(T th);
	// Converts a numeric value in degrees into an angle value.
	template <arithmetic T> constexpr angle degs(T th);
	// Converts a numeric value in turns into an angle value.
	template <arithmetic T> constexpr angle turns(T th);

	// Converts a sine value into an angle value.
	template <arithmetic T> constexpr angle asin(T sin);
	// Converts a cosine value into an angle value.
	template <arithmetic T> constexpr angle acos(T cos);
	// Converts a tangent value into an angle value.
	template <arithmetic T> constexpr angle atan(T tan);
	// Converts tangent x and y values into an angle value.
	template <arithmetic T> constexpr angle atan2(T y, T x);

	// Inline namespace containing angle value literals.
	inline namespace angle_literals {
		// Degree literal.
		consteval angle operator""_deg(long double deg);
		// Degree literal.
		consteval angle operator""_deg(unsigned long long deg);
		// Radian literal.
		consteval angle operator""_rad(long double rad);
		// Radian literal.
		consteval angle operator""_rad(unsigned long long rad);
		// Turns literal.
		consteval angle operator""_turns(long double tr);
		// Turns literal.
		consteval angle operator""_turns(unsigned long long tr);
	} // namespace angle_literals

	/// @}
} // namespace tr

// Angle formatter.
// The formatting specification is: [r, d, t] + a valid floating point formatting specification.
template <> class std::formatter<tr::angle> : public std::formatter<float>, public std::formatter<const char*> {
  public:
	template <class ParseContext> constexpr auto parse(ParseContext& ctx)
	{
		auto it = ctx.begin();
		if (it == ctx.end() || (*it != 'r' && *it != 'd' && *it != 't')) {
			throw std::format_error{"One of {r, d, t} must start an angle formatting specification."};
		}

		switch (*it) {
		case 'r':
			unit = unit_type::RADS;
			break;
		case 'd':
			unit = unit_type::DEGREES;
			break;
		case 't':
			unit = unit_type::TURNS;
			break;
		}

		ctx.advance_to(it + 1);
		return std::formatter<float>::parse(ctx);
	}

	template <typename FormatContext> constexpr auto format(const tr::angle& p, FormatContext& ctx) const
	{
		switch (unit) {
		case unit_type::RADS:
			ctx.advance_to(std::formatter<float>::format(p.rads(), ctx));
			ctx.advance_to(std::formatter<const char*>::format("rad", ctx));
			break;
		case unit_type::DEGREES:
			ctx.advance_to(std::formatter<float>::format(p.degs(), ctx));
			ctx.advance_to(std::formatter<const char*>::format("deg", ctx));
			break;
		case unit_type::TURNS:
			ctx.advance_to(std::formatter<float>::format(p.turns(), ctx));
			ctx.advance_to(std::formatter<const char*>::format("tr", ctx));
			break;
		}
		return ctx.out();
	}

  private:
	enum class unit_type {
		RADS,
		DEGREES,
		TURNS
	};

	unit_type unit{unit_type::RADS};
};

#include "angle_impl.hpp"