/// @file
/// @brief Provides `tr::logger` and related utilities.

#pragma once
#include <tr/utility/common.hpp>

//

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
		virtual ~logger_backend() noexcept = default;

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
		[[nodiscard]] console_logger(std::string&& name);

		/// Destroys the logger.
		~console_logger() noexcept override;

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
		[[nodiscard]] file_logger(std::filesystem::path&& path);

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
		[[nodiscard]] console_and_file_logger(std::string&& name, std::filesystem::path&& path);

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
		[[nodiscard]] logger();

		/// Creates a logger with a backend.
		/// @param backend Logger backend to forward messages to.
		[[nodiscard]] logger(std::unique_ptr<logger_backend>&& backend);

		/// @}
		/// @name Status
		/// @{

		/// Gets whether the logger is active.
		/// @return `true` if the logger contains a backend, `false` otherwise.
		[[nodiscard]] bool active() const noexcept;

		/// Gets the backend of the logger.
		/// @return Reference to the backend of the logger.
		[[nodiscard]] const logger_backend& backend() const noexcept;

		/// Gets the backend of the logger.
		/// @return Reference to the backend of the logger.
		[[nodiscard]] logger_backend& backend() noexcept;

		/// @}
		/// @name Backend manipulation
		/// @{

		/// Clears the logger's backend.
		void clear_backend() noexcept;

		/// Replaces the logger's backend.
		/// @tparam Backend Backend to emplace into the logger.
		/// @tparam Args Types of the arguments to the backend's constructor.
		/// @param args Arguments to the backend's constructor.
		template <std::derived_from<logger_backend> Backend, typename... Args>
			requires(std::constructible_from<Backend, Args...>)
		void replace_backend_with(Args&&... args)
		{
			m_backend.reset();
			m_backend = std::make_unique<Backend>(std::forward<Args>(args)...);
		}

		/// @}
		/// @name Logging
		/// @{

		/// Logs a message.
		/// @param severity Severity of the message.
		/// @param string String of the message.
		void log(severity severity, std::string_view string);

		/// Logs an exception.
		/// @param severity Severity of the message.
		/// @param error Exception to extract a message from.
		void log(severity severity, const std::exception& error);

		/// Logs a message.
		/// @tparam Args Formatting argument types.
		/// @param severity Severity of the message.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void log(severity severity, std::format_string<Args...> fmt, Args&&... args)
		{
			log(severity, std::format(fmt, std::forward<Args>(args)...));
		}

		/// Logs a message continuing from a previous line.
		/// @param string String of the message continuation.
		void log_continue(std::string_view string);

		/// Logs an exception continuing from a previous line.
		/// @param error Exception to extract a message continuation from.
		void log_continue(const std::exception& error);

		/// Logs a message continuing from a previous line.
		/// @tparam Args Formatting argument types.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void log_continue(std::format_string<Args...> fmt, Args&&... args)
		{
			log_continue(std::format(fmt, std::forward<Args>(args)...));
		}

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
	[[nodiscard]] logger make_logger(Args&&... args)
	{
		return logger{std::make_unique<Backend>(std::forward<Args>(args)...)};
	}

	//

	/// tr's default error logger, may be redirected.
	inline logger error_logger{make_logger<console_logger>("tr")};
} // namespace tr