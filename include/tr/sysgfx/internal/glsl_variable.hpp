/// @file
/// @brief Provides `tr::internal::glsl_variable.hpp`.

#pragma once
#include <tr/sysgfx/internal/glsl_type.hpp>

//

namespace tr::internal
{
	/// GLSL variable information.
	struct glsl_variable
	{
		// Name of the variable.
		std::string name;

		// Type of the variable.
		glsl_type type;

		// Array size of the variable, or 1 if not an array.
		int array_size;
	};
} // namespace tr::internal

//

/// GLSL variable formatter.
template <>
class std::formatter<tr::internal::glsl_variable>
{
  public:
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid GLSL variable format specification."};
		}
		return context.begin();
	}

	/// Formats a GLSL variable.
	/// @tparam FormatContext Formatting context type.
	/// @param variable GLSL variable to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::internal::glsl_variable& variable, FormatContext& context) const
	{
		context.advance_to(std::format_to(context.out(), "{}", variable.type));
		if (variable.array_size > 1) {
			context.advance_to(std::format_to(context.out(), "[{}]", variable.array_size));
		}
		return std::format_to(context.out(), " {}", variable.name);
	}
};