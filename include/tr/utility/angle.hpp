#pragma once
#include "concepts.hpp"

namespace tr {
	/** @defgroup utility Utility
	 *  Miscellaneous utility features.
	 *  @{
	 *  @}
	 */

	/** @ingroup utility
	 *  @defgroup angle Angles
	 *  Angle datatype and related functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Angular value type.
	 *
	 * @tparam T The base type of the angle, must be a floating point type.
	 ******************************************************************************************************************/
	template <std::floating_point T> class angle {
	  public:
		/**************************************************************************************************************
		 * Default-constructs an angle to 0 radians.
		 **************************************************************************************************************/
		constexpr angle() = default;

		/**************************************************************************************************************
		 * Constructs an angle from a numeric value.
		 *
		 * @note While this constructor is equivalent to tr::rads(), that function should be preferred for readibility.
		 *
		 * @param[in] rads The numeric value of the angle in radians.
		 **************************************************************************************************************/
		constexpr explicit angle(T rads);

		/**************************************************************************************************************
		 * Constructs an angle from another angle (potentially of a different type).
		 *
		 * @param[in] th The angle whose value to copy.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr angle(angle<U> th);

		/**************************************************************************************************************
		 * Three-way comparison operator.
		 *
		 * @param[in] r An angle to compare against.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr auto operator<=>(const angle<U>& r) const;

		/**************************************************************************************************************
		 * Equality comparison operator.
		 *
		 * @param[in] r An angle to compare against.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr bool operator==(const angle<U>& r) const;

		/**************************************************************************************************************
		 * Performs addition on the angle.
		 *
		 * @param[in] r The angle value to add.
		 *
		 * @return A reference to the angle being added to.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr angle& operator+=(const angle<U>& r);

		/**************************************************************************************************************
		 * Performs subtraction on the angle.
		 *
		 * @param[in] r The angle value to subtract.
		 *
		 * @return A reference to the angle being subtracted from.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr angle& operator-=(const angle<U>& r);

		/**************************************************************************************************************
		 * Performs multiplication on the angle.
		 *
		 * @param[in] r The multiplication factor.
		 *
		 * @return A reference to the angle being multiplied.
		 **************************************************************************************************************/
		template <arithmetic U> constexpr angle& operator*=(const U& r);

		/**************************************************************************************************************
		 * Performs division on the angle.
		 *
		 * @param[in] r The division factor.
		 *
		 * @return A reference to the angle being divided.
		 **************************************************************************************************************/
		template <arithmetic U> constexpr angle& operator/=(const U& r);

		/**************************************************************************************************************
		 * Negates the angle.

		 * @return The negated value of the angle.
		 **************************************************************************************************************/
		constexpr angle operator-() const;

		/**************************************************************************************************************
		 * Adds two angles.
		 *
		 * @param[in] r The angle value to add.
		 *
		 * @return The sum of the angles. Standard promotion rules apply to the resulting angle type.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr auto operator+(const angle<U>& r) const;

		/**************************************************************************************************************
		 * Subtracts two angles.
		 *
		 * @param[in] r The angle value to subtract.
		 *
		 * @return The difference of the angles. Standard promotion rules apply to the resulting angle type.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr auto operator-(const angle<U>& r) const;

		/**************************************************************************************************************
		 * Multiplies an angle by a factor.
		 *
		 * @param[in] r The multiplication factor.
		 *
		 * @return The value of the left side angle multiplied by the factor. Standard promotion rules apply to the
		 *		   resulting angle type.
		 **************************************************************************************************************/
		template <arithmetic U> constexpr auto operator*(const U& r) const;

		/**************************************************************************************************************
		 * Divides an angle by a factor.
		 *
		 * @param[in] r The division factor.
		 *
		 * @return The value of the left side angle divided by the factor. Standard promotion rules apply to the
		 * 		   resulting angle type.
		 **************************************************************************************************************/
		template <arithmetic U> constexpr auto operator/(const U& r) const;

		/**************************************************************************************************************
		 * Divides two angles.
		 *
		 * @param[in] r The angle value to divide by.
		 *
		 * @return The ratio between the angles. Standard promotion rules apply to the resulting angle type.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr auto operator/(const angle<U>& r) const;

		/**************************************************************************************************************
		 * Modulos an angle by another.
		 *
		 * @param[in] r The angle value to modulo by.
		 *
		 * @return The modulod value of the left side angle. Standard promotion rules apply to the resulting angle type.
		 **************************************************************************************************************/
		template <std::floating_point U> constexpr auto operator%(const angle<U>& r) const;

		/**************************************************************************************************************
		 * Converts the angle value into a numeric radian value.
		 *
		 * @return The value of the angle in radians.
		 **************************************************************************************************************/
		constexpr T rads() const;

		/**************************************************************************************************************
		 * Converts the angle value into a numeric degree value.
		 *
		 * @return The value of the angle in degrees.
		 **************************************************************************************************************/
		constexpr T degs() const;

		/**************************************************************************************************************
		 * Converts the angle value into a numeric turn value.
		 *
		 * @return The value of the angle in turns.
		 **************************************************************************************************************/
		constexpr T turns() const;

		/**************************************************************************************************************
		 * Computes the sine of the angle
		 *
		 * @return The sine of the angle.
		 **************************************************************************************************************/
		constexpr T sin() const;

		/**************************************************************************************************************
		 * Computes the cosine of the angle
		 *
		 * @return The cosine of the angle.
		 **************************************************************************************************************/
		constexpr T cos() const;

		/**************************************************************************************************************
		 * Computes the tangent of the angle
		 *
		 * @return The tangent of the angle.
		 **************************************************************************************************************/
		constexpr T tan() const;

	  private:
		T _rads;
	};

