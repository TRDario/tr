///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an abstract angle datatype, as well as related functions and literals.                                                       //
//                                                                                                                                       //
// Angles are internally stored with their value in radians in 32-bit floating point form, so all caveats pertaining to precision and    //
// comparison of floating-point values applies to angles as well.                                                                        //
//                                                                                                                                       //
// Angles are constructed with the deafult constructor, one of three unit functions/literals, or one of the trigonometric functions:     //
//     - tr::angle th; -> zero angle.                                                                                                    //
//     - tr::rads(5) or 5_rad -> 5-radian angle.                                                                                         //
//     - tr::degs(45) or 45_deg -> 45-degree angle.                                                                                      //
//     - tr::turns(1) or 1_tr -> 1-turn angle.                                                                                           //
//     - tr::asin(1) -> 90-degree angle.                                                                                                 //
//     - tr::acos(0) -> 90-degree angle.                                                                                                 //
//     - tr::atan(1) -> 45-degree angle.                                                                                                 //
//     - tr::atan2(-1, 1) -> 335-degree angle.                                                                                           //
// The literals are in the inline namespace tr::angle_literals, as well as tr::literals.                                                 //
//                                                                                                                                       //
// Angles can be added, subtracted, divided or moduloed, or can be multiplied and divided with a scalar:                                 //
//     - 0.5_tr + 90_deg -> 270-degree angle.                                                                                            //
//     - 1_tr - 45_deg -> 335-degree angle.                                                                                              //
//     - 1_tr / 1_deg -> 360.0f                                                                                                          //
//     - 414_deg % 1_tr -> 54-degree angle.                                                                                              //
//     - 1_rad * 2.5 -> 2.5-radian angle.                                                                                                //
//     - 1_tr / 4 -> 0.25-turn angle.                                                                                                    //
//                                                                                                                                       //
// The sine, cosine, or tangent of an angle can be gotten through a method:                                                              //
//     - (90_deg).sin() -> 1.0f                                                                                                          //
//     - (90_deg).cos() -> 0.0f                                                                                                          //
//     - (45_deg).tan() -> 1.0f                                                                                                          //
//                                                                                                                                       //
// Angles can be turned back into raw numbers using one of three unit methods:                                                           //
//     - (1_tr).rads() -> 6.28319f                                                                                                       //
//     - (1_tr).degs() -> 360.0f                                                                                                         //
//     - (1_tr).turns() -> 1.0f                                                                                                          //
//                                                                                                                                       //
// Angles can be formatted with std::format in radians, degrees, or turns, and may use floating-point formatting specifications:         //
//     - std::format("{:r.2f}", 90_deg) -> 1.57rad                                                                                       //
//     - std::format("{:d05.1f}", 90_deg) -> 090.0deg                                                                                    //
//     - std::format("{:t}", 90_deg) -> 0.25tr                                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Abstract angle datatype.
	class angle {
	  public:
		// Constructs a zero angle.
		constexpr angle() = default;

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
		inline float sin() const;
		// Computes the cosine of the angle.
		inline float cos() const;
		// Computes the tangent of the angle.
		inline float tan() const;

	  private:
		// The value of the angle in radians.
		float m_rads;

		// Constructs an angle from a numeric value in radians.
		constexpr explicit angle(float rads);

		template <arithmetic T> friend constexpr angle rads(T th);
	};

	// Constructs an angle expressed in radians.
	template <arithmetic T> constexpr angle rads(T th);
	// Constructs an angle expressed in degrees.
	template <arithmetic T> constexpr angle degs(T th);
	// Constructs ang angle expressed in turns.
	template <arithmetic T> constexpr angle turns(T th);

	// Computes the principal value of the arc sine.
	template <arithmetic T> inline angle asin(T sin);
	// Computes the principal value of the arc cosine.
	template <arithmetic T> inline angle acos(T cos);
	// Computes the principal value of the arc tangent.
	template <arithmetic T> inline angle atan(T tan);
	// Computes the principal value of the arc tangent y/x.
	template <arithmetic T> inline angle atan2(T y, T x);

	inline namespace literals {
		// Angle literals.
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
	} // namespace literals
} // namespace tr

namespace TR_FMT {
	// Angle formatter.
	template <> class formatter<tr::angle> : public formatter<float>, public formatter<const char*> {
	  public:
		// Parses the format specification.
		template <class ParseContext> constexpr auto parse(ParseContext& ctx);
		// Formats an angle.
		template <class FormatContext> constexpr auto format(const tr::angle& p, FormatContext& ctx) const;

	  private:
		// Units the formatted angle can be displayed in.
		enum class unit {
			radians, // Display the angle in radians.
			degrees, // Display the angle in degrees.
			turns    // Display the angle in turns.
		};

		// The unit to use for the formatted angle.
		unit m_unit;
	};
} // namespace TR_FMT

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "angle_impl.hpp" // IWYU pragma: export