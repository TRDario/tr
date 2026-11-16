/// @file
/// @brief Provides the `tr::logger` singleton.

#pragma once
#include <tr/sysgfx/log_level.hpp>
#include <tr/sysgfx/log_message.hpp>
#include <tr/sysgfx/log_sink.hpp>
#include <tr/utility/chrono.hpp>
#include <tr/utility/inplace_string.hpp>
#include <tr/utility/lock_free_queue.hpp>
#include <tr/utility/ranges.hpp>

//

namespace tr
{
	/// @brief Logging subsystem singleton.
	class logger
	{
	  public:
		/// @name Singleton
		/// @{

		/// @brief Returns a reference to the logger instance.
		/// @details This function is only allowed to throw on its first invocation.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Reference to the logger instance.
		static logger& instance();

		/// @}
		/// @name Logging
		/// @{

		/// @brief Returns the minimum handled log level.
		/// @return Minimum handled log level.
		[[nodiscard]] log_level min_level() const noexcept;

		/// @brief Sets the minimum handled log level.
		/// @param level Minimum handled log level.
		void set_min_level(log_level level) noexcept;

		/// @brief Logs a message to the logger.
		/// @note The logger has a fixed-size queue of messages it can handle. If the queue is full, the message is silently dropped.
		/// @tparam Args Formatting argument types.
		/// @param level Log level of the message.
		/// @param source Message source identifier.
		/// @param format_string Format string.
		/// @param args Formatting arguments.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		template <typename... Args>
		void log(log_level level, std::string_view source, std::format_string<Args...> format_string, Args&&... args)
		{
			TR_ASSERT(level < log_level::off, "Tried to log a message with an invalid log level.");

			if (level < m_min_level) {
				return;
			}

			inplace_string<log_message::max_length> message;
			std::format_to_n(std::back_inserter(message), log_message::max_length, format_string, std::forward<Args>(args)...);
			if (m_message_queue.try_emplace_back(localtime(std::time(nullptr)), level, source, message)) {
				m_sent_messages.fetch_add(1, std::memory_order_release);
				m_sent_messages.notify_one();
			}
		}

		/// @}
		/// @name Sink management
		/// @{

		/// @brief Registers a sink to the logger.
		/// @param sink Shared pointer to the sink to register.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void register_sink(std::shared_ptr<log_sink> sink);

		/// @brief Flushes all pending log messages to the registered sinks and flushes the sinks afterwards.
		/// @par Exception safety
		/// If an exception is thrown, any number of sinks may have been been flushed and left in an implementation-defined state, while the
		/// logger itself is guaranteed to remain in a valid state (basic exception guarantee).
		void flush();

		/// @brief Clears all sinks from the logger.
		void clear_sinks() noexcept;

		/// @}

	  private:
		/// @brief Minimum handled log level.
		log_level m_min_level;

		/// @brief List of sinks registered to the logger.
		std::vector<std::shared_ptr<log_sink>> m_sinks;

		/// @brief Mutex protecting the sink vector.
		std::mutex m_sink_mutex;

		/// @brief Log message queue.
		lock_free_queue<log_message, 4096> m_message_queue;

		/// @brief Number of messages sent to the message queue.
		std::atomic<usize> m_sent_messages;

		/// @brief Number of messages processed and dispatched to the sinks.
		std::atomic<usize> m_processed_messages;

		/// @brief Log message dispatcher thread.
		std::jthread m_dispatcher_thread;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs the logger.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		logger();

		/// @brief Destroys the logger.
		~logger() noexcept;

		/// @}
		/// @name Dispatcher thread functions
		/// @{

		/// @brief Dispatches a message to the registered sinks.
		/// @param message Message to dispatch.
		void dispatch_to_sinks(const log_message& message) noexcept;

		/// @brief Loop thread used by the logger thread.
		/// @param stoken Thread stop token.
		void dispatcher_thread(std::stop_token stoken) noexcept;

		/// @}
	};
} // namespace tr

