/// @file
/// @brief Provides color datatypes.

#pragma once
#include <tr/utility/concepts.hpp>
#include <tr/utility/default_binary_io.hpp>
#include <tr/utility/math.hpp>
#include <tr/utility/norm_cast.hpp>

namespace tr
{
	struct rgbf;
	struct rgba8;
	struct rgbaf;
	struct hsv;
} // namespace tr

//

namespace tr
{
	/// 8-bit RGB color.
	struct rgb8
	{
		/// Red color channel.
		u8 r;

		/// Green color channel.
		u8 g;

		/// Blue color channel.
		u8 b;

		/// @name Constructors
		/// @{

		/// Default-constructs an RGB color.
		[[nodiscard]] constexpr rgb8() noexcept = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		[[nodiscard]] constexpr rgb8(u8 r, u8 g, u8 b) noexcept
			: r{r}
			, g{g}
			, b{b}
		{
		}

		/// Converts a float RGB color into an 8-bit one.
		/// @param rgb Color to convert to RGB.
		[[nodiscard]] constexpr rgb8(rgbf rgb) noexcept;

		/// Removes the alpha component from an RGBA color.
		/// @param rgba Color to convert to RGB.
		[[nodiscard]] constexpr rgb8(rgba8 rgba) noexcept;

		/// Removes the alpha component from an RGBA color and converts it to 8-bit.
		/// @param rgba Color to convert to RGB.
		[[nodiscard]] constexpr rgb8(rgbaf rgba) noexcept;

		/// Converts an HSV color to an RGB one.
		/// @param hsv Color to convert to RGB.
		[[nodiscard]] constexpr rgb8(hsv hsv) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		[[nodiscard]] friend constexpr bool operator==(rgb8 lhs, rgb8 rhs) noexcept = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color.
		/// @tparam Scalar Scalar type.
		/// @param addend Scalar to add to the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator+=(Scalar addend) noexcept
		{
			r += addend;
			g += addend;
			b += addend;
			return *this;
		}

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgb8 operator+(rgb8 lhs, Scalar addend) noexcept
		{
			rgb8 temp{lhs};
			temp += addend;
			return temp;
		}

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator-=(Scalar subtrahend) noexcept
		{
			r -= subtrahend;
			g -= subtrahend;
			b -= subtrahend;
			return *this;
		}

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgb8 operator-(rgb8 lhs, Scalar subtrahend) noexcept
		{
			rgb8 temp{lhs};
			temp -= subtrahend;
			return temp;
		}

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator*=(Scalar multiplier) noexcept
		{
			r *= multiplier;
			g *= multiplier;
			b *= multiplier;
			return *this;
		}

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgb8 operator*(rgb8 lhs, Scalar multiplier) noexcept
		{
			rgb8 temp{lhs};
			temp *= multiplier;
			return temp;
		}

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator/=(Scalar divisor) noexcept
		{
			r /= divisor;
			g /= divisor;
			b /= divisor;
			return *this;
		}

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgb8 operator/(rgb8 lhs, Scalar divisor) noexcept
		{
			rgb8 temp{lhs};
			temp /= divisor;
			return temp;
		}

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgb8& operator+=(rgb8 addend) noexcept
		{
			r += addend.r;
			g += addend.g;
			b += addend.b;
			return *this;
		}

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		[[nodiscard]] friend constexpr rgb8 operator+(rgb8 lhs, rgb8 rhs) noexcept
		{
			rgb8 temp{lhs};
			temp += rhs;
			return temp;
		}

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgb8& operator-=(rgb8 subtrahend) noexcept
		{
			r -= subtrahend.r;
			g -= subtrahend.g;
			b -= subtrahend.b;
			return *this;
		}

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		[[nodiscard]] friend constexpr rgb8 operator-(rgb8 lhs, rgb8 rhs) noexcept
		{
			rgb8 temp{lhs};
			temp -= rhs;
			return temp;
		}

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgb8& operator*=(rgb8 multiplier) noexcept
		{
			r = r * multiplier.r / 255;
			g = g * multiplier.g / 255;
			b = b * multiplier.b / 255;
			return *this;
		}

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		[[nodiscard]] friend constexpr rgb8 operator*(rgb8 lhs, rgb8 rhs) noexcept
		{
			rgb8 temp{lhs};
			temp *= rhs;
			return temp;
		}

