/// @file
/// @brief Provides `tr::string_literal`.

#pragma once
#include "integer.hpp"

//

namespace tr
{
	/// Literal object holding a character array intended to allow passing string literals as non-type template parameters.
	/// @tparam Size Number of characters in the literal + 1 for the NUL-terminator.
	template <usize Size>
	struct string_literal
	{
		/// @name Constructors
		/// @{

		/// Constructs a template string literal from a real string literal.
		/// @param[in] str Source string literal to copy.
		consteval string_literal(const char (&str)[Size]);

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets a C-string pointing to the string literal.
		/// @return C-string pointing to the string literal.
		consteval operator const char*() const;

		/// Gets a string view to the string literal.
		/// @return String view to the string literal.
		consteval operator std::string_view() const;

		/// Gets a format string view to the string literal.
		/// @tparam Args List of arguments to the formatting function.
		/// @return Format string view to the string literal.
		template <typename... Args>
		consteval operator std::format_string<Args...>() const;

		/// @}
		/// @name Data
		/// @{

		/// Backing character array of the string.
		char data[Size];

		/// Gets the length of the literal.
		/// @return Length of the string literal.
		consteval static usize size();

		/// @}
	};

	/// Concatenates two or more string literals into a new string literal.
	/// @tparam First, Second, Rest Raw string literals and/or `tr::string_literal` specializations.
	/// @param first, second, rest Strings to concatenate.
	/// @return New string literal stored in a `tr::string_literal` object.
	template <typename First, typename Second, typename... Rest>
	consteval auto concatenate_string_literals(First&& first, Second&& second, Rest&&... rest);
} // namespace tr

#include "impl/string_literal.hpp" // IWYU pragma: export