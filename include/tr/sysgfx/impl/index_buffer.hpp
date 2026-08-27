/// @file
/// @brief Implements the templated parts of index_buffer.hpp.

#pragma once
#include "../../utility/concepts.hpp"
#include "../index_buffer.hpp"

//

/// Index buffer formatter.
/// @tparam IndexBuffer Index buffer type.
template <typename IndexBuffer>
	requires tr::one_of<IndexBuffer, tr::static_index_buffer, tr::dyn_index_buffer>
struct std::formatter<IndexBuffer>
{
	/// Parses the context.
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	/// Formats the index buffer.
	template <typename FormatContext>
	auto format(const IndexBuffer& buffer, FormatContext& ctx) const
	{
		if (buffer.valid()) {
			return std::format_to(ctx.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(ctx.out(), "<invalid index buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};