		/// @}
	};

	/// Floating-point RGB color.
	struct rgbf
	{
		/// Red color channel.
		float r;

		/// Green color channel.
		float g;

		/// Blue color channel.
		float b;

		/// @name Constructors
		/// @{

		/// Default-constructs an RGB color.
		[[nodiscard]] constexpr rgbf() noexcept = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		[[nodiscard]] constexpr rgbf(float r, float g, float b) noexcept
			: r{r}
			, g{g}
			, b{b}
		{
		}

		/// Converts an 8-bit RGB color into a float one.
		/// @param rgb Color to convert to RGB.
		[[nodiscard]] constexpr rgbf(rgb8 rgb) noexcept
			: r{norm_cast<float>(rgb.r)}
			, g{norm_cast<float>(rgb.g)}
			, b{norm_cast<float>(rgb.b)}
		{
		}

		/// Removes the alpha component from an RGBA color and converts it to float.
		/// @param rgba Color to convert to RGB.
		[[nodiscard]] constexpr rgbf(rgba8 rgba) noexcept;

		/// Removes the alpha component from an RGBA color.
		/// @param rgba Color to convert to RGB.
		[[nodiscard]] constexpr rgbf(rgbaf rgba) noexcept;

		/// Converts an HSV color to an RGB one.
		/// @param hsv Color to convert to RGB.
		[[nodiscard]] constexpr rgbf(hsv hsv) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		[[nodiscard]] friend constexpr bool operator==(rgbf lhs, rgbf rhs) noexcept = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color.
		/// @tparam Scalar Scalar type.
		/// @param addend Scalar to add to the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator+=(Scalar addend) noexcept
		{
			r += addend;
			g += addend;
			b += addend;
			return *this;
		}

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgbf operator+(rgbf lhs, Scalar addend) noexcept
		{
			rgbf temp{lhs};
			temp += addend;
			return temp;
		}

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator-=(Scalar subtrahend) noexcept
		{
			r -= subtrahend;
			g -= subtrahend;
			b -= subtrahend;
			return *this;
		}

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgbf operator-(rgbf lhs, Scalar subtrahend) noexcept
		{
			rgbf temp{lhs};
			temp -= subtrahend;
			return temp;
		}

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator*=(Scalar multiplier) noexcept
		{
			r *= multiplier;
			g *= multiplier;
			b *= multiplier;
			return *this;
		}

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgbf operator*(rgbf lhs, Scalar multiplier) noexcept
		{
			rgbf temp{lhs};
			temp *= multiplier;
			return temp;
		}

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator/=(Scalar divisor) noexcept
		{
			r /= divisor;
			g /= divisor;
			b /= divisor;
			return *this;
		}

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgbf operator/(rgbf lhs, Scalar divisor) noexcept
		{
			rgbf temp{lhs};
			temp /= divisor;
			return temp;
		}

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgbf& operator+=(rgbf addend) noexcept
		{
			r += addend.r;
			g += addend.g;
			b += addend.b;
			return *this;
		}

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		[[nodiscard]] friend constexpr rgbf operator+(rgbf lhs, rgbf rhs) noexcept
		{
			rgbf temp{lhs};
			temp += rhs;
			return temp;
		}

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgbf& operator-=(rgbf subtrahend) noexcept
		{
			r -= subtrahend.r;
			g -= subtrahend.g;
			b -= subtrahend.b;
			return *this;
		}

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		[[nodiscard]] friend constexpr rgbf operator-(rgbf lhs, rgbf rhs) noexcept
		{
			rgbf temp{lhs};
			temp -= rhs;
			return temp;
		}

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgbf& operator*=(rgbf multiplier) noexcept
		{
			r *= multiplier.r;
			g *= multiplier.g;
			b *= multiplier.b;
			return *this;
		}

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		[[nodiscard]] friend constexpr rgbf operator*(rgbf lhs, rgbf rhs) noexcept
		{
			rgbf temp{lhs};
			temp *= rhs;
			return temp;
		}

		/// @}
	};

	/// 8-bit RGBA color.
	struct rgba8
	{
		/// Red color channel.
		u8 r;

		/// Green color channel.
		u8 g;

		/// Blue color channel.
		u8 b;

