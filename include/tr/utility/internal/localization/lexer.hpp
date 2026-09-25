/// @file
/// @brief Provides `tr::internal::localization::lexer`.

#pragma once
#include <tr/utility/internal/localization/lexer_error.hpp>
#include <tr/utility/internal/localization/token.hpp>
#include <tr/utility/optional_result.hpp>

//

namespace tr::internal::localization
{
	/// Localization file lexer.
	class lexer
	{
	  public:
		/// @name Results
		/// @{

		/// Result of `handle_char()`.
		struct handle_char_result
			: optional_result<result_values<symbol_token, string_token, equals_token, opening_brace_token, closing_brace_token>,
							  result_errors<unexpected_backslash, unexpected_slash, unknown_escape_sequence>>
		{
			/// Whether the processed character was consumed.
			bool consumed_character;
		};

		/// Result of `handle_eof()`.
		using handle_eof_result = optional_result<result_values<symbol_token>,
												  result_errors<unexpected_slash, unterminated_multiline_comment, unterminated_string>>;

		/// @}

		/// Constructs a localization lexer.
		[[nodiscard]] lexer() noexcept = default;

		//

		/// Handles a character.
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_char(char chr);

		/// Handles EOF.
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_eof_result handle_eof() noexcept;

	  private:
		/// Lexer states.
		enum class state
		{
			/// The lexer is ready to match any token.
			matching_any,

			/// Recieved '/', waiting for '/' or '*' to determine the type.
			matching_comment,

			/// Consuming characters belonging to a line comment.
			matching_line_comment,

			/// Consuming characters belonging to a multline comment.
			matching_multiline_comment,

			/// Recieved '*' in a multiline comment, waiting for '/' to end the comment or something else to continue.
			matching_multiline_comment_asterisk,

			/// Matching a symbol.
			matching_symbol,

			/// Matching a string.
			matching_string,

			/// Matching an escape sequence within a string.
			matching_string_escape_sequence,
		};

		//

		/// Current lexer state.
		state m_state{state::matching_any};

		/// Current line number.
		int m_line{1};

		/// Starting line number of the token currently being matched.
		int m_token_start_line;

		/// Buffer used to store the characters of a symbol name or string.
		std::string m_token_buffer;

		//

		/// Branch of `handle_char()` for `state::matching_any`.
		[[nodiscard]] handle_char_result handle_any_char(char character);

		/// Branch of `handle_char()` for `state::matching_comment`.
		[[nodiscard]] handle_char_result handle_comment_type_char(char character);

		/// Branch of `handle_char()` for `state::matching_line_comment`.
		[[nodiscard]] handle_char_result handle_line_comment_char(char character);

		/// Branch of `handle_char()` for `state::matching_multiline_comment`.
		[[nodiscard]] handle_char_result handle_multiline_comment_char(char character);

		/// Branch of `handle_char()` for `state::matching_multiline_comment_asterisk`.
		[[nodiscard]] handle_char_result handle_multiline_comment_asterisk_char(char character);

		/// Branch of `handle_char()` for `state::matching_symbol`.
		[[nodiscard]] handle_char_result handle_symbol_char(char character);

		/// Branch of `handle_char()` for `state::matching_string`.
		[[nodiscard]] handle_char_result handle_string_char(char character);

		/// Branch of `handle_char()` for `state::matching_string_escape_sequence`.
		[[nodiscard]] handle_char_result handle_string_escape_sequence_char(char character);
	};
} // namespace tr::internal::localization