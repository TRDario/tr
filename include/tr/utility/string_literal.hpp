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
		/// @name Constructors
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

	/// Concatenates two or more string literals into a new string literal.
	/// @tparam First, Second, Rest Raw string literals and/or `tr::string_literal` specializations.
	/// @param first, second, rest Strings to concatenate.
	/// @return New string literal stored in a `tr::string_literal` object.
	template <typename First, typename Second, typename... Rest>
	[[nodiscard]] consteval auto concatenate_string_literals(First&& first, Second&& second, Rest&&... rest) noexcept
	{
		if constexpr (sizeof...(Rest) == 0) {
			const tr::string_literal left{first};
			const tr::string_literal right{second};
			tr::string_literal<decltype(left)::size() + decltype(right)::size() + 1> concatenated{{}};
			std::ranges::copy(left.data, std::ranges::begin(concatenated.data));
			std::ranges::copy(right.data, std::ranges::begin(concatenated.data) + left.size());
			return concatenated;
		}
		else {
			return concatenate_string_literals(concatenate_string_literals(std::forward<First>(first), std::forward<Second>(second)),
											   std::forward<Rest>(rest)...);
		}
	}
} // namespace tr