		/// Alpha channel.
		u8 a;

		/// @name Constructors
		/// @{

		/// Default-constructs an RGBA color.
		[[nodiscard]] constexpr rgba8() noexcept = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgba8(u8 r, u8 g, u8 b, u8 a) noexcept
			: r{r}
			, g{g}
			, b{b}
			, a{a}
		{
		}

		/// Adds an alpha component to an RGB color.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgba8(rgb8 rgb, u8 a = 255) noexcept
			: r{rgb.r}
			, g{rgb.g}
			, b{rgb.b}
			, a{a}
		{
		}

		/// Converts a float RGB color into an 8-bit one and adds an alpha component.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgba8(rgbf rgb, u8 a = 255) noexcept
			: r{norm_cast<u8>(rgb.r)}
			, g{norm_cast<u8>(rgb.g)}
			, b{norm_cast<u8>(rgb.b)}
			, a{a}
		{
		}

		/// Converts a float RGBA color into an 8-bit one.
		/// @param rgba Color to convert to RGBA.
		[[nodiscard]] constexpr rgba8(rgbaf rgba) noexcept;

		/// Converts an HSV color to an RGBA one.
		/// @param hsv Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgba8(hsv hsv, u8 a = 255) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		[[nodiscard]] friend constexpr bool operator==(rgba8 lhs, rgba8 rhs) noexcept = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color.
		/// @tparam Scalar Scalar type.
		/// @param addend Scalar to add to the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator+=(Scalar addend) noexcept
		{
			r += addend;
			g += addend;
			b += addend;
			a += addend;
			return *this;
		}

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgba8 operator+(rgba8 lhs, Scalar addend) noexcept
		{
			rgba8 temp{lhs};
			temp += addend;
			return temp;
		}

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator-=(Scalar subtrahend) noexcept
		{
			r -= subtrahend;
			g -= subtrahend;
			b -= subtrahend;
			a -= subtrahend;
			return *this;
		}

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgba8 operator-(rgba8 lhs, Scalar subtrahend) noexcept
		{
			rgba8 temp{lhs};
			temp -= subtrahend;
			return temp;
		}

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator*=(Scalar multiplier) noexcept
		{
			r *= multiplier;
			g *= multiplier;
			b *= multiplier;
			a *= multiplier;
			return *this;
		}

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgba8 operator*(rgba8 lhs, Scalar multiplier) noexcept
		{
			rgba8 temp{lhs};
			temp *= multiplier;
			return temp;
		}

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator/=(Scalar divisor) noexcept
		{
			r /= divisor;
			g /= divisor;
			b /= divisor;
			a /= divisor;
			return *this;
		}

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgba8 operator/(rgba8 lhs, Scalar divisor) noexcept
		{
			rgba8 temp{lhs};
			temp /= divisor;
			return temp;
		}

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgba8& operator+=(rgba8 addend) noexcept
		{
			r += addend.r;
			g += addend.g;
			b += addend.b;
			a += addend.a;
			return *this;
		}

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		[[nodiscard]] friend constexpr rgba8 operator+(rgba8 lhs, rgba8 rhs) noexcept
		{
			rgba8 temp{lhs};
			temp += rhs;
			return temp;
		}

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgba8& operator-=(rgba8 subtrahend) noexcept
		{
			r -= subtrahend.r;
			g -= subtrahend.g;
			b -= subtrahend.b;
			a -= subtrahend.a;
			return *this;
		}

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		[[nodiscard]] friend constexpr rgba8 operator-(rgba8 lhs, rgba8 rhs) noexcept
		{
			rgba8 temp{lhs};
			temp -= rhs;
			return temp;
		}

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgba8& operator*=(rgba8 multiplier) noexcept
		{
			r = r * multiplier.r / 255;
			g = g * multiplier.g / 255;
			b = b * multiplier.b / 255;
			a = a * multiplier.a / 255;
			return *this;
		}

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		[[nodiscard]] friend constexpr rgba8 operator*(rgba8 lhs, rgba8 rhs) noexcept
		{
			rgba8 temp{lhs};
			temp *= rhs;
			return temp;
		}

		/// @}
	};

	/// Floating-point RGBA color.
	struct rgbaf
	{
		/// Red color channel.
		float r;

