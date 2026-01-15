///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a logging class.                                                                                                             //
//                                                                                                                                       //
// Loggers may be inactive or active, and may output to a file in addition to std::cout. Whether a logger is active can be checked with  //
// the .active() method:                                                                                                                 //
//     - tr::logger{}.active() -> false                                                                                                  //
//     - tr::log.active() -> true by default                                                                                             //
//     - tr::log = {} -> turning the tr logger inactive                                                                                  //
//     - tr::logger log{"log"} -> active logger that outputs to std::cout with the prefix "log"                                          //
//     - tr::logger log{"log", "log.txt"} -> active logger that outputs to std::cout with the prefix "log, as well as log.txt            //
//                                                                                                                                       //
// Logging a formatted message is done with the .log() method, or the TR_LOG(logger, ...) macro that checks if the log is active before  //
// evaluating the arguments. Writing a multiline message should be doen line-by-line and by passing all but the first line to the        //
// .log_continue() method, or the TR_LOG_CONTINUE(logger, ...) macro. Most formatting is done as if by std::format, but exceptions are   //
// special and can be passed without a format string, they are formatted automatically. Messages have one of 4 severity levels           //
// (info, warning, error, fatal):                                                                                                        //
//     - tr::logger log{"log"}; TR_LOG(log, tr::severity::info, "Example {}", "message") -> [14:44:56] [log] [I] Example message         //
//     - TR_LOG(log, tr::severity::warning, "Multiline example:"); TR_LOG_CONTINUE(log, "Blah blah blah")                                //
//       -> [14:49:05] [log] [W] Multiline example:                                                                                      //
//                           --- Blah blah blah                                                                                          //
//     - TR_LOG(log, tr::severity::error, tr::out_of_memory{"bitmap allocation"})                                                        //
//       -> [14:51:02] [log] [E] Exception raised: Out of memory.                                                                        //
//                           --- Error occurred during bitmap allocation.                                                                //
//                                                                                                                                       //
// tr exposes the logger used internally as tr::log, and it may freely be redirected to file or disabled:                                //
//     - tr::log = tr::logger{"tr", "logs/tr.txt"} -> the tr logger will now output to logs/tr.txt                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "chrono.hpp"
#include "print.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Log message severity levels.
	enum class severity : char {
		info = 'I',    // Information.
		warning = 'W', // Warning.
		error = 'E',   // Non-fatal error.
		fatal = 'F'    // Fatal error.
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
	const std::string time_str{TR_FMT::format("{:02}:{:02}:{:02}", time.tm_hour, time.tm_min, time.tm_sec)};
	const std::string fmt_str{TR_FMT::format(fmt, std::forward<Args>(args)...)};

	println("[{}] [{}] [{}] {}", time_str, m_prefix, char(level), fmt_str);
	if (!m_path.empty()) {
		std::ofstream file{m_path, std::ios::app};
		if (file.is_open()) {
			println_to(file, "[{}] [{}] {}", time_str, char(level), fmt_str);
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