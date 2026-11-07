/// @file
/// @brief Provides `tr::rgb` and related functionality.

/// @ingroup color
/// @defgroup rgb RGB
/// @brief Contains `tr::rgb` and related functionality.

#pragma once
#include <tr/utility/arithmetic_concepts.hpp>
#include <tr/utility/binary_io.hpp>
#include <tr/utility/color_cast.hpp>

//

namespace tr
{
	/// @ingroup rgb
	/// @brief Color stored in the RGB color model.
	/// @tparam Rep Color channel representation type.
	template <arithmetic Rep>
	struct rgb
	{
		/// @brief Red color channel.
		Rep r;

		/// @brief Green color channel.
		Rep g;

		/// @brief Blue color channel.
		Rep b;

		/// @name Conversion operators
		/// @{

		/// @brief Converts the color to the equivalent vector type.
		/// @return Equivalent color vector.
		[[nodiscard]] constexpr explicit operator glm::tvec3<Rep>() const noexcept
		{
			return glm::tvec3<Rep>{r, g, b};
		}

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// @brief Adds a scalar to the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to add to the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb& operator+=(Scalar rhs) noexcept
		{
			r = static_cast<Rep>(r + rhs);
			g = static_cast<Rep>(g + rhs);
			b = static_cast<Rep>(b + rhs);
			return *this;
		}

		/// @brief Adds a color to the color.
		/// @tparam RhsRep Right-hand side color representation type.
		/// @param rhs Color to add to the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgb& operator+=(const rgb<RhsRep>& rhs) noexcept
		{
			r = static_cast<Rep>(r + rhs.r);
			g = static_cast<Rep>(g + rhs.g);
			b = static_cast<Rep>(b + rhs.b);
			return *this;
		}

		/// @brief Subtracts a scalar from the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to subtract from the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgb& operator-=(RhsRep rhs) noexcept
		{
			r = static_cast<Rep>(r - rhs);
			g = static_cast<Rep>(g - rhs);
			b = static_cast<Rep>(b - rhs);
			return *this;
		}

		/// @brief Subtracts a color from the color.
		/// @tparam RhsRep Right-hand side color representation type.
		/// @param rhs Color to subtract from the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgb& operator-=(const rgb<RhsRep>& rhs) noexcept
		{
			r = static_cast<Rep>(r - rhs.r);
			g = static_cast<Rep>(g - rhs.g);
			b = static_cast<Rep>(b - rhs.b);
			return *this;
		}

		/// @brief Multiplies a scalar with the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to multiply with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgb& operator*=(RhsRep rhs) noexcept
		{
			r = static_cast<Rep>(r * rhs);
			g = static_cast<Rep>(g * rhs);
			b = static_cast<Rep>(b * rhs);
			return *this;
		}

		/// @brief Multiplies a color with the color.
		/// @tparam RhsRep Right-hand side color representation type.
		/// @param rhs Color to multiply with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgb& operator*=(const rgb<RhsRep>& rhs) noexcept
		{
			r = static_cast<Rep>(r * rhs.r);
			g = static_cast<Rep>(g * rhs.g);
			b = static_cast<Rep>(b * rhs.b);
			return *this;
		}

		/// @brief Divides a scalar with the color memberwise.
		/// @tparam Scalar Type of the scalar.
		/// @param rhs Scalar to divide with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgb& operator/=(RhsRep rhs) noexcept
		{
			r = static_cast<Rep>(r / rhs);
			g = static_cast<Rep>(g / rhs);
			b = static_cast<Rep>(b / rhs);
			return *this;
		}

		/// @brief Divides a color with the color.
		/// @tparam RhsRep Right-hand side color representation type.
		/// @param rhs Color to divide with the color memberwise.
		/// @return Reference to `*this`.
		template <arithmetic RhsRep>
		constexpr rgb& operator/=(const rgb<RhsRep>& rhs) noexcept
		{
			r = static_cast<Rep>(r / rhs.r);
			g = static_cast<Rep>(g / rhs.g);
			b = static_cast<Rep>(b / rhs.b);
			return *this;
		}

		/// @}
		/// @name Alpha
		/// @{

		/// @brief Adds an alpha component to the color.
		/// @param a Alpha component to add to the color.
		/// @return Color with the alpha component added.
		[[nodiscard]] constexpr rgba<Rep> with_alpha(Rep a) const noexcept
		{
			return rgba<Rep>{r, g, b, a};
		}

		/// @}
	};

	//

	/// @ingroup rgb
	/// @brief Checks two colors for memberwise equality.
	/// @tparam LhsRep, RhsRep Color representation types.
	/// @param lhs, rhs Colors to compare memberwise.
	/// @return `true` if the colors are equal memberwise, `false` otherwise.
	template <arithmetic LhsRep, arithmetic RhsRep>
	[[nodiscard]] constexpr bool operator==(const rgb<LhsRep>& lhs, const rgb<RhsRep>& rhs) noexcept
	{
		return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
	}

	/// @ingroup rgb
	/// @brief Adds a scalar memberwise to an RGB color.
	/// @tparam Rep Color representation type.
	/// @param lhs Color to add.
	/// @param rhs Scalar to add memberwise.
	/// @return Memberwise sum of the elements of the color with the scalar.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgb<Rep> operator+(rgb<Rep> lhs, Rep rhs)
	{
		lhs += rhs;
		return lhs;
	}

