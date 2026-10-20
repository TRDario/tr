/// @file
/// @brief Tests utility/handle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/handle.hpp>

/// Default-constructible, stateless deleter that keeps track of deletions.
struct stateless_deleter
{
	/// Number of tracked deletions.
	inline static int deleted_count{0};

	/// Last deleted value.
	inline static int last_deleted_value{0};

	//

	/// Mock deletion.
	void operator()(int value) const noexcept
	{
		++deleted_count;
		last_deleted_value = value;
	}
};

/// Non-default-constructible deleter with an integer tag.
struct tagged_deleter
{
	/// Deleter tag.
	int id;

	//

	/// Constructs a deleter.
	tagged_deleter(int id)
		: id{id}
	{
	}

	//

	/// Mock deletion.
	void operator()(int value) const noexcept {}
};

/// Handle using a stateless deleter.
using stateless_deleter_handle = tr::handle<int, 0, stateless_deleter>;

/// Handle using a tagged deleter.
using tagged_deleter_handle = tr::handle<int, 0, tagged_deleter>;

//

/// Handle text fixture.
class HandleTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		stateless_deleter::deleted_count = 0;
	}
};

//

/// Tests that a stateless deleter satisfies `tr::handle_deleter`.
static_assert(tr::handle_deleter<stateless_deleter, int>);

/// Tests that a tagged deleter satisfies `tr::handle_deleter`.
static_assert(tr::handle_deleter<tagged_deleter, int>);

/// Tests that a lambda satisfies `tr::handle_deleter`.
static_assert(tr::handle_deleter<decltype([](int) noexcept {}), int>);

/// Tests that a function pointer satisfies `tr::handle_deleter`.
static_assert(tr::handle_deleter<void (*)(int) noexcept, int>);

/// Tests that `stateless_deleter` is default-constructible.
static_assert(tr::default_constructible_handle_deleter<stateless_deleter>);

/// Tests that `tagged_deleter` is not default-constructible.
static_assert(!tr::default_constructible_handle_deleter<tagged_deleter>);

/// Tests that a function pointer is not default-constructible.
static_assert(!tr::default_constructible_handle_deleter<void (*)(int)>);

/// Tests that a stateless deleter handle only takes up the size of the value.
static_assert(sizeof(tr::handle<int, 0, stateless_deleter>) == sizeof(int));

//

/// Tests that a handle default-constructs empty.
TEST_F(HandleTest, DefaultConstructsEmpty)
{
	const stateless_deleter_handle empty;
	EXPECT_FALSE(empty);
	EXPECT_FALSE(empty.has_value());
	EXPECT_EQ(empty.get(tr::maybe_empty), 0);
}

/// Tests constructing an empty handle with a deleter.
TEST_F(HandleTest, ConstructsEmptyWithDeleter)
{
	const tagged_deleter_handle empty{tagged_deleter{1}};
	EXPECT_FALSE(empty);
	EXPECT_FALSE(empty.has_value());
	EXPECT_EQ(empty.get(tr::maybe_empty), 0);
	EXPECT_EQ(empty.get_deleter().id, 1);
}

/// Tests constructing a handle with a value.
TEST_F(HandleTest, ConstructsWithValue)
{
	const stateless_deleter_handle full{1};
	EXPECT_TRUE(full);
	EXPECT_TRUE(full.has_value());
	EXPECT_EQ(full.get(), 1);
}

/// Tests constructing a handle with a value and a deleter.
TEST_F(HandleTest, ConstructsWithValueAndDeleter)
{
	const tagged_deleter_handle full{1, tagged_deleter{1}};
	EXPECT_TRUE(full);
	EXPECT_TRUE(full.has_value());
	EXPECT_EQ(full.get(), 1);
	EXPECT_EQ(full.get_deleter().id, 1);
}

/// Tests constructing a handle with `tr::maybe_empty`.
TEST_F(HandleTest, ConstructsMaybeEmpty)
{
	const stateless_deleter_handle empty{0, tr::maybe_empty};
	EXPECT_FALSE(empty);
	EXPECT_FALSE(empty.has_value());
	EXPECT_EQ(empty.get(tr::maybe_empty), 0);
}

/// Tests constructing a handle with `tr::maybe_empty` and a deleter.
TEST_F(HandleTest, ConstructsMaybeEmptyWithDeleter)
{
	const tagged_deleter_handle empty{0, tagged_deleter{1}, tr::maybe_empty};
	EXPECT_FALSE(empty);
	EXPECT_FALSE(empty.has_value());
	EXPECT_EQ(empty.get(tr::maybe_empty), 0);
	EXPECT_EQ(empty.get_deleter().id, 1);
}

/// Tests that move-constructing a handle transfers ownership.
TEST_F(HandleTest, MoveConstructorTransfersOwnership)
{
	stateless_deleter_handle handle1{5};
	const stateless_deleter_handle handle2{std::move(handle1)};
	EXPECT_FALSE(handle1.has_value());
	EXPECT_TRUE(handle2.has_value());
	EXPECT_EQ(handle2.get(), 5);
}

/// Tests that the destructor of a handle calls the deleter, but only if not empty.
TEST_F(HandleTest, DestructorCallsDeleterIfNonEmpty)
{
	{
		stateless_deleter_handle empty;
	}
	EXPECT_EQ(stateless_deleter::deleted_count, 0);

	{
		stateless_deleter_handle full{5};
	}
	EXPECT_EQ(stateless_deleter::deleted_count, 1);
	EXPECT_EQ(stateless_deleter::last_deleted_value, 5);
}

/// Tests that move-assigning a handle deletes the previous value and transfers ownership.
TEST_F(HandleTest, MoveAssignmentDeletesValueAndTransfersOwnership)
{
	stateless_deleter_handle handle1{5};
	stateless_deleter_handle handle2{10};
	handle2 = std::move(handle1);
	EXPECT_FALSE(handle1.has_value());
	EXPECT_TRUE(handle2.has_value());
	EXPECT_EQ(handle2.get(), 5);
	EXPECT_EQ(stateless_deleter::deleted_count, 1);
	EXPECT_EQ(stateless_deleter::last_deleted_value, 10);
}

/// Tests that releasing a handle returns the value without deleting it and empties the handle.
TEST_F(HandleTest, ReleaseReturnsValueWithoutDeletingAndEmpties)
{
	stateless_deleter_handle handle{5};
	int released_value{handle.release()};
	EXPECT_FALSE(handle.has_value());
	EXPECT_EQ(stateless_deleter::deleted_count, 0);
	EXPECT_EQ(released_value, 5);
}

/// Tests that resetting a handle calls the deleter and empties the handle.
TEST_F(HandleTest, ResetCallsDeleterAndEmpties)
{
	stateless_deleter_handle handle{5};

	handle.reset(6);
	EXPECT_TRUE(handle.has_value());
	EXPECT_EQ(handle.get(), 6);
	EXPECT_EQ(stateless_deleter::deleted_count, 1);

	handle.reset();
	EXPECT_FALSE(handle.has_value());
	EXPECT_EQ(stateless_deleter::deleted_count, 2);

	handle.reset(7);
	EXPECT_TRUE(handle.has_value());
	EXPECT_EQ(handle.get(), 7);
	EXPECT_EQ(stateless_deleter::deleted_count, 2);

	handle.reset(0, tr::maybe_empty);
	EXPECT_FALSE(handle.has_value());
	EXPECT_EQ(stateless_deleter::deleted_count, 3);
}