	/******************************************************************************************************************
	 * Shorthand for a float angle.
	 ******************************************************************************************************************/
	using fangle = angle<float>;

	/******************************************************************************************************************
	 * Shorthand for a double angle.
	 ******************************************************************************************************************/
	using dangle = angle<double>;

	/******************************************************************************************************************
	 * Shorthand for a long double angle.
	 ******************************************************************************************************************/
	using langle = angle<long double>;

	/******************************************************************************************************************
	 * Converts a numeric value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an dangle value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] th The numeric value in radians.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <arithmetic T> constexpr auto rads(T th);

	/******************************************************************************************************************
	 * Converts a numeric value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an dangle value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] th The numeric value in degrees.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <arithmetic T> constexpr auto degs(T th);

	/******************************************************************************************************************
	 * Converts a numeric value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an dangle value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] th The numeric value in turns.
	 *
	 * @return An angle value.
	 ******************************************************************************************************************/
	template <arithmetic T> constexpr auto turns(T th);

	/******************************************************************************************************************
	 * Converts a sine value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an dangle value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] sin The value of the sine.
	 *
	 * @return An angle value as if produced by `tr::rads(std::asin(sin))`.
	 ******************************************************************************************************************/
	template <arithmetic T> constexpr auto asin(T sin);

	/******************************************************************************************************************
	 * Converts a cosine value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an dangle value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] cos The value of the cosine.
	 *
	 * @return An angle value as if produced by `tr::rads(std::acos(sin))`.
	 ******************************************************************************************************************/
	template <arithmetic T> constexpr auto acos(T cos);

	/******************************************************************************************************************
	 * Converts a tangent value into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an dangle value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] tan The value of the tangent.
	 *
	 * @return An angle value as if produced by `tr::rads(std::atan(sin))`.
	 ******************************************************************************************************************/
	template <arithmetic T> constexpr auto atan(T tan);

	/******************************************************************************************************************
	 * Converts tangent x and y values into an angle value.
	 *
	 * @tparam T An arithmetic type. Integer values return an dangle value, while floating points return an angle of
	 *           the same base type.
	 *
	 * @param[in] y, x Tangent values.
	 *
	 * @return An angle value as if produced by `tr::rads(std::atan2(y, x))`.
	 ******************************************************************************************************************/
	template <arithmetic T> constexpr auto atan2(T y, T x);

