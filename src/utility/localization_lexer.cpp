/// @file
/// @brief Implements internal/localization/lexer.hpp.

#include <tr/utility/internal/localization/lexer.hpp>

//

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_char(char chr)
{
	if (chr == '\n') {
		++m_line;
	}

	switch (m_state) {
	case state::matching_any:
		return handle_any_char(chr);
	case state::matching_comment:
		return handle_comment_type_char(chr);
	case state::matching_line_comment:
		return handle_line_comment_char(chr);
	case state::matching_multiline_comment:
		return handle_multiline_comment_char(chr);
	case state::matching_multiline_comment_asterisk:
		return handle_multiline_comment_asterisk_char(chr);
	case state::matching_symbol:
		return handle_symbol_char(chr);
	case state::matching_string:
		return handle_string_char(chr);
	case state::matching_string_escape_sequence:
		return handle_string_escape_sequence_char(chr);
	default:
		TR_UNREACHABLE;
	}
}

tr::internal::localization::lexer::handle_eof_result tr::internal::localization::lexer::handle_eof() noexcept
{
	switch (m_state) {
	case state::matching_any:
	case state::matching_line_comment:
		return empty_variant;
	case state::matching_comment:
		return unexpected_slash{m_line};
	case state::matching_multiline_comment:
	case state::matching_multiline_comment_asterisk:
		return unterminated_multiline_comment{m_line};
	case state::matching_symbol:
		return symbol_token{m_token_start_line, std::exchange(m_token_buffer, std::string{})};
	case state::matching_string:
	case state::matching_string_escape_sequence:
		return unterminated_string{m_line};
	default:
		TR_UNREACHABLE;
	}
}

//

namespace
{
	/// Value indicating the processed character wasn't consumed.
	constexpr bool didnt_consume_character{false};

	/// Value indicating the processed character was consumed.
	constexpr bool consumed_character{true};
} // namespace

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_any_char(char chr)
{
	switch (chr) {
	case '/':
		m_state = state::matching_comment;
		return {empty_variant, consumed_character};
	case '\\':
		return {unexpected_backslash{m_line}, consumed_character};
	case '=':
		return {equals_token{m_line}, consumed_character};
	case '{':
		return {opening_brace_token{m_line}, consumed_character};
	case '}':
		return {closing_brace_token{m_line}, consumed_character};
	case '"':
		m_state = state::matching_string;
		m_token_start_line = m_line;
		return {empty_variant, consumed_character};
	default:
		if (!std::isspace(static_cast<unsigned char>(chr)) && chr != '\0') {
			m_state = state::matching_symbol;
			m_token_start_line = m_line;
			m_token_buffer.push_back(chr);
		}
		return {empty_variant, consumed_character};
	}
}

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_comment_type_char(char chr)
{
	switch (chr) {
	case '/':
		m_state = state::matching_line_comment;
		return {empty_variant, consumed_character};
	case '*':
		m_state = state::matching_multiline_comment;
		return {empty_variant, consumed_character};
	default:
		m_state = state::matching_any;
		return {unexpected_slash{m_line}, didnt_consume_character};
	}
}

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_line_comment_char(char chr)
{
	if (chr == '\n') {
		m_state = state::matching_any;
	}
	return {empty_variant, consumed_character};
}

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_multiline_comment_char(char chr)
{
	if (chr == '*') {
		m_state = state::matching_multiline_comment_asterisk;
	}
	return {empty_variant, consumed_character};
}

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_multiline_comment_asterisk_char(char chr)
{
	switch (chr) {
	case '/':
		m_state = state::matching_any;
	case '*':
		break;
	default:
		m_state = state::matching_multiline_comment;
	}
	return {empty_variant, consumed_character};
}

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_symbol_char(char chr)
{
	switch (chr) {
	case '"':
	case '/':
	case '=':
	case '\\':
	case '{':
	case '}':
		m_state = state::matching_any;
		return {symbol_token{m_token_start_line, std::exchange(m_token_buffer, std::string{})}, didnt_consume_character};
	default:
		if (std::isspace(static_cast<unsigned char>(chr)) || chr == '\0') {
			m_state = state::matching_any;
			return {symbol_token{m_token_start_line, std::exchange(m_token_buffer, std::string{})}, consumed_character};
		}
		else {
			m_token_buffer.push_back(chr);
			return {empty_variant, consumed_character};
		}
	}
}

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_string_char(char chr)
{
	switch (chr) {
	case '"':
		m_state = state::matching_any;
		return {string_token{m_token_start_line, std::exchange(m_token_buffer, std::string{})}, consumed_character};
	case '\\':
		m_state = state::matching_string_escape_sequence;
		return {empty_variant, consumed_character};
	default:
		m_token_buffer.push_back(chr);
		return {empty_variant, consumed_character};
	}
}

tr::internal::localization::lexer::handle_char_result tr::internal::localization::lexer::handle_string_escape_sequence_char(char chr)
{
	m_state = state::matching_string;
	switch (chr) {
	case '\\':
	case '"':
		m_token_buffer.push_back(chr);
		return {empty_variant, consumed_character};
	case 'n':
		m_token_buffer.push_back('\n');
		return {empty_variant, consumed_character};
	default:
		return {unknown_escape_sequence{m_line, chr}, consumed_character};
	}
}