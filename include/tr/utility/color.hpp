///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides color datatypes.                                                                                                             //
//                                                                                                                                       //
// RGB(A) color datatypes with 8-bit and floating point components are provided, as well as tr::hsv for HSV colors. All colors can be    //
// default-constructed to zero, constructed from their components, or converted from one of the other color types. Literals to construct //
// any of the RGB(A) color types using hex codes are also provided in tr::color_literals, as well as tr::literals:                       //
//     - tr::rgb8{} -> constructs an 8-bit RGB color with {.r = 0, .g = 0, .b = 0}                                                       //
//     - tr::rgba8{255, 0, 0, 127} -> constructs an 8-bit RGBA color with {.r = 255, .g = 0, .b = 0, .a = 127}                           //
//     - tr::rgbf{"FF0000FF"_rgba8} -> constructs a floating-point RGB color with {.r = 1.0f, .g = 0.0f, b = 0.0f}                       //
//     - "FF0000"_rgbf -> equivalent to the above                                                                                        //
//     - tr::rgbaf{tr::hsv{0, 1, 1}, 0.5f} -> constructs a floating-point RGBA color with {.r = 1.0f, .g = 0.0f, b = 0.,0f, a = 0.5f}    //
//                                                                                                                                       //
// All RGB(A) colors can be compared for equality, added, subtracted and multiplied together with another color or scalar, and divided   //
// with a scalar. The type of the left operand will be used for the returned value:                                                      //
//     - "808080"_rgb8 + 64 -> "C0C0C0"_rgb8                                                                                             //
//     - "FF0000"_rgba8 - "800000"_rgbf -> "7F0000FF"_rgba8                                                                              //
//     - "FFFFFF"_rgbf * 0.75f -> tr::rgbf{0.75f, 0.75f, 0.75f}                                                                          //
//     - "FFFFFF"_rgbaf / 2 -> tr::rgbaf{0.5f, 0.5f, 0.5f, 0.5f}                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

namespace tr {
	struct rgb8;
	struct rgbf;
	struct rgba8;
	struct rgbaf;
	struct hsv;
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// 8-bit RGB color.
	struct rgb8 {
		// The red color channel.
		u8 r;
		// The green color channel.
		u8 g;
		// The blue color channel.
		u8 b;

		constexpr rgb8() = default;
		// Creates a color from its components.
		constexpr rgb8(u8 r, u8 g, u8 b);
		// Converts a float RGB color into an 8-bit one.
		constexpr rgb8(const rgbf& rgb);
		// Removes the alpha component from an RGBA color.
		constexpr rgb8(const rgba8& rgba);
		// Removes the alpha component from an RGBA color and converts it to 8-bit.
		constexpr rgb8(const rgbaf& rgba);
		// Converts an HSV color to an RGB one.
		constexpr rgb8(const hsv& hsv);

		friend constexpr bool operator==(const rgb8&, const rgb8&) = default;

		// Adds a scalar to the color.
		template <arithmetic T> constexpr rgb8& operator+=(T addend);
		// Adds two colors.
		constexpr rgb8& operator+=(const rgb8& addend);
		// Adds a scalar to the color.
		template <arithmetic T> friend constexpr rgb8 operator+(const rgb8& l, T addend);
		// Adds two colors.
		friend constexpr rgb8 operator+(const rgb8& l, const rgb8& r);
		// Subtracts a scalar from the color.
		template <arithmetic T> constexpr rgb8& operator-=(T subtrahend);
		// Subtracts two colors.
		constexpr rgb8& operator-=(const rgb8& subtrahend);
		// Subtracts a scalar from the color.
		template <arithmetic T> friend constexpr rgb8 operator-(const rgb8& l, T subtrahend);
		// Subtracts two colors.
		friend constexpr rgb8 operator-(const rgb8& l, const rgb8& r);
		// Multiplies the color by a scalar.
		template <arithmetic T> constexpr rgb8& operator*=(T multiplier);
		// Multiplies two colors.
		constexpr rgb8& operator*=(const rgb8& multiplier);
		// Multiplies the color by a scalar.
		template <arithmetic T> friend constexpr rgb8 operator*(const rgb8& l, T multiplier);
		// Multiplies two colors.
		friend constexpr rgb8 operator*(const rgb8& l, const rgb8& r);
		// Divides the color by a scalar.
		template <arithmetic T> constexpr rgb8& operator/=(T divisor);
		// Divides the color by a scalar.
		template <arithmetic T> friend constexpr rgb8 operator/(const rgb8& l, T divisor);
	};

