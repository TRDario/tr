/// @file
/// @brief Provides `tr::internal::localization::parser`.

#pragma once
#include <tr/utility/hash_map.hpp>
#include <tr/utility/internal/localization/lexer.hpp>
#include <tr/utility/internal/localization/parser_error.hpp>
#include <tr/utility/string_pool.hpp>

//

namespace tr::internal::localization
{
	/// @brief Key-value pair produced by the parser.
	struct key_value_pair
	{
		/// @brief Localization key.
		std::string key;

		/// @brief Value associated with the localization key.
		std::string value;

		/// @brief Line the key is located on.
		int key_line;
	};

	/// @brief Localization file parser.
	class parser
	{
	  public:
		/// @brief Result of `handle_token()`.
		using handle_token_result =
			optional_result<result_values<key_value_pair>,
							result_errors<extraneous_closing_brace, expected_symbol, expected_symbol_or_closing_brace, expected_string,
										  expected_equals_or_opening_brace>>;

		/// @brief Result of handle_eof()`.
		using handle_eof_result = std::vector<variant<expected_closing_brace, expected_equals_or_opening_brace, expected_string>>;

		//

		/// @brief Constructs a localization parser.
		parser() noexcept = default;

		//

		/// @brief Handles a token.
		/// @tparam Token Localization token type.
		/// @param token Token to handle.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return A key-value pair if matched successfully, a parser error if something went wrong, or nothing otherwise.
		template <token Token>
		[[nodiscard]] handle_token_result handle_token(Token token)
		{
			switch (m_state) {
			case state::expecting_symbol_or_closing_brace:
				if constexpr (std::same_as<Token, symbol_token>) {
					m_state = state::expecting_equals_or_opening_brace;
					m_last_symbol = token;
					return empty_variant;
				}
				else if constexpr (std::same_as<Token, closing_brace_token>) {
					if (m_open_namespaces.empty()) {
						return extraneous_closing_brace{token.line};
					}
					else {
						m_open_namespaces.pop_back();
						return empty_variant;
					}
				}
				else {
					if (m_open_namespaces.empty()) {
						return expected_symbol{token.line, Token::type};
					}
					else {
						return expected_symbol_or_closing_brace{token.line, Token::type};
					}
				}
			case state::expecting_equals_or_opening_brace:
				if constexpr (std::same_as<Token, equals_token>) {
					m_state = state::expecting_string;
					return empty_variant;
				}
				else if constexpr (std::same_as<Token, opening_brace_token>) {
					m_state = state::expecting_symbol_or_closing_brace;
					m_open_namespaces.emplace_back(std::move(m_last_symbol.name), token.line);
					return empty_variant;
				}
				else {
					m_state = state::expecting_symbol_or_closing_brace;
					return expected_equals_or_opening_brace{token.line, Token::type};
				}
			case state::expecting_string:
				m_state = state::expecting_symbol_or_closing_brace;
				if constexpr (std::same_as<Token, string_token>) {
					std::string key;
					for (const open_namespace& namespace_ : m_open_namespaces) {
						std::format_to(std::back_inserter(key), "{}.", namespace_.name);
					}
					key.append(m_last_symbol.name);
					return key_value_pair{std::move(key), std::move(token.value), m_last_symbol.line};
				}
				else {
					return expected_string{token.line, Token::type};
				}
			default:
				TR_UNREACHABLE;
			}
		}

		/// @brief Handles end-of-file.
		/// @return A parser error if something went wrong, or nothing otherwise.
		[[nodiscard]] handle_eof_result handle_eof() noexcept;

	  private:
		/// @brief Parser state.
		enum class state
		{
			/// @brief Expecting an expression symbol, or a closing brace if `!m_open_namespaces.empty()`.
			expecting_symbol_or_closing_brace,

			/// @brief Expecting an expression continuation ('=' or '{').
			expecting_equals_or_opening_brace,

			/// @brief Expecting a string.
			expecting_string,
		};

		/// @brief Information about an open namespace.
		struct open_namespace
		{
			/// @brief Name of the namespace.
			std::string name;

			/// @brief Line the opening brace of the namespace is on.
			int opening_brace_line;
		};

		//

		/// @brief Current parser state.
		state m_state{state::expecting_symbol_or_closing_brace};

		/// @brief Stack of opened namespaces.
		std::vector<open_namespace> m_open_namespaces;

		/// @brief Last parsed symbol token.
		symbol_token m_last_symbol;
	};
} // namespace tr::internal::localization