		/// Green color channel.
		float g;

		/// Blue color channel.
		float b;

		/// Alpha channel.
		float a;

		/// @name Constructors
		/// @{

		/// Default-constructs an RGBA color.
		[[nodiscard]] constexpr rgbaf() noexcept = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgbaf(float r, float g, float b, float a) noexcept
			: r{r}
			, g{g}
			, b{b}
			, a{a}
		{
		}

		/// Converts an 8-bit RGB color into a float one and adds an alpha component.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgbaf(rgb8 rgb, float a = 1.0f) noexcept
			: r{norm_cast<float>(rgb.r)}
			, g{norm_cast<float>(rgb.g)}
			, b{norm_cast<float>(rgb.b)}
			, a{a}
		{
		}

		/// Adds an alpha component to an RGB color.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgbaf(rgbf rgb, float a = 1.0f) noexcept
			: r{rgb.r}
			, g{rgb.g}
			, b{rgb.b}
			, a{a}
		{
		}

		/// Converts a float RGBA color to float.
		/// @param rgba Color to convert to RGBA.
		[[nodiscard]] constexpr rgbaf(rgba8 rgba) noexcept
			: r{norm_cast<float>(rgba.r)}
			, g{norm_cast<float>(rgba.g)}
			, b{norm_cast<float>(rgba.b)}
			, a{norm_cast<float>(rgba.a)}
		{
		}

		/// Converts an HSV color to an RGBA one.
		/// @param hsv Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgbaf(hsv hsv, float a = 1.0f) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		[[nodiscard]] friend constexpr bool operator==(rgbaf lhs, rgbaf rhs) noexcept = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color.
		/// @tparam Scalar Scalar type.
		/// @param addend Scalar to add to the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator+=(Scalar addend) noexcept
		{
			r += addend;
			g += addend;
			b += addend;
			a += addend;
			return *this;
		}

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgbaf operator+(rgbaf lhs, Scalar addend) noexcept
		{
			rgbaf temp{lhs};
			temp += addend;
			return temp;
		}

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator-=(Scalar subtrahend) noexcept
		{
			r -= subtrahend;
			g -= subtrahend;
			b -= subtrahend;
			a -= subtrahend;
			return *this;
		}

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgbaf operator-(rgbaf lhs, Scalar subtrahend) noexcept
		{
			rgbaf temp{lhs};
			temp -= subtrahend;
			return temp;
		}

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator*=(Scalar multiplier) noexcept
		{
			r *= multiplier;
			g *= multiplier;
			b *= multiplier;
			a *= multiplier;
			return *this;
		}

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgbaf operator*(rgbaf lhs, Scalar multiplier) noexcept
		{
			rgbaf temp{lhs};
			temp *= multiplier;
			return temp;
		}

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator/=(Scalar divisor) noexcept
		{
			r /= divisor;
			g /= divisor;
			b /= divisor;
			a /= divisor;
			return *this;
		}

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		[[nodiscard]] friend constexpr rgbaf operator/(rgbaf lhs, Scalar divisor) noexcept
		{
			rgbaf temp{lhs};
			temp /= divisor;
			return temp;
		}

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator+=(rgbaf addend) noexcept
		{
			r += addend.r;
			g += addend.g;
			b += addend.b;
			a += addend.a;
			return *this;
		}

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		[[nodiscard]] friend constexpr rgbaf operator+(rgbaf lhs, rgbaf rhs) noexcept
		{
			rgbaf temp{lhs};
			temp += rhs;
			return temp;
		}

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator-=(rgbaf subtrahend) noexcept
		{
			r -= subtrahend.r;
			g -= subtrahend.g;
			b -= subtrahend.b;
			a -= subtrahend.a;
			return *this;
		}

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		[[nodiscard]] friend constexpr rgbaf operator-(rgbaf lhs, rgbaf rhs) noexcept
		{
			rgbaf temp{lhs};
			temp -= rhs;
			return temp;
		}

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator*=(rgbaf multiplier) noexcept
		{
			r *= multiplier.r;
			g *= multiplier.g;
			b *= multiplier.b;
			a *= multiplier.a;
			return *this;
		}

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		[[nodiscard]] friend constexpr rgbaf operator*(rgbaf lhs, rgbaf rhs) noexcept
		{
			rgbaf temp{lhs};
			temp *= rhs;
			return temp;
		}

