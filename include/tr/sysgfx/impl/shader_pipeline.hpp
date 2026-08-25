/// @file
/// @brief Implements formatters for `tr::shader_pipeline` and `tr::owning_shader_pipeline`.

#pragma once
#include "../shader_pipeline.hpp"

//

/// Shader pipeline formatter.
template <>
struct std::formatter<tr::shader_pipeline>
{
	/// Parses the context.
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	/// Formats the pipeline.
	template <typename FormatContext>
	auto format(const tr::shader_pipeline& pipeline, FormatContext& ctx) const
	{
		if (pipeline.valid()) {
			return std::format_to(ctx.out(), "\"{}\" (OpenGL ID: {})", pipeline.label(), pipeline.unwrap());
		}
		else {
			return std::format_to(ctx.out(), "<invalid shader pipeline at {}>", static_cast<const void*>(&pipeline));
		}
	}
};

/// Owning shader pipeline formatter.
template <>
struct std::formatter<tr::owning_shader_pipeline> : public std::formatter<tr::shader_pipeline>
{
};