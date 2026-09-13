/// @file
/// @brief Implements logger.hpp.

#include <tr/sysgfx/logger.hpp>

//

tr::logger::logger()
#ifdef TR_ENABLE_LOG_TRACE
	: m_min_level{log_level::trace}
#elifdef TR_ENABLE_LOG_DEBUG
	: m_min_level{log_level::debug}
#else
	: m_min_level{log_level::info}
#endif
	, m_sent_messages{}
	, m_dispatcher_thread{[this](std::stop_token stoken) { dispatcher_thread(stoken); }}
{
}

tr::logger::~logger() noexcept
{
	m_dispatcher_thread.request_stop();
	m_sent_messages.fetch_add(1, std::memory_order_release);
	m_sent_messages.notify_one();
	m_dispatcher_thread.join();
	const std::lock_guard sink_lock{m_sink_mutex};
	for (log_sink& sink : deref(m_sinks)) {
		sink.flush();
	}
}

tr::logger& tr::logger::instance()
{
	static logger logger{};
	return logger;
}

//

tr::log_level tr::logger::min_level() const noexcept
{
	return m_min_level;
}

void tr::logger::set_min_level(log_level level) noexcept
{
	m_min_level = level;
}

//

void tr::logger::register_sink(std::shared_ptr<log_sink> sink)
{
	const std::lock_guard sink_lock{m_sink_mutex};
	m_sinks.push_back(std::move(sink));
}

void tr::logger::clear_sinks() noexcept
{
	const std::lock_guard sink_lock{m_sink_mutex};
	m_sinks.clear();
}

//

void tr::logger::flush()
{
	const usize sent_messages{m_sent_messages.load(std::memory_order_acquire)};
	while (m_processed_messages.load(std::memory_order_acquire) < sent_messages) {
		std::this_thread::yield();
	}

	const std::lock_guard sink_lock{m_sink_mutex};
	for (log_sink& sink : deref(m_sinks)) {
		sink.flush();
	}
}

//

void tr::logger::dispatch_to_sinks(const log_message& message) noexcept
{
	try {
		const std::lock_guard sink_lock{m_sink_mutex};
		for (log_sink& sink : deref(m_sinks)) {
			try {
				sink.log(message);
			}
			catch (...) {
			}
		}
	}
	catch (...) {
	}
	m_processed_messages.fetch_add(1, std::memory_order_release);
}

//

void tr::logger::dispatcher_thread(std::stop_token stoken) noexcept
{
	log_message message;
	while (!stoken.stop_requested()) {
		while (m_message_queue.try_pop_front(message)) {
			dispatch_to_sinks(message);
		}

		const usize old_signal{m_sent_messages.load(std::memory_order_acquire)};
		if (m_message_queue.try_pop_front(message)) {
			dispatch_to_sinks(message);
		}
		m_sent_messages.wait(old_signal, std::memory_order_relaxed);
	}

	while (m_message_queue.try_pop_front(message)) {
		dispatch_to_sinks(message);
	}
}