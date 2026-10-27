/// @file
/// @brief Tests utility/timer.hpp.

#include <gtest/gtest.h>
#include <tr/utility/integer.hpp>
#include <tr/utility/timer.hpp>

using namespace std::chrono_literals;

//

/// Incrementable counter used by the callback of the timer.
class callback_counter
{
  public:
	/// Increments the counter.
	void increment()
	{
		/* Lock scope */ {
			std::lock_guard lock{m_mutex};
			m_timestamps.push_back(std::chrono::steady_clock::now());
		}
		m_cv.notify_one();
	}

	/// Waits for a minimum count.
	/// @param minimum_count Minimum count to wait for.
	/// @return Whether the wait was successful.
	[[nodiscard]] bool wait_for(tr::usize minimum_count)
	{
		std::unique_lock lock{m_mutex};
		return m_cv.wait_for(lock, 2s, [&] { return m_timestamps.size() >= minimum_count; });
	}

	/// Gets the list of timestamps.
	/// @return Reference to the list of timestamps.
	[[nodiscard]] const std::vector<std::chrono::steady_clock::time_point>& timestamps() const noexcept
	{
		return m_timestamps;
	}

  private:
	/// Callback counter mutex.
	std::mutex m_mutex;

	/// Callback counter condition variable.
	std::condition_variable m_cv;

	/// List of callback timestamps.
	std::vector<std::chrono::steady_clock::time_point> m_timestamps;
};

//

/// Tests that a default-constructed timer is inactive.
TEST(TimerTest, DefaultConstructsEmpty)
{
	tr::timer timer;
	EXPECT_FALSE(timer.active());
}

/// Tests a timer constructed active.
TEST(TimerTest, ConstructsActive)
{
	callback_counter counter;
	const std::chrono::steady_clock::time_point start{std::chrono::steady_clock::now()};
	tr::timer timer{10ms, [&] { counter.increment(); }};

	EXPECT_TRUE(timer.active());
	EXPECT_TRUE(counter.wait_for(3));
	const std::vector<std::chrono::steady_clock::time_point>& times{counter.timestamps()};
	ASSERT_GE(times.size(), 3);
	EXPECT_GE(times[2] - start, 25ms);
	EXPECT_LT(times[2] - start, 2s);
}

/// Tests that a timer with a non-positive interval becomes inactive.
TEST(TimerTest, NonPositiveIntervalBecomesInactive)
{
	tr::timer timer{0ns, [] {}};
	const std::chrono::steady_clock::time_point deadline{std::chrono::steady_clock::now() + 2s};
	while (timer.active() && std::chrono::steady_clock::now() < deadline) {
		std::this_thread::yield();
	}
	EXPECT_FALSE(timer.active());
}

/// Tests that a timer becomes inactive after its callback throws.
TEST(TimerTest, ThrowingCallbackBecomesInactive)
{
	tr::timer timer{1ms, [] { throw std::runtime_error{"timer callback failure"}; }};
	const std::chrono::steady_clock::time_point deadline{std::chrono::steady_clock::now() + 2s};
	while (timer.active() && std::chrono::steady_clock::now() < deadline) {
		std::this_thread::yield();
	}
	EXPECT_FALSE(timer.active());
}

/// Tests that moving the timer transfers ownership.
TEST(TimerTest, MovingTransfersOwnership)
{
	callback_counter counter;
	tr::timer timer{10ms, [&] { counter.increment(); }};
	ASSERT_TRUE(timer.active());
	ASSERT_TRUE(counter.wait_for(2));

	tr::timer timer2{std::move(timer)};
	EXPECT_FALSE(timer.active());
	ASSERT_TRUE(timer2.active());
	ASSERT_TRUE(counter.wait_for(4));

	timer = std::move(timer2);
	EXPECT_TRUE(timer.active());
	EXPECT_FALSE(timer2.active());
	EXPECT_TRUE(counter.wait_for(6));
}