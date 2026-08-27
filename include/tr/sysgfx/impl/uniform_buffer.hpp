/// @file
/// @brief Implements the templated parts of uniform_buffer.hpp.

#pragma once
#include "../../utility/specialization_of.hpp"
#include "../uniform_buffer.hpp"

//

template <typename Object>
tr::uniform_buffer<Object>::uniform_buffer(graphics_context& context)
	: basic_uniform_buffer{context, sizeof(Object)}
{
}

template <typename Object>
void tr::uniform_buffer<Object>::set(const Object& data)
{
	basic_uniform_buffer::set(as_bytes(data));
}

template <typename Object>
tr::graphics_buffer_object_map<Object> tr::uniform_buffer<Object>::map()
{
	return basic_uniform_buffer::map();
}

//

/// Uniform buffer formatter.
/// @tparam UniformBuffer Uniform buffer type.
template <typename UniformBuffer>
	requires std::same_as<UniformBuffer, tr::basic_uniform_buffer> || tr::specialization_of<UniformBuffer, tr::uniform_buffer>
struct std::formatter<UniformBuffer>
{
	/// Parses the context.
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	/// Formats the uniform buffer.
	template <typename FormatContext>
	auto format(const UniformBuffer& buffer, FormatContext& ctx) const
	{
		if (buffer.valid()) {
			return std::format_to(ctx.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(ctx.out(), "<invalid uniform buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};