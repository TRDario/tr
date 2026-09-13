/// @file
/// @brief Provides `tr::log_message`.

#pragma once
#include <tr/sysgfx/log_level.hpp>
#include <tr/utility/inplace_string.hpp>

//

namespace tr
{
	/// Structure containing logging message information.
	struct log_message
	{
		/// Maximum log message length.
		static constexpr usize max_length{255};

		//

		/// Timestamp of the message.
		std::tm timestamp;

		/// Log level of the message.
		log_level level;

		/// Name of the source of the message.
		std::string_view source;

		/// Message string.
		inplace_string<max_length> string;
	};
} // namespace tr

//

/// Logging message formatter.
/// @details
/// By default, logging messages are outputted with the source of the message indicated in the message. This may be disabled with `{:n}`.
///
/// Example format output: std::format("{}", message) -> `[15:03:12] [tr] [WARNING] This is a warning message`.
template <>
class std::formatter<tr::log_message>
{
  public:
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		typename ParseContext::iterator context_it{context.begin()};
		if (context_it == context.end()) {
			return context_it;
		}

		if (*context_it == 'n') {
			m_message_format = message_format::no_source;
			++context_it;
		}

		if (context_it != context.end() && *context_it != '}') {
			throw std::format_error{"Invalid logging message format specification."};
		}
		return context_it;
	}

	/// Formats a logging level.
	/// @tparam FormatContext Formatting context type.
	/// @param message Logging message to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::log_message& message, FormatContext& context) const
	{
		switch (m_message_format) {
		case message_format::no_source:
			return std::format_to(context.out(), "[{:02}:{:02}:{:02}] [{}] {}", message.timestamp.tm_hour, message.timestamp.tm_min,
								  message.timestamp.tm_sec, message.level, message.string);
		case message_format::with_source:
			return std::format_to(context.out(), "[{:02}:{:02}:{:02}] [{}] [{}] {}", message.timestamp.tm_hour, message.timestamp.tm_min,
								  message.timestamp.tm_sec, message.source, message.level, message.string);
		default:
			TR_UNREACHABLE;
		}
	}

  private:
	/// Message formats.
	enum class message_format : bool
	{
		/// Do not include the source of the message in the final string.
		no_source,

		/// Include the source of the message in the final string.
		with_source
	};

	/// Output format.
	message_format m_message_format{message_format::with_source};
};