/// @file
/// @brief Provides `tr::angle` and related functions.

#pragma once
#include <tr/utility/default_binary_io.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/math.hpp>
#include <tr/utility/rng.hpp>

//

namespace tr
{
	/// Abstract angle datatype.
	/// @tparam Rep Representation type of the angle.
	template <std::floating_point Rep>
	class angle
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a zero angle.
		[[nodiscard]] constexpr angle() noexcept = default;

		/// Constructs an angle from a numeric value in radians.
		/// @param rads Representation of the angle in radians.
		[[nodiscard]] explicit constexpr angle(Rep rads) noexcept
			: m_rads{rads}
		{
		}

		/// @}
		/// @name Conversion operators
		/// @{

		/// Casts the angle to an angle represented with another datatype.
		/// @tparam TargetRep Representation type to cast to.
		/// @return Cast angle value.
		template <std::floating_point TargetRep>
		[[nodiscard]] constexpr operator angle<TargetRep>() const noexcept
		{
			return angle<TargetRep>{static_cast<TargetRep>(m_rads)};
		}

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds an angle to the angle.
		/// @tparam RhsRep Representation type of the right-hand side angle.
		/// @param rhs Angle to add to the current angle value.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr angle& operator+=(angle<RhsRep> rhs) noexcept
		{
			m_rads += rhs.rads();
			return *this;
		}

		/// Subtracts an angle from the angle.
		/// @tparam RhsRep Representation type of the right-hand side angle.
		/// @param rhs Angle to subtract from the current angle value.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr angle& operator-=(angle<RhsRep> rhs) noexcept
		{
			m_rads -= rhs.rads();
			return *this;
		}

		/// Multiplies the angle by a scalar.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Factor to multiply the angle with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr angle& operator*=(Scalar rhs) noexcept
		{
			m_rads *= rhs;
			return *this;
		}

		/// Divides the angle by a scalar.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Factor to divide the angle with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr angle& operator/=(Scalar rhs) noexcept
		{
			m_rads /= rhs;
			return *this;
		}

		/// Modulos the angle by another angle.
		/// @tparam RhsRep Representation type of the right-hand side angle.
		/// @param rhs Factor to divide the angle with.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr angle& operator%=(angle<RhsRep> rhs) noexcept
		{
			m_rads = std::fmod(m_rads, rhs.rads());
			return *this;
		}

		/// @}
		/// @name Conversion
		/// @{

		/// Converts the angle value into a numeric radian value.
		/// @return Angle represented in radians.
		[[nodiscard]] constexpr Rep rads() const noexcept
		{
			return m_rads;
		}

		/// Converts the angle value into a numeric degree value.
		/// @return Angle represented in degrees.
		[[nodiscard]] constexpr Rep degs() const noexcept
		{
			return static_cast<Rep>(m_rads * (180 / std::numbers::pi));
		}

		/// Converts the angle value into a numeric turn value.
		/// @return Angle represented in turns.
		[[nodiscard]] constexpr Rep turns() const noexcept
		{
			return static_cast<Rep>(m_rads / (2 * std::numbers::pi));
		}

		/// @}
		/// @name Trigonometry
		/// @{

		/// Computes the sine of the angle.
		/// @return Sine of the angle.
		[[nodiscard]] inline Rep sin() const noexcept
		{
			return std::sin(m_rads);
		}

		/// Computes the cosine of the angle.
		/// @return Cosine of the angle.
		[[nodiscard]] inline Rep cos() const noexcept
		{
			return std::cos(m_rads);
		}

		/// Computes the tangent of the angle.
		/// @return Tangent of the angle.
		[[nodiscard]] inline Rep tan() const noexcept
		{
			return std::tan(m_rads);
		}

