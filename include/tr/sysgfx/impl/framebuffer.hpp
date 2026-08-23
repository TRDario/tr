#pragma once
#include "../framebuffer.hpp"

//

/// Framebuffer formatter.
template <>
struct std::formatter<tr::framebuffer>
{
	/// Parses the context.
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	/// Formats the framebuffer.
	template <typename FormatContext>
	auto format(const tr::framebuffer& framebuffer, FormatContext& ctx) const
	{
		if (framebuffer.valid()) {
			return std::format_to(ctx.out(), "\"{}\" (GID: {})", framebuffer.label(), framebuffer.gid());
		}
		else {
			return std::format_to(ctx.out(), "<invalid framebuffer at {}>", static_cast<const void*>(&framebuffer));
		}
	}
};