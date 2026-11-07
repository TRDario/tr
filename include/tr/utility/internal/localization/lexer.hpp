/// @file
/// @brief Provides `tr::internal::localization::lexer`.

#pragma once
#include <tr/utility/internal/localization/lexer_error.hpp>
#include <tr/utility/internal/localization/token.hpp>
#include <tr/utility/optional_result.hpp>

//

namespace tr::internal::localization
{
	/// @brief Localization file lexer.
	class lexer
	{
	  public:
		/// @name Results
		/// @{

		/// @brief Result of `handle_char()`.
		struct handle_char_result
			: optional_result<result_values<symbol_token, string_token, equals_token, opening_brace_token, closing_brace_token>,
							  result_errors<unexpected_backslash, unexpected_slash, unknown_escape_sequence>>
		{
			/// @brief Whether the processed character was consumed.
			bool consumed_character;
		};

		/// @brief Result of `handle_eof()`.
		using handle_eof_result = optional_result<result_values<symbol_token>,
												  result_errors<unexpected_slash, unterminated_multiline_comment, unterminated_string>>;

		/// @}

		/// @brief Constructs a localization lexer.
		[[nodiscard]] lexer() noexcept = default;

		//

		/// @brief Handles a character.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_char(char character);

		/// @brief Handles end-of-file.
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_eof_result handle_eof() noexcept;

	  private:
		/// @brief Lexer states.
		enum class state
		{
			/// @brief The lexer is ready to match any token.
			matching_any,

			/// @brief Recieved '/', waiting for '/' or '*' to determine the type.
			matching_comment,

			/// @brief Consuming characters belonging to a line comment.
			matching_line_comment,

			/// @brief Consuming characters belonging to a multline comment.
			matching_multiline_comment,

			/// @brief Recieved '*' in a multiline comment, waiting for '/' to end the comment or something else to continue.
			matching_multiline_comment_asterisk,

			/// @brief Matching a symbol.
			matching_symbol,

			/// @brief Matching a string.
			matching_string,

			/// @brief Matching an escape sequence within a string.
			matching_string_escape_sequence,
		};

		//

		/// @brief Current lexer state.
		state m_state{state::matching_any};

		/// @brief Current line number.
		int m_line{1};

		/// @brief Starting line number of the token currently being matched.
		int m_token_start_line;

		/// @brief Buffer used to store the characters of a symbol name or string.
		std::string m_token_buffer;

		/// @name Character handling
		/// @{

		/// @brief Branch of `handle_char()` for `state::matching_any`.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_any_char(char character);

		/// @brief Branch of `handle_char()` for `state::matching_comment`.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_comment_type_char(char character);

		/// @brief Branch of `handle_char()` for `state::matching_line_comment`.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_line_comment_char(char character);

		/// @brief Branch of `handle_char()` for `state::matching_multiline_comment`.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_multiline_comment_char(char character);

		/// @brief Branch of `handle_char()` for `state::matching_multiline_comment_asterisk`.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_multiline_comment_asterisk_char(char character);

		/// @brief Branch of `handle_char()` for `state::matching_symbol`.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_symbol_char(char character);

		/// @brief Branch of `handle_char()` for `state::matching_string`.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_string_char(char character);

		/// @brief Branch of `handle_char()` for `state::matching_string_escape_sequence`.
		/// @param character Character to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A token if matched successfully, a lexer error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_char_result handle_string_escape_sequence_char(char character);

		/// @}
	};
} // namespace tr::internal::localization