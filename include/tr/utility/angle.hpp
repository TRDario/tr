#pragma once
#include "concepts.hpp"

namespace tr {
	// Angular value type.
	template <std::floating_point T> class angle {
	  public:
		constexpr angle() = default;
		// Constructs an angle from a numeric value in radians.
		constexpr explicit angle(T rads);
		// Constructs an angle from another angle (potentially of a different type).
		template <std::floating_point U> constexpr angle(angle<U> th);

		template <std::floating_point U> constexpr auto operator<=>(const angle<U>& r) const;
		template <std::floating_point U> constexpr bool operator==(const angle<U>& r) const;

		template <std::floating_point U> constexpr angle& operator+=(const angle<U>& r);
		template <std::floating_point U> constexpr angle& operator-=(const angle<U>& r);
		template <arithmetic U> constexpr angle& operator*=(const U& r);
		template <arithmetic U> constexpr angle& operator/=(const U& r);

		constexpr angle operator-() const;
		template <std::floating_point U> constexpr auto operator+(const angle<U>& r) const;
		template <std::floating_point U> constexpr auto operator-(const angle<U>& r) const;
		template <arithmetic U> constexpr auto operator*(const U& r) const;
		template <arithmetic U> constexpr auto operator/(const U& r) const;
		template <std::floating_point U> constexpr auto operator/(const angle<U>& r) const;
		template <std::floating_point U> constexpr auto operator%(const angle<U>& r) const;

		// Converts the angle value into a numeric radian value.
		constexpr T rads() const;
		// Converts the angle value into a numeric degree value.
		constexpr T degs() const;
		// Converts the angle value into a numeric turn value.
		constexpr T turns() const;

		// Computes the sine of the angle.
		constexpr T sin() const;
		// Computes the cosine of the angle.
		constexpr T cos() const;
		// Computes the tangent of the angle.
		constexpr T tan() const;

	  private:
		// The underlying value in radians.
		T base;
	};
	using fangle = angle<float>;
	using dangle = angle<double>;
	using langle = angle<long double>;

	// Converts a numeric value in radians into an angle value.
	template <arithmetic T> constexpr auto rads(T th);
	// Converts a numeric value in degrees into an angle value.
	template <arithmetic T> constexpr auto degs(T th);
	// Converts a numeric value in turns into an angle value.
	template <arithmetic T> constexpr auto turns(T th);

	// Converts a sine value into an angle value.
	template <arithmetic T> constexpr auto asin(T sin);
	// Converts a cosine value into an angle value.
	template <arithmetic T> constexpr auto acos(T cos);
	// Converts a tangent value into an angle value.
	template <arithmetic T> constexpr auto atan(T tan);
	// Converts tangent x and y values into an angle value.
	template <arithmetic T> constexpr auto atan2(T y, T x);

	// Inline namespace containing angle value literals.
	inline namespace angle_literals {
		// fangle degree literal.
		consteval fangle operator""_degf(long double deg);
		// fangle degree literal.
		consteval fangle operator""_degf(unsigned long long deg);
		// dangle degree literal.
		consteval dangle operator""_deg(long double deg);
		// dangle degree literal.
		consteval dangle operator""_deg(unsigned long long deg);
		// langle degree literal.
		consteval langle operator""_degl(long double deg);
		// langle degree literal.
		consteval langle operator""_degl(unsigned long long deg);
		// fangle radian literal.
		consteval fangle operator""_radf(long double rad);
		// fangle radian literal.
		consteval fangle operator""_radf(unsigned long long rad);
		// dangle radian literal.
		consteval dangle operator""_rad(long double rad);
		// dangle radian literal.
		consteval dangle operator""_rad(unsigned long long rad);
		// langle radian literal.
		consteval langle operator""_radl(long double rad);
		// langle radian literal.
		consteval langle operator""_radl(unsigned long long rad);
		// fangle turns literal.
		consteval fangle operator""_turnsf(long double tr);
		// fangle turns literal.
		consteval fangle operator""_turnsf(unsigned long long tr);
		// dangle turns literal.
		consteval dangle operator""_turns(long double tr);
		// dangle turns literal.
		consteval dangle operator""_turns(unsigned long long tr);
		// langle turns literal.
		consteval langle operator""_turnsl(long double tr);
		// langle turns literal.
		consteval langle operator""_turnsl(unsigned long long tr);
	} // namespace angle_literals

	/// @}
} // namespace tr

// Angle formatter.
// The formatting specification is: [r, d, t] + a valid floating point formatting specification.
template <std::floating_point T> class std::formatter<tr::angle<T>> : public std::formatter<T>, std::formatter<const char*> {
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
		return std::formatter<T>::parse(ctx);
	}

	template <typename FormatContext> constexpr auto format(const tr::angle<T>& p, FormatContext& ctx) const
	{
		switch (unit) {
		case unit_type::RADS:
			ctx.advance_to(std::formatter<T>::format(p.rads(), ctx));
			ctx.advance_to(std::formatter<const char*>::format("rad", ctx));
			break;
		case unit_type::DEGREES:
			ctx.advance_to(std::formatter<T>::format(p.degs(), ctx));
			ctx.advance_to(std::formatter<const char*>::format("deg", ctx));
			break;
		case unit_type::TURNS:
			ctx.advance_to(std::formatter<T>::format(p.turns(), ctx));
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