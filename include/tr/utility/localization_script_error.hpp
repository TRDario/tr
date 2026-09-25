/// @file
/// @brief Provides localization script errors.

#pragma once
#include <tr/utility/string_literal.hpp>
#include <tr/utility/variant.hpp>

//

namespace tr
{
	/// Token type enumerator.
	enum class localization_token_type
	{
		/// Lack of a token.
		none,

		/// Symbol token.
		symbol,

		/// String token.
		string,

		/// Equals token.
		equals,

		/// Opening brace token.
		opening_brace,

		/// Closing brace token.
		closing_brace
	};

	//

	/// Localization script unexpected backslash error.
	struct localization_script_unexpected_backslash
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;
	};

	/// Localization script unexpected slash error.
	struct localization_script_unexpected_slash
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;
	};

	/// Localization script unknown escape sequence error.
	struct localization_script_unknown_escape_sequence
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;

		/// Escaped character.
		char character;
	};

	/// Localization script unterminated multiline comment error.
	struct localization_script_unterminated_multiline_comment
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;
	};

	/// Localization script unterminated string error.
	struct localization_script_unterminated_string
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;
	};

	/// Localization script expected symbol error.
	struct localization_script_expected_symbol
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;

		/// Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// Localization script expected closing brace error.
	struct localization_script_expected_closing_brace
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;

		/// Line the opening brace is on.
		int opening_brace_line;
	};

	/// Localization script extraneous closing brace error.
	struct localization_script_extraneous_closing_brace
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;
	};

	/// Localization script expected symbol or closing brace error.
	struct localization_script_expected_symbol_or_closing_brace
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;

		/// Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// Localization script expected equals or opening brace error.
	struct localization_script_expected_equals_or_opening_brace
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;

		/// Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// Localization script expected string error.
	struct localization_script_expected_string
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;

		/// Actual parsed token type, if any.
		localization_token_type actual_type;
	};

	/// Localization script duplicate key error.
	struct localization_script_duplicate_key
	{
		/// Chunk the error occurred in.
		std::string chunk;

		/// Line the error occurred on.
		int line;

		/// Line the original key declaration is on.
		int original_line;

		/// Duplicate key name.
		std::string key;
	};

	// clang-format off
	/// Generic localization script error.
	using localization_script_error = variant<
		localization_script_unexpected_backslash,
		localization_script_unexpected_slash,
		localization_script_unknown_escape_sequence,
		localization_script_unterminated_multiline_comment,
		localization_script_unterminated_string,
		localization_script_expected_symbol,
		localization_script_expected_closing_brace,
		localization_script_extraneous_closing_brace,
		localization_script_expected_symbol_or_closing_brace,
		localization_script_expected_equals_or_opening_brace,
		localization_script_expected_string,
		localization_script_duplicate_key
	>;
	// clang-format on
} // namespace tr

//

namespace tr
{
	/// Localization error formatter mixin.
	/// @tparam Error Error type being formatted.
	/// @tparam Base Base formatter type. To qualify, a type must contain a method with the signature `FormatContext::iterator
	/// format_message(const Error& error, FormatContext& context) const`.
	template <typename Error, typename Base = std::formatter<Error>>
	struct localization_error_formatter
	{
		/// Parses the format specification.
		/// @tparam ParseContext Parsing context type.
		/// @param context Parsing context.
		/// @return Iterator to the end of the parsed specification.
		template <typename ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& context)
		{
			if (context.begin() != context.end() && *context.begin() != '}') {
				throw std::format_error{"Invalid localization script error format specification."};
			}
			return context.begin();
		}

		/// Formats a localization error.
		/// @tparam FormatContext Formatting context type.
		/// @param error Error to format.
		/// @param context Formatting context.
		/// @return Iterator to the end of the output range.
		template <typename FormatContext>
		FormatContext::iterator format(const Error& error, FormatContext& context) const
		{
			context.advance_to(std::format_to(context.out(), "{}:", error.chunk));
			context.advance_to(error.line != EOF ? std::format_to(context.out(), "{}: ", error) : std::format_to(context.out(), "<eof>: "));
			return static_cast<const Base*>(this)->format_message(error, context);
		}
	};
} // namespace tr

