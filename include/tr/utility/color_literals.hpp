#pragma once
#include "color.hpp"

namespace tr::color_literals {
	// 8-bit RGBA color literal.
	constexpr rgba8 operator""_rgba8(const char* str, std::size_t length) noexcept;
} // namespace tr::color_literals

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

constexpr tr::rgba8 tr::color_literals::operator""_rgba8(const char* str, std::size_t length) noexcept
{
	TR_ASSERT(length == 6 || length == 8, "Invalid color literal string length {} (must be 6 or 8).", length);
	TR_ASSERT(std::ranges::all_of(
				  std::string_view{str, length},
				  [](char chr) { return (chr >= '0' && chr <= '9') || (chr >= 'A' && chr <= 'F') || (chr >= 'a' && chr <= 'f'); }),
			  "Invalid color literal hex string '{}'.", str);

	constexpr auto to_num{[](char chr) -> std::uint8_t {
		if (chr >= '0' && chr <= '9') {
			return chr - '0';
		}
		else if (chr >= 'A' && chr <= 'F') {
			return chr - 'A' + 0xA;
		}
		else {
			return chr - 'a' + 0xA;
		}
	}};

	return {
		static_cast<std::uint8_t>((to_num(str[0]) << 4) | to_num(str[1])),
		static_cast<std::uint8_t>((to_num(str[2]) << 4) | to_num(str[3])),
		static_cast<std::uint8_t>((to_num(str[4]) << 4) | to_num(str[5])),
		static_cast<std::uint8_t>(length == 8 ? (to_num(str[6]) << 4) | to_num(str[7]) : 255),
	};
}