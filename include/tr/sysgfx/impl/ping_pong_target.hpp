/// @file
/// @brief Implements formatters for `tr::ping_pong_target`.

#pragma once
#include "../ping_pong_target.hpp"

//

/// Ping-pong target formatter.
template <>
struct std::formatter<tr::ping_pong_target>
{
	/// Parses the context.
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	/// Formats the ping-pong target.
	template <typename FormatContext>
	auto format(const tr::ping_pong_target& target, FormatContext& ctx) const
	{
		if (target.valid()) {
			return std::format_to(ctx.out(), "\"{}\"", target.label());
		}
		else {
			return std::format_to(ctx.out(), "<invalid ping-pong target at {}>", static_cast<const void*>(&target));
		}
	}
};