#pragma once
#include "chrono.hpp"
#include "print.hpp"

namespace tr {
	// Log message severity levels.
	enum class severity : char {
		// Information.
		INFO = 'I',
		// Warning.
		WARN = 'W',
		// Non-fatal error.
		ERROR = 'E',
		// Fatal error.
		FATAL = 'F'
	};

	// Simple logger class.
	class logger {
	  public:
		// Creates an inactive logger.
		logger() = default;
		// Creates a logger that only logs to cout.
		logger(std::string_view prefix);
		// Creates a logger that logs to cout and file.
		logger(std::string_view prefix, std::filesystem::path path);

		// Gets whether the logger is active.
		bool active() const;

		// Logs a message.
		template <class... Args> void log(severity level, TR_FORMAT_STRING<Args...> fmt, Args&&... args);
		// Logs a message continuing from a previous line.
		template <class... Args> void log_continue(TR_FORMAT_STRING<Args...> fmt, Args&&... args);
		// Logs an exception.
		void log(severity level, const std::exception& err);
		// Logs an exception continuing from a previous line.
		void log_continue(const std::exception& err);

	  private:
		// The prefix of the logger.
		std::string_view m_prefix;
		// The path to the log file (or empty to not log to file).
		std::filesystem::path m_path;
	};

	// tr's default logger, can be redirected.
	inline logger log{"tr"};
} // namespace tr

// Logging macro to avoid instantiating arguments to the logging function when not needed.
#define TR_LOG(logger, ...)                                                                                                                \
	do {                                                                                                                                   \
		if (logger.active()) {                                                                                                             \
			logger.log(__VA_ARGS__);                                                                                                       \
		}                                                                                                                                  \
	} while (0)

// Logging macro to avoid instantiating arguments to the logging function when not needed.
#define TR_LOG_CONTINUE(logger, ...)                                                                                                       \
	do {                                                                                                                                   \
		if (logger.active()) {                                                                                                             \
			logger.log_continue(__VA_ARGS__);                                                                                              \
		}                                                                                                                                  \
	} while (0)

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class... Args> void tr::logger::log(severity level, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	const std::tm time{tr::localtime(std::time(nullptr))};
	const std::string time_str{TR_FMT::format("[{:02}:{:02}:{:02}] ", time.tm_hour, time.tm_min, time.tm_sec)};
	const std::string severity_str{TR_FMT::format("[{}] ", char(level))};
	const std::string fmt_str{TR_FMT::format(fmt, std::forward<Args>(args)...)};

	println("{}[{}] {}{}", time_str, m_prefix, severity_str, fmt_str);
	if (!m_path.empty()) {
		std::ofstream file{m_path, std::ios::app};
		if (file.is_open()) {
			println_to(file, "{}{}{}", time_str, severity_str, fmt_str);
		}
	}
}

template <class... Args> void tr::logger::log_continue(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	const std::string fmt_str{TR_FMT::format(fmt, std::forward<Args>(args)...)};
	const std::string fill(m_prefix.size() + 14, ' ');

	println("{}--- {}", fill, fmt_str);
	if (!m_path.empty()) {
		std::ofstream file{m_path, std::ios::app};
		if (file.is_open()) {
			println_to(file, "{}--- {}", std::string_view{fill.begin(), fill.end() - (m_prefix.size() + 3)}, fmt_str);
		}
	}
}