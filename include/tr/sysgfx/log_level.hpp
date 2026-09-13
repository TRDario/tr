/// @file
/// @brief Provides `tr::log_level` and related functions.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Logging levels.
	enum class log_level : u8
	{
		/// Debug trace information.
		trace,

		/// Debug information.
		debug,

		/// Non-error information.
		info,

		/// Non-error abnormality.
		warning,

		/// Non-fatal error.
		error,

		/// Fatal error.
		fatal,

		/// When used
		off
	};

	/// Compares two logging levels.
	/// @param lhs, rhs Logging levels to compare.
	/// @return Ordering of the logging levels.
	[[nodiscard]] constexpr std::strong_ordering operator<=>(log_level lhs, log_level rhs) noexcept
	{
		return std::to_underlying(lhs) <=> std::to_underlying(rhs);
	}

	/// Gets the name of a logging level.
	/// @param level Logging level to get the name of.
	/// @return View to the name of the logging level.
	[[nodiscard]] constexpr std::string_view name(log_level level) noexcept
	{
		switch (level) {
		case log_level::trace:
			return "TRACE";
		case log_level::debug:
			return "DEBUG";
		case log_level::info:
			return "INFO";
		case log_level::warning:
			return "WARNING";
		case log_level::error:
			return "ERROR";
		case log_level::fatal:
			return "FATAL";
		case log_level::off:
			return "OFF";
		default:
			return "UNKNOWN";
		}
	}
} // namespace tr

//

/// Logging level formatter.
template <>
class std::formatter<tr::log_level>
{
  public:
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid logging level format specification."};
		}
		return context.begin();
	}

	/// Formats a logging level.
	/// @tparam FormatContext Formatting context type.
	/// @param level Logging level to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	constexpr FormatContext::iterator format(tr::log_level level, FormatContext& context) const
	{
		return std::ranges::copy(name(level), context.out()).out;
	}
};