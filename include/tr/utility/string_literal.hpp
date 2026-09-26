/// @file
/// @brief Provides `tr::string_literal`.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Literal object holding a character array intended to allow passing string literals as non-type template parameters.
	/// @tparam Size Number of characters in the literal + 1 for the NUL-terminator.
	template <usize Size>
	struct string_literal
	{
		/// @name Constructors and destructors
		/// @{

		/// Constructs a template string literal from a real string literal.
		/// @param[in] str Source string literal to copy.
		[[nodiscard]] consteval string_literal(const char (&str)[Size]) noexcept
		{
			std::ranges::copy(str, data);
		}

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets a C-string pointing to the string literal.
		/// @return C-string pointing to the string literal.
		[[nodiscard]] consteval operator const char*() const noexcept
		{
			return data;
		}

		/// Gets a string view to the string literal.
		/// @return String view to the string literal.
		[[nodiscard]] consteval operator std::string_view() const noexcept
		{
			return data;
		}

		/// Gets a format string view to the string literal.
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

		/// Backing character array of the string.
		char data[Size];

		/// Gets the length of the literal.
		/// @return Length of the string literal.
		[[nodiscard]] consteval static usize size() noexcept
		{
			return Size - 1;
		}

		/// @}
	};

	//

	/// Concatenates two or more string literals into a new string literal.
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