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
		[[nodiscard]] constexpr rgb8() noexcept = default;

		/// Creates a color from its components.
		/// @param r Red color channel.
		/// @param g Green color channel.
		/// @param b Blue color channel.
		[[nodiscard]] constexpr rgb8(u8 r, u8 g, u8 b) noexcept;

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
		constexpr rgb8& operator+=(Scalar addend) noexcept;

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		friend constexpr rgb8 operator+(rgb8 lhs, Scalar addend) noexcept;

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator-=(Scalar subtrahend) noexcept;

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		friend constexpr rgb8 operator-(rgb8 lhs, Scalar subtrahend) noexcept;

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator*=(Scalar multiplier) noexcept;

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		friend constexpr rgb8 operator*(rgb8 lhs, Scalar multiplier) noexcept;

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgb8& operator/=(Scalar divisor) noexcept;

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		friend constexpr rgb8 operator/(rgb8 lhs, Scalar divisor) noexcept;

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgb8& operator+=(rgb8 addend) noexcept;

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		friend constexpr rgb8 operator+(rgb8 lhs, rgb8 rhs) noexcept;

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgb8& operator-=(rgb8 subtrahend) noexcept;

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		friend constexpr rgb8 operator-(rgb8 lhs, rgb8 rhs) noexcept;

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgb8& operator*=(rgb8 multiplier) noexcept;

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		friend constexpr rgb8 operator*(rgb8 lhs, rgb8 rhs) noexcept;

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
		[[nodiscard]] constexpr rgbf(float r, float g, float b) noexcept;

		/// Converts an 8-bit RGB color into a float one.
		/// @param rgb Color to convert to RGB.
		[[nodiscard]] constexpr rgbf(rgb8 rgb) noexcept;

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
		constexpr rgbf& operator+=(Scalar addend) noexcept;

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		friend constexpr rgbf operator+(rgbf lhs, Scalar addend) noexcept;

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator-=(Scalar subtrahend) noexcept;

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		friend constexpr rgbf operator-(rgbf lhs, Scalar subtrahend) noexcept;

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator*=(Scalar multiplier) noexcept;

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		friend constexpr rgbf operator*(rgbf lhs, Scalar multiplier) noexcept;

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbf& operator/=(Scalar divisor) noexcept;

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		friend constexpr rgbf operator/(rgbf lhs, Scalar divisor) noexcept;

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgbf& operator+=(rgbf addend) noexcept;

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		friend constexpr rgbf operator+(rgbf lhs, rgbf rhs) noexcept;

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgbf& operator-=(rgbf subtrahend) noexcept;

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		friend constexpr rgbf operator-(rgbf lhs, rgbf rhs) noexcept;

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgbf& operator*=(rgbf multiplier) noexcept;

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		friend constexpr rgbf operator*(rgbf lhs, rgbf rhs) noexcept;

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
		[[nodiscard]] constexpr rgba8(u8 r, u8 g, u8 b, u8 a) noexcept;

		/// Adds an alpha component to an RGB color.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgba8(rgb8 rgb, u8 a = 255) noexcept;

		/// Converts a float RGB color into an 8-bit one and adds an alpha component.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgba8(rgbf rgb, u8 a = 255) noexcept;

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
		constexpr rgba8& operator+=(Scalar addend) noexcept;

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		friend constexpr rgba8 operator+(rgba8 lhs, Scalar addend) noexcept;

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator-=(Scalar subtrahend) noexcept;

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		friend constexpr rgba8 operator-(rgba8 lhs, Scalar subtrahend) noexcept;

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator*=(Scalar multiplier) noexcept;

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		friend constexpr rgba8 operator*(rgba8 lhs, Scalar multiplier) noexcept;

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgba8& operator/=(Scalar divisor) noexcept;

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		friend constexpr rgba8 operator/(rgba8 lhs, Scalar divisor) noexcept;

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgba8& operator+=(rgba8 addend) noexcept;

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		friend constexpr rgba8 operator+(rgba8 lhs, rgba8 rhs) noexcept;

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgba8& operator-=(rgba8 subtrahend) noexcept;

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		friend constexpr rgba8 operator-(rgba8 lhs, rgba8 rhs) noexcept;

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgba8& operator*=(rgba8 multiplier) noexcept;

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		friend constexpr rgba8 operator*(rgba8 lhs, rgba8 rhs) noexcept;

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
		[[nodiscard]] constexpr rgbaf(float r, float g, float b, float a) noexcept;

		/// Converts an 8-bit RGB color into a float one and adds an alpha component.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgbaf(rgb8 rgb, float a = 1.0f) noexcept;

		/// Adds an alpha component to an RGB color.
		/// @param rgb Color to convert to RGBA.
		/// @param a Alpha channel.
		[[nodiscard]] constexpr rgbaf(rgbf rgb, float a = 1.0f) noexcept;

		/// Converts a float RGBA color to float.
		/// @param rgba Color to convert to RGBA.
		[[nodiscard]] constexpr rgbaf(rgba8 rgba) noexcept;

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
		constexpr rgbaf& operator+=(Scalar addend) noexcept;

		/// Adds a scalar to a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to add to.
		/// @param addend Scalar to add to the color.
		/// @return Color with the scalar added memberwise to the color.
		template <arithmetic Scalar>
		friend constexpr rgbaf operator+(rgbaf lhs, Scalar addend) noexcept;

		/// Subtracts a scalar from the color.
		/// @tparam Scalar Scalar type.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator-=(Scalar subtrahend) noexcept;

		/// Subtracts a scalar from a color.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to subtract from.
		/// @param subtrahend Scalar to subtract from the color.
		/// @return Color with the scalar subtracted memberwise from the color.
		template <arithmetic Scalar>
		friend constexpr rgbaf operator-(rgbaf lhs, Scalar subtrahend) noexcept;

		/// Multiplies the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator*=(Scalar multiplier) noexcept;

		/// Multiplies a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to multiply.
		/// @param multiplier Scalar to multiply the color with.
		/// @return Color multiplied memberwise with the scalar.
		template <arithmetic Scalar>
		friend constexpr rgbaf operator*(rgbaf lhs, Scalar multiplier) noexcept;

		/// Divides the color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param divisor Scalar to divide the color by.
		/// @return Reference to `*this`.
		template <arithmetic Scalar>
		constexpr rgbaf& operator/=(Scalar divisor) noexcept;

		/// Divides a color by a scalar.
		/// @tparam Scalar Scalar type.
		/// @param lhs Color to divide.
		/// @param divisor Scalar to divide the color by.
		/// @return Color divided memberwise by the scalar.
		template <arithmetic Scalar>
		friend constexpr rgbaf operator/(rgbaf lhs, Scalar divisor) noexcept;

		/// Adds another color to the color.
		/// @param addend Color to add.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator+=(rgbaf addend) noexcept;

		/// Adds two colors.
		/// @param lhs, rhs Colors to add.
		/// @return Memberwise sum of the colors.
		friend constexpr rgbaf operator+(rgbaf lhs, rgbaf rhs) noexcept;

		/// Subtracts another color from the color.
		/// @param subtrahend Color to subtract.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator-=(rgbaf subtrahend) noexcept;

		/// Subtracts two colors.
		/// @param lhs, rhs Colors to subtract.
		/// @return Memberwise difference of the colors.
		friend constexpr rgbaf operator-(rgbaf lhs, rgbaf rhs) noexcept;

		/// Multiplies the color with another color.
		/// @param multiplier Color to multiply with.
		/// @return Reference to `*this`.
		constexpr rgbaf& operator*=(rgbaf multiplier) noexcept;

		/// Multiplies two colors.
		/// @param lhs, rhs Colors to multiply.
		/// @return Memberwise multiplication of the colors.
		friend constexpr rgbaf operator*(rgbaf lhs, rgbaf rhs) noexcept;

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
		[[nodiscard]] constexpr hsv(float h, float s, float v) noexcept;

		/// Converts an RGB color into an HSV one.
		/// @param rgb Color to convert to HSV.
		[[nodiscard]] constexpr hsv(rgb8 rgb) noexcept;

		/// Converts an RGB color into an HSV one.
		/// @param rgb Color to convert to HSV.
		[[nodiscard]] constexpr hsv(rgbf rgb) noexcept;

		/// Converts an RGBA color into an HSV one.
		/// @param rgba Color to convert to HSV.
		[[nodiscard]] constexpr hsv(rgba8 rgba) noexcept;

		/// Converts an RGBA color into an HSV one.
		/// @param rgba Color to convert to HSV.
		[[nodiscard]] constexpr hsv(rgbaf rgba) noexcept;

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

			/// 8-bit RGB hex color literal.
			/// @param str Color hex string.
			/// @param length Length of the hex string.
			/// @return Corresponding 8-bit RGB color.
			[[nodiscard]] consteval rgb8 operator""_rgb8(const char* str, usize length) noexcept;

			/// Floating-point RGB hex color literal.
			/// @param str Color hex string.
			/// @param length Length of the hex string.
			/// @return Corresponding floating-point RGB color.
			[[nodiscard]] consteval rgbf operator""_rgbf(const char* str, usize length) noexcept;

			/// 8-bit RGBA hex color literal.
			/// @param str Color hex string (may or may not contain alpha information).
			/// @param length Length of the hex string.
			/// @return Corresponding 8-bit RGBA color.
			[[nodiscard]] consteval rgba8 operator""_rgba8(const char* str, usize length) noexcept;

			/// Floating-point RGBA hex color literal.
			/// @param str Color hex string (may or may not contain alpha information).
			/// @param length Length of the hex string.
			/// @return Corresponding floating-point RGBA color.
			[[nodiscard]] consteval rgbaf operator""_rgbaf(const char* str, usize length) noexcept;

			/// @}
		} // namespace color_literals
	} // namespace literals
} // namespace tr

#include "impl/color.hpp" // IWYU pragma: export
