/// @file
/// @brief Tests utility/logger.hpp.

#include <gtest/gtest.h>
#include <tr/sysgfx/logger.hpp>

using namespace std::chrono_literals;

//

/// Testing logging sink that collects messages dispatched to it.
class test_sink final : public tr::log_sink
{
  public:
	/// Appends a message to the vector of collected messages.
	void log(const tr::log_message& message) override
	{
		m_messages.push_back(message);
	}

	/// No-op flush.
	void flush() noexcept override {}

	//

	/// Gets the messages that the sink collected.
	/// @return Reference to a vector of messages that the sink collected.
	const std::vector<tr::log_message> collected_messages() const noexcept
	{
		return m_messages;
	}

  private:
	/// Collected messages.
	std::vector<tr::log_message> m_messages;
};

//

/// Logger testing fixture.
class logger_test : public testing::Test
{
  protected:
	/// Testing sink.
	std::shared_ptr<test_sink> sink;

	//

	/// Constructs and registers a testing sink to the logger.
	logger_test()
		: sink{std::make_shared<test_sink>()}
	{
		tr::logger::instance().set_min_level(tr::log_level::info);
		tr::logger::instance().clear_sinks();
		tr::logger::instance().register_sink(sink);
	}
};

//

//

TEST_F(logger_test, basic_logging)
{
	TR_LOG_INFO("test", "Foo");
	TR_LOG_WARNING("test", "Bar");
	TR_LOG_ERROR("test", "Baz");
	TR_LOG_FATAL("test", "Qux");
	tr::logger::instance().flush();
	EXPECT_EQ(sink->collected_messages().size(), 4);
	EXPECT_EQ(sink->collected_messages()[0].level, tr::log_level::info);
	EXPECT_EQ(sink->collected_messages()[0].string, "Foo");
	EXPECT_EQ(sink->collected_messages()[1].level, tr::log_level::warning);
	EXPECT_EQ(sink->collected_messages()[1].string, "Bar");
	EXPECT_EQ(sink->collected_messages()[2].level, tr::log_level::error);
	EXPECT_EQ(sink->collected_messages()[2].string, "Baz");
	EXPECT_EQ(sink->collected_messages()[3].level, tr::log_level::fatal);
	EXPECT_EQ(sink->collected_messages()[3].string, "Qux");
}

TEST_F(logger_test, min_level_filtering)
{
	tr::logger::instance().set_min_level(tr::log_level::error);
	EXPECT_EQ(tr::logger::instance().min_level(), tr::log_level::error);

	TR_LOG_INFO("test", "Foo");
	TR_LOG_WARNING("test", "Bar");
	TR_LOG_ERROR("test", "Baz");
	TR_LOG_FATAL("test", "Qux");
	tr::logger::instance().flush();
	EXPECT_EQ(sink->collected_messages().size(), 2);
	EXPECT_EQ(sink->collected_messages()[0].level, tr::log_level::error);
	EXPECT_EQ(sink->collected_messages()[0].string, "Baz");
	EXPECT_EQ(sink->collected_messages()[1].level, tr::log_level::fatal);
	EXPECT_EQ(sink->collected_messages()[1].string, "Qux");
}

TEST_F(logger_test, multiple_threads_logging)
{
	// thread_count * thread_messages < 4096 (logger queue capacity) to avoid dropped messages.
	constexpr tr::usize thread_count{10};
	constexpr tr::usize thread_messages{400};

	auto test_thread = [](int id) {
		for (int i = 0; i < thread_messages; ++i) {
			TR_LOG_INFO("test", "{} {}", id, i);
		}
	};

	std::array<std::thread, thread_count> threads;
	for (tr::usize id = 0; std::thread& thread : threads) {
		thread = std::thread{test_thread, id++};
	}
	for (std::thread& thread : threads) {
		thread.join();
	}
	tr::logger::instance().flush();

	EXPECT_EQ(sink->collected_messages().size(), thread_count * thread_messages);
	std::array<bool, thread_count * thread_messages> found_messages{};
	for (const auto& message : sink->collected_messages()) {
		int thread_id;
		int message_id;
		const auto begin{message.string.data()};
		const auto end{message.string.data() + message.string.size()};
		const auto message_id_begin{std::from_chars(begin, end, thread_id).ptr + 1};
		std::from_chars(message_id_begin, end, message_id);
		found_messages[thread_id * thread_messages + message_id] = true;
	}
	EXPECT_TRUE(std::ranges::all_of(found_messages, [](bool v) { return v; }));
}