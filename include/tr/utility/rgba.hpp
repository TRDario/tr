/// @file
/// @brief Provides `tr::rgba` and related functionality.

/// @ingroup color
/// @defgroup rgba RGBA
/// @brief Contains `tr::rgba` and related functionality.

#pragma once
#include <tr/utility/arithmetic_concepts.hpp>
#include <tr/utility/color_cast.hpp>
#include <tr/utility/default_binary_io.hpp>
#include <tr/utility/norm_cast.hpp>

namespace tr
{
	template <arithmetic Element>
	struct rgb;
}

//

namespace tr
{
	/// @ingroup rgba
	/// @brief Color stored in the RGBA color model.
	/// @tparam Rep Color channel representation type.
	template <arithmetic Rep>
	struct rgba
	{
		/// @brief Red color channel.
		Rep r;

		/// @brief Green color channel.
		Rep g;

		/// @brief Blue color channel.
		Rep b;

		/// @brief Alpha channel.
		Rep a{norm_cast<Rep>(1.0)};

		/// @name Conversion operators
		/// @{

		/// @brief Converts the color to the equivalent vector type.
		/// @return Equivalent color vector.
		[[nodiscard]] constexpr explicit operator glm::tvec4<Rep>() const noexcept
		{
			return glm::tvec4<Rep>{r, g, b, a};
		}

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// @brief Adds a scalar to the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to add to the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba& operator+=(Scalar rhs) noexcept
		{
			r = static_cast<Rep>(r + rhs);
			g = static_cast<Rep>(g + rhs);
			b = static_cast<Rep>(b + rhs);
			a = static_cast<Rep>(a + rhs);
			return *this;
		}

		/// @brief Adds a color to the color.
		/// @tparam RhsRep Right-hand side color representation type.
		/// @param rhs Color to add to the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgba& operator+=(const rgba<RhsRep>& rhs) noexcept
		{
			r = static_cast<Rep>(r + rhs.r);
			g = static_cast<Rep>(g + rhs.g);
			b = static_cast<Rep>(b + rhs.b);
			a = static_cast<Rep>(a + rhs.a);
			return *this;
		}

		/// @brief Subtracts a scalar from the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to subtract from the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgba& operator-=(RhsRep rhs) noexcept
		{
			r = static_cast<Rep>(r - rhs);
			g = static_cast<Rep>(g - rhs);
			b = static_cast<Rep>(b - rhs);
			a = static_cast<Rep>(a - rhs);
			return *this;
		}

		/// @brief Subtracts a color from the color.
		/// @tparam RhsRep Right-hand side color representation type.
		/// @param rhs Color to subtract from the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgba& operator-=(const rgba<RhsRep>& rhs) noexcept
		{
			r = static_cast<Rep>(r - rhs.r);
			g = static_cast<Rep>(g - rhs.g);
			b = static_cast<Rep>(b - rhs.b);
			a = static_cast<Rep>(a - rhs.a);
			return *this;
		}

		/// @brief Multiplies a scalar with the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to multiply with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgba& operator*=(RhsRep rhs) noexcept
		{
			r = static_cast<Rep>(r * rhs);
			g = static_cast<Rep>(g * rhs);
			b = static_cast<Rep>(b * rhs);
			a = static_cast<Rep>(a * rhs);
			return *this;
		}

		/// @brief Multiplies a color with the color.
		/// @tparam RhsRep Right-hand side color representation type.
		/// @param rhs Color to multiply with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgba& operator*=(const rgba<RhsRep>& rhs) noexcept
		{
			r = static_cast<Rep>(r * rhs.r);
			g = static_cast<Rep>(g * rhs.g);
			b = static_cast<Rep>(b * rhs.b);
			a = static_cast<Rep>(a * rhs.a);
			return *this;
		}

		/// @brief Divides a scalar with the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to divide with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgba& operator/=(RhsRep rhs) noexcept
		{
			r = static_cast<Rep>(r / rhs);
			g = static_cast<Rep>(g / rhs);
			b = static_cast<Rep>(b / rhs);
			a = static_cast<Rep>(a / rhs);
			return *this;
		}

		/// @brief Divides a color with the color.
		/// @tparam RhsRep Right-hand side color representation type.
		/// @param rhs Color to divide with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgba& operator/=(const rgba<RhsRep>& rhs) noexcept
		{
			r = static_cast<Rep>(r / rhs.r);
			g = static_cast<Rep>(g / rhs.g);
			b = static_cast<Rep>(b / rhs.b);
			a = static_cast<Rep>(a / rhs.a);
			return *this;
		}

		/// @}
	};

	//

	/// @ingroup rgba
	/// @brief Checks two colors for memberwise equality.
	/// @tparam LhsRep, RhsRep Color representation types.
	/// @param lhs, rhs Colors to compare memberwise.
	/// @return `true` if the colors are equal memberwise, `false` otherwise.
	template <arithmetic LhsRep, arithmetic RhsRep>
	[[nodiscard]] constexpr bool operator==(const rgba<LhsRep>& lhs, const rgba<RhsRep>& rhs) noexcept
	{
		return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
	}

