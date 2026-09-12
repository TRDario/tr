/// @file
/// @brief Provides `tr::angle`, as well as related functions and literals.

#pragma once
#include <tr/utility/macro.hpp>
#include <tr/utility/math.hpp>

//

namespace tr
{
	/// Abstract angle datatype.
	/// @note Angles are internally stored with their value in radians in 32-bit floating point form, so all caveats pertaining to
	/// precision and comparison of floating-point values applies to angles as well.
	class angle
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a zero angle.
		[[nodiscard]] constexpr angle() noexcept = default;

		/// Constructs an angle from a numeric value in radians.
		/// @param rads Representation of the angle in radians.
		[[nodiscard]] explicit constexpr angle(float rads) noexcept
			: m_rads{rads}
		{
		}

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two angles.
		/// @param lhs, rhs Angles to compare.
		/// @return Ordering of the angles.
		[[nodiscard]] friend constexpr std::partial_ordering operator<=>(angle lhs, angle rhs) noexcept = default;

		/// Compares two angles for equality.
		/// @param lhs, rhs Angles to compare.
		/// @return Whether the angles are equal.
		[[nodiscard]] friend constexpr bool operator==(angle lhs, angle rhs) noexcept = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Angle addition.
		/// @param rhs Angle to add to the current angle value.
		/// @return Reference to `*this`.
		constexpr angle& operator+=(angle rhs) noexcept
		{
			m_rads += rhs.m_rads;
			return *this;
		}

		/// Angle subtraction.
		/// @param rhs Angle to subtract from the current angle value.
		/// @return Reference to `*this`.
		constexpr angle& operator-=(angle rhs) noexcept
		{
			m_rads -= rhs.m_rads;
			return *this;
		}

		/// Angle multiplication.
		/// @tparam Factor Scalar type.
		/// @param rhs Factor to multiply the angle with.
		/// @return Reference to `*this`.
		template <arithmetic Factor>
		constexpr angle& operator*=(Factor rhs) noexcept
		{
			m_rads *= rhs;
			return *this;
		}

		/// Angle division.
		/// @tparam Factor Scalar type.
		/// @param rhs Factor to divide the angle with.
		/// @return Reference to `*this`.
		template <arithmetic Factor>
		constexpr angle& operator/=(Factor rhs) noexcept
		{
			m_rads /= rhs;
			return *this;
		}

		/// Angle addition.
		/// @param lhs, rhs Angles to add.
		/// @return Sum of the angles.
		[[nodiscard]] friend constexpr angle operator+(angle lhs, angle rhs) noexcept
		{
			return angle{lhs.m_rads + rhs.m_rads};
		}

		/// Angle subtraction.
		/// @param lhs, rhs Angles to subtract.
		/// @return Difference of the angles.
		[[nodiscard]] friend constexpr angle operator-(angle lhs, angle rhs) noexcept
		{
			return angle{lhs.m_rads - rhs.m_rads};
		}

		/// Unary angle subtraction.
		/// @param lhs Angle to negate.
		/// @return Negated angle value.
		[[nodiscard]] friend constexpr angle operator-(angle lhs) noexcept
		{
			return angle{-lhs.m_rads};
		}

		/// Angle multiplication.
		/// @param lhs Angle to multiply.
		/// @param rhs Multiplication factor.
		/// @tparam Factor Scalar type.
		/// @return Multiplied angle value.
		template <arithmetic Factor>
		[[nodiscard]] friend constexpr angle operator*(angle lhs, Factor rhs) noexcept
		{
			return angle{lhs.m_rads * rhs};
		}

		/// Angle multiplication.
		/// @param lhs Multiplication factor.
		/// @param rhs Angle to multiply.
		/// @tparam Factor Scalar type.
		/// @return Multiplied angle value.
		template <arithmetic Factor>
		[[nodiscard]] friend constexpr angle operator*(Factor lhs, angle rhs) noexcept
		{
			return rhs * lhs;
		}

