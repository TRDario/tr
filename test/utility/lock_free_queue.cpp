/// @file
/// @brief Tests utility/lock_free_queue.hpp.

#include <gtest/gtest.h>
#include <tr/utility/lock_free_queue.hpp>

//

/// Tracked integer type used in the lock-free queue tests.
class tracked_queue_int
{
  public:
	/// Number of alive tracked integers.
	inline static tr::usize alive{0};

	//

	/// Constructs a tracked integer.
	/// @param value Value of the integer.
	[[nodiscard]] explicit tracked_queue_int(int value) noexcept
		: m_value{value}
	{
		++alive;
	}

	/// Copy-constructs a tracked integer.
	/// @param rhs Integer to copy.
	[[nodiscard]] tracked_queue_int(const tracked_queue_int& rhs) noexcept
		: m_value{rhs.m_value}
	{
		++alive;
	}

	/// Destroys a tracked integer.
	~tracked_queue_int() noexcept
	{
		--alive;
	}

	//

	/// Copy-assigns a tracked integer.
	/// @param rhs Integer to copy.
	/// @return Reference to `*this`.
	tracked_queue_int& operator=(const tracked_queue_int& rhs) noexcept
	{
		m_value = rhs.m_value;
		return *this;
	}

	//

	/// Gets the value of the tracked integer.
	/// @return Value of the tracked integer.
	[[nodiscard]] int value() const noexcept
	{
		return m_value;
	}

  private:
	/// Value of the tracked integer.
	int m_value;
};

//

/// Tests that a lock-free queue is not copyable.
static_assert(!std::copyable<tr::lock_free_queue<tracked_queue_int, 2>>, "Lock-free queue should not be copyable, but is!");

/// Tests that a lock-free queue is not movable.
static_assert(!std::movable<tr::lock_free_queue<tracked_queue_int, 2>>, "Lock-free queue should not be movable, but is!");

//

/// Tests that a default-constructed lock-free queue is empty.
TEST(LockFreeQueueTest, DefaultConstructsEmpty)
{
	tr::lock_free_queue<int, 4> queue;
	EXPECT_FALSE(queue.try_pop_front().has_value());
}

/// Tests emplacing back and popping from a lock-free queue.
TEST(LockFreeQueueTest, EmplacesBackAndPops)
{
	tr::lock_free_queue<int, 2> queue;
	EXPECT_TRUE(queue.try_emplace_back(0));
	EXPECT_TRUE(queue.try_emplace_back(1));
	EXPECT_FALSE(queue.try_emplace_back(2));
	EXPECT_EQ(queue.try_pop_front(), 0);
	EXPECT_TRUE(queue.try_emplace_back(3));
	EXPECT_EQ(queue.try_pop_front(), 1);
	EXPECT_EQ(queue.try_pop_front(), 3);
	EXPECT_FALSE(queue.try_pop_front().has_value());
}

/// Tests popping from a lock-free queue into an output parameter.
TEST(LockFreeQueueTest, PopsIntoOutputParameter)
{
	tr::lock_free_queue<int, 2> queue;
	int output{0};
	EXPECT_FALSE(queue.try_pop_front(output));
	EXPECT_TRUE(queue.try_emplace_back(42));
	EXPECT_TRUE(queue.try_pop_front(output));
	EXPECT_EQ(output, 42);
	EXPECT_FALSE(queue.try_pop_front(output));
}

/// Tests that a lock-free queue correctly handles the lifetimes of its elements.
TEST(LockFreeQueueTest, CorrectlyHandlesElementLifetimes)
{
	tracked_queue_int::alive = 0;
	/* Queue scope */ {
		tr::lock_free_queue<tracked_queue_int, 4> queue;
		EXPECT_TRUE(queue.try_emplace_back(1));
		EXPECT_TRUE(queue.try_emplace_back(2));
		EXPECT_EQ(tracked_queue_int::alive, 2);

		tracked_queue_int output{0};
		EXPECT_TRUE(queue.try_pop_front(output));
		EXPECT_EQ(output.value(), 1);
		EXPECT_EQ(tracked_queue_int::alive, 2);
	}
	EXPECT_EQ(tracked_queue_int::alive, 0);
}

/// Tests using multiple concurrent producers and consumers on a lock-free queue.
TEST(LockFreeQueueTest, SupportsConcurrentProducersAndConsumers)
{
	constexpr tr::usize thread_count{4};
	constexpr tr::usize items_per_thread{256};
	constexpr tr::usize item_count{thread_count * items_per_thread};

	tr::lock_free_queue<tr::usize, 64> queue;
	std::array<std::atomic<tr::usize>, item_count> seen{};
	std::atomic<tr::usize> consumed{0};
	std::atomic_bool producers_done{false};
	std::array<std::thread, thread_count> producers;
	std::array<std::thread, thread_count> consumers;

	for (tr::usize producer_id = 0; producer_id < thread_count; ++producer_id) {
		producers[producer_id] = std::thread{[&, producer_id] {
			for (tr::usize item_id = 0; item_id < items_per_thread; ++item_id) {
				const tr::usize value{producer_id * items_per_thread + item_id};
				while (!queue.try_emplace_back(value)) {
					std::this_thread::yield();
				}
			}
		}};
	}

	for (std::thread& consumer : consumers) {
		consumer = std::thread{[&] {
			while (!producers_done.load(std::memory_order_acquire)) {
				if (const std::optional<tr::usize> value{queue.try_pop_front()}) {
					seen[*value].fetch_add(1, std::memory_order_relaxed);
					consumed.fetch_add(1, std::memory_order_relaxed);
				}
				else {
					std::this_thread::yield();
				}
			}
			while (const std::optional<tr::usize> value{queue.try_pop_front()}) {
				seen[*value].fetch_add(1, std::memory_order_relaxed);
				consumed.fetch_add(1, std::memory_order_relaxed);
			}
		}};
	}

	for (std::thread& producer : producers) {
		producer.join();
	}
	producers_done.store(true, std::memory_order_release);
	for (std::thread& consumer : consumers) {
		consumer.join();
	}

	EXPECT_EQ(consumed, item_count);
	for (const std::atomic<tr::usize>& count : seen) {
		EXPECT_EQ(count, 1);
	}
}