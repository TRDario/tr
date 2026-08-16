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
		return std::format_to(ctx.out(), "[Shader pipeline \"{}\" (ID: {})]", pipeline.label(), pipeline.gid());
	}
};

/// Owning shader pipeline formatter.
template <>
struct std::formatter<tr::owning_shader_pipeline> : public std::formatter<tr::shader_pipeline>
{
};