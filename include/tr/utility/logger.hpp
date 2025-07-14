#pragma once
#include "exception.hpp"
#include "iostream.hpp"

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
		logger() noexcept = default;
		// Creates a logger that only logs to cout.
		logger(std::string_view prefix) noexcept;
		// Creates a logger that logs to cout and file.
		logger(std::string_view prefix, std::filesystem::path path) noexcept;

		// Gets whether the logger is active.
		bool active() const noexcept;

		// Logs a message.
		template <class... Args> void log(severity level, std::format_string<Args...> fmt, Args&&... args) noexcept;
		// Logs a message continuing from a previous line.
		template <class... Args> void log_continue(std::format_string<Args...> fmt, Args&&... args) noexcept;
		// Logs an exception.
		void log(severity level, const exception& err) noexcept;
		// Logs an exception.
		void log(severity level, const std::exception& err) noexcept;

	  private:
		// The prefix of the logger.
		std::string_view _prefix;
		// The path to the log file (or empty to not log to file).
		std::filesystem::path _path;
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

template <class... Args> void tr::logger::log(severity level, std::format_string<Args...> fmt, Args&&... args) noexcept
{
	try {
		const auto time{std::chrono::floor<std::chrono::seconds>(
			std::chrono::current_zone()->std::chrono::time_zone::to_local(std::chrono::system_clock::now()))};
		const std::string time_str{std::format("[{:%T}] ", time)};
		const std::string severity_str{std::format("[{}] ", static_cast<char>(level))};
		const std::string fmt_str{std::format(fmt, std::forward<Args>(args)...)};

		std::cout << time_str << "[" << _prefix << "] " << severity_str << fmt_str << "\n";
		if (!_path.empty()) {
			std::ofstream file{open_file_w(_path, std::ios::app)};
			file << time_str << severity_str << fmt_str << "\n";
		}
	}
	catch (...) {
		return;
	}
}

template <class... Args> void tr::logger::log_continue(std::format_string<Args...> fmt, Args&&... args) noexcept
{
	try {
		const std::string fmt_str{std::format(fmt, std::forward<Args>(args)...)};
		const std::string fill(_prefix.size() + 14, ' ');

		std::cout << fill << "--- " << fmt_str << "\n";
		if (!_path.empty()) {
			std::ofstream file{open_file_w(_path, std::ios::app)};
			file << std::string_view{fill.begin(), fill.end() - (_prefix.size() + 3)} << "--- " << fmt_str << "\n";
		}
	}
	catch (...) {
		return;
	}
}