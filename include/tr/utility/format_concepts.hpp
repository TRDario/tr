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
	template <typename T, typename Context, typename Formatter = typename Context::template formatter_type<std::remove_const_t<T>>>
	concept formattable_with = std::semiregular<Formatter> && requires(Formatter& f, Formatter const& cf, T&& t, Context fc,
																	   std::basic_format_parse_context<typename Context::char_type> pc) {
		{ f.parse(pc) } -> std::same_as<typename decltype(pc)::iterator>;
		{ cf.format(t, fc) } -> std::same_as<typename Context::iterator>;
	};

	/// Formattable type.
	/// @tparam CharT Output character type.
	template <typename T, typename CharT = char>
	concept formattable = formattable_with<std::remove_reference_t<T>, std::basic_format_context<CharT*, CharT>>;

	/// Format string that is valid for a set of types.
	/// @tparam Args List of arguments to the formatting function.
	template <string_literal Fmt, typename... Args>
	concept valid_format_string_for = (formattable<Args, char> && ...) &&
									  requires { typename std::integral_constant<int, (std::format_string<Args...>(Fmt.data), 1)>; };
} // namespace tr