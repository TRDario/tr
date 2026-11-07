/// @file
/// @brief Provides localization parser errors.

#pragma once
#include <tr/utility/internal/localization/token.hpp>

//

namespace tr::internal::localization
{
	/// @brief Expected symbol error.
	struct expected_symbol
	{
		/// @brief Line the error occurred on.
		int line;

		/// @brief Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// @brief Expected closing brace error.
	struct expected_closing_brace
	{
		/// @brief Line the opening brace is on.
		int opening_brace_line;
	};

	/// @brief Extraneous closing brace error.
	struct extraneous_closing_brace
	{
		/// @brief Line the error occurred on.
		int line;
	};

	/// @brief Expected symbol or closing brace error.
	struct expected_symbol_or_closing_brace
	{
		/// @brief Line the error occurred on.
		int line;

		/// @brief Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// @brief Expected equals or opening brace error.
	struct expected_equals_or_opening_brace
	{
		/// @brief Line the error occurred on.
		int line;

		/// @brief Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// @brief Expected string error.
	struct expected_string
	{
		/// @brief Line the error occurred on.
		int line;

		/// @brief Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// @brief Concept checking for a localization parser error.
	/// @tparam Target Type to check for being a localization parser error.
	template <typename Target>
	concept parser_error = one_of<Target, expected_symbol, expected_closing_brace, extraneous_closing_brace,
								  expected_symbol_or_closing_brace, expected_equals_or_opening_brace, expected_string>;
} // namespace tr::internal::localization