/// @file
/// @brief Implements the templated parts of shader.hpp.

#pragma once
#include "../shader.hpp"

//

template <typename Header, typename ArrayElement>
void tr::shader::set_storage_buffer(unsigned int index, shader_buffer<Header, ArrayElement>& buffer)
{
	set_storage_buffer(index, buffer.unwrap(), sizeof(Header) + sizeof(ArrayElement) * buffer.array_size());
}

template <typename Element>
void tr::shader::set_storage_buffer(unsigned int index, shader_array<Element>& buffer)
{
	set_storage_buffer(index, buffer.unwrap(), sizeof(Element) * buffer.size());
}

template <typename Object>
void tr::shader::set_uniform_buffer(unsigned int index, const uniform_buffer<Object>& buffer)
{
	set_uniform_buffer(index, buffer.unwrap());
}

//

/// Shader formatter.
template <typename Shader>
	requires tr::one_of<Shader, tr::shader, tr::vertex_shader, tr::fragment_shader>
struct std::formatter<Shader>
{
	/// Parses the context.
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	/// Formats the shader.
	template <typename FormatContext>
	auto format(const Shader& shader, FormatContext& ctx) const
	{
		if (shader.valid()) {
			return std::format_to(ctx.out(), "\"{}\" (OpenGL ID: {})", shader.label(), shader.unwrap());
		}
		else {
			return std::format_to(ctx.out(), "<invalid shader at {}>", static_cast<const void*>(&shader));
		}
	}
};