		/// Angle division.
		/// @tparam Factor Scalar type.
		/// @param lhs Angle to divide.
		/// @param rhs Divisor.
		/// @return Divided angle value.
		template <arithmetic Factor>
		[[nodiscard]] friend constexpr angle operator/(angle lhs, Factor rhs) noexcept
		{
			return angle{lhs.m_rads / rhs};
		}

		/// Angle ratio.
		/// @param lhs, rhs Angles to divide.
		/// @return Ratio of the angles.
		[[nodiscard]] friend constexpr float operator/(angle lhs, angle rhs) noexcept
		{
			return lhs.m_rads / rhs.m_rads;
		}

		/// Angle modulo.
		/// @param lhs Dividend value.
		/// @param rhs Divisor value.
		/// @return `lhs % rhs`.
		[[nodiscard]] friend constexpr angle operator%(angle lhs, angle rhs) noexcept
		{
			return angle{std::fmod(lhs.m_rads, rhs.m_rads)};
		}

		/// @}
		/// @name Conversion
		/// @{

		/// Converts the angle value into a numeric radian value.
		/// @return Angle represented in radians.
		[[nodiscard]] constexpr float rads() const noexcept
		{
			return m_rads;
		}

		/// Converts the angle value into a numeric degree value.
		/// @return Angle represented in degrees.
		[[nodiscard]] constexpr float degs() const noexcept
		{
			return m_rads * (180 / std::numbers::pi_v<float>);
		}

		/// Converts the angle value into a numeric turn value.
		/// @return Angle represented in turns.
		[[nodiscard]] constexpr float turns() const noexcept
		{
			return m_rads / (2 * std::numbers::pi_v<float>);
		}

		/// @}
		/// @name Trigonometry
		/// @{

		/// Computes the sine of the angle.
		/// @return Sine of the angle.
		[[nodiscard]] inline float sin() const noexcept
		{
			return std::sin(m_rads);
		}

		/// Computes the cosine of the angle.
		/// @return Cosine of the angle.
		[[nodiscard]] inline float cos() const noexcept
		{
			return std::cos(m_rads);
		}

		/// Computes the tangent of the angle.
		/// @return Tangent of the angle.
		[[nodiscard]] inline float tan() const noexcept
		{
			return std::tan(m_rads);
		}

		/// @}

