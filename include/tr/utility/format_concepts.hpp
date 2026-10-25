/// @file
/// @brief Provides formatting concepts.

#pragma once
#include <tr/utility/string_literal.hpp>

//

namespace tr
{
	/// Formattable type.
	/// @tparam Context Formatting context type.
	/// @tparam Formatter Formatter type.
	template <typename Target, typename Context,
			  typename Formatter = typename Context::template formatter_type<std::remove_const_t<Target>>>
	concept formattable_with = std::semiregular<Formatter> && requires(Formatter& f, Formatter const& cf, Target&& t, Context fc,
																	   std::basic_format_parse_context<typename Context::char_type> pc) {
		{ f.parse(pc) } -> std::same_as<typename decltype(pc)::iterator>;
		{ cf.format(t, fc) } -> std::same_as<typename Context::iterator>;
	};

	/// Formattable type.
	/// @tparam CharT Output character type.
	template <typename Target, typename CharT = char>
	concept formattable = formattable_with<std::remove_reference_t<Target>, std::basic_format_context<CharT*, CharT>>;

	/// Format string that is valid for a set of types.
	/// @tparam Args List of arguments to the formatting function.
	template <string_literal Target, typename... Args>
	concept valid_format_string_for = (formattable<Args, char> && ...) &&
									  requires { typename std::integral_constant<int, (std::format_string<Args...>(Target.data), 1)>; };
} // namespace tr