/// @file
/// @brief Provides localization lexer errors.

#pragma once
#include <tr/utility/variant.hpp>

//

namespace tr::internal::localization
{
	/// Unexpected backslash lexer error.
	struct unexpected_backslash
	{
		/// Line the error occurred on.
		int line;
	};

	/// Unexpected slash lexer error.
	struct unexpected_slash
	{
		/// Line the error occurred on.
		int line;
	};

	/// Unknown escape sequence lexer error.
	struct unknown_escape_sequence
	{
		/// Line the error occurred on.
		int line;

		/// Escaped character.
		char character;
	};

	/// Unterminated multiline comment lexer error.
	struct unterminated_multiline_comment
	{
		/// Line the error occurred on.
		int line;
	};

	/// Unterminated string lexer error.
	struct unterminated_string
	{
		/// Line the error occurred on.
		int line;
	};

	/// concept denoting a localization lexer error.
	template <typename T>
	concept lexer_error =
		one_of<T, unexpected_backslash, unexpected_slash, unknown_escape_sequence, unterminated_multiline_comment, unterminated_string>;
} // namespace tr::internal::localization