/// @brief Logs a trace message.
/// @details
/// Formatting is done through `std::format`, so `fmt` must be a valid format string and all further arguments must be formattable.
///
/// This macro only has an effect if `TR_ENABLE_LOG_TRACE` is enabled.
/// @param source Message source identifier.
/// @param fmt Format string.
/// @param ... Formatting arguments.
/// @hideinitializer
#ifdef TR_ENABLE_LOG_TRACE
#define TR_LOG_TRACE(source, fmt, ...)                                                                                                     \
	do {                                                                                                                                   \
		::tr::logger& logger{::tr::logger::instance()};                                                                                    \
		if (logger.min_level() <= ::tr::log_level::trace) {                                                                                \
			logger.log(::tr::log_level::trace, source, fmt __VA_OPT__(, ) __VA_ARGS__);                                                    \
		}                                                                                                                                  \
	} while (0)
#else
#define TR_LOG_TRACE(source, fmt, ...) void(0)
#endif

/// @brief Logs a debug message.
/// @details
/// Formatting is done through `std::format`, so `fmt` must be a valid format string and all further arguments must be formattable.
///
/// This macro only has an effect if `TR_ENABLE_LOG_DEBUG` is enabled.
/// @param source Message source identifier.
/// @param fmt Format string.
/// @param ... Formatting arguments.
/// @hideinitializer
#ifdef TR_ENABLE_LOG_DEBUG
#define TR_LOG_DEBUG(source, fmt, ...)                                                                                                     \
	do {                                                                                                                                   \
		::tr::logger& logger{::tr::logger::instance()};                                                                                    \
		if (logger.min_level() <= ::tr::log_level::debug) {                                                                                \
			logger.log(::tr::log_level::debug, source, fmt __VA_OPT__(, ) __VA_ARGS__);                                                    \
		}                                                                                                                                  \
	} while (0)
#else
#define TR_LOG_DEBUG(source, fmt, ...) void(0)
#endif

/// @brief Logs information.
/// @details Formatting is done through `std::format`, so `fmt` must be a valid format string and all further arguments must be formattable.
/// @param source Message source identifier.
/// @param fmt Format string.
/// @param ... Formatting arguments.
/// @hideinitializer
#define TR_LOG_INFO(source, fmt, ...)                                                                                                      \
	do {                                                                                                                                   \
		::tr::logger& logger{::tr::logger::instance()};                                                                                    \
		if (logger.min_level() <= ::tr::log_level::info) {                                                                                 \
			logger.log(::tr::log_level::info, source, fmt __VA_OPT__(, ) __VA_ARGS__);                                                     \
		}                                                                                                                                  \
	} while (0)

/// @brief Logs a warning.
/// @details Formatting is done through `std::format`, so `fmt` must be a valid format string and all further arguments must be formattable.
/// @param source Message source identifier.
/// @param fmt Format string.
/// @param ... Formatting arguments.
/// @hideinitializer
#define TR_LOG_WARNING(source, fmt, ...)                                                                                                   \
	do {                                                                                                                                   \
		::tr::logger& logger{::tr::logger::instance()};                                                                                    \
		if (logger.min_level() <= ::tr::log_level::warning) {                                                                              \
			logger.log(::tr::log_level::warning, source, fmt __VA_OPT__(, ) __VA_ARGS__);                                                  \
		}                                                                                                                                  \
	} while (0)

/// @brief Logs an error.
/// @details Formatting is done through `std::format`, so `fmt` must be a valid format string and all further arguments must be formattable.
/// @param source Message source identifier.
/// @param fmt Format string.
/// @param ... Formatting arguments.
/// @hideinitializer
#define TR_LOG_ERROR(source, fmt, ...)                                                                                                     \
	do {                                                                                                                                   \
		::tr::logger& logger{::tr::logger::instance()};                                                                                    \
		if (logger.min_level() <= ::tr::log_level::error) {                                                                                \
			logger.log(::tr::log_level::error, source, fmt __VA_OPT__(, ) __VA_ARGS__);                                                    \
		}                                                                                                                                  \
	} while (0)

/// @brief Logs a fatal error.
/// @details Formatting is done through `std::format`, so `fmt` must be a valid format string and all further arguments must be formattable.
/// @param source Message source identifier.
/// @param fmt Format string.
/// @param ... Formatting arguments.
/// @hideinitializer
#define TR_LOG_FATAL(source, fmt, ...)                                                                                                     \
	do {                                                                                                                                   \
		::tr::logger& logger{::tr::logger::instance()};                                                                                    \
		if (logger.min_level() <= ::tr::log_level::fatal) {                                                                                \
			logger.log(::tr::log_level::fatal, source, fmt __VA_OPT__(, ) __VA_ARGS__);                                                    \
		}                                                                                                                                  \
	} while (0)