template <>
struct std::formatter<tr::localization_token_type>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid localization token type format specification."};
		}
		return context.begin();
	}

	/// Formats a localization token type.
	/// @tparam FormatContext Formatting context type.
	/// @param type Localization token type to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(tr::localization_token_type type, FormatContext& context) const
	{
		switch (type) {
		case tr::localization_token_type::none:
			return std::format_to(context.out(), "none");
		case tr::localization_token_type::symbol:
			return std::format_to(context.out(), "symbol");
		case tr::localization_token_type::string:
			return std::format_to(context.out(), "string");
		case tr::localization_token_type::equals:
			return std::format_to(context.out(), "'='");
		case tr::localization_token_type::opening_brace:
			return std::format_to(context.out(), "'{{'");
		case tr::localization_token_type::closing_brace:
			return std::format_to(context.out(), "'}}'");
		default:
			return std::format_to(context.out(), "<unknown>");
		}
	}
};

/// Localization script unexpected backslash error formatter.
template <>
struct std::formatter<tr::localization_script_unexpected_backslash>
	: tr::localization_error_formatter<tr::localization_script_unexpected_backslash>
{
	/// Formats a localization script unexpected backslash error message.
	/// @tparam FormatContext Formatting context type.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_unexpected_backslash&, FormatContext& context) const
	{
		return std::format_to(context.out(), "Unexpected '\\'.");
	}
};

/// Localization script unexpected slash error formatter.
template <>
struct std::formatter<tr::localization_script_unexpected_slash> : tr::localization_error_formatter<tr::localization_script_unexpected_slash>
{
	/// Formats a localization script unexpected slash error message.
	/// @tparam FormatContext Formatting context type.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_unexpected_slash&, FormatContext& context) const
	{
		return std::format_to(context.out(), "Unexpected '/'.");
	}
};

/// Localization script unexpected slash error formatter.
template <>
struct std::formatter<tr::localization_script_unknown_escape_sequence>
	: tr::localization_error_formatter<tr::localization_script_unknown_escape_sequence>
{
	/// Formats a localization script unknown escape sequence error message.
	/// @tparam FormatContext Formatting context type.
	/// @param error Error to format the error message for.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_unknown_escape_sequence& error, FormatContext& context) const
	{
		return std::format_to(context.out(), "Unknown escape sequence '\\{}'.", error.character);
	}
};

/// Localization script unterminated multiline comment error formatter.
template <>
struct std::formatter<tr::localization_script_unterminated_multiline_comment>
	: tr::localization_error_formatter<tr::localization_script_unterminated_multiline_comment>
{
	/// Formats a localization script unterminated multiline comment error message.
	/// @tparam FormatContext Formatting context type.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_unterminated_multiline_comment&, FormatContext& context) const
	{
		return std::format_to(context.out(), "Unterminated multiline comment.");
	}
};

/// Localization script unterminated string error formatter.
template <>
struct std::formatter<tr::localization_script_unterminated_string>
	: tr::localization_error_formatter<tr::localization_script_unterminated_string>
{
	/// Formats a localization script unterminated string error message.
	/// @tparam FormatContext Formatting context type.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_unterminated_string&, FormatContext& context) const
	{
		return std::format_to(context.out(), "Unterminated string.");
	}
};

/// Localization script expected symbol error formatter.
template <>
struct std::formatter<tr::localization_script_expected_symbol> : tr::localization_error_formatter<tr::localization_script_expected_symbol>
{
	/// Formats a localization script unterminated string error message.
	/// @tparam FormatContext Formatting context type.
	/// @param error Error to format the error message for.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_expected_symbol& error, FormatContext& context) const
	{
		if (error.actual_type == tr::localization_token_type::none) {
			return std::format_to(context.out(), "Expected a key/namespace symbol.");
		}
		else {
			return std::format_to(context.out(), "Unexpected {}, expected a key/namespace symbol.", error.actual_type);
		}
	}
};