	// Floating-point RGB color.
	struct rgbf {
		// The red color channel.
		float r;
		// The green color channel.
		float g;
		// The blue color channel.
		float b;

		constexpr rgbf() = default;
		// Creates a color from its components.
		constexpr rgbf(float r, float g, float b);
		// Converts an 8-bit RGB color into a float one.
		constexpr rgbf(const rgb8& rgb);
		// Removes the alpha component from an RGBA color and converts it to float.
		constexpr rgbf(const rgba8& rgba);
		// Removes the alpha component from an RGBA color.
		constexpr rgbf(const rgbaf& rgba);
		// Converts an HSV color to an RGB one.
		constexpr rgbf(const hsv& hsv);

		friend constexpr bool operator==(const rgbf&, const rgbf&) = default;

		// Adds a scalar to the color.
		template <arithmetic T> constexpr rgbf& operator+=(T addend);
		// Adds two colors.
		constexpr rgbf& operator+=(const rgbf& addend);
		// Adds a scalar to the color.
		template <arithmetic T> friend constexpr rgbf operator+(const rgbf& l, T addend);
		// Adds two colors.
		friend constexpr rgbf operator+(const rgbf& l, const rgbf& r);
		// Subtracts a scalar from the color.
		template <arithmetic T> constexpr rgbf& operator-=(T subtrahend);
		// Subtracts two colors.
		constexpr rgbf& operator-=(const rgbf& subtrahend);
		// Subtracts a scalar from the color.
		template <arithmetic T> friend constexpr rgbf operator-(const rgbf& l, T subtrahend);
		// Subtracts two colors.
		friend constexpr rgbf operator-(const rgbf& l, const rgbf& r);
		// Multiplies the color by a scalar.
		template <arithmetic T> constexpr rgbf& operator*=(T multiplier);
		// Multiplies two colors.
		constexpr rgbf& operator*=(const rgbf& multiplier);
		// Multiplies the color by a scalar.
		template <arithmetic T> friend constexpr rgbf operator*(const rgbf& l, T multiplier);
		// Multiplies two colors.
		friend constexpr rgbf operator*(const rgbf& l, const rgbf& r);
		// Divides the color by a scalar.
		template <arithmetic T> constexpr rgbf& operator/=(T divisor);
		// Divides the color by a scalar.
		template <arithmetic T> friend constexpr rgbf operator/(const rgbf& l, T divisor);
	};

	// 8-bit RGBA color.
	struct rgba8 {
		// The red color channel.
		u8 r;
		// The green color channel.
		u8 g;
		// The blue color channel.
		u8 b;
		// The alpha channel.
		u8 a;

		constexpr rgba8() = default;
		// Creates a color from its components.
		constexpr rgba8(u8 r, u8 g, u8 b, u8 a);
		// Adds an alpha component to an RGB color.
		constexpr rgba8(const rgb8& rgb, u8 a = 255);
		// Converts a float RGB color into an 8-bit one and adds an alpha component.
		constexpr rgba8(const rgbf& rgb, u8 a = 255);
		// Converts a float RGBA color into an 8-bit one.
		constexpr rgba8(const rgbaf& rgba);
		// Converts an HSV color to an RGBA one.
		constexpr rgba8(const hsv& hsv, u8 a = 255);

		friend constexpr bool operator==(const rgba8&, const rgba8&) = default;

		// Adds a scalar to the color.
		template <arithmetic T> constexpr rgba8& operator+=(T addend);
		// Adds two colors.
		constexpr rgba8& operator+=(const rgba8& addend);
		// Adds a scalar to the color.
		template <arithmetic T> friend constexpr rgba8 operator+(const rgba8& l, T addend);
		// Adds two colors.
		friend constexpr rgba8 operator+(const rgba8& l, const rgba8& r);
		// Subtracts a scalar from the color.
		template <arithmetic T> constexpr rgba8& operator-=(T subtrahend);
		// Subtracts two colors.
		constexpr rgba8& operator-=(const rgba8& subtrahend);
		// Subtracts a scalar from the color.
		template <arithmetic T> friend constexpr rgba8 operator-(const rgba8& l, T subtrahend);
		// Subtracts two colors.
		friend constexpr rgba8 operator-(const rgba8& l, const rgba8& r);
		// Multiplies the color by a scalar.
		template <arithmetic T> constexpr rgba8& operator*=(T multiplier);
		// Multiplies two colors.
		constexpr rgba8& operator*=(const rgba8& multiplier);
		// Multiplies the color by a scalar.
		template <arithmetic T> friend constexpr rgba8 operator*(const rgba8& l, T multiplier);
		// Multiplies two colors.
		friend constexpr rgba8 operator*(const rgba8& l, const rgba8& r);
		// Divides the color by a scalar.
		template <arithmetic T> constexpr rgba8& operator/=(T divisor);
		// Divides the color by a scalar.
		template <arithmetic T> friend constexpr rgba8 operator/(const rgba8& l, T divisor);
	};

