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
//     - tr::logger log{tr::make_logger<tr::file_logger>(tr::user_directory() / "log" / "log.txt")};                                     //
//       -> creates a logger that logs to <USER DIRECTORY>/log/log.txt                                                                   //
//                                                                                                                                       //
// tr::console_and_file_logger combines the functionalities of tr::console_logger and tr::file_logger:                                   //
//     - tr::logger log{tr::make_logger<tr::console_and_file_logger>("my_log", tr::user_directory() / "log" / "log.txt")};               //
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
// A logger's backend may be reset with .clear_backend(), or reset with .replace_backend_with<Backend>():                                //
//     - tr::log.clear_backend() -> clears the tr logger's backend, disabling it                                                         //
//     - tr::log.replace_backend_with<tr::console_and_file_logger>("tr", "logs/tr.log") -> replaces the tr logger's backend              //
//                                                                                                                                       //
// Logging a formatted message is done with the .log() method. Writing a multiline message should be done line-by-line and by passing    //
// all but the first line to the .log_continue() method. Most formatting is done as if by std::format, but exceptions are special and    //
// can be passed without a format string, they are formatted automatically.                                                              //
// Messages have one of 4 severity levels (info, warning, error, fatal):                                                                 //
//     - log.log(tr::severity::info, "Example {}", "message")                                                                            //
//       -> [14:44:56] [log] [I] Example message                                                                                         //
//     - log.log(tr::severity::warning, "Multiline example:"); log.log_continue(log, "Blah blah blah")                                   //
//       -> [14:49:05] [log] [W] Multiline example:                                                                                      //
//                           --- Blah blah blah                                                                                          //
//     - log.log(tr::severity::error, tr::out_of_memory{"bitmap allocation"})                                                            //
//       -> [14:51:02] [log] [E] Exception raised: Out of memory.                                                                        //
//                           --- Error occurred during bitmap allocation.                                                                //
//                                                                                                                                       //
// tr exposes the logger used internally as tr::log, and it may freely be redirected or disabled:                                        //
//     - tr::log = tr::make_logger<tr::console_and_file_logger>("tr", "logs/tr.txt") -> the tr logger will now output to logs/tr.txt     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.hpp"

////////////////////////////////////////////////////////////////// LOGGER /////////////////////////////////////////////////////////////////

namespace tr
{
	/// Log message severity levels.
	enum class severity : char
	{
		/// Information.
		info = 'I',
		/// Warning.
		warning = 'W',
		/// Non-fatal error.
		error = 'E',
		/// Fatal error.
		fatal = 'F'
	};

	//

	/// Logger backend interface.
	class logger_backend
	{
	  public:
		/// @name Constructors
		/// @{

		/// Virtual destructor.
		virtual ~logger_backend() = default;

		/// @}
		/// @name Logging
		/// @{

		/// Logs a message or message beginning.
		/// @param time Timestamp of the message.
		/// @param severity Severity of the message.
		/// @param string String of the message.
		virtual void log(const std::tm& time, severity severity, std::string_view string) = 0;

		/// Logs a message continuation.
		/// @param string String of the message continuation.
		virtual void log_continue(std::string_view string) = 0;

		/// @}
	};

	/// Console logger backend.
	class console_logger : virtual public logger_backend
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates a console logger.
		/// @param name Name of the logger.
		console_logger(std::string&& name);

		/// Destroys the logger.
		~console_logger() override;

		/// @}
		/// @name Logging
		/// @{

		/// Logs a message or message beginning.
		/// @param time Timestamp of the message.
		/// @param severity Severity of the message.
		/// @param string String of the message.
		void log(const std::tm& time, severity severity, std::string_view string) override;

		/// Logs a message continuation.
		/// @param string String of the message continuation.
		void log_continue(std::string_view string) override;

		/// @}

