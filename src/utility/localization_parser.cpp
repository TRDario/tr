/// @file
/// @brief Implements internal/localization/parser.hpp.

#include <tr/utility/internal/localization/parser.hpp>

//

tr::internal::localization::parser::handle_eof_result tr::internal::localization::parser::handle_eof() noexcept
{
	handle_eof_result errors;

	switch (m_state) {
	case state::expecting_symbol_or_closing_brace:
		break;
	case state::expecting_equals_or_opening_brace:
		errors.emplace_back(std::in_place_type<expected_equals_or_opening_brace>, EOF, localization_token_type::none);
		break;
	case state::expecting_string:
		errors.emplace_back(std::in_place_type<expected_string>, EOF, localization_token_type::none);
		break;
	}

	for (const open_namespace& namespace_ : std::views::reverse(m_open_namespaces)) {
		errors.emplace_back(std::in_place_type<expected_closing_brace>, namespace_.opening_brace_line);
	}

	return errors;
}