		/// @}
	  private:
		/// Value of the angle in radians.
		Rep m_rads;
	};

	/// @name Angle operators
	/// @{

	/// Compares two angles.
	/// @tparam LhsRep, RhsRep Representation types of the angles.
	/// @param lhs, rhs Angles to compare.
	/// @return Ordering of the angles.
	template <std::floating_point LhsRep, std::floating_point RhsRep>
	[[nodiscard]] constexpr std::partial_ordering operator<=>(angle<LhsRep> lhs, angle<RhsRep> rhs) noexcept
	{
		return lhs.rads() <=> rhs.rads();
	}

	/// Compares two angles for equality.
	/// @tparam LhsRep, RhsRep Representation types of the angles.
	/// @param lhs, rhs Angles to compare.
	/// @return Whether the angles are equal.
	template <std::floating_point LhsRep, std::floating_point RhsRep>
	[[nodiscard]] constexpr bool operator==(angle<LhsRep> lhs, angle<RhsRep> rhs) noexcept
	{
		return lhs.rads() == rhs.rads();
	}

	/// Sums two angles.
	/// @tparam LhsRep, RhsRep Representation types of the angles.
	/// @param lhs, rhs Angles to sum.
	/// @return Sum of the angles.
	template <std::floating_point LhsRep, std::floating_point RhsRep>
	[[nodiscard]] constexpr angle<std::common_type_t<LhsRep, RhsRep>> operator+(angle<LhsRep> lhs, angle<RhsRep> rhs) noexcept
	{
		return angle<std::common_type_t<LhsRep, RhsRep>>{lhs.rads() + rhs.rads()};
	}

	/// Inverts an angle.
	/// @tparam Rep Representation type of the angle.
	/// @param lhs Angle to invert.
	/// @return Inverted angle.
	template <std::floating_point Rep>
	[[nodiscard]] constexpr angle<Rep> operator-(angle<Rep> lhs) noexcept
	{
		return angle<Rep>{-lhs.rads()};
	}

	/// Subtracts two angles.
	/// @tparam LhsRep, RhsRep Representation types of the angles.
	/// @param lhs, rhs Angles to subtract.
	/// @return Difference of the angles.
	template <std::floating_point LhsRep, std::floating_point RhsRep>
	[[nodiscard]] constexpr angle<std::common_type_t<LhsRep, RhsRep>> operator-(angle<LhsRep> lhs, angle<RhsRep> rhs) noexcept
	{
		return angle<std::common_type_t<LhsRep, RhsRep>>{lhs.rads() - rhs.rads()};
	}

	/// Multiplies an angle by a scalar.
	/// @tparam Rep Representation type of the left-hand side angle.
	/// @tparam Scalar Type of the right-hand side scalar.
	/// @param lhs Angle to multiply.
	/// @param rhs Scalar to multiply with.
	/// @return Angle multiplied by the scalar.
	template <std::floating_point Rep, arithmetic Scalar>
	[[nodiscard]] constexpr angle<std::common_type_t<Rep, Scalar>> operator*(angle<Rep> lhs, Scalar rhs) noexcept
	{
		return angle<std::common_type_t<Rep, Scalar>>{lhs.rads() * rhs};
	}

	/// Multiplies an angle by a scalar.
	/// @tparam Scalar Type of the left-hand side scalar.
	/// @tparam Rep Representation type of the right-hand side angle.
	/// @param lhs Scalar to multiply with.
	/// @param rhs Angle to multiply.
	/// @return Angle multiplied by the scalar.
	template <arithmetic Scalar, std::floating_point Rep>
	[[nodiscard]] constexpr angle<std::common_type_t<Rep, Scalar>> operator*(Scalar lhs, angle<Rep> rhs) noexcept
	{
		return angle<std::common_type_t<Rep, Scalar>>{lhs * rhs.rads()};
	}

	/// Divides an angle by a scalar.
	/// @tparam Rep Representation type of the left-hand side angle.
	/// @tparam Scalar Type of the right-hand side scalar.
	/// @param lhs Angle to divide.
	/// @param rhs Scalar to divide with.
	/// @return Angle divided by the scalar.
	template <std::floating_point Rep, arithmetic Scalar>
	[[nodiscard]] constexpr angle<std::common_type_t<Rep, Scalar>> operator/(angle<Rep> lhs, Scalar rhs) noexcept
	{
		return angle<std::common_type_t<Rep, Scalar>>{lhs.rads() / rhs};
	}

	/// Divides two angles.
	/// @tparam LhsRep, RhsRep Representation types of the angles.
	/// @param lhs, rhs Angles to divide.
	/// @return Ratio of the angles.
	template <std::floating_point LhsRep, arithmetic RhsRep>
	[[nodiscard]] constexpr std::common_type_t<LhsRep, RhsRep> operator/(angle<LhsRep> lhs, angle<RhsRep> rhs) noexcept
	{
		return lhs.rads() / rhs.rads();
	}

	/// Modulos two angles.
	/// @tparam LhsRep, RhsRep Representation types of the angles.
	/// @param lhs, rhs Angles to modulo.
	/// @return Modulo of the angles.
	template <std::floating_point LhsRep, arithmetic RhsRep>
	[[nodiscard]] constexpr angle<std::common_type_t<LhsRep, RhsRep>> operator%(angle<LhsRep> lhs, angle<RhsRep> rhs) noexcept
	{
		return angle<std::common_type_t<LhsRep, RhsRep>>{std::fmod(lhs.rads(), rhs.rads())};
	}

	/// @}
	/// @name Angle constructors
	/// @{

	/// Constructs an angle expressed in radians.
	/// @tparam Scalar Scalar type.
	/// @param th Angle in radians.
	/// @return Equivalent angle value.
	template <arithmetic Scalar>
	[[nodiscard]] constexpr auto rads(Scalar th) noexcept
	{
		if constexpr (std::integral<Scalar>) {
			return angle<double>{static_cast<double>(th)};
		}
		else {
			return angle<Scalar>{th};
		}
	}

	/// Constructs an angle expressed in degrees.
	/// @tparam Scalar Scalar type.
	/// @param th Angle in degrees.
	/// @return Equivalent angle value.
	template <arithmetic Scalar>
	[[nodiscard]] constexpr auto degs(Scalar th) noexcept
	{
		if constexpr (std::integral<Scalar>) {
			return angle<double>{th * std::numbers::pi / 180};
		}
		else {
			return angle<Scalar>{static_cast<Scalar>(th * std::numbers::pi / 180)};
		}
	}

	/// Constructs an angle expressed in turns.
	/// @tparam Scalar Scalar type.
	/// @param th Angle in turns.
	/// @return Equivalent angle value.
	template <arithmetic Scalar>
	[[nodiscard]] constexpr auto turns(Scalar th) noexcept
	{
		if constexpr (std::integral<Scalar>) {
			return angle<double>{th * std::numbers::pi * 2};
		}
		else {
			return angle<Scalar>{static_cast<Scalar>(th * std::numbers::pi * 2)};
		}
	}

	/// Computes the principal value of the arc sine.
	/// @tparam Scalar Scalar type.
	/// @param sin Sine value.
	/// @return Principal arc sine value.
	template <arithmetic Scalar>
	[[nodiscard]] auto asin(Scalar sin) noexcept
	{
		return rads(std::asin(sin));
	}

	/// Computes the principal value of the arc cosine.
	/// @tparam Scalar Scalar type.
	/// @param cos Cosine value.
	/// @return Principal arc cosine value.
	template <arithmetic Scalar>
	[[nodiscard]] auto acos(Scalar cos) noexcept
	{
		return rads(std::acos(cos));
	}

	/// Computes the principal value of the arc tangent.
	/// @tparam Scalar Scalar type.
	/// @param tan Tangent value.
	/// @return Principal arc tangent value.
	template <arithmetic Scalar>
	[[nodiscard]] auto atan(Scalar tan) noexcept
	{
		return rads(std::atan(tan));
	}

	/// Computes the principal value of the arc tangent `y / x`.
	/// @tparam Y, X Scalar types.
	/// @param y, x Tangent ratio.
	/// @return Principal arc tangent value.
	template <arithmetic Y, arithmetic X>
	[[nodiscard]] auto atan2(Y y, X x) noexcept
	{
		return rads(std::atan2(y, x));
	}

	/// Computes the principal value of the arc tangent `p.y / p.x`.
	/// @tparam Element Type of the elements of the vector.
	/// @param p Tangent ratio stored as `p.y / p.x`.
	/// @return Principal arc tangent value.
	template <arithmetic Element>
	[[nodiscard]] auto atan2(glm::tvec2<Element> p) noexcept
	{
		return atan2(p.y, p.x);
	}

	/// @}
	/// @name Mathematical operations
	/// @{

	/// Calculates the absolute value of an angle.
	/// @tparam Rep Representation type of the value.
	/// @param th Angle to get the absolute value of.
	/// @return Absolute value of the angle.
	template <std::floating_point Rep>
	[[nodiscard]] constexpr angle<Rep> abs(angle<Rep> th) noexcept
	{
		return th.rads() >= 0 ? th : -th;
	}

	/// Linearly interpolates between two angles.
	/// @tparam XRep, YRep Representation types of the angles.
	/// @tparam A Type of the interpolation factor.
	/// @param x, y Sample values.
	/// @param a Interpolation factor.
	/// @return `a + t * (b − a)`.
	template <std::floating_point XRep, std::floating_point YRep, arithmetic A>
	[[nodiscard]] constexpr angle<std::common_type_t<XRep, YRep, A>> lerp(angle<XRep> x, angle<YRep> y, A a) noexcept
	{
		x = eucmod(x, turns(1.0f));
		y = eucmod(y, turns(1.0f));
		if (abs(y - x) >= turns(0.5f)) {
			if (x > y) {
				x -= turns(1.0f);
			}
			else {
				y -= turns(1.0f);
			}
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

			/// Constructs an angle represented with a float expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<float> operator""_degf(long double deg) noexcept
			{
				return static_cast<angle<float>>(degs(deg));
			}

			/// Constructs an angle represented with a float expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<float> operator""_degf(unsigned long long deg) noexcept
			{
				return static_cast<angle<float>>(degs(deg));
			}

			/// Constructs an angle represented with a double expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<double> operator""_deg(long double deg) noexcept
			{
				return static_cast<angle<double>>(degs(deg));
			}

			/// Constructs an angle represented with a double expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<double> operator""_deg(unsigned long long deg) noexcept
			{
				return degs(deg);
			}

			/// Constructs an angle represented with a long double expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<long double> operator""_degl(long double deg) noexcept
			{
				return degs(deg);
			}

			/// Constructs an angle represented with a long double expressed in degrees.
			/// @param deg Angle expressed in degrees.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<long double> operator""_degl(unsigned long long deg) noexcept
			{
				return degs(static_cast<long double>(deg));
			}

			//

			/// Constructs an angle represented with a float expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<float> operator""_radf(long double rad) noexcept
			{
				return static_cast<angle<float>>(rads(rad));
			}

			/// Constructs an angle represented with a float expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<float> operator""_radf(unsigned long long rad) noexcept
			{
				return static_cast<angle<float>>(rads(rad));
			}

			/// Constructs an angle represented with a double expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<double> operator""_rad(long double rad) noexcept
			{
				return static_cast<angle<double>>(rads(rad));
			}

			/// Constructs an angle represented with a double expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<double> operator""_rad(unsigned long long rad) noexcept
			{
				return rads(rad);
			}

			/// Constructs an angle represented with a long double expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<long double> operator""_radl(long double rad) noexcept
			{
				return rads(rad);
			}

			/// Constructs an angle represented with a long double expressed in radians.
			/// @param rad Angle expressed in radians.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<long double> operator""_radl(unsigned long long rad) noexcept
			{
				return rads(static_cast<long double>(rad));
			}

			//

			/// Constructs an angle represented with a float expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<float> operator""_trf(long double tr) noexcept
			{
				return static_cast<angle<float>>(turns(tr));
			}

			/// Constructs an angle represented with a float expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<float> operator""_trf(unsigned long long tr) noexcept
			{
				return static_cast<angle<float>>(turns(tr));
			}

			/// Constructs an angle represented with a double expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<double> operator""_tr(long double tr) noexcept
			{
				return turns(static_cast<double>(tr));
			}

			/// Constructs an angle represented with a double expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<double> operator""_tr(unsigned long long tr) noexcept
			{
				return turns(tr);
			}

			/// Constructs an angle represented with a long double expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<long double> operator""_trl(long double tr) noexcept
			{
				return turns(tr);
			}

			/// Constructs an angle represented with a long double expressed in turns.
			/// @param tr Angle expressed in turns.
			/// @return Angle value equivalent.
			[[nodiscard]] consteval angle<long double> operator""_trl(unsigned long long tr) noexcept
			{
				return turns(static_cast<long double>(tr));
			}

			/// @}
		} // namespace angle_literals
	} // namespace literals

	/// @name Random number generation
	/// @{

	/// Generates a random angle value in the range [0tr, 1tr].
	/// @tparam Rep Representation type of the angle.
	/// @param rng RNG used to generate the value.
	/// @return Random angle value constrained to [0tr, 1tr].
	template <std::floating_point Rep = double>
	[[nodiscard]] angle<Rep> generate_angle(rng& rng) noexcept
	{
		return rads(generate_float(rng, Rep{0}, 2 * std::numbers::pi_v<Rep>));
	}

	/// Generates a random angle value in the range [0, `max`).
	/// @tparam Rep Representation type of the angle.
	/// @param rng RNG used to generate the value.
	/// @param max Upper bound of the random range.
	/// @pre `max` must be greater than 0.
	/// @return Random angle value constrained to [0, `max`).
	template <std::floating_point Rep>
	[[nodiscard]] angle<Rep> generate_angle(rng& rng, angle<Rep> max) noexcept
	{
		return rads(generate_float(rng, max.rads()));
	}

	/// Generates a random angle value in the range [`min`, `max`).
	/// @tparam Rep Representation type of the angle.
	/// @param rng RNG used to generate the value.
	/// @param min Lower bound of the random range.
	/// @param max Upper bound of the random range.
	/// @pre `min` must be less than `max`.
	/// @return Random angle value constrained to [`min`, `max`).
	template <std::floating_point Rep>
	[[nodiscard]] angle<Rep> generate_angle(rng& rng, angle<Rep> min, angle<Rep> max) noexcept
	{
		return rads(generate_float(rng, min.rads(), max.rads()));
	}

	/// @}
} // namespace tr

