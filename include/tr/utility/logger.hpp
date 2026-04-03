///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a flexible logger class.                                                                                                     //
//                                                                                                                                       //
// The logging functionality provided by tr is ultimately based around the tr::logger_backend interface, as it is what ultimately does   //
// the actual logging. Backends for console, file, or combined console/file logging are provided by tr, and custom backends can be       //
// created by inhereting from tr::logger_backend and implementing .log() and .log_continue().                                            //
//                                                                                                                                       //
// tr::console_logger logs to the output console. All instances of tr::console_logger have a unique string identifier they use:          //
//     - tr::logger log{tr::make_logger<tr::console_logger>("my_log")}; log.log(tr::severity::info, "test")                              //
//       -> [18:22:45] [my_log] [I] test                                                                                                 //
//                                                                                                                                       //
// tr::file_logger logs to a file. The log file is truncated on construction, and logging appends to the file:                           //
//     - tr::logger log{tr::make_logger<tr::file_logger>(tr::sys::user_directory() / "log" / "log.txt")};                                //
//       -> creates a logger that logs to <USER DIRECTORY>/log/log.txt                                                                   //
//                                                                                                                                       //
// tr::console_and_file_logger combines the functionalities of tr::console_logger and tr::file_logger:                                   //
//     - tr::logger log{tr::make_logger<tr::console_and_file_logger>("my_log", tr::sys::user_directory() / "log" / "log.txt")};          //
//       -> creates a logger that logs to <USER DIRECTORY>/log/log.txt AND to the console under the name "my_log"                        //
//                                                                                                                                       //
// A logger may be default-constructed (in which case it will be inactive), constructed with a backend unique pointer, or constructed    //
// with tr::make_logger, which creates a backend in-place:                                                                               //
//     - tr::logger log{} -> creates an inactive logger                                                                                  //
//     - tr::logger log{std::move(backend_ptr)} -> moves a backend unique pointer into the created logger                                //
//     - tr::logger log{tr::make_logger<tr::console_logger>("log")} -> creates a logger with the tr::console_logger backend              //
//                                                                                                                                       //
// Whether a logger is active can be checked with the .active() method:                                                                  //
//     - tr::logger{}.active() -> false                                                                                                  //
//     - tr::make_logger<tr::console_logger>("test").active() -> true                                                                    //
//                                                                                                                                       //
// Logging a formatted message is done with the .log() method, or the TR_LOG(logger, ...) macro that checks if the log is active before  //
// evaluating the arguments. Writing a multiline message should be done line-by-line and by passing all but the first line to the        //
// .log_continue() method, or the TR_LOG_CONTINUE(logger, ...) macro. Most formatting is done as if by std::format, but exceptions are   //
// special and can be passed without a format string, they are formatted automatically. Messages have one of 4 severity levels           //
// (info, warning, error, fatal):                                                                                                        //
//     - TR_LOG(log, tr::severity::info, "Example {}", "message")                                                                        //
//       -> [14:44:56] [log] [I] Example message                                                                                         //
//     - TR_LOG(log, tr::severity::warning, "Multiline example:"); TR_LOG_CONTINUE(log, "Blah blah blah")                                //
//       -> [14:49:05] [log] [W] Multiline example:                                                                                      //
//                           --- Blah blah blah                                                                                          //
//     - TR_LOG(log, tr::severity::error, tr::out_of_memory{"bitmap allocation"})                                                        //
//       -> [14:51:02] [log] [E] Exception raised: Out of memory.                                                                        //
//                           --- Error occurred during bitmap allocation.                                                                //
//                                                                                                                                       //
// tr exposes the logger used internally as tr::log, and it may freely be redirected or disabled:                                        //
//     - tr::log = tr::make_logger<tr::console_and_file_logger>("tr", "logs/tr.txt") -> the tr logger will now output to logs/tr.txt     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////// LOGGER /////////////////////////////////////////////////////////////////

namespace tr {
	// Log message severity levels.
	enum class severity : char {
		info = 'I',    // Information.
		warning = 'W', // Warning.
		error = 'E',   // Non-fatal error.
		fatal = 'F'    // Fatal error.
	};

	// Logger backend interface.
	class logger_backend {
	  public:
		// Virtual destructor.
		virtual ~logger_backend() = default;

		// Logs a message or message beginning.
		virtual void log(const std::tm& time, severity severity, std::string_view string) = 0;
		// Logs a message continuation.
		virtual void log_continue(std::string_view string) = 0;
	};
	// Console logger backend.
	class console_logger : public logger_backend {
	  public:
		// Creates a console logger.
		console_logger(std::string&& name);
		// Destroys the logger.
		~console_logger() override;

		// Logs a message or message beginning.
		void log(const std::tm& time, severity severity, std::string_view string) override;
		// Logs a message continuation.
		void log_continue(std::string_view string) override;

	  private:
		// List of registered console logger names.
		inline static std::vector<std::string> g_registered_console_loggers{};

		// Name of the logger.
		std::string m_name;
	};
	// File logger backend.
	class file_logger : public logger_backend {
	  public:
		// Creates a file logger.
		file_logger(std::filesystem::path&& path);

		// Logs a message or message beginning.
		void log(const std::tm& time, severity severity, std::string_view string) override;
		// Logs a message continuation.
		void log_continue(std::string_view string) override;

	  private:
		// Path to the log file.
		std::filesystem::path m_path;
	};
	// Joint console and file logger backend.
	class console_and_file_logger : public console_logger, public file_logger {
	  public:
		// Creates a console and file logger.
		console_and_file_logger(std::string&& name, std::filesystem::path&& path);

		// Logs a message or message beginning.
		void log(const std::tm& time, severity severity, std::string_view string) override;
		// Logs a message continuation.
		void log_continue(std::string_view string) override;
	};

	// Flexible logger class.
	class logger {
	  public:
		// Creates an empty logger
		logger();
		// Creates a logger with a backend.
		logger(std::unique_ptr<logger_backend>&& backend);

		// Gets whether the logger is active.
		bool active() const;

		// Logs a message.
		void log(severity severity, std::string_view str);
		// Logs an exception.
		void log(severity severity, const std::exception& err);
		// Logs a message.
		template <typename... Args> void log(severity severity, TR_FORMAT_STRING<Args...> fmt, Args&&... args);
		// Logs a message continuing from a previous line.
		void log_continue(std::string_view str);
		// Logs an exception continuing from a previous line.
		void log_continue(const std::exception& err);
		// Logs a message continuing from a previous line.
		template <typename... Args> void log_continue(TR_FORMAT_STRING<Args...> fmt, Args&&... args);

	  private:
		// Owning pointer to the logger backend.
		std::unique_ptr<logger_backend> m_backend;
	};
	// Creates a logger wtih a specific backend created in-place.
	template <std::derived_from<logger_backend> Backend, typename... Args>
		requires(std::constructible_from<Backend, Args...>)
	logger make_logger(Args&&... args);

	// tr's default logger, can be redirected.
	inline logger log{make_logger<console_logger>("tr")};
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

#include "impl/logger.hpp" // IWYU pragma: export