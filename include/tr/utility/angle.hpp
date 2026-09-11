/// @file
/// @brief Provides `tr::angle`, as well as related functions and literals.

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
		[[nodiscard]] constexpr angle() noexcept = default;

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
		constexpr angle& operator+=(angle rhs) noexcept;

		/// Angle subtraction.
		/// @param rhs Angle to subtract from the current angle value.
		/// @return Reference to `*this`.
		constexpr angle& operator-=(angle rhs) noexcept;

		/// Angle multiplication.
		/// @tparam Factor Scalar type.
		/// @param rhs Factor to multiply the angle with.
		/// @return Reference to `*this`.
		template <arithmetic Factor>
		constexpr angle& operator*=(Factor rhs) noexcept;

		/// Angle division.
		/// @tparam Factor Scalar type.
		/// @param rhs Factor to divide the angle with.
		/// @return Reference to `*this`.
		template <arithmetic Factor>
		constexpr angle& operator/=(Factor rhs) noexcept;

		/// Angle addition.
		/// @param lhs, rhs Angles to add.
		/// @return Sum of the angles.
		friend constexpr angle operator+(angle lhs, angle rhs) noexcept;

		/// Angle subtraction.
		/// @param lhs, rhs Angles to subtract.
		/// @return Difference of the angles.
		friend constexpr angle operator-(angle lhs, angle rhs) noexcept;

		/// Unary angle subtraction.
		/// @param lhs Angle to negate.
		/// @return Negated angle value.
		friend constexpr angle operator-(angle lhs) noexcept;

		/// Angle multiplication.
		/// @param lhs Angle to multiply.
		/// @param rhs Multiplication factor.
		/// @tparam Factor Scalar type.
		/// @return Multiplied angle value.
		template <arithmetic Factor>
		friend constexpr angle operator*(angle lhs, Factor rhs) noexcept;

		/// Angle multiplication.
		/// @param lhs Multiplication factor.
		/// @param rhs Angle to multiply.
		/// @tparam Factor Scalar type.
		/// @return Multiplied angle value.
		template <arithmetic Factor>
		friend constexpr angle operator*(Factor lhs, angle rhs) noexcept;

		/// Angle division.
		/// @tparam Factor Scalar type.
		/// @param lhs Angle to divide.
		/// @param rhs Divisor.
		/// @return Divided angle value.
		template <arithmetic Factor>
		friend constexpr angle operator/(angle lhs, Factor rhs) noexcept;

		/// Angle ratio.
		/// @param lhs, rhs Angles to divide.
		/// @return Ratio of the angles.
		friend constexpr float operator/(angle lhs, angle rhs) noexcept;

		/// Angle modulo.
		/// @param lhs Dividend value.
		/// @param rhs Divisor value.
		/// @return `lhs % rhs`.
		friend constexpr angle operator%(angle lhs, angle rhs) noexcept;

		/// @}
		/// @name Conversion
		/// @{

		/// Converts the angle value into a numeric radian value.
		/// @return Angle represented in radians.
		[[nodiscard]] constexpr float rads() const noexcept;

		/// Converts the angle value into a numeric degree value.
		/// @return Angle represented in degrees.
		[[nodiscard]] constexpr float degs() const noexcept;

		/// Converts the angle value into a numeric turn value.
		/// @return Angle represented in turns.
		[[nodiscard]] constexpr float turns() const noexcept;

		/// @}
		/// @name Trigonometry
		/// @{

		/// Computes the sine of the angle.
		/// @return Sine of the angle.
		[[nodiscard]] inline float sin() const noexcept;

		/// Computes the cosine of the angle.
		/// @return Cosine of the angle.
		[[nodiscard]] inline float cos() const noexcept;

		/// Computes the tangent of the angle.
		/// @return Tangent of the angle.
		[[nodiscard]] inline float tan() const noexcept;

		/// @}

	  private:
		/// Value of the angle in radians.
		float m_rads;

		//

		/// Constructs an angle from a numeric value in radians.
		/// @return rads Representation of the angle in radians.
		[[nodiscard]] constexpr explicit angle(float rads) noexcept;

		//

		// Uses the private constructor.
		template <arithmetic Number>
		friend constexpr angle rads(Number th) noexcept;
	};

	/// @name Angle constructors
	/// @{

	/// Constructs an angle expressed in radians.
	/// @tparam Number Scalar type.
	/// @param th Angle in radians.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	[[nodiscard]] constexpr angle rads(Number th) noexcept;

	/// Constructs an angle expressed in degrees.
	/// @tparam Number Scalar type.
	/// @param th Angle in degrees.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	[[nodiscard]] constexpr angle degs(Number th) noexcept;

	/// Constructs ang angle expressed in turns.
	/// @tparam Number Scalar type.
	/// @param th Angle in turns.
	/// @return Equivalent angle value.
	template <arithmetic Number>
	[[nodiscard]] constexpr angle turns(Number th) noexcept;

	//

	/// Computes the principal value of the arc sine.
	/// @tparam Number Scalar type.
	/// @param sin Sine value.
	/// @return Principal arc sine value.
	template <arithmetic Number>
	[[nodiscard]] angle asin(Number sin) noexcept;

	/// Computes the principal value of the arc cosine.
	/// @tparam Number Scalar type.
	/// @param cos Cosine value.
	/// @return Principal arc cosine value.
	template <arithmetic Number>
	[[nodiscard]] angle acos(Number cos) noexcept;

	/// Computes the principal value of the arc tangent.
	/// @tparam Number Scalar type.
	/// @param tan Tangent value.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	[[nodiscard]] angle atan(Number tan) noexcept;

	/// Computes the principal value of the arc tangent `y / x`.
	/// @tparam Number Scalar type.
	/// @param y, x Tangent ratio.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	[[nodiscard]] angle atan2(Number y, Number x) noexcept;

	/// Computes the principal value of the arc tangent `p.y / p.x`.
	/// @tparam Number Scalar type.
	/// @param p Tangent ratio stored as `p.y / p.x`.
	/// @return Principal arc tangent value.
	template <arithmetic Number>
	[[nodiscard]] angle atan2(glm::tvec2<Number> p) noexcept;

	/// @}
	/// @name Mathematical operations
	/// @{

	/// Calculates the absolute value of an angle.
	/// @param th Angle to get the absolute value of.
	/// @return Absolute value of the angle.
	[[nodiscard]] constexpr angle abs(angle th) noexcept;

	/// Linearly interpolates between two angles.
	/// @tparam Ratio Ratio type.
	/// @param x, y Sample values.
	/// @param a Interpolation factor.
	/// @return `a + t * (b − a)`.
	template <std::floating_point Ratio>
	[[nodiscard]] constexpr angle lerp(angle x, angle y, Ratio a) noexcept;

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
			[[nodiscard]] consteval angle operator""_deg(long double deg) noexcept;

			/// Constructs an angle expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_deg(unsigned long long deg) noexcept;

			/// Constructs an angle expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_rad(long double rad) noexcept;

			/// Constructs an angle expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_rad(unsigned long long rad) noexcept;

			/// Constructs an angle expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_tr(long double tr) noexcept;

			/// Constructs an angle expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle operator""_tr(unsigned long long tr) noexcept;

			/// @}
		} // namespace angle_literals
	} // namespace literals
} // namespace tr

#include "impl/angle.hpp" // IWYU pragma: export