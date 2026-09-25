/// @file
/// @brief Provides `tr::internal::localization::parse_localization_script`.

#pragma once
#include <tr/utility/internal/localization/parser.hpp>
#include <tr/utility/localization_script_error.hpp>

//

namespace tr::internal::localization
{
	/// Parser output visitor.
	/// @tparam ErrorOut Error output iterator type.
	template <std::output_iterator<localization_script_error> ErrorOut>
	struct parser_output_visitor
	{
		/// Error output iterator
		ErrorOut error_out;

		/// Key-value pairs.
		std::vector<key_value_pair>& pairs;

		/// Name of the parsed chunk.
		std::string_view chunk_name;

		//

		/// Handles an empty output.
		ErrorOut operator()(empty_variant_t) noexcept
		{
			return error_out;
		}

		/// Handles a key-value pair.
		/// @param pair Key-value pair to handle.
		ErrorOut operator()(key_value_pair pair)
		{
			const auto duplicate_it{std::ranges::find_if(pairs, [&](const key_value_pair& old) { return old.key == pair.key; })};
			if (duplicate_it != pairs.end()) {
				*error_out++ = localization_script_duplicate_key{std::string{chunk_name}, pair.key_line, duplicate_it->key_line,
																 std::move(pair.key)};
				return error_out;
			}
			else {
				pairs.push_back(std::move(pair));
				return error_out;
			}
		}

		/// Handles an expected symbol error.
		/// @param error Error to handle.
		ErrorOut operator()(expected_symbol error)
		{
			*error_out++ = localization_script_expected_symbol{std::string{chunk_name}, error.line, error.actual_type};
			return error_out;
		}

		/// Handles an expected closing brace error.
		/// @param error Error to handle.
		ErrorOut operator()(expected_closing_brace error)
		{
			*error_out++ = localization_script_expected_closing_brace{std::string{chunk_name}, EOF, error.opening_brace_line};
			return error_out;
		}

		/// Handles an extraneous closing brace error.
		/// @param error Error to handle.
		ErrorOut operator()(extraneous_closing_brace error)
		{
			*error_out++ = localization_script_extraneous_closing_brace{std::string{chunk_name}, error.line};
			return error_out;
		}

		/// Handles an expected symbol or closing brace error.
		/// @param error Error to handle.
		ErrorOut operator()(expected_symbol_or_closing_brace error)
		{
			*error_out++ = localization_script_expected_symbol_or_closing_brace{std::string{chunk_name}, error.line, error.actual_type};
			return error_out;
		}

		/// Handles an expected equals or opening brace error.
		/// @param error Error to handle.
		ErrorOut operator()(expected_equals_or_opening_brace error)
		{
			*error_out++ = localization_script_expected_equals_or_opening_brace{std::string{chunk_name}, error.line, error.actual_type};
			return error_out;
		}

		/// Handles an expected string error.
		/// @param error Error to handle.
		ErrorOut operator()(expected_string error)
		{
			*error_out++ = localization_script_expected_string{std::string{chunk_name}, error.line, error.actual_type};
			return error_out;
		}
	};

	/// Lexer output visitor.
	/// @tparam ErrorOut Error output iterator type.
	template <std::output_iterator<localization_script_error> ErrorOut>
	struct lexer_output_visitor
	{
		/// Error output iterator
		ErrorOut error_out;

		/// Localization parser.
		parser& parser;

		/// Key-value pairs.
		std::vector<key_value_pair>& pairs;

		/// Name of the parsed chunk.
		std::string_view chunk_name;

		//

		/// Handles an empty output.
		ErrorOut operator()(empty_variant_t) noexcept
		{
			return error_out;
		}

		/// Handles a token.
		/// @tparam Token Token type.
		/// @param token Token to handle.
		ErrorOut operator()(token auto token)
		{
			return parser.handle_token(token).visit(parser_output_visitor{error_out, pairs, chunk_name});
		}

		/// Handles an unexpected backslash error.
		/// @param error Error to handle.
		ErrorOut operator()(unexpected_backslash error)
		{
			*error_out++ = localization_script_unexpected_backslash{std::string{chunk_name}, error.line};
			return error_out;
		}

		/// Handles an unexpected slash error.
		/// @param error Error to handle.
		ErrorOut operator()(unexpected_slash error)
		{
			*error_out++ = localization_script_unexpected_slash{std::string{chunk_name}, error.line};
			return error_out;
		}

		/// Handles an unknown escape sequence error.
		/// @param error Error to handle.
		ErrorOut operator()(unknown_escape_sequence error)
		{
			*error_out++ = localization_script_unknown_escape_sequence{std::string{chunk_name}, error.line, error.character};
			return error_out;
		}

		/// Handles an unterminated multiline comment error.
		/// @param error Error to handle.
		ErrorOut operator()(unterminated_multiline_comment error)
		{
			*error_out++ = localization_script_unterminated_multiline_comment{std::string{chunk_name}, error.line};
			return error_out;
		}

		/// Handles an unterminated string error.
		/// @param error Error to handle.
		ErrorOut operator()(unterminated_string error)
		{
			*error_out++ = localization_script_unterminated_string{std::string{chunk_name}, error.line};
			return error_out;
		}
	};

	//

	/// Result of `tr::internal::parse_localization_script`.
	/// @tparam ErrorOut Error output iterator type.
	template <std::output_iterator<localization_script_error> ErrorOut>
	struct parse_localization_script_result
	{
		/// Key-value pairs.
		std::vector<std::pair<std::string, std::string>> pairs;

		/// Error output iterator.
		ErrorOut error_out;
	};

	/// Raw localization script parsing function.
	/// @tparam ErrorOut Error output iterator type.
	/// @tparam Iterator Character input iterator type.
	/// @tparam Sentinel Sentinel type for `Iterator`.
	/// @param error_out Error output iterator.
	/// @param begin Iterator to the beginning of the character range.
	/// @param end Iterator to the end of the character range.
	/// @param chunk_name Name of the parsed chunk.
	/// @return List of localization key-value pairs.
	template <std::output_iterator<localization_script_error> ErrorOut, input_iterator_to_convertible_to<char> Iterator,
			  std::sentinel_for<Iterator> Sentinel>
	[[nodiscard]] parse_localization_script_result<ErrorOut> parse_localization_script(ErrorOut error_out, Iterator begin, Sentinel end,
																					   std::string_view chunk_name)
	{
		lexer lexer;
		parser parser;
		std::vector<key_value_pair> pairs;
		for (Iterator it = begin; it != end; ++it) {
			const char character{*it};
			lexer::handle_char_result lexer_result;
			do {
				lexer_result = lexer.handle_char(character);
				error_out = std::move(lexer_result).visit(lexer_output_visitor{error_out, parser, pairs, chunk_name});
			} while (!lexer_result.consumed_character);
		}

		error_out = lexer.handle_eof().visit(lexer_output_visitor{error_out, parser, pairs, chunk_name});
		for (auto error : parser.handle_eof()) {
			error_out = error.visit(parser_output_visitor{error_out, pairs, chunk_name});
		}

		std::vector<std::pair<std::string, std::string>> out_pairs;
		for (key_value_pair& pair : pairs) {
			out_pairs.emplace_back(std::move(pair.key), std::move(pair.value));
		}
		return {std::move(out_pairs), error_out};
	}
} // namespace tr::internal::localization