	/// @ingroup rgba
	/// @brief Adds a scalar memberwise to an RGBA color.
	/// @tparam Rep Color representation type.
	/// @param lhs Color to add.
	/// @param rhs Scalar to add memberwise.
	/// @return Memberwise sum of the elements of the color with the scalar.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgba<Rep> operator+(rgba<Rep> lhs, Rep rhs)
	{
		lhs += rhs;
		return lhs;
	}

	/// @ingroup rgba
	/// @brief Adds two RGBA colors memberwise.
	/// @tparam Rep Color representation type.
	/// @param lhs, rhs Colors to add.
	/// @return Memberwise sum of the colors.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgba<Rep> operator+(rgba<Rep> lhs, const rgba<Rep>& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	/// @ingroup rgba
	/// @brief Subtracts a scalar memberwise to an RGBA color.
	/// @tparam Rep Color representation type.
	/// @param lhs Color to subtract from.
	/// @param rhs Scalar to subtract memberwise.
	/// @return Memberwise subtraction of the scalar from the elements of the color.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgba<Rep> operator-(rgba<Rep> lhs, Rep rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	/// @ingroup rgba
	/// @brief Subtracts two RGBA colors memberwise.
	/// @tparam Rep Color representation type.
	/// @param lhs, rhs Colors to subtract.
	/// @return Memberwise difference of the colors.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgba<Rep> operator-(rgba<Rep> lhs, rgba<Rep> rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	/// @ingroup rgba
	/// @brief Multiplies a scalar memberwise with an RGBA color.
	/// @tparam Rep Color representation type.
	/// @param lhs Color to multiply.
	/// @param rhs Scalar to multiply with.
	/// @return Memberwise multiplication of the elements of the color with the scalar.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgba<Rep> operator*(rgba<Rep> lhs, Rep rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	/// @ingroup rgba
	/// @brief Multiplies two RGBA colors memberwise.
	/// @tparam Rep Color representation type.
	/// @param lhs, rhs Colors to multiply.
	/// @return Memberwise multiplication of the colors.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgba<Rep> operator*(rgba<Rep> lhs, const rgba<Rep>& rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	/// @ingroup rgba
	/// @brief Divides a scalar memberwise by an RGBA color.
	/// @tparam Rep Color representation type.
	/// @param lhs Color to divide.
	/// @param rhs Scalar to divide by.
	/// @return Memberwise division of the elements of the color by the scalar.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgba<Rep> operator/(rgba<Rep> lhs, Rep rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	/// @ingroup rgba
	/// @brief Divides two RGBA colors memberwise.
	/// @tparam Rep Color representation type.
	/// @param lhs, rhs Colors to divide.
	/// @return Memberwise division of the colors.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgba<Rep> operator/(rgba<Rep> lhs, const rgba<Rep>& rhs)
	{
		lhs /= rhs;
		return lhs;
	}
} // namespace tr

//

/// @brief Enables default binary IO for `tr::rgba`.
template <typename Rep>
inline constexpr bool tr::enable_default_binary_io<tr::rgba<Rep>>{true};

//

/// @ingroup rgba
/// @brief Color caster between RGBA colors of different element types.
/// @tparam RepFrom Representation type of the source color.
/// @tparam RepTo Representation type of the result color.
template <tr::arithmetic RepFrom, tr::arithmetic RepTo>
struct tr::color_caster<tr::rgba<RepFrom>, tr::rgba<RepTo>>
{
	/// @brief Casts a color from `tr::rgba<RepFrom>` to an equivalent value of type `tr::rgba<RepTo>`.
	/// @param from Color to cast.
	/// @return Equivalent color value of type `tr::rgba<RepTo>`.
	[[nodiscard]] static constexpr rgba<RepTo> operator()(const rgba<RepFrom>& from) noexcept
	{
		return rgba<RepTo>{
			norm_cast<RepTo>(from.r),
			norm_cast<RepTo>(from.g),
			norm_cast<RepTo>(from.b),
			norm_cast<RepTo>(from.a),
		};
	}
};

/// @ingroup rgba
/// @brief Color caster from an RGBA color to an RGB color.
/// @tparam RepFrom Representation type of the source color.
/// @tparam RepTo Representation type of the result color.
template <tr::arithmetic RepFrom, tr::arithmetic RepTo>
struct tr::color_caster<tr::rgba<RepFrom>, tr::rgb<RepTo>>
{
	/// @brief Casts a color from `tr::rgba<RepFrom>` to an equivalent value of type `tr::rgb<RepTo>`.
	/// @param from Color to cast.
	/// @return Equivalent color value of type `tr::rgb<RepTo>`.
	[[nodiscard]] static constexpr rgb<RepTo> operator()(const rgba<RepFrom>& from) noexcept
	{
		return rgb<RepTo>{
			norm_cast<RepTo>(from.r),
			norm_cast<RepTo>(from.g),
			norm_cast<RepTo>(from.b),
		};
	}
};