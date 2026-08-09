/// @file
/// @brief Provides an abstract angle datatype, as well as related functions and literals.

#pragma once
#include "concepts.hpp"

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
		constexpr angle() = default;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two angles.
		/// @param lhs, rhs Angles to compare.
		/// @return Ordering of the angles.
		friend constexpr std::partial_ordering operator<=>(angle lhs, angle rhs) = default;

		/// Compares two angles for equality.
		/// @param lhs, rhs Angles to compare.
		/// @return Whether the angles are equal.
		friend constexpr bool operator==(angle lhs, angle rhs) = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Angle addition.
		/// @param rhs Angle to add to the current angle value.
		/// @return Reference to `*this`.
		constexpr angle& operator+=(angle rhs);

		/// Angle subtraction.
		/// @param rhs Angle to subtract from the current angle value.
		/// @return Reference to `*this`.
		constexpr angle& operator-=(angle rhs);

		/// Angle multiplication.
		/// @tparam Factor Scalar type.
		/// @param rhs Factor to multiply the angle with.
		/// @return Reference to `*this`.
		template <arithmetic Factor>
		constexpr angle& operator*=(Factor rhs);

		/// Angle division.
		/// @tparam Factor Scalar type.
		/// @param rhs Factor to divide the angle with.
		/// @return Reference to `*this`.
		template <arithmetic Factor>
		constexpr angle& operator/=(Factor rhs);

		/// Angle addition.
		/// @param lhs, rhs Angles to add.
		/// @return Sum of the angles.
		friend constexpr angle operator+(angle lhs, angle rhs);

		/// Angle subtraction.
		/// @param lhs, rhs Angles to subtract.
		/// @return Difference of the angles.
		friend constexpr angle operator-(angle lhs, angle rhs);

		/// Unary angle subtraction.
		/// @param lhs Angle to negate.
		/// @return Negated angle value.
		friend constexpr angle operator-(angle lhs);

		/// Angle multiplication.
		/// @param lhs Angle to multiply.
		/// @param rhs Multiplication factor.
		/// @tparam Factor Scalar type.
		/// @return Multiplied angle value.
		template <arithmetic Factor>
		friend constexpr angle operator*(angle lhs, Factor rhs);

		/// Angle multiplication.
		/// @param lhs Multiplication factor.
		/// @param rhs Angle to multiply.
		/// @tparam Factor Scalar type.
		/// @return Multiplied angle value.
		template <arithmetic Factor>
		friend constexpr angle operator*(Factor lhs, angle rhs);

		/// Angle division.
		/// @tparam Factor Scalar type.
		/// @param lhs Angle to divide.
		/// @param rhs Divisor.
		/// @return Divided angle value.
		template <arithmetic Factor>
		friend constexpr angle operator/(angle lhs, Factor rhs);

		/// Angle ratio.
		/// @param lhs, rhs Angles to divide.
		/// @return Ratio of the angles.
		friend constexpr float operator/(angle lhs, angle rhs);

		/// Angle modulo.
		/// @param lhs Dividend value.
		/// @param rhs Divisor value.
		/// @return `lhs % rhs`.
		friend constexpr angle operator%(angle lhs, angle rhs);

		/// @}
		/// @name Conversion
		/// @{

		/// Converts the angle value into a numeric radian value.
		/// @return Angle represented in radians.
		constexpr float rads() const;

		/// Converts the angle value into a numeric degree value.
		/// @return Angle represented in degrees.
		constexpr float degs() const;

		/// Converts the angle value into a numeric turn value.
		/// @return Angle represented in turns.
		constexpr float turns() const;

		/// @}
		/// @name Trigonometry
		/// @{

		/// Computes the sine of the angle.
		/// @return Sine of the angle.
		inline float sin() const;

		/// Computes the cosine of the angle.
		/// @return Cosine of the angle.
		inline float cos() const;

		/// Computes the tangent of the angle.
		/// @return Tangent of the angle.
		inline float tan() const;

		/// @}

	  private:
		/// Value of the angle in radians.
		float m_rads;

		//

		/// Constructs an angle from a numeric value in radians.
		/// @return rads Representation of the angle in radians.
		constexpr explicit angle(float rads);

		//

		// Uses the private constructor.
		template <arithmetic Number>
		friend constexpr angle rads(Number th);
	};

	/// @name Angle constructors
	/// @{

	/// Constructs an angle expressed in radians.
	/// @tparam Number Scalar type.
	/// @param th Angle in radians.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	constexpr angle rads(Number th);

	/// Constructs an angle expressed in degrees.
	/// @tparam Number Scalar type.
	/// @param th Angle in degrees.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	constexpr angle degs(Number th);

	/// Constructs ang angle expressed in turns.
	/// @tparam Number Scalar type.
	/// @param th Angle in turns.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	constexpr angle turns(Number th);

	//

	/// Computes the principal value of the arc sine.
	/// @tparam Number Scalar type.
	/// @param sin Sine value.
	/// @return Principal arc sine value.
	template <arithmetic Number>
	inline angle asin(Number sin);

	/// Computes the principal value of the arc cosine.
	/// @tparam Number Scalar type.
	/// @param cos Cosine value.
	/// @return Principal arc cosine value.
	template <arithmetic Number>
	inline angle acos(Number cos);

	/// Computes the principal value of the arc tangent.
	/// @tparam Number Scalar type.
	/// @param tan Tangent value.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	inline angle atan(Number tan);

	/// Computes the principal value of the arc tangent `y / x`.
	/// @tparam Number Scalar type.
	/// @param y, x Tangent ratio.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	inline angle atan2(Number y, Number x);

	/// Computes the principal value of the arc tangent `p.y / p.x`.
	/// @tparam Number Scalar type.
	/// @param p Tangent ratio stored as `p.y / p.x`.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	inline angle atan2(glm::tvec2<Number> p);

	/// @}
	/// @name Mathematical operations
	/// @{

	/// Calculates the absolute value of an angle.
	/// @param th Angle to get the absolute value of.
	/// @return Absolute value of the angle.
	constexpr angle abs(angle th);

	/// Linearly interpolates between two angles.
	/// @tparam Ratio Ratio type.
	/// @param x, y Sample values.
	/// @param a Interpolation factor.
	/// @return `a + t * (b − a)`.
	template <std::floating_point Ratio>
	constexpr angle lerp(angle x, angle y, Ratio a);

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
			consteval angle operator""_deg(long double deg);

			/// Constructs an angle expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			consteval angle operator""_deg(unsigned long long deg);

			/// Constructs an angle expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			consteval angle operator""_rad(long double rad);

			/// Constructs an angle expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			consteval angle operator""_rad(unsigned long long rad);

			/// Constructs an angle expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			consteval angle operator""_tr(long double tr);

			/// Constructs an angle expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			consteval angle operator""_tr(unsigned long long tr);

			/// @}
		} // namespace angle_literals
	} // namespace literals
} // namespace tr

#include "impl/angle.hpp" // IWYU pragma: export