		/// @}
	};

	/// Floating-point HSV color.
	struct hsv
	{
		/// Hue channel.
		float h;

		/// Saturation channel.
		float s;

		/// Value channel.
		float v;

		/// @name Constructors
		/// @{

		/// Default-constructs a HSV color.
		[[nodiscard]] constexpr hsv() noexcept = default;

		/// Creates a color from its components.
		/// @param h Hue channel.
		/// @param s Saturation channel.
		/// @param v Value channel.
		[[nodiscard]] constexpr hsv(float h, float s, float v) noexcept
			: h{h}
			, s{s}
			, v{v}
		{
		}

		/// Converts an RGB color into an HSV one.
		/// @param rgb Color to convert to HSV.
		[[nodiscard]] constexpr hsv(rgb8 rgb) noexcept
			: hsv{rgbf{rgb}}
		{
		}

		/// Converts an RGB color into an HSV one.
		/// @param rgb Color to convert to HSV.
		[[nodiscard]] constexpr hsv(rgbf rgb) noexcept
		{
			v = std::max({rgb.r, rgb.g, rgb.b});
			const float delta{v - std::min({rgb.r, rgb.g, rgb.b})};

			if (delta == 0) {
				h = 0;
			}
			else if (v == rgb.r) {
				h = 60.0f * eucmod((rgb.g - rgb.b) / delta, 6.0f);
			}
			else if (v == rgb.g) {
				h = 60.0f * ((rgb.b - rgb.r) / delta + 2.0f);
			}
			else {
				h = 60.0f * ((rgb.r - rgb.g) / delta + 4.0f);
			}

			s = v != 0 ? delta / v : 0;
		}

		/// Converts an RGBA color into an HSV one.
		/// @param rgba Color to convert to HSV.
		[[nodiscard]] constexpr hsv(rgba8 rgba) noexcept
			: hsv{rgbf{rgba}}
		{
		}

		/// Converts an RGBA color into an HSV one.
		/// @param rgba Color to convert to HSV.
		[[nodiscard]] constexpr hsv(rgbaf rgba) noexcept
			: hsv{rgbf{rgba}}
		{
		}

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		[[nodiscard]] friend constexpr bool operator==(hsv lhs, hsv rhs) noexcept = default;

		/// @}
	};

	inline namespace literals
	{
		/// Namespace containing color literals.
		inline namespace color_literals
		{
			/// @name Literals
			/// @{

			/// 8-bit RGBA hex color literal.
			/// @param str Color hex string (may or may not contain alpha information).
			/// @param length Length of the hex string.
			/// @return Corresponding 8-bit RGBA color.
			[[nodiscard]] consteval rgba8 operator""_rgba8(const char* str, usize length) noexcept
			{
				constexpr auto is_digit{[](char chr) { return chr >= '0' && chr <= '9'; }};
				constexpr auto is_uppercase_hex{[](char chr) { return chr >= 'A' && chr <= 'F'; }};
				constexpr auto to_number{
					[=](char chr) -> u8 { return is_digit(chr) ? chr - '0' : chr - (is_uppercase_hex(chr) ? 'A' : 'a') + 0xA; }};

#ifdef TR_ENABLE_ASSERTS
				constexpr auto is_lowercase_hex{[](char chr) { return chr >= 'a' && chr <= 'f'; }};
				constexpr auto is_hex_digit{[=](char chr) { return is_digit(chr) || is_uppercase_hex(chr) || is_lowercase_hex(chr); }};
				TR_ASSERT(length == 7 || length == 9, "Invalid RGBA literal string length {} (must be 7 or 9).", length);
				TR_ASSERT(std::ranges::all_of(std::string_view{str + 1, length - 1}, is_hex_digit),
						  "Invalid color literal hex string '{}'.", str);
#endif

				return {
					static_cast<u8>((to_number(str[1]) << 4) | to_number(str[2])),
					static_cast<u8>((to_number(str[3]) << 4) | to_number(str[4])),
					static_cast<u8>((to_number(str[5]) << 4) | to_number(str[6])),
					static_cast<u8>(length == 9 ? (to_number(str[7]) << 4) | to_number(str[8]) : 255),
				};
			}

			/// Floating-point RGBA hex color literal.
			/// @param str Color hex string (may or may not contain alpha information).
			/// @param length Length of the hex string.
			/// @return Corresponding floating-point RGBA color.
			[[nodiscard]] consteval rgbaf operator""_rgbaf(const char* str, usize length) noexcept
			{
				return operator""_rgba8(str, length);
			}

			/// 8-bit RGB hex color literal.
			/// @param str Color hex string.
			/// @param length Length of the hex string.
			/// @return Corresponding 8-bit RGB color.
			[[nodiscard]] consteval rgb8 operator""_rgb8(const char* str, usize length) noexcept
			{
				TR_ASSERT(length == 7, "Invalid RGB literal string length {} (must be 7).", length);

				return operator""_rgba8(str, length);
			}

			/// Floating-point RGB hex color literal.
			/// @param str Color hex string.
			/// @param length Length of the hex string.
			/// @return Corresponding floating-point RGB color.
			[[nodiscard]] consteval rgbf operator""_rgbf(const char* str, usize length) noexcept
			{
				return operator""_rgb8(str, length);
			}

			/// @}
		} // namespace color_literals
	} // namespace literals
} // namespace tr