	/******************************************************************************************************************
	 * Inline namespace containing angle value literals.
	 ******************************************************************************************************************/
	inline namespace angle_literals {
		/**************************************************************************************************************
		 * fangle degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An fangle value.
		 **************************************************************************************************************/
		consteval fangle operator""_degf(long double deg);

		/**************************************************************************************************************
		 * fangle degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An fangle value.
		 **************************************************************************************************************/
		consteval fangle operator""_degf(unsigned long long deg);

		/**************************************************************************************************************
		 * dangle degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An dangle value.
		 **************************************************************************************************************/
		consteval dangle operator""_deg(long double deg);

		/**************************************************************************************************************
		 * dangle degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An dangle value.
		 **************************************************************************************************************/
		consteval dangle operator""_deg(unsigned long long deg);

		/**************************************************************************************************************
		 * langle degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An langle value.
		 **************************************************************************************************************/
		consteval langle operator""_degl(long double deg);

		/**************************************************************************************************************
		 * langle degree literal.
		 *
		 * @param[in] deg The value of the angle in degrees.
		 *
		 * @return An langle value.
		 **************************************************************************************************************/
		consteval langle operator""_degl(unsigned long long deg);

		/**************************************************************************************************************
		 * langle radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An fangle value.
		 **************************************************************************************************************/
		consteval fangle operator""_radf(long double rad);

		/**************************************************************************************************************
		 * fangle radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An fangle value.
		 **************************************************************************************************************/
		consteval fangle operator""_radf(unsigned long long rad);

		/**************************************************************************************************************
		 * dangle radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An dangle value.
		 **************************************************************************************************************/
		consteval dangle operator""_rad(long double rad);

		/**************************************************************************************************************
		 * dangle radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An dangle value.
		 **************************************************************************************************************/
		consteval dangle operator""_rad(unsigned long long rad);

		/**************************************************************************************************************
		 * langle radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An langle value.
		 **************************************************************************************************************/
		consteval langle operator""_radl(long double rad);

		/**************************************************************************************************************
		 * langle radian literal.
		 *
		 * @param[in] rad The value of the angle in radians.
		 *
		 * @return An langle value.
		 **************************************************************************************************************/
		consteval langle operator""_radl(unsigned long long rad);

		/**************************************************************************************************************
		 * fangle turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An fangle value.
		 **************************************************************************************************************/
		consteval fangle operator""_turnsf(long double tr);

		/**************************************************************************************************************
		 * fangle turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An fangle value.
		 **************************************************************************************************************/
		consteval fangle operator""_turnsf(unsigned long long tr);

		/**************************************************************************************************************
		 * dangle turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An dangle value.
		 **************************************************************************************************************/
		consteval dangle operator""_turns(long double tr);

		/**************************************************************************************************************
		 * dangle turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An dangle value.
		 **************************************************************************************************************/
		consteval dangle operator""_turns(unsigned long long tr);

		/**************************************************************************************************************
		 * langle turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An langle value.
		 **************************************************************************************************************/
		consteval langle operator""_turnsl(long double tr);

		/**************************************************************************************************************
		 * langle turns literal.
		 *
		 * @param[in] tr The value of the angle in turns.
		 *
		 * @return An langle value.
		 **************************************************************************************************************/
		consteval langle operator""_turnsl(unsigned long long tr);
	} // namespace angle_literals

	/// @}
} // namespace tr

/// Angle formatter.
/// The formatting specification is: [r, d, t] + a valid floating point formatting specification.
template <std::floating_point T> class std::formatter<tr::angle<T>> : public std::formatter<T>, std::formatter<const char*> {
	/// @cond IMPLEMENTATION
  public:
	enum class format_type {
		RADS,
		DEGREES,
		TURNS
	};

	template <class ParseContext> constexpr auto parse(ParseContext& ctx)
	{
		auto it = ctx.begin();
		if (it == ctx.end() || (*it != 'r' && *it != 'd' && *it != 't')) {
			throw std::format_error{"One of {r, d, t} must start an angle formatting specification."};
		}

		switch (*it) {
		case 'r':
			_format = format_type::RADS;
			break;
		case 'd':
			_format = format_type::DEGREES;
			break;
		case 't':
			_format = format_type::TURNS;
			break;
		}

		ctx.advance_to(it + 1);
		return std::formatter<T>::parse(ctx);
	}

	template <typename FormatContext> constexpr auto format(const tr::angle<T>& p, FormatContext& ctx) const
	{
		switch (_format) {
		case format_type::RADS:
			ctx.advance_to(std::formatter<T>::format(p.rads(), ctx));
			ctx.advance_to(std::formatter<const char*>::format("rad", ctx));
			break;
		case format_type::DEGREES:
			ctx.advance_to(std::formatter<T>::format(p.degs(), ctx));
			ctx.advance_to(std::formatter<const char*>::format("deg", ctx));
			break;
		case format_type::TURNS:
			ctx.advance_to(std::formatter<T>::format(p.turns(), ctx));
			ctx.advance_to(std::formatter<const char*>::format("tr", ctx));
			break;
		}
		return ctx.out();
	}

  private:
	format_type _format{format_type::RADS};

	/// @endcond
};

#include "angle_impl.hpp"