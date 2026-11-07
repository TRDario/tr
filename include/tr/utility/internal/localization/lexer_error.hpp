/// @file
/// @brief Provides localization lexer errors.

#pragma once
#include <tr/utility/variant.hpp>

//

namespace tr::internal::localization
{
	/// @brief Unexpected backslash lexer error.
	struct unexpected_backslash
	{
		/// @brief Line the error occurred on.
		int line;
	};

	/// @brief Unexpected slash lexer error.
	struct unexpected_slash
	{
		/// @brief Line the error occurred on.
		int line;
	};

	/// @brief Unknown escape sequence lexer error.
	struct unknown_escape_sequence
	{
		/// @brief Line the error occurred on.
		int line;

		/// @brief Escaped character.
		char character;
	};

	/// @brief Unterminated multiline comment lexer error.
	struct unterminated_multiline_comment
	{
		/// @brief Line the error occurred on.
		int line;
	};

	/// @brief Unterminated string lexer error.
	struct unterminated_string
	{
		/// @brief Line the error occurred on.
		int line;
	};

	/// @brief Concept checking for a localization lexer error.
	/// @tparam Target Type to check for being a localization lexer error.
	template <typename Target>
	concept lexer_error = one_of<Target, unexpected_backslash, unexpected_slash, unknown_escape_sequence, unterminated_multiline_comment,
								 unterminated_string>;
} // namespace tr::internal::localization