	  private:
		/// Name of the logger.
		std::string m_name;
	};

	/// File logger backend.
	class file_logger : virtual public logger_backend
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates a file logger.
		/// @param path Path to the log file.
		file_logger(std::filesystem::path&& path);

		/// @}
		/// @name Logging
		/// @{

		/// Logs a message or message beginning.
		/// @param time Timestamp of the message.
		/// @param severity Severity of the message.
		/// @param string String of the message.
		void log(const std::tm& time, severity severity, std::string_view string) override;

		/// Logs a message continuation.
		/// @param string String of the message continuation.
		void log_continue(std::string_view string) override;

		/// @}

	  private:
		/// Path to the log file.
		std::filesystem::path m_path;
	};

	/// Joint console and file logger backend.
	class console_and_file_logger : public console_logger, public file_logger
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates a console and file logger.
		/// @param name Name of the console logger.
		/// @param path Path to the log file.
		console_and_file_logger(std::string&& name, std::filesystem::path&& path);

		/// @}
		/// @name Logging
		/// @{

		/// Logs a message or message beginning.
		/// @param time Timestamp of the message.
		/// @param severity Severity of the message.
		/// @param string String of the message.
		void log(const std::tm& time, severity severity, std::string_view string) override;

		/// Logs a message continuation.
		/// @param string String of the message continuation.
		void log_continue(std::string_view string) override;

		/// @}
	};

	//

	/// Flexible logger class.
	class logger
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an empty logger.
		logger();

		/// Creates a logger with a backend.
		/// @param backend Logger backend to forward messages to.
		logger(std::unique_ptr<logger_backend>&& backend);

		/// @}
		/// @name Status
		/// @{

		/// Gets whether the logger is active.
		/// @return `true` if the logger contains a backend, `false` otherwise.
		bool active() const;

		/// Gets the backend of the logger.
		/// @return Reference to the backend of the logger.
		const logger_backend& backend() const;

		/// Gets the backend of the logger.
		/// @return Reference to the backend of the logger.
		logger_backend& backend();

		/// @}
		/// @name Backend manipulation
		/// @{

		/// Clears the logger's backend.
		void clear_backend();

		/// Replaces the logger's backend.
		/// @tparam Backend Backend to emplace into the logger.
		/// @tparam Args Types of the arguments to the backend's constructor.
		/// @param args Arguments to the backend's constructor.
		template <std::derived_from<logger_backend> Backend, typename... Args>
			requires(std::constructible_from<Backend, Args...>)
		void replace_backend_with(Args&&... args);

		/// @}
		/// @name Logging
		/// @{

		/// Logs a message.
		/// @param severity Severity of the message.
		/// @param string String of the message.
		void log(severity severity, std::string_view str);

		/// Logs an exception.
		/// @param severity Severity of the message.
		/// @param err Exception to extract a message from.
		void log(severity severity, const std::exception& err);

		/// Logs a message.
		/// @tparam Args Formatting argument types.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void log(severity severity, std::format_string<Args...> fmt, Args&&... args);

		/// Logs a message continuing from a previous line.
		/// @param string String of the message continuation.
		void log_continue(std::string_view str);

		/// Logs an exception continuing from a previous line.
		/// @param err Exception to extract a message continuation from.
		void log_continue(const std::exception& err);

		/// Logs a message continuing from a previous line.
		/// @tparam Args Formatting argument types.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void log_continue(std::format_string<Args...> fmt, Args&&... args);

		/// @}

	  private:
		/// Owning pointer to the logger backend.
		std::unique_ptr<logger_backend> m_backend;
	};

	/// Creates a logger with an initial backend created in-place.
	/// @tparam Backend Backend to emplace into the logger.
	/// @tparam Args Types of the arguments to the backend's constructor.
	/// @param args Arguments to the backend's constructor.
	/// @return Logger with an initial backend created in-place.
	template <std::derived_from<logger_backend> Backend, typename... Args>
		requires(std::constructible_from<Backend, Args...>)
	logger make_logger(Args&&... args);

	//

	/// tr's default error logger, may be redirected.
	inline logger error_logger{make_logger<console_logger>("tr")};
} // namespace tr

#include "impl/logger.hpp" // IWYU pragma: export