/// @file
/// @brief Provides `tr::log_level` and related functions.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// @brief Logging levels.
	enum class log_level : u8
	{
		/// @brief Debug trace information.
		trace,

		/// @brief Debug information.
		debug,

		/// @brief Non-error information.
		info,

		/// @brief Non-error abnormality.
		warning,

		/// @brief Non-fatal error.
		error,

		/// @brief Fatal error.
		fatal,

		/// @brief Turns off all logging when used with `tr::logger().set_min_level()`.
		off
	};

	/// Compares two logging levels.
	/// @param lhs, rhs Logging levels to compare.
	/// @return Ordering of the logging levels.
	[[nodiscard]] constexpr std::strong_ordering operator<=>(log_level lhs, log_level rhs) noexcept
	{
		return std::to_underlying(lhs) <=> std::to_underlying(rhs);
	}

	/// @brief Returns the name of a logging level.
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
			return "WARN";
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

/// @brief Logging level formatter.
template <>
class std::formatter<tr::log_level>
{
  public:
	/// @brief Parses the format specification.
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

	/// @brief Formats a logging level.
	/// @tparam FormatContext Formatting context type.
	/// @param level Logging level to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	constexpr FormatContext::iterator format(tr::log_level level, FormatContext& context) const
	{
		return std::format_to(context.out(), "{}", name(level));
	}
};