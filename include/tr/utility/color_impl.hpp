#pragma once
#include "color.hpp"
#include "math.hpp"
#include "norm_cast.hpp"

//

constexpr tr::rgb8::rgb8(u8 r, u8 g, u8 b)
	: r{r}, g{g}, b{b}
{
}

constexpr tr::rgb8::rgb8(const rgbf& rgb)
	: r{norm_cast<u8>(rgb.r)}, g{norm_cast<u8>(rgb.g)}, b{norm_cast<u8>(rgb.b)}
{
}

constexpr tr::rgb8::rgb8(const rgba8& rgba)
	: r{rgba.r}, g{rgba.g}, b{rgba.b}
{
}

constexpr tr::rgb8::rgb8(const rgbaf& rgba)
	: r{norm_cast<u8>(rgba.r)}, g{norm_cast<u8>(rgba.g)}, b{norm_cast<u8>(rgba.b)}
{
}

constexpr tr::rgb8::rgb8(const hsv& hsv)
	: rgb8{rgbf{hsv}}
{
}

template <tr::arithmetic T> constexpr tr::rgb8& tr::rgb8::operator+=(T addend)
{
	r += addend;
	g += addend;
	b += addend;
	return *this;
}

constexpr tr::rgb8& tr::rgb8::operator+=(const rgb8& addend)
{
	r += addend.r;
	g += addend.g;
	b += addend.b;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgb8 tr::operator+(const rgb8& l, T addend)
{
	rgb8 temp{l};
	temp += addend;
	return temp;
}

constexpr tr::rgb8 tr::operator+(const rgb8& l, const rgb8& r)
{
	rgb8 temp{l};
	temp += r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgb8& tr::rgb8::operator-=(T subtrahend)
{
	r -= subtrahend;
	g -= subtrahend;
	b -= subtrahend;
	return *this;
}

constexpr tr::rgb8& tr::rgb8::operator-=(const rgb8& subtrahend)
{
	r -= subtrahend.r;
	g -= subtrahend.g;
	b -= subtrahend.b;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgb8 tr::operator-(const rgb8& l, T subtrahend)
{
	rgb8 temp{l};
	temp -= subtrahend;
	return temp;
}

constexpr tr::rgb8 tr::operator-(const rgb8& l, const rgb8& r)
{
	rgb8 temp{l};
	temp -= r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgb8& tr::rgb8::operator*=(T multiplier)
{
	r *= multiplier;
	g *= multiplier;
	b *= multiplier;
	return *this;
}

constexpr tr::rgb8& tr::rgb8::operator*=(const rgb8& multiplier)
{
	r = r * multiplier.r / 255;
	g = g * multiplier.g / 255;
	b = b * multiplier.b / 255;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgb8 tr::operator*(const rgb8& l, T multiplier)
{
	rgb8 temp{l};
	temp *= multiplier;
	return temp;
}

constexpr tr::rgb8 tr::operator*(const rgb8& l, const rgb8& r)
{
	rgb8 temp{l};
	temp *= r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgb8& tr::rgb8::operator/=(T divisor)
{
	r /= divisor;
	g /= divisor;
	b /= divisor;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgb8 tr::operator/(const rgb8& l, T divisor)
{
	rgb8 temp{l};
	temp /= divisor;
	return temp;
}

//

constexpr tr::rgbf::rgbf(float r, float g, float b)
	: r{r}, g{g}, b{b}
{
}

constexpr tr::rgbf::rgbf(const rgb8& rgb)
	: r{norm_cast<float>(rgb.r)}, g{norm_cast<float>(rgb.g)}, b{norm_cast<float>(rgb.b)}
{
}

constexpr tr::rgbf::rgbf(const rgba8& rgba)
	: r{norm_cast<float>(rgba.r)}, g{norm_cast<float>(rgba.g)}, b{norm_cast<float>(rgba.b)}
{
}

constexpr tr::rgbf::rgbf(const rgbaf& rgba)
	: r{rgba.r}, g{rgba.g}, b{rgba.b}
{
}

constexpr tr::rgbf::rgbf(const hsv& hsv)
{
	const float h{eucmod(hsv.h, 360)};
	const float c{hsv.v * hsv.s};
	const float m{hsv.v - c};
	const float x{c * (1 - abs(mod(h / 60, 2) - 1)) + m};

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

template <tr::arithmetic T> constexpr tr::rgbf& tr::rgbf::operator+=(T addend)
{
	r += addend;
	g += addend;
	b += addend;
	return *this;
}

constexpr tr::rgbf& tr::rgbf::operator+=(const rgbf& addend)
{
	r += addend.r;
	g += addend.g;
	b += addend.b;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgbf tr::operator+(const rgbf& l, T addend)
{
	rgbf temp{l};
	temp += addend;
	return temp;
}

constexpr tr::rgbf tr::operator+(const rgbf& l, const rgbf& r)
{
	rgbf temp{l};
	temp += r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgbf& tr::rgbf::operator-=(T subtrahend)
{
	r -= subtrahend;
	g -= subtrahend;
	b -= subtrahend;
	return *this;
}

constexpr tr::rgbf& tr::rgbf::operator-=(const rgbf& subtrahend)
{
	r -= subtrahend.r;
	g -= subtrahend.g;
	b -= subtrahend.b;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgbf tr::operator-(const rgbf& l, T subtrahend)
{
	rgbf temp{l};
	temp -= subtrahend;
	return temp;
}

constexpr tr::rgbf tr::operator-(const rgbf& l, const rgbf& r)
{
	rgbf temp{l};
	temp -= r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgbf& tr::rgbf::operator*=(T multiplier)
{
	r *= multiplier;
	g *= multiplier;
	b *= multiplier;
	return *this;
}

constexpr tr::rgbf& tr::rgbf::operator*=(const rgbf& multiplier)
{
	r *= multiplier.r;
	g *= multiplier.g;
	b *= multiplier.b;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgbf tr::operator*(const rgbf& l, T multiplier)
{
	rgbf temp{l};
	temp *= multiplier;
	return temp;
}

constexpr tr::rgbf tr::operator*(const rgbf& l, const rgbf& r)
{
	rgbf temp{l};
	temp *= r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgbf& tr::rgbf::operator/=(T divisor)
{
	r /= divisor;
	g /= divisor;
	b /= divisor;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgbf tr::operator/(const rgbf& l, T divisor)
{
	rgbf temp{l};
	temp /= divisor;
	return temp;
}

//

constexpr tr::rgba8::rgba8(u8 r, u8 g, u8 b, u8 a)
	: r{r}, g{g}, b{b}, a{a}
{
}

constexpr tr::rgba8::rgba8(const rgb8& rgb, u8 a)
	: r{rgb.r}, g{rgb.g}, b{rgb.b}, a{a}
{
}

constexpr tr::rgba8::rgba8(const rgbf& rgb, u8 a)
	: r{norm_cast<u8>(rgb.r)}, g{norm_cast<u8>(rgb.g)}, b{norm_cast<u8>(rgb.b)}, a{a}
{
}

constexpr tr::rgba8::rgba8(const rgbaf& rgba)
	: r{norm_cast<u8>(rgba.r)}, g{norm_cast<u8>(rgba.g)}, b{norm_cast<u8>(rgba.b)}, a{norm_cast<u8>(rgba.a)}
{
}

constexpr tr::rgba8::rgba8(const hsv& hsv, u8 a)
	: rgba8{rgbf{hsv}, a}
{
}

template <tr::arithmetic T> constexpr tr::rgba8& tr::rgba8::operator+=(T addend)
{
	r += addend;
	g += addend;
	b += addend;
	a += addend;
	return *this;
}

constexpr tr::rgba8& tr::rgba8::operator+=(const rgba8& addend)
{
	r += addend.r;
	g += addend.g;
	b += addend.b;
	a += addend.a;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgba8 tr::operator+(const rgba8& l, T addend)
{
	rgba8 temp{l};
	temp += addend;
	return temp;
}

constexpr tr::rgba8 tr::operator+(const rgba8& l, const rgba8& r)
{
	rgba8 temp{l};
	temp += r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgba8& tr::rgba8::operator-=(T subtrahend)
{
	r -= subtrahend;
	g -= subtrahend;
	b -= subtrahend;
	a -= subtrahend;
	return *this;
}

constexpr tr::rgba8& tr::rgba8::operator-=(const rgba8& subtrahend)
{
	r -= subtrahend.r;
	g -= subtrahend.g;
	b -= subtrahend.b;
	a -= subtrahend.a;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgba8 tr::operator-(const rgba8& l, T subtrahend)
{
	rgba8 temp{l};
	temp -= subtrahend;
	return temp;
}

constexpr tr::rgba8 tr::operator-(const rgba8& l, const rgba8& r)
{
	rgba8 temp{l};
	temp -= r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgba8& tr::rgba8::operator*=(T multiplier)
{
	r *= multiplier;
	g *= multiplier;
	b *= multiplier;
	a *= multiplier;
	return *this;
}

constexpr tr::rgba8& tr::rgba8::operator*=(const rgba8& multiplier)
{
	r = r * multiplier.r / 255;
	g = g * multiplier.g / 255;
	b = b * multiplier.b / 255;
	a = a * multiplier.a / 255;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgba8 tr::operator*(const rgba8& l, T multiplier)
{
	rgba8 temp{l};
	temp *= multiplier;
	return temp;
}

constexpr tr::rgba8 tr::operator*(const rgba8& l, const rgba8& r)
{
	rgba8 temp{l};
	temp *= r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgba8& tr::rgba8::operator/=(T divisor)
{
	r /= divisor;
	g /= divisor;
	b /= divisor;
	a /= divisor;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgba8 tr::operator/(const rgba8& l, T divisor)
{
	rgba8 temp{l};
	temp /= divisor;
	return temp;
}

//

constexpr tr::rgbaf::rgbaf(float r, float g, float b, float a)
	: r{r}, g{g}, b{b}, a{a}
{
}

constexpr tr::rgbaf::rgbaf(const rgb8& rgb, float a)
	: r{norm_cast<float>(rgb.r)}, g{norm_cast<float>(rgb.g)}, b{norm_cast<float>(rgb.b)}, a{a}
{
}

constexpr tr::rgbaf::rgbaf(const rgbf& rgb, float a)
	: r{rgb.r}, g{rgb.g}, b{rgb.b}, a{a}
{
}

constexpr tr::rgbaf::rgbaf(const rgba8& rgba)
	: r{norm_cast<float>(rgba.r)}, g{norm_cast<float>(rgba.g)}, b{norm_cast<float>(rgba.b)}, a{norm_cast<float>(rgba.a)}
{
}

constexpr tr::rgbaf::rgbaf(const hsv& hsv, float a)
	: rgbaf{rgbf{hsv}, a}
{
}

template <tr::arithmetic T> constexpr tr::rgbaf& tr::rgbaf::operator+=(T addend)
{
	r += addend;
	g += addend;
	b += addend;
	a += addend;
	return *this;
}

constexpr tr::rgbaf& tr::rgbaf::operator+=(const rgbaf& addend)
{
	r += addend.r;
	g += addend.g;
	b += addend.b;
	a += addend.a;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgbaf tr::operator+(const rgbaf& l, T addend)
{
	rgbaf temp{l};
	temp += addend;
	return temp;
}

constexpr tr::rgbaf tr::operator+(const rgbaf& l, const rgbaf& r)
{
	rgbaf temp{l};
	temp += r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgbaf& tr::rgbaf::operator-=(T subtrahend)
{
	r -= subtrahend;
	g -= subtrahend;
	b -= subtrahend;
	a -= subtrahend;
	return *this;
}

constexpr tr::rgbaf& tr::rgbaf::operator-=(const rgbaf& subtrahend)
{
	r -= subtrahend.r;
	g -= subtrahend.g;
	b -= subtrahend.b;
	a -= subtrahend.a;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgbaf tr::operator-(const rgbaf& l, T subtrahend)
{
	rgbaf temp{l};
	temp -= subtrahend;
	return temp;
}

constexpr tr::rgbaf tr::operator-(const rgbaf& l, const rgbaf& r)
{
	rgbaf temp{l};
	temp -= r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgbaf& tr::rgbaf::operator*=(T multiplier)
{
	r *= multiplier;
	g *= multiplier;
	b *= multiplier;
	a *= multiplier;
	return *this;
}

constexpr tr::rgbaf& tr::rgbaf::operator*=(const rgbaf& multiplier)
{
	r *= multiplier.r;
	g *= multiplier.g;
	b *= multiplier.b;
	a *= multiplier.a;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgbaf tr::operator*(const rgbaf& l, T multiplier)
{
	rgbaf temp{l};
	temp *= multiplier;
	return temp;
}

constexpr tr::rgbaf tr::operator*(const rgbaf& l, const rgbaf& r)
{
	rgbaf temp{l};
	temp *= r;
	return temp;
}

template <tr::arithmetic T> constexpr tr::rgbaf& tr::rgbaf::operator/=(T divisor)
{
	r /= divisor;
	g /= divisor;
	b /= divisor;
	a /= divisor;
	return *this;
}

template <tr::arithmetic T> constexpr tr::rgbaf tr::operator/(const rgbaf& l, T divisor)
{
	rgbaf temp{l};
	temp /= divisor;
	return temp;
}

//

constexpr tr::hsv::hsv(float h, float s, float v)
	: h{h}, s{s}, v{v}
{
}

constexpr tr::hsv::hsv(const rgb8& rgb)
	: hsv{rgbf{rgb}}
{
}

constexpr tr::hsv::hsv(const rgbf& rgb)
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

constexpr tr::hsv::hsv(const rgba8& rgba)
	: hsv{rgbf{rgba}}
{
}

constexpr tr::hsv::hsv(const rgbaf& rgba)
	: hsv{rgbf{rgba}}
{
}

//

consteval tr::rgb8 tr::literals::color_literals::operator""_rgb8(const char* str, usize length)
{
	TR_ASSERT(length == 7, "Invalid RGB literal string length {} (must be 7).", length);

	return operator""_rgba8(str, length);
}

consteval tr::rgbf tr::literals::color_literals::operator""_rgbf(const char* str, usize length)
{
	return operator""_rgb8(str, length);
}

consteval tr::rgba8 tr::literals::color_literals::operator""_rgba8(const char* str, usize length)
{
	constexpr auto is_digit{[](char chr) { return chr >= '0' && chr <= '9'; }};
	constexpr auto is_uppercase_hex{[](char chr) { return chr >= 'A' && chr <= 'F'; }};
	constexpr auto is_lowercase_hex{[](char chr) { return chr >= 'a' && chr <= 'f'; }};
	constexpr auto is_hex_digit{[=](char chr) { return is_digit(chr) || is_uppercase_hex(chr) || is_lowercase_hex(chr); }};
	constexpr auto to_num{[=](char chr) { return u8(is_digit(chr) ? chr - '0' : chr - (is_uppercase_hex(chr) ? 'A' : 'a') + 0xA); }};

	TR_ASSERT(length == 7 || length == 9, "Invalid RGBA literal string length {} (must be 7 or 9).", length);
	TR_ASSERT(std::ranges::all_of(std::string_view{str + 1, length - 1}, is_hex_digit), "Invalid color literal hex string '{}'.", str);

	return {
		u8((to_num(str[1]) << 4) | to_num(str[2])),
		u8((to_num(str[3]) << 4) | to_num(str[4])),
		u8((to_num(str[5]) << 4) | to_num(str[6])),
		u8(length == 9 ? (to_num(str[7]) << 4) | to_num(str[8]) : 255),
	};
}

consteval tr::rgbaf tr::literals::color_literals::operator""_rgbaf(const char* str, usize length)
{
	return operator""_rgba8(str, length);
}