/// Localization script expected closing brace error formatter.
template <>
struct std::formatter<tr::localization_script_expected_closing_brace>
	: tr::localization_error_formatter<tr::localization_script_expected_closing_brace>
{
	/// Formats a localization script expected closing brace error message.
	/// @tparam FormatContext Formatting context type.
	/// @param error Error to format the error message for.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_expected_closing_brace& error, FormatContext& context) const
	{
		return std::format_to(context.out(), "Expected a closing brace to match the brace at line {}.", error.opening_brace_line);
	}
};

/// Localization script extraneous closing brace error formatter.
template <>
struct std::formatter<tr::localization_script_extraneous_closing_brace>
	: tr::localization_error_formatter<tr::localization_script_extraneous_closing_brace>
{
	/// Formats a localization script extraneous closing brace error message.
	/// @tparam FormatContext Formatting context type.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_extraneous_closing_brace&, FormatContext& context) const
	{
		return std::format_to(context.out(), "Extraneous closing brace.");
	}
};

/// Localization script expected symbol or closing brace error formatter.
template <>
struct std::formatter<tr::localization_script_expected_symbol_or_closing_brace>
	: tr::localization_error_formatter<tr::localization_script_expected_symbol_or_closing_brace>
{
	/// Formats a localization script expected symbol or closing brace error message.
	/// @tparam FormatContext Formatting context type.
	/// @param error Error to format the error message for.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_expected_symbol_or_closing_brace& error,
										   FormatContext& context) const
	{
		if (error.actual_type == tr::localization_token_type::none) {
			return std::format_to(context.out(), "Expected a key/namespace symbol or '}}'.");
		}
		else {
			return std::format_to(context.out(), "Unexpected {}, expected a key/namespace symbol or '}}'.", error.actual_type);
		}
	}
};

/// Localization script expected equals or opening brace error formatter.
template <>
struct std::formatter<tr::localization_script_expected_equals_or_opening_brace>
	: tr::localization_error_formatter<tr::localization_script_expected_equals_or_opening_brace>
{
	/// Formats a localization script expected equals or opening brace error message.
	/// @tparam FormatContext Formatting context type.
	/// @param error Error to format the error message for.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_expected_equals_or_opening_brace& error,
										   FormatContext& context) const
	{
		if (error.actual_type == tr::localization_token_type::none) {
			return std::format_to(context.out(), "Expected '=' or '{{' after a symbol.");
		}
		else {
			return std::format_to(context.out(), "Unexpected {} after a symbol, expected '=' or '{{'.", error.actual_type);
		}
	}
};

/// Localization script expected string error formatter.
template <>
struct std::formatter<tr::localization_script_expected_string> : tr::localization_error_formatter<tr::localization_script_expected_string>
{
	/// Formats a localization script expected string error message.
	/// @tparam FormatContext Formatting context type.
	/// @param error Error to format the error message for.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_expected_string& error, FormatContext& context) const
	{
		if (error.actual_type == tr::localization_token_type::none) {
			return std::format_to(context.out(), "Expected a string after '='.");
		}
		else {
			return std::format_to(context.out(), "Unexpected {} after '=', expected a string.", error.actual_type);
		}
	}
};

/// Localization script duplicate key error formatter.
template <>
struct std::formatter<tr::localization_script_duplicate_key> : tr::localization_error_formatter<tr::localization_script_duplicate_key>
{
	/// Formats a localization script duplicate key error message.
	/// @tparam FormatContext Formatting context type.
	/// @param error Error to format the error message for.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format_message(const tr::localization_script_duplicate_key& error, FormatContext& context) const
	{
		return std::format_to(context.out(), "Duplicate key '{}' (original on line {}).", error.key, error.original_line);
	}
};

/// Localization script error formatter.
template <>
struct std::formatter<tr::localization_script_error>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid localization script error format specification."};
		}
		return context.begin();
	}

	/// Formats a localization token type.
	/// @tparam FormatContext Formatting context type.
	/// @param error Localization script error to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::localization_script_error& error, FormatContext& context) const
	{
		return error.visit([&context](const auto& error) { return std::format_to(context.out(), "{}", error); });
	}
};