	// Floating-point RGBA color.
	struct rgbaf {
		// The red color channel.
		float r;
		// The green color channel.
		float g;
		// The blue color channel.
		float b;
		// The alpha channel.
		float a;

		constexpr rgbaf() = default;
		// Creates a color from its components.
		constexpr rgbaf(float r, float g, float b, float a);
		// Converts an 8-bit RGB color into a float one and adds an alpha component.
		constexpr rgbaf(const rgb8& rgb, float a = 1.0f);
		// Adds an alpha component to an RGB color.
		constexpr rgbaf(const rgbf& rgb, float a = 1.0f);
		// Converts a float RGBA color to float.
		constexpr rgbaf(const rgba8& rgba);
		// Converts an HSV color to an RGBA one.
		constexpr rgbaf(const hsv& hsv, float a = 1.0f);

		friend constexpr bool operator==(const rgbaf&, const rgbaf&) = default;

		// Adds a scalar to the color.
		template <arithmetic T> constexpr rgbaf& operator+=(T addend);
		// Adds two colors.
		constexpr rgbaf& operator+=(const rgbaf& addend);
		// Adds a scalar to the color.
		template <arithmetic T> friend constexpr rgbaf operator+(const rgbaf& l, T addend);
		// Adds two colors.
		friend constexpr rgbaf operator+(const rgbaf& l, const rgbaf& r);
		// Subtracts a scalar from the color.
		template <arithmetic T> constexpr rgbaf& operator-=(T subtrahend);
		// Subtracts two colors.
		constexpr rgbaf& operator-=(const rgbaf& subtrahend);
		// Subtracts a scalar from the color.
		template <arithmetic T> friend constexpr rgbaf operator-(const rgbaf& l, T subtrahend);
		// Subtracts two colors.
		friend constexpr rgbaf operator-(const rgbaf& l, const rgbaf& r);
		// Multiplies the color by a scalar.
		template <arithmetic T> constexpr rgbaf& operator*=(T multiplier);
		// Multiplies two colors.
		constexpr rgbaf& operator*=(const rgbaf& multiplier);
		// Multiplies the color by a scalar.
		template <arithmetic T> friend constexpr rgbaf operator*(const rgbaf& l, T multiplier);
		// Multiplies two colors.
		friend constexpr rgbaf operator*(const rgbaf& l, const rgbaf& r);
		// Divides the color by a scalar.
		template <arithmetic T> constexpr rgbaf& operator/=(T divisor);
		// Divides the color by a scalar.
		template <arithmetic T> friend constexpr rgbaf operator/(const rgbaf& l, T divisor);
	};

	// Floating-point HSV color.
	struct hsv {
		// The hue channel.
		float h;
		// The saturation channel.
		float s;
		// The value channel.
		float v;

		constexpr hsv() = default;
		// Creates a color from its components.
		constexpr hsv(float h, float s, float v);
		// Converts an RGB color into an HSV one.
		constexpr hsv(const rgb8& rgb);
		// Converts an RGB color into an HSV one.
		constexpr hsv(const rgbf& rgb);
		// Converts an RGBA color into an HSV one.
		constexpr hsv(const rgba8& rgba);
		// Converts an RGBA color into an HSV one.
		constexpr hsv(const rgbaf& rgba);
	};

	inline namespace literals {
		// Color literals.
		inline namespace color_literals {
			// 8-bit RGB color literal.
			consteval rgb8 operator""_rgb8(const char* str, usize length);
			// Floating-point RGB color literal.
			consteval rgbf operator""_rgbf(const char* str, usize length);
			// 8-bit RGBA color literal.
			consteval rgba8 operator""_rgba8(const char* str, usize length);
			// Floating-point RGBA color literal.
			consteval rgbaf operator""_rgbaf(const char* str, usize length);
		} // namespace color_literals
	} // namespace literals
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "color_impl.hpp" // IWYU pragma: keep
