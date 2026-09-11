/// @file
/// @brief Implements color.hpp.

#pragma once
#include "../color.hpp"
#include "../math.hpp"
#include "../norm_cast.hpp"

//

constexpr tr::rgb8::rgb8(u8 r, u8 g, u8 b) noexcept
	: r{r}
	, g{g}
	, b{b}
{
}

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

template <tr::arithmetic Scalar>
constexpr tr::rgb8& tr::rgb8::operator+=(Scalar addend) noexcept
{
	r += addend;
	g += addend;
	b += addend;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgb8 tr::operator+(rgb8 lhs, Scalar addend) noexcept
{
	rgb8 temp{lhs};
	temp += addend;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgb8& tr::rgb8::operator-=(Scalar subtrahend) noexcept
{
	r -= subtrahend;
	g -= subtrahend;
	b -= subtrahend;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgb8 tr::operator-(rgb8 lhs, Scalar subtrahend) noexcept
{
	rgb8 temp{lhs};
	temp -= subtrahend;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgb8& tr::rgb8::operator*=(Scalar multiplier) noexcept
{
	r *= multiplier;
	g *= multiplier;
	b *= multiplier;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgb8 tr::operator*(rgb8 lhs, Scalar multiplier) noexcept
{
	rgb8 temp{lhs};
	temp *= multiplier;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgb8& tr::rgb8::operator/=(Scalar divisor) noexcept
{
	r /= divisor;
	g /= divisor;
	b /= divisor;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgb8 tr::operator/(rgb8 lhs, Scalar divisor) noexcept
{
	rgb8 temp{lhs};
	temp /= divisor;
	return temp;
}

//

constexpr tr::rgb8& tr::rgb8::operator+=(rgb8 addend) noexcept
{
	r += addend.r;
	g += addend.g;
	b += addend.b;
	return *this;
}

[[nodiscard]] constexpr tr::rgb8 tr::operator+(rgb8 lhs, rgb8 rhs) noexcept
{
	rgb8 temp{lhs};
	temp += rhs;
	return temp;
}

constexpr tr::rgb8& tr::rgb8::operator-=(rgb8 subtrahend) noexcept
{
	r -= subtrahend.r;
	g -= subtrahend.g;
	b -= subtrahend.b;
	return *this;
}

[[nodiscard]] constexpr tr::rgb8 tr::operator-(rgb8 lhs, rgb8 rhs) noexcept
{
	rgb8 temp{lhs};
	temp -= rhs;
	return temp;
}

constexpr tr::rgb8& tr::rgb8::operator*=(rgb8 multiplier) noexcept
{
	r = r * multiplier.r / 255;
	g = g * multiplier.g / 255;
	b = b * multiplier.b / 255;
	return *this;
}

[[nodiscard]] constexpr tr::rgb8 tr::operator*(rgb8 lhs, rgb8 rhs) noexcept
{
	rgb8 temp{lhs};
	temp *= rhs;
	return temp;
}

//

constexpr tr::rgbf::rgbf(float r, float g, float b) noexcept
	: r{r}
	, g{g}
	, b{b}
{
}

constexpr tr::rgbf::rgbf(rgb8 rgb) noexcept
	: r{norm_cast<float>(rgb.r)}
	, g{norm_cast<float>(rgb.g)}
	, b{norm_cast<float>(rgb.b)}
{
}

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

template <tr::arithmetic Scalar>
constexpr tr::rgbf& tr::rgbf::operator+=(Scalar addend) noexcept
{
	r += addend;
	g += addend;
	b += addend;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgbf tr::operator+(rgbf lhs, Scalar addend) noexcept
{
	rgbf temp{lhs};
	temp += addend;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgbf& tr::rgbf::operator-=(Scalar subtrahend) noexcept
{
	r -= subtrahend;
	g -= subtrahend;
	b -= subtrahend;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgbf tr::operator-(rgbf lhs, Scalar subtrahend) noexcept
{
	rgbf temp{lhs};
	temp -= subtrahend;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgbf& tr::rgbf::operator*=(Scalar multiplier) noexcept
{
	r *= multiplier;
	g *= multiplier;
	b *= multiplier;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgbf tr::operator*(rgbf lhs, Scalar multiplier) noexcept
{
	rgbf temp{lhs};
	temp *= multiplier;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgbf& tr::rgbf::operator/=(Scalar divisor) noexcept
{
	r /= divisor;
	g /= divisor;
	b /= divisor;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgbf tr::operator/(rgbf lhs, Scalar divisor) noexcept
{
	rgbf temp{lhs};
	temp /= divisor;
	return temp;
}

//

constexpr tr::rgbf& tr::rgbf::operator+=(rgbf addend) noexcept
{
	r += addend.r;
	g += addend.g;
	b += addend.b;
	return *this;
}

[[nodiscard]] constexpr tr::rgbf tr::operator+(rgbf lhs, rgbf rhs) noexcept
{
	rgbf temp{lhs};
	temp += rhs;
	return temp;
}

constexpr tr::rgbf& tr::rgbf::operator-=(rgbf subtrahend) noexcept
{
	r -= subtrahend.r;
	g -= subtrahend.g;
	b -= subtrahend.b;
	return *this;
}

[[nodiscard]] constexpr tr::rgbf tr::operator-(rgbf lhs, rgbf rhs) noexcept
{
	rgbf temp{lhs};
	temp -= rhs;
	return temp;
}

constexpr tr::rgbf& tr::rgbf::operator*=(rgbf multiplier) noexcept
{
	r *= multiplier.r;
	g *= multiplier.g;
	b *= multiplier.b;
	return *this;
}

[[nodiscard]] constexpr tr::rgbf tr::operator*(rgbf lhs, rgbf rhs) noexcept
{
	rgbf temp{lhs};
	temp *= rhs;
	return temp;
}

//

constexpr tr::rgba8::rgba8(u8 r, u8 g, u8 b, u8 a) noexcept
	: r{r}
	, g{g}
	, b{b}
	, a{a}
{
}

constexpr tr::rgba8::rgba8(rgb8 rgb, u8 a) noexcept
	: r{rgb.r}
	, g{rgb.g}
	, b{rgb.b}
	, a{a}
{
}

constexpr tr::rgba8::rgba8(rgbf rgb, u8 a) noexcept
	: r{norm_cast<u8>(rgb.r)}
	, g{norm_cast<u8>(rgb.g)}
	, b{norm_cast<u8>(rgb.b)}
	, a{a}
{
}

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

template <tr::arithmetic Scalar>
constexpr tr::rgba8& tr::rgba8::operator+=(Scalar addend) noexcept
{
	r += addend;
	g += addend;
	b += addend;
	a += addend;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgba8 tr::operator+(rgba8 lhs, Scalar addend) noexcept
{
	rgba8 temp{lhs};
	temp += addend;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgba8& tr::rgba8::operator-=(Scalar subtrahend) noexcept
{
	r -= subtrahend;
	g -= subtrahend;
	b -= subtrahend;
	a -= subtrahend;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgba8 tr::operator-(rgba8 lhs, Scalar subtrahend) noexcept
{
	rgba8 temp{lhs};
	temp -= subtrahend;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgba8& tr::rgba8::operator*=(Scalar multiplier) noexcept
{
	r *= multiplier;
	g *= multiplier;
	b *= multiplier;
	a *= multiplier;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgba8 tr::operator*(rgba8 lhs, Scalar multiplier) noexcept
{
	rgba8 temp{lhs};
	temp *= multiplier;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgba8& tr::rgba8::operator/=(Scalar divisor) noexcept
{
	r /= divisor;
	g /= divisor;
	b /= divisor;
	a /= divisor;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgba8 tr::operator/(rgba8 lhs, Scalar divisor) noexcept
{
	rgba8 temp{lhs};
	temp /= divisor;
	return temp;
}

//

constexpr tr::rgba8& tr::rgba8::operator+=(rgba8 addend) noexcept
{
	r += addend.r;
	g += addend.g;
	b += addend.b;
	a += addend.a;
	return *this;
}

[[nodiscard]] constexpr tr::rgba8 tr::operator+(rgba8 lhs, rgba8 rhs) noexcept
{
	rgba8 temp{lhs};
	temp += rhs;
	return temp;
}

constexpr tr::rgba8& tr::rgba8::operator-=(rgba8 subtrahend) noexcept
{
	r -= subtrahend.r;
	g -= subtrahend.g;
	b -= subtrahend.b;
	a -= subtrahend.a;
	return *this;
}

[[nodiscard]] constexpr tr::rgba8 tr::operator-(rgba8 lhs, rgba8 rhs) noexcept
{
	rgba8 temp{lhs};
	temp -= rhs;
	return temp;
}

constexpr tr::rgba8& tr::rgba8::operator*=(rgba8 multiplier) noexcept
{
	r = r * multiplier.r / 255;
	g = g * multiplier.g / 255;
	b = b * multiplier.b / 255;
	a = a * multiplier.a / 255;
	return *this;
}

[[nodiscard]] constexpr tr::rgba8 tr::operator*(rgba8 lhs, rgba8 rhs) noexcept
{
	rgba8 temp{lhs};
	temp *= rhs;
	return temp;
}

//

constexpr tr::rgbaf::rgbaf(float r, float g, float b, float a) noexcept
	: r{r}
	, g{g}
	, b{b}
	, a{a}
{
}

constexpr tr::rgbaf::rgbaf(rgb8 rgb, float a) noexcept
	: r{norm_cast<float>(rgb.r)}
	, g{norm_cast<float>(rgb.g)}
	, b{norm_cast<float>(rgb.b)}
	, a{a}
{
}

constexpr tr::rgbaf::rgbaf(rgbf rgb, float a) noexcept
	: r{rgb.r}
	, g{rgb.g}
	, b{rgb.b}
	, a{a}
{
}

constexpr tr::rgbaf::rgbaf(rgba8 rgba) noexcept
	: r{norm_cast<float>(rgba.r)}
	, g{norm_cast<float>(rgba.g)}
	, b{norm_cast<float>(rgba.b)}
	, a{norm_cast<float>(rgba.a)}
{
}

constexpr tr::rgbaf::rgbaf(hsv hsv, float a) noexcept
	: rgbaf{rgbf{hsv}, a}
{
}

//

template <tr::arithmetic Scalar>
constexpr tr::rgbaf& tr::rgbaf::operator+=(Scalar addend) noexcept
{
	r += addend;
	g += addend;
	b += addend;
	a += addend;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgbaf tr::operator+(rgbaf lhs, Scalar addend) noexcept
{
	rgbaf temp{lhs};
	temp += addend;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgbaf& tr::rgbaf::operator-=(Scalar subtrahend) noexcept
{
	r -= subtrahend;
	g -= subtrahend;
	b -= subtrahend;
	a -= subtrahend;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgbaf tr::operator-(rgbaf lhs, Scalar subtrahend) noexcept
{
	rgbaf temp{lhs};
	temp -= subtrahend;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgbaf& tr::rgbaf::operator*=(Scalar multiplier) noexcept
{
	r *= multiplier;
	g *= multiplier;
	b *= multiplier;
	a *= multiplier;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgbaf tr::operator*(rgbaf lhs, Scalar multiplier) noexcept
{
	rgbaf temp{lhs};
	temp *= multiplier;
	return temp;
}

template <tr::arithmetic Scalar>
constexpr tr::rgbaf& tr::rgbaf::operator/=(Scalar divisor) noexcept
{
	r /= divisor;
	g /= divisor;
	b /= divisor;
	a /= divisor;
	return *this;
}

template <tr::arithmetic Scalar>
[[nodiscard]] constexpr tr::rgbaf tr::operator/(rgbaf lhs, Scalar divisor) noexcept
{
	rgbaf temp{lhs};
	temp /= divisor;
	return temp;
}

//

constexpr tr::rgbaf& tr::rgbaf::operator+=(rgbaf addend) noexcept
{
	r += addend.r;
	g += addend.g;
	b += addend.b;
	a += addend.a;
	return *this;
}

[[nodiscard]] constexpr tr::rgbaf tr::operator+(rgbaf lhs, rgbaf rhs) noexcept
{
	rgbaf temp{lhs};
	temp += rhs;
	return temp;
}

constexpr tr::rgbaf& tr::rgbaf::operator-=(rgbaf subtrahend) noexcept
{
	r -= subtrahend.r;
	g -= subtrahend.g;
	b -= subtrahend.b;
	a -= subtrahend.a;
	return *this;
}

[[nodiscard]] constexpr tr::rgbaf tr::operator-(rgbaf lhs, rgbaf rhs) noexcept
{
	rgbaf temp{lhs};
	temp -= rhs;
	return temp;
}

constexpr tr::rgbaf& tr::rgbaf::operator*=(rgbaf multiplier) noexcept
{
	r *= multiplier.r;
	g *= multiplier.g;
	b *= multiplier.b;
	a *= multiplier.a;
	return *this;
}

[[nodiscard]] constexpr tr::rgbaf tr::operator*(rgbaf lhs, rgbaf rhs) noexcept
{
	rgbaf temp{lhs};
	temp *= rhs;
	return temp;
}

//

constexpr tr::hsv::hsv(float h, float s, float v) noexcept
	: h{h}
	, s{s}
	, v{v}
{
}

constexpr tr::hsv::hsv(rgb8 rgb) noexcept
	: hsv{rgbf{rgb}}
{
}

constexpr tr::hsv::hsv(rgbf rgb) noexcept
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

constexpr tr::hsv::hsv(rgba8 rgba) noexcept
	: hsv{rgbf{rgba}}
{
}

constexpr tr::hsv::hsv(rgbaf rgba) noexcept
	: hsv{rgbf{rgba}}
{
}

//

consteval tr::rgb8 tr::literals::color_literals::operator""_rgb8(const char* str, usize length) noexcept
{
	TR_ASSERT(length == 7, "Invalid RGB literal string length {} (must be 7).", length);

	return operator""_rgba8(str, length);
}

consteval tr::rgbf tr::literals::color_literals::operator""_rgbf(const char* str, usize length) noexcept
{
	return operator""_rgb8(str, length);
}

consteval tr::rgba8 tr::literals::color_literals::operator""_rgba8(const char* str, usize length) noexcept
{
	constexpr auto is_digit{[](char chr) { return chr >= '0' && chr <= '9'; }};
	constexpr auto is_uppercase_hex{[](char chr) { return chr >= 'A' && chr <= 'F'; }};
	constexpr auto to_number{[=](char chr) -> u8 { return is_digit(chr) ? chr - '0' : chr - (is_uppercase_hex(chr) ? 'A' : 'a') + 0xA; }};

#ifdef TR_ENABLE_ASSERTS
	constexpr auto is_lowercase_hex{[](char chr) { return chr >= 'a' && chr <= 'f'; }};
	constexpr auto is_hex_digit{[=](char chr) { return is_digit(chr) || is_uppercase_hex(chr) || is_lowercase_hex(chr); }};
	TR_ASSERT(length == 7 || length == 9, "Invalid RGBA literal string length {} (must be 7 or 9).", length);
	TR_ASSERT(std::ranges::all_of(std::string_view{str + 1, length - 1}, is_hex_digit), "Invalid color literal hex string '{}'.", str);
#endif

	return {
		static_cast<u8>((to_number(str[1]) << 4) | to_number(str[2])),
		static_cast<u8>((to_number(str[3]) << 4) | to_number(str[4])),
		static_cast<u8>((to_number(str[5]) << 4) | to_number(str[6])),
		static_cast<u8>(length == 9 ? (to_number(str[7]) << 4) | to_number(str[8]) : 255),
	};
}

consteval tr::rgbaf tr::literals::color_literals::operator""_rgbaf(const char* str, usize length) noexcept
{
	return operator""_rgba8(str, length);
}