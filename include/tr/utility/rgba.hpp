/// @file
/// @brief Provides `tr::rgba`.

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
	/// Color stored in the RGBA color model.
	/// @tparam Element Type of the elements of the color.
	template <arithmetic Element>
	struct rgba
	{
		/// Type of the elements of the color.
		using element_type = Element;

		//

		/// Red color channel.
		Element r;

		/// Green color channel.
		Element g;

		/// Blue color channel.
		Element b;

		/// Alpha channel.
		Element a{norm_cast<Element>(1.0)};

		/// @name Conversion operators
		/// @{

		/// Converts the color to the equivalent vector type.
		/// @return Equivalent color vector.
		[[nodiscard]] constexpr explicit operator glm::tvec4<Element>() const noexcept
		{
			return glm::tvec4<Element>{r, g, b, a};
		}

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for memberwise equality.
		/// @tparam ElementR Type of the elements of the right-hand side color.
		/// @param lhs, rhs Colors to compare memberwise.
		/// @return `true` if the colors are equal memberwise, `false` otherwise.
		template <arithmetic ElementR>
		[[nodiscard]] friend constexpr bool operator==(const rgba& lhs, const rgba<ElementR>& rhs) noexcept
		{
			return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
		}

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to add to the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba& operator+=(Scalar rhs) noexcept
		{
			r += rhs;
			g += rhs;
			b += rhs;
			a += rhs;
			return *this;
		}

		/// Adds a color to the color.
		/// @tparam ElementR Type of the elements of the added color.
		/// @param rhs Color to add to the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic ElementR>
		constexpr rgba& operator+=(const rgba<ElementR>& rhs) noexcept
		{
			r += rhs.r;
			g += rhs.g;
			b += rhs.b;
			a += rhs.a;
			return *this;
		}

		/// Subtracts a scalar from the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to subtract from the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic ElementR>
		constexpr rgba& operator-=(ElementR rhs) noexcept
		{
			r -= rhs;
			g -= rhs;
			b -= rhs;
			a -= rhs;
			return *this;
		}

		/// Subtracts a color from the color.
		/// @tparam ElementR Type of the elements of the added color.
		/// @param rhs Color to subtract from the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic ElementR>
		constexpr rgba& operator-=(const rgba<ElementR>& rhs) noexcept
		{
			r -= rhs.r;
			g -= rhs.g;
			b -= rhs.b;
			a -= rhs.a;
			return *this;
		}

		/// Multiplies a scalar with the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to multiply with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic ElementR>
		constexpr rgba& operator*=(ElementR rhs) noexcept
		{
			r *= rhs;
			g *= rhs;
			b *= rhs;
			a *= rhs;
			return *this;
		}

		/// Multiplies a color with the color.
		/// @tparam ElementR Type of the elements of the added color.
		/// @param rhs Color to multiply with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic ElementR>
		constexpr rgba& operator*=(const rgba<ElementR>& rhs) noexcept
		{
			r *= rhs.r;
			g *= rhs.g;
			b *= rhs.b;
			a *= rhs.a;
			return *this;
		}

		/// Divides a scalar with the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to divide with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic ElementR>
		constexpr rgba& operator/=(ElementR rhs) noexcept
		{
			r /= rhs;
			g /= rhs;
			b /= rhs;
			a /= rhs;
			return *this;
		}

		/// Divides a color with the color.
		/// @tparam ElementR Type of the elements of the added color.
		/// @param rhs Color to divide with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic ElementR>
		constexpr rgba& operator/=(const rgba<ElementR>& rhs) noexcept
		{
			r /= rhs.r;
			g /= rhs.g;
			b /= rhs.b;
			a /= rhs.a;
			return *this;
		}

		/// @}
	};

	/// @name RGBA arithmetic operators
	/// @{

	/// Adds a scalar memberwise to an RGBA color.
	/// @tparam Element Type of the elements of the color.
	/// @param lhs Color to add.
	/// @param rhs Scalar to add memberwise.
	/// @return Memberwise sum of the elements of the color with the scalar.
	template <arithmetic Element>
	[[nodiscard]] constexpr rgba<Element> operator+(rgba<Element> lhs, Element rhs)
	{
		lhs += rhs;
		return lhs;
	}

	/// Adds two RGBA colors memberwise.
	/// @tparam Element Type of the elements of the colors.
	/// @param lhs, rhs Colors to add.
	/// @return Memberwise sum of the colors.
	template <arithmetic Element>
	[[nodiscard]] constexpr rgba<Element> operator+(rgba<Element> lhs, const rgba<Element>& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	/// Subtracts a scalar memberwise to an RGBA color.
	/// @tparam Element Type of the elements of the color.
	/// @param lhs Color to subtract from.
	/// @param rhs Scalar to subtract memberwise.
	/// @return Memberwise subtraction of the scalar from the elements of the color.
	template <arithmetic Element>
	[[nodiscard]] constexpr rgba<Element> operator-(rgba<Element> lhs, Element rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	/// Subtracts two RGBA colors memberwise.
	/// @tparam Element Type of the elements of the colors.
	/// @param lhs, rhs Colors to subtract.
	/// @return Memberwise difference of the colors.
	template <arithmetic Element>
	[[nodiscard]] constexpr rgba<Element> operator-(rgba<Element> lhs, rgba<Element> rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	/// Multiplies a scalar memberwise with an RGBA color.
	/// @tparam Element Type of the elements of the color.
	/// @param lhs Color to multiply.
	/// @param rhs Scalar to multiply with.
	/// @return Memberwise multiplication of the elements of the color with the scalar.
	template <arithmetic Element>
	[[nodiscard]] constexpr rgba<Element> operator*(rgba<Element> lhs, Element rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	/// Multiplies two RGBA colors memberwise.
	/// @tparam Element Type of the elements of the colors.
	/// @param lhs, rhs Colors to multiply.
	/// @return Memberwise multiplication of the colors.
	template <arithmetic Element>
	[[nodiscard]] constexpr rgba<Element> operator*(rgba<Element> lhs, const rgba<Element>& rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	/// Divides a scalar memberwise with an RGBA color.
	/// @tparam Element Type of the elements of the color.
	/// @param lhs Color to divide.
	/// @param rhs Scalar to divide with.
	/// @return Memberwise division of the elements of the color with the scalar.
	template <arithmetic Element>
	[[nodiscard]] constexpr rgba<Element> operator/(rgba<Element> lhs, Element rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	/// Divides two RGBA colors memberwise.
	/// @tparam Element Type of the elements of the colors.
	/// @param lhs, rhs Colors to divide.
	/// @return Memberwise division of the colors.
	template <arithmetic Element>
	[[nodiscard]] constexpr rgba<Element> operator/(rgba<Element> lhs, const rgba<Element>& rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	/// @}
} // namespace tr

//

/// Enables default binary IO for `tr::rgba`.
template <tr::arithmetic Element>
inline constexpr bool tr::enable_default_binary_io<tr::rgba<Element>>{true};

//

/// Color caster between RGBA colors of different element types.
/// @tparam ElementFrom Type of the elements of the source color.
/// @tparam ElementTo Type of the elements of the result color.
template <tr::arithmetic ElementFrom, tr::arithmetic ElementTo>
struct tr::color_caster<tr::rgba<ElementFrom>, tr::rgba<ElementTo>>
{
	/// Casts a color from `tr::rgba<ElementFrom>` to an equivalent value of type `tr::rgba<ElementTo>`.
	/// @param from Color to cast.
	/// @return Equivalent color value of type `tr::rgba<ElementTo>`.
	[[nodiscard]] static constexpr rgba<ElementTo> operator()(const rgba<ElementFrom>& from) noexcept
	{
		return rgba<ElementTo>{
			norm_cast<ElementTo>(from.r),
			norm_cast<ElementTo>(from.g),
			norm_cast<ElementTo>(from.b),
			norm_cast<ElementTo>(from.a),
		};
	}
};

/// Color caster from an RGBA color to an RGB color.
/// @tparam ElementFrom Type of the elements of the source color.
/// @tparam ElementTo Type of the elements of the result color.
template <tr::arithmetic ElementFrom, tr::arithmetic ElementTo>
struct tr::color_caster<tr::rgba<ElementFrom>, tr::rgb<ElementTo>>
{
	/// Casts a color from `tr::rgba<ElementFrom>` to an equivalent value of type `tr::rgb<ElementTo>`.
	/// @param from Color to cast.
	/// @return Equivalent color value of type `tr::rgb<ElementTo>`.
	[[nodiscard]] static constexpr rgb<ElementTo> operator()(const rgba<ElementFrom>& from) noexcept
	{
		return rgb<ElementTo>{
			norm_cast<ElementTo>(from.r),
			norm_cast<ElementTo>(from.g),
			norm_cast<ElementTo>(from.b),
		};
	}
};