	/// @ingroup rgb
	/// @brief Adds two RGB colors memberwise.
	/// @tparam Rep Color representation type.
	/// @param lhs, rhs Colors to add.
	/// @return Memberwise sum of the colors.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgb<Rep> operator+(rgb<Rep> lhs, const rgb<Rep>& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	/// @ingroup rgb
	/// @brief Subtracts a scalar memberwise to an RGB color.
	/// @tparam Rep Color representation type.
	/// @param lhs Color to subtract from.
	/// @param rhs Scalar to subtract memberwise.
	/// @return Memberwise subtraction of the scalar from the elements of the color.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgb<Rep> operator-(rgb<Rep> lhs, Rep rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	/// @ingroup rgb
	/// @brief Subtracts two RGB colors memberwise.
	/// @tparam Rep Color representation type.
	/// @param lhs, rhs Colors to subtract.
	/// @return Memberwise difference of the colors.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgb<Rep> operator-(rgb<Rep> lhs, rgb<Rep> rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	/// @ingroup rgb
	/// @brief Multiplies a scalar memberwise with an RGB color.
	/// @tparam Rep Color representation type.
	/// @param lhs Color to multiply.
	/// @param rhs Scalar to multiply with.
	/// @return Memberwise multiplication of the elements of the color with the scalar.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgb<Rep> operator*(rgb<Rep> lhs, Rep rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	/// @ingroup rgb
	/// @brief Multiplies two RGB colors memberwise.
	/// @tparam Rep Color representation type.
	/// @param lhs, rhs Colors to multiply.
	/// @return Memberwise multiplication of the colors.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgb<Rep> operator*(rgb<Rep> lhs, const rgb<Rep>& rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	/// @ingroup rgb
	/// @brief Divides a scalar memberwise by an RGB color.
	/// @tparam Rep Color representation type.
	/// @param lhs Color to divide.
	/// @param rhs Scalar to divide by.
	/// @return Memberwise division of the elements of the color by the scalar.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgb<Rep> operator/(rgb<Rep> lhs, Rep rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	/// @ingroup rgb
	/// @brief Divides two RGB colors memberwise.
	/// @tparam Rep Color representation type.
	/// @param lhs, rhs Colors to divide.
	/// @return Memberwise division of the colors.
	template <arithmetic Rep>
	[[nodiscard]] constexpr rgb<Rep> operator/(rgb<Rep> lhs, const rgb<Rep>& rhs)
	{
		lhs /= rhs;
		return lhs;
	}
} // namespace tr

//

/// @ingroup rgb
/// @brief Specialization of `std::tuple_size` for `tr::rgb`.
template <typename Rep>
struct std::tuple_size<tr::rgb<Rep>> : std::integral_constant<std::size_t, 3>
{
};

/// @ingroup rgb
/// @brief Specialization of `std::tuple_element` for `tr::rgb`.
/// @tparam Index Index of the element.
template <std::size_t Index, typename Rep>
struct std::tuple_element<Index, tr::rgb<Rep>> : std::tuple_element<Index, std::tuple<Rep, Rep, Rep>>
{
};

namespace tr
{
	/// @ingroup rgb
	/// @brief Returns an element from an RGB color.
	/// @tparam Index Index of the element to return.
	/// @tparam Rgb Potentially cvref-qualified instance of `tr::rgb`.
	/// @param rgb Color to return an element from.
	/// @return Element from `rgb`.
	template <usize Index, typename Rgb>
		requires cvref_specialization_of<Rgb, rgb>
	[[nodiscard]] decltype(auto) get(Rgb&& rgb) noexcept
	{
		if constexpr (Index == 0) {
			return std::forward_like<Rgb>(rgb.r);
		}
		else if constexpr (Index == 1) {
			return std::forward_like<Rgb>(rgb.g);
		}
		else if constexpr (Index == 2) {
			return std::forward_like<Rgb>(rgb.b);
		}
		else {
			TR_UNREACHABLE;
		}
	}
} // namespace tr

//

/// @ingroup rgb
/// @brief Color caster between RGB colors of different element types.
/// @tparam RepFrom Representation type of the source color.
/// @tparam RepTo Representation type of the result color.
template <tr::arithmetic RepFrom, tr::arithmetic RepTo>
struct tr::color_caster<tr::rgb<RepFrom>, tr::rgb<RepTo>>
{
	/// @brief Casts a color from `tr::rgb<RepFrom>` to an equivalent value of type `tr::rgb<RepTo>`.
	/// @param from Color to cast.
	/// @return Equivalent color value of type `tr::rgb<RepTo>`.
	[[nodiscard]] static constexpr rgb<RepTo> operator()(const rgb<RepFrom>& from) noexcept
	{
		return rgb<RepTo>{
			norm_cast<RepTo>(from.r),
			norm_cast<RepTo>(from.g),
			norm_cast<RepTo>(from.b),
		};
	}
};

/// @ingroup rgb
/// @brief Color caster from an RGB color to an RGBA color.
/// @tparam RepFrom Representation type of the source color.
/// @tparam RepTo Representation type of the result color.
template <tr::arithmetic RepFrom, tr::arithmetic RepTo>
struct tr::color_caster<tr::rgb<RepFrom>, tr::rgba<RepTo>>
{
	/// @brief Casts a color from `tr::rgb<RepFrom>` to an equivalent value of type `tr::rgba<RepTo>`.
	/// @param from Color to cast.
	/// @return Equivalent color value of type `tr::rgba<RepTo>`.
	[[nodiscard]] static constexpr rgba<RepTo> operator()(const rgb<RepFrom>& from) noexcept
	{
		return rgba<RepTo>{
			norm_cast<RepTo>(from.r),
			norm_cast<RepTo>(from.g),
			norm_cast<RepTo>(from.b),
		};
	}
};