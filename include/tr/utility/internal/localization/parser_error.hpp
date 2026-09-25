/// @file
/// @brief Provides localization parser errors.

#pragma once
#include <tr/utility/internal/localization/token.hpp>

//

namespace tr::internal::localization
{
	/// Expected symbol error.
	struct expected_symbol
	{
		/// Line the error occurred on.
		int line;

		/// Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// Expected closing brace error.
	struct expected_closing_brace
	{
		/// Line the opening brace is on.
		int opening_brace_line;
	};

	/// Extraneous closing brace error.
	struct extraneous_closing_brace
	{
		/// Line the error occurred on.
		int line;
	};

	/// Expected symbol or closing brace error.
	struct expected_symbol_or_closing_brace
	{
		/// Line the error occurred on.
		int line;

		/// Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// Expected equals or opening brace error.
	struct expected_equals_or_opening_brace
	{
		/// Line the error occurred on.
		int line;

		/// Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// Expected string error.
	struct expected_string
	{
		/// Line the error occurred on.
		int line;

		/// Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// Concept denoting a localization parser error.
	template <typename T>
	concept parser_error = one_of<T, expected_symbol, expected_closing_brace, extraneous_closing_brace, expected_symbol_or_closing_brace,
								  expected_equals_or_opening_brace, expected_string>;
} // namespace tr::internal::localization