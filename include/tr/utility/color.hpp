/// @file
/// @brief Provides color datatypes.

#pragma once
#include "concepts.hpp"

namespace tr
{
	struct rgb8;
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
		constexpr rgb8() = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		constexpr rgb8(u8 r, u8 g, u8 b);

		/// Converts a float RGB color into an 8-bit one.
		/// @param rgb Color to convert to RGB.
		constexpr rgb8(rgbf rgb);

		/// Removes the alpha component from an RGBA color.
		/// @param rgba Color to convert to RGB.
		constexpr rgb8(rgba8 rgba);

		/// Removes the alpha component from an RGBA color and converts it to 8-bit.
		/// @param rgba Color to convert to RGB.
		constexpr rgb8(rgbaf rgba);

		/// Converts an HSV color to an RGB one.
		/// @param hsv Color to convert to RGB.
		constexpr rgb8(hsv hsv);

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		friend constexpr bool operator==(rgb8 lhs, rgb8 rhs) = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color.
		/// @tparam Scalar Scalar type.
		/// @param addend Scalar to add to the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator+=(Scalar addend);

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		friend constexpr rgb8 operator+(rgb8 lhs, Scalar addend);

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator-=(Scalar subtrahend);

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		friend constexpr rgb8 operator-(rgb8 lhs, Scalar subtrahend);

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator*=(Scalar multiplier);

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		friend constexpr rgb8 operator*(rgb8 lhs, Scalar multiplier);

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator/=(Scalar divisor);

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		friend constexpr rgb8 operator/(rgb8 lhs, Scalar divisor);

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgb8& operator+=(rgb8 addend);

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		friend constexpr rgb8 operator+(rgb8 lhs, rgb8 rhs);

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgb8& operator-=(rgb8 subtrahend);

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		friend constexpr rgb8 operator-(rgb8 lhs, rgb8 rhs);

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgb8& operator*=(rgb8 multiplier);

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		friend constexpr rgb8 operator*(rgb8 lhs, rgb8 rhs);

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
		constexpr rgbf() = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		constexpr rgbf(float r, float g, float b);

		/// Converts an 8-bit RGB color into a float one.
		/// @param rgb Color to convert to RGB.
		constexpr rgbf(rgb8 rgb);

		/// Removes the alpha component from an RGBA color and converts it to float.
		/// @param rgba Color to convert to RGB.
		constexpr rgbf(rgba8 rgba);

		/// Removes the alpha component from an RGBA color.
		/// @param rgba Color to convert to RGB.
		constexpr rgbf(rgbaf rgba);

		/// Converts an HSV color to an RGB one.
		/// @param hsv Color to convert to RGB.
		constexpr rgbf(hsv hsv);

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		friend constexpr bool operator==(rgbf lhs, rgbf rhs) = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color.
		/// @tparam Scalar Scalar type.
		/// @param addend Scalar to add to the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator+=(Scalar addend);

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		friend constexpr rgbf operator+(rgbf lhs, Scalar addend);

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator-=(Scalar subtrahend);

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		friend constexpr rgbf operator-(rgbf lhs, Scalar subtrahend);

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator*=(Scalar multiplier);

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		friend constexpr rgbf operator*(rgbf lhs, Scalar multiplier);

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator/=(Scalar divisor);

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		friend constexpr rgbf operator/(rgbf lhs, Scalar divisor);

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgbf& operator+=(rgbf addend);

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		friend constexpr rgbf operator+(rgbf lhs, rgbf rhs);

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgbf& operator-=(rgbf subtrahend);

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		friend constexpr rgbf operator-(rgbf lhs, rgbf rhs);

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgbf& operator*=(rgbf multiplier);

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		friend constexpr rgbf operator*(rgbf lhs, rgbf rhs);

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
		constexpr rgba8() = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		/// @param a Alpha channel.
		constexpr rgba8(u8 r, u8 g, u8 b, u8 a);

		/// Adds an alpha component to an RGB color.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		constexpr rgba8(rgb8 rgb, u8 a = 255);

		/// Converts a float RGB color into an 8-bit one and adds an alpha component.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		constexpr rgba8(rgbf rgb, u8 a = 255);

		/// Converts a float RGBA color into an 8-bit one.
		/// @param rgba Color to convert to RGBA.
		constexpr rgba8(rgbaf rgba);