//

/// Enables default binary IO for `tr::rgb8`.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::rgb8>{true};

/// Enables default binary IO for `tr::rgbf`.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::rgbf>{true};

/// Enables default binary IO for `tr::rgba8`.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::rgba8>{true};

/// Enables default binary IO for `tr::rgbaf`.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::rgbaf>{true};

/// Enables default binary IO for `tr::hsv`.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::hsv>{true};

//

constexpr tr::rgb8::rgb8(rgbf rgb) noexcept
	: r{norm_cast<u8>(rgb.r)}
	, g{norm_cast<u8>(rgb.g)}
	, b{norm_cast<u8>(rgb.b)}
{
}

constexpr tr::rgb8::rgb8(rgba8 rgba) noexcept
	: r{rgba.r}
	, g{rgba.g}
	, b{rgba.b}
{
}

constexpr tr::rgb8::rgb8(rgbaf rgba) noexcept
	: r{norm_cast<u8>(rgba.r)}
	, g{norm_cast<u8>(rgba.g)}
	, b{norm_cast<u8>(rgba.b)}
{
}

constexpr tr::rgb8::rgb8(hsv hsv) noexcept
	: rgb8{rgbf{hsv}}
{
}

//

constexpr tr::rgbf::rgbf(rgba8 rgba) noexcept
	: r{norm_cast<float>(rgba.r)}
	, g{norm_cast<float>(rgba.g)}
	, b{norm_cast<float>(rgba.b)}
{
}

constexpr tr::rgbf::rgbf(rgbaf rgba) noexcept
	: r{rgba.r}
	, g{rgba.g}
	, b{rgba.b}
{
}

constexpr tr::rgbf::rgbf(hsv hsv) noexcept
{
	const float h{eucmod(hsv.h, 360)};
	const float c{hsv.v * hsv.s};
	const float m{hsv.v - c};
	const float x{c * (1 - std::abs(mod(h / 60, 2) - 1)) + m};

	if (h < 60) {
		r = hsv.v;
		g = x;
		b = m;
	}
	else if (h < 120) {
		r = x;
		g = hsv.v;
		b = m;
	}
	else if (h < 180) {
		r = m;
		g = hsv.v;
		b = x;
	}
	else if (h < 240) {
		r = m;
		g = x;
		b = hsv.v;
	}
	else if (h < 300) {
		r = x;
		g = m;
		b = hsv.v;
	}
	else {
		r = hsv.v;
		g = m;
		b = x;
	}
}

//

constexpr tr::rgba8::rgba8(rgbaf rgba) noexcept
	: r{norm_cast<u8>(rgba.r)}
	, g{norm_cast<u8>(rgba.g)}
	, b{norm_cast<u8>(rgba.b)}
	, a{norm_cast<u8>(rgba.a)}
{
}

constexpr tr::rgba8::rgba8(hsv hsv, u8 a) noexcept
	: rgba8{rgbf{hsv}, a}
{
}

//

constexpr tr::rgbaf::rgbaf(hsv hsv, float a) noexcept
	: rgbaf{rgbf{hsv}, a}
{
}