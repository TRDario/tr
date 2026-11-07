/// @file
/// @brief Provides `tr::string_literal` and related functionality.

/// @ingroup containers
/// @defgroup string_literal String literals
/// @brief Contains `tr::string_literal` and related functionality.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// @ingroup string_literal
	/// @brief Literal object holding a character array intended to allow passing string literals as non-type template parameters.
	/// @tparam Size Number of characters in the literal + 1 for the NUL-terminator.
	template <usize Size>
	struct string_literal
	{
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a template string literal from a real string literal.
		/// @param source Source string literal to copy.
		[[nodiscard]] consteval string_literal(const char (&source)[Size]) noexcept
		{
			std::ranges::copy(source, data);
		}

		/// @}
		/// @name Conversion operators
		/// @{

		/// @brief Returns a C-string pointing to the string literal.
		/// @return C-string pointing to the string literal.
		[[nodiscard]] consteval operator const char*() const noexcept
		{
			return data;
		}

		/// @brief Returns a string view to the string literal.
		/// @return String view to the string literal.
		[[nodiscard]] consteval operator std::string_view() const noexcept
		{
			return data;
		}

		/// @brief Returns a format string view to the string literal.
		/// @tparam Args List of arguments to the formatting function.
		/// @return Format string view to the string literal.
		template <typename... Args>
		[[nodiscard]] consteval operator std::format_string<Args...>() const noexcept
		{
			return std::string_view{*this};
		}

		/// @}
		/// @name Data
		/// @{

		/// @brief NUL-terminated backing character array of the string.
		char data[Size];

		/// @brief Returns the length of the literal.
		/// @return Length of the literal in characters, excluding the NUL terminator.
		[[nodiscard]] consteval static usize size() noexcept
		{
			return Size - 1;
		}

		/// @}
	};

	//

	/// @ingroup string_literal
	/// @brief Concatenates two or more string literals into a new string literal.
	/// @tparam StringLiterals Types convertible to `tr::string_literal`.
	/// @param literals String literals to concatenate.
	/// @return New string literal stored in a `tr::string_literal` object.
	template <typename... StringLiterals>
		requires(requires(StringLiterals&& t) { string_literal{t}; } && ...)
	[[nodiscard]] consteval auto concatenate_string_literals(StringLiterals&&... literals) noexcept
	{
		char buffer[(decltype(string_literal{std::declval<StringLiterals>()})::size() + ...) + 1]{};
		char* buffer_it{std::ranges::begin(buffer)};
		(..., (buffer_it = std::ranges::copy(std::string_view{string_literal{literals}}, buffer_it).out));
		return string_literal{buffer};
	}
} // namespace tr