		/// Converts an HSV color to an RGBA one.
		/// @param hsv Color to convert to RGBA.
		/// @param a Alpha channel.
		constexpr rgba8(hsv hsv, u8 a = 255);

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		friend constexpr bool operator==(rgba8 lhs, rgba8 rhs) = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color.
		/// @tparam Scalar Scalar type.
		/// @param addend Scalar to add to the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator+=(Scalar addend);

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		friend constexpr rgba8 operator+(rgba8 lhs, Scalar addend);

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator-=(Scalar subtrahend);

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		friend constexpr rgba8 operator-(rgba8 lhs, Scalar subtrahend);

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator*=(Scalar multiplier);

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		friend constexpr rgba8 operator*(rgba8 lhs, Scalar multiplier);

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator/=(Scalar divisor);

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		friend constexpr rgba8 operator/(rgba8 lhs, Scalar divisor);

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgba8& operator+=(rgba8 addend);

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		friend constexpr rgba8 operator+(rgba8 lhs, rgba8 rhs);

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgba8& operator-=(rgba8 subtrahend);

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		friend constexpr rgba8 operator-(rgba8 lhs, rgba8 rhs);

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgba8& operator*=(rgba8 multiplier);

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		friend constexpr rgba8 operator*(rgba8 lhs, rgba8 rhs);

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
		constexpr rgbaf() = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		/// @param a Alpha channel.
		constexpr rgbaf(float r, float g, float b, float a);

		/// Converts an 8-bit RGB color into a float one and adds an alpha component.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		constexpr rgbaf(rgb8 rgb, float a = 1.0f);

		/// Adds an alpha component to an RGB color.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		constexpr rgbaf(rgbf rgb, float a = 1.0f);

		/// Converts a float RGBA color to float.
		/// @param rgba Color to convert to RGBA.
		constexpr rgbaf(rgba8 rgba);

		/// Converts an HSV color to an RGBA one.
		/// @param hsv Color to convert to RGBA.
		/// @param a Alpha channel.
		constexpr rgbaf(hsv hsv, float a = 1.0f);

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		friend constexpr bool operator==(rgbaf lhs, rgbaf rhs) = default;

		/// @}
		/// @name Arithmetic operators
		/// @{

		/// Adds a scalar to the color.
		/// @tparam Scalar Scalar type.
		/// @param addend Scalar to add to the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator+=(Scalar addend);

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		friend constexpr rgbaf operator+(rgbaf lhs, Scalar addend);

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator-=(Scalar subtrahend);

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		friend constexpr rgbaf operator-(rgbaf lhs, Scalar subtrahend);

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator*=(Scalar multiplier);

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		friend constexpr rgbaf operator*(rgbaf lhs, Scalar multiplier);

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator/=(Scalar divisor);

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		friend constexpr rgbaf operator/(rgbaf lhs, Scalar divisor);

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator+=(rgbaf addend);

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		friend constexpr rgbaf operator+(rgbaf lhs, rgbaf rhs);

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator-=(rgbaf subtrahend);

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		friend constexpr rgbaf operator-(rgbaf lhs, rgbaf rhs);

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator*=(rgbaf multiplier);

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		friend constexpr rgbaf operator*(rgbaf lhs, rgbaf rhs);

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
		constexpr hsv() = default;

		/// Creates a color from its components.
		/// @param h Hue channel.
		/// @param s Saturation channel.
		/// @param v Value channel.
		constexpr hsv(float h, float s, float v);

		/// Converts an RGB color into an HSV one.
		/// @param rgb Color to convert to HSV.
		constexpr hsv(rgb8 rgb);

		/// Converts an RGB color into an HSV one.
		/// @param rgb Color to convert to HSV.
		constexpr hsv(rgbf rgb);

		/// Converts an RGBA color into an HSV one.
		/// @param rgba Color to convert to HSV.
		constexpr hsv(rgba8 rgba);

		/// Converts an RGBA color into an HSV one.
		/// @param rgba Color to convert to HSV.
		constexpr hsv(rgbaf rgba);

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two colors for equality.
		/// @param lhs, rhs Colors to compare.
		/// @return Whether the two colors are equal.
		friend constexpr bool operator==(hsv lhs, hsv rhs) = default;

		/// @}
	};

	inline namespace literals
	{
		/// Namespace containing color literals.
		inline namespace color_literals
		{
			/// @name Literals
			/// @{

			/// 8-bit RGB hex color literal.
			/// @param str Color hex string.
			/// @param length Length of the hex string.
			/// @return Corresponding 8-bit RGB color.
			consteval rgb8 operator""_rgb8(const char* str, usize length);

			/// Floating-point RGB hex color literal.
			/// @param str Color hex string.
			/// @param length Length of the hex string.
			/// @return Corresponding floating-point RGB color.
			consteval rgbf operator""_rgbf(const char* str, usize length);

			/// 8-bit RGBA hex color literal.
			/// @param str Color hex string (may or may not contain alpha information).
			/// @param length Length of the hex string.
			/// @return Corresponding 8-bit RGBA color.
			consteval rgba8 operator""_rgba8(const char* str, usize length);

			/// Floating-point RGBA hex color literal.
			/// @param str Color hex string (may or may not contain alpha information).
			/// @param length Length of the hex string.
			/// @return Corresponding floating-point RGBA color.
			consteval rgbaf operator""_rgbaf(const char* str, usize length);

			/// @}
		} // namespace color_literals
	} // namespace literals
} // namespace tr

#include "impl/color.hpp" // IWYU pragma: export
