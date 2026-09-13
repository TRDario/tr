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
	/// Logging subsystem singleton.
	class logger
	{
	  public:
		/// @name Instance
		/// @{

		/// Gets a reference to the logger instance.
		/// @return Reference to the logger instance.
		static logger& instance();

		/// @}
		/// @name Level
		/// @{

		/// Gets the minimum handled log level.
		/// @return Minimum handled log level.
		log_level min_level() const noexcept;

		/// Sets the minimum handled log level.
		/// @param level Minimum handled log level.
		void set_min_level(log_level level) noexcept;

		/// @}
		/// @name Sinks
		/// @{

		/// Registers a sink to the logger.
		/// @param sink Shared pointer to the sink to register.
		void register_sink(std::shared_ptr<log_sink> sink);

		/// Clears all sinks from the logger.
		void clear_sinks() noexcept;

		/// @}
		/// @name Logging
		/// @{

		/// Logs a message to the logger.
		/// @note The logger has a fixed-size queue of messages it can handle. If the queue is full, the message is silently dropped.
		/// @tparam Args Formatting argument types.
		/// @param level Log level of the message.
		/// @param source Message source identifier.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void log(log_level level, std::string_view source, std::format_string<Args...> fmt, Args&&... args)
		{
			TR_ASSERT(level < log_level::off, "Tried to log a message with an invalid log level.");

			if (level < m_min_level) {
				return;
			}

			inplace_string<log_message::max_length> message;
			std::format_to_n(std::back_inserter(message), log_message::max_length, fmt, std::forward<Args>(args)...);
			if (m_message_queue.try_emplace_back(localtime(std::time(nullptr)), level, source, message)) {
				m_sent_messages.fetch_add(1, std::memory_order_release);
				m_sent_messages.notify_one();
			}
		}

		/// Flushes all pending log messages to the registered sinks and flushes the sinks afterwards.
		void flush();

		/// @}

	  private:
		/// Minimum handled log level.
		log_level m_min_level;

		/// List of sinks registered to the logger.
		std::vector<std::shared_ptr<log_sink>> m_sinks;

		/// Mutex protecting the sink vector.
		std::mutex m_sink_mutex;

		/// Message queue.
		lock_free_queue<log_message, 4096> m_message_queue;

		/// Number of messages sent to the message queue.
		std::atomic<usize> m_sent_messages;

		/// Number of messages processed and dispatched to the sinks.
		std::atomic<usize> m_processed_messages;

		/// Log message dispatcher thread.
		std::jthread m_dispatcher_thread;

		//

		/// Constructs the logger.
		logger();

		/// Destroys the logger.
		~logger() noexcept;

		//

		/// Dispatches a message to the registered sinks.
		/// @param message Message to dispatch.
		void dispatch_to_sinks(const log_message& message) noexcept;

		/// Loop thread used by the logger thread.
		/// @param stoken Thread stop token.
		void dispatcher_thread(std::stop_token stoken) noexcept;
	};
} // namespace tr

/// Logs a trace message.
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
	}
#else
#define TR_LOG_TRACE(source, fmt, ...) void(0)
#endif

/// Logs a debug message.
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

/// Logs information.
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

/// Logs a warning.
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

/// Logs an error.
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

/// Logs a fatal error.
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