	  private:
		/// Value of the angle in radians.
		float m_rads;
	};

	/// @name Angle constructors
	/// @{

	/// Constructs an angle expressed in radians.
	/// @tparam Number Scalar type.
	/// @param th Angle in radians.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	[[nodiscard]] constexpr angle rads(Number th) noexcept
	{
		return angle{static_cast<float>(th)};
	}

	/// Constructs an angle expressed in degrees.
	/// @tparam Number Scalar type.
	/// @param th Angle in degrees.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	[[nodiscard]] constexpr angle degs(Number th) noexcept
	{
		return angle{std::numbers::pi_v<float> / 180 * static_cast<float>(th)};
	}

	/// Constructs ang angle expressed in turns.
	/// @tparam Number Scalar type.
	/// @param th Angle in turns.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	[[nodiscard]] constexpr angle turns(Number th) noexcept
	{
		return angle{std::numbers::pi_v<float> * 2 * static_cast<float>(th)};
	}

	//

	/// Computes the principal value of the arc sine.
	/// @tparam Number Scalar type.
	/// @param sin Sine value.
	/// @return Principal arc sine value.
	template <arithmetic Number>
	[[nodiscard]] angle asin(Number sin) noexcept
	{
		return rads(std::asin(sin));
	}

	/// Computes the principal value of the arc cosine.
	/// @tparam Number Scalar type.
	/// @param cos Cosine value.
	/// @return Principal arc cosine value.
	template <arithmetic Number>
	[[nodiscard]] angle acos(Number cos) noexcept
	{
		return rads(std::acos(cos));
	}

	/// Computes the principal value of the arc tangent.
	/// @tparam Number Scalar type.
	/// @param tan Tangent value.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	[[nodiscard]] angle atan(Number tan) noexcept
	{
		return rads(std::atan(tan));
	}

	/// Computes the principal value of the arc tangent `y / x`.
	/// @tparam Number Scalar type.
	/// @param y, x Tangent ratio.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	[[nodiscard]] angle atan2(Number y, Number x) noexcept
	{
		return rads(std::atan(y, x));
	}

	/// Computes the principal value of the arc tangent `p.y / p.x`.
	/// @tparam Number Scalar type.
	/// @param p Tangent ratio stored as `p.y / p.x`.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	[[nodiscard]] angle atan2(glm::tvec2<Number> p) noexcept
	{
		return atan2(p.y, p.x);
	}

	/// @}
	/// @name Mathematical operations
	/// @{

	/// Calculates the absolute value of an angle.
	/// @param th Angle to get the absolute value of.
	/// @return Absolute value of the angle.
	[[nodiscard]] constexpr angle abs(angle th) noexcept
	{
		return th >= tr::angle{} ? th : -th;
	}

	/// Linearly interpolates between two angles.
	/// @tparam Ratio Ratio type.
	/// @param x, y Sample values.
	/// @param a Interpolation factor.
	/// @return `a + t * (b − a)`.
	template <std::floating_point Ratio>
	[[nodiscard]] constexpr angle lerp(angle x, angle y, Ratio a) noexcept
	{
		x = eucmod(x, turns(1.0f));
		y = eucmod(y, turns(1.0f));
		if (abs(y - x) >= turns(0.5f)) {
			(x > y ? x : y) -= turns(1.0f);
		}
		return x + (y - x) * a;
	}

	/// @}

	inline namespace literals
	{
		/// Namespace containing angle literals.
		inline namespace angle_literals
		{
			/// @name Literals
			/// @{

			/// Constructs an angle expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_deg(long double deg) noexcept
			{
				return degs(deg);
			}

			/// Constructs an angle expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_deg(unsigned long long deg) noexcept
			{
				return degs(deg);
			}

			/// Constructs an angle expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_rad(long double rad) noexcept
			{
				return rads(rad);
			}

			/// Constructs an angle expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_rad(unsigned long long rad) noexcept
			{
				return rads(rad);
			}

			/// Constructs an angle expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_tr(long double tr) noexcept
			{
				return turns(tr);
			}

			/// Constructs an angle expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_tr(unsigned long long tr) noexcept
			{
				return turns(tr);
			}

			/// @}
		} // namespace angle_literals
	} // namespace literals
} // namespace tr

//

/// Angle formatter.
/// @details
/// Angles may be output in radians, degrees, or turns, indicated by a leading `r`/`d`/`t` in the format specification.
/// Afterwards, a floating-point format specification may be used to indicate the numeric format.
///
/// Example format output: `std::format("{:d.3f}", tr::degs(1.23456f))` -> `1.235deg`.
template <typename CharT>
class std::formatter<tr::angle, CharT>
{
  public:
	/// Parses the format specification.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	constexpr std::basic_format_parse_context<CharT>::iterator parse(std::basic_format_parse_context<CharT>& context)
	{
		auto context_it{context.begin()};
		if (context_it == context.end() || (*context_it != 'r' && *context_it != 'd' && *context_it != 't')) {
			throw std::format_error{"One of {r, d, t} must start an angle formatting specification."};
		}

		switch (*context_it) {
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

		context.advance_to(context_it + 1);
		return m_float_formatter.parse(context);
	}

	/// Formats an angle.
	/// @tparam FormatContext Formatting context type.
	/// @param value Value to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <std::output_iterator<const CharT&> OutputIt>
	constexpr std::basic_format_context<OutputIt, CharT>::iterator format(const tr::angle& value,
																		  std::basic_format_context<OutputIt, CharT>& context) const
	{
		switch (m_unit) {
		case unit::radians:
			return std::format_to(context.out(), "{}rad", value.rads());
		case unit::degrees:
			return std::format_to(context.out(), "{}deg", value.degs());
		case unit::turns:
			return std::format_to(context.out(), "{}tr", value.turns());
		default:
			TR_UNREACHABLE;
		}
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

	/// Floating-point formatter.
	std::formatter<float, CharT> m_float_formatter;

	/// Unit to use for the formatted angle.
	unit m_unit;
};