//

/// Specialization of `std::common_type` for angles.
template <typename LhsRep, typename RhsRep>
struct std::common_type<tr::angle<LhsRep>, tr::angle<RhsRep>>
{
	using type = tr::angle<std::common_type_t<LhsRep, RhsRep>>;
};

/// Angle formatter.
/// @details
/// Angles may be output in radians, degrees, or turns, indicated by a leading `r`/`d`/`t` in the format specification.
/// Afterwards, a floating-point format specification may be used to indicate the numeric format.
///
/// Example format output: `std::format("{:d.3f}", tr::degs(1.23456f))` -> `1.235deg`.
template <typename Rep, typename CharT>
class std::formatter<tr::angle<Rep>, CharT>
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
	constexpr std::basic_format_context<OutputIt, CharT>::iterator format(const tr::angle<Rep>& value,
																		  std::basic_format_context<OutputIt, CharT>& context) const
	{
		switch (m_unit) {
		case unit::radians:
			return std::format_to(m_float_formatter.format(value.rads(), context), "rad");
		case unit::degrees:
			return std::format_to(m_float_formatter.format(value.degs(), context), "deg");
		case unit::turns:
			return std::format_to(m_float_formatter.format(value.turns(), context), "tr");
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
	std::formatter<Rep, CharT> m_float_formatter;

	/// Unit to use for the formatted angle.
	unit m_unit;
};

//

/// Enables default binary IO for rectangles.
template <typename Rep>
inline constexpr bool tr::enable_default_binary_io<tr::angle<Rep>>{true};