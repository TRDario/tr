/// @file
/// @brief Tests utility/unique_handle.hpp.

#include <gtest/gtest.h>
#include <tr/utility/unique_handle.hpp>

/// @brief Default-constructible, stateless deleter that keeps track of deletions.
struct stateless_deleter
{
	/// @brief Number of tracked deletions.
	inline static int deleted_count{0};

	/// @brief Last deleted handle.
	inline static int last_deleted_handle{0};

	//

	/// @brief Mock deletion.
	/// @param handle Handle to "delete".
	void operator()(int handle) const noexcept
	{
		++deleted_count;
		last_deleted_handle = handle;
	}
};

/// @brief Non-default-constructible deleter with an integer tag.
struct tagged_deleter
{
	/// @brief Deleter tag.
	int id;

	//

	/// @brief Constructs a deleter.
	tagged_deleter(int id)
		: id{id}
	{
	}

	//

	/// @brief Mock deletion.
	void operator()(int) const noexcept {}
};

/// @brief Unique handle using a stateless deleter.
using stateless_unique_handle = tr::unique_handle<int, 0, stateless_deleter>;

/// @brief Unique handle using a tagged deleter.
using tagged_unique_handle = tr::unique_handle<int, 0, tagged_deleter>;

//

/// @brief Unique handle text fixture.
class UniqueHandleTest : public testing::Test
{
  protected:
	/// @brief Resets the deletion count of the stateless deleter.
	void SetUp() override
	{
		stateless_deleter::deleted_count = 0;
	}
};

//

/// @brief Tests that a unique handle with a stateless deleter only takes up the size of the value.
static_assert(sizeof(tr::unique_handle<int, 0, stateless_deleter>) == sizeof(int));

//

/// @brief Tests that a unique handle default-constructs empty.
TEST_F(UniqueHandleTest, DefaultConstructsEmpty)
{
	const stateless_unique_handle empty;
	EXPECT_FALSE(empty);
	EXPECT_EQ(empty.get(), 0);
}

/// @brief Tests constructing an empty unique handle with a copied deleter.
TEST_F(UniqueHandleTest, ConstructsEmptyWithCopiedDeleter)
{
	const tagged_deleter deleter{1};
	const tagged_unique_handle empty{deleter};
	EXPECT_FALSE(empty);
	EXPECT_EQ(empty.get(), 0);
	EXPECT_EQ(empty.get_deleter().id, 1);
}

/// @brief Tests constructing an empty unique handle with a moved deleter.
TEST_F(UniqueHandleTest, ConstructsEmptyWithMovedDeleter)
{
	const tagged_unique_handle empty{tagged_deleter{1}};
	EXPECT_FALSE(empty);
	EXPECT_EQ(empty.get(), 0);
	EXPECT_EQ(empty.get_deleter().id, 1);
}

/// @brief Tests constructing a unique handle with a value.
TEST_F(UniqueHandleTest, ConstructsWithValue)
{
	const stateless_unique_handle full{1};
	EXPECT_TRUE(full);
	EXPECT_EQ(full.get(), 1);
}

/// @brief Tests constructing a unique handle with a value and a copied deleter.
TEST_F(UniqueHandleTest, ConstructsWithValueAndCopiedDeleter)
{
	const tagged_deleter deleter{1};
	const tagged_unique_handle full{1, deleter};
	EXPECT_TRUE(full);
	EXPECT_EQ(full.get(), 1);
	EXPECT_EQ(full.get_deleter().id, 1);
}

/// @brief Tests constructing a unique handle with a value and a moved deleter.
TEST_F(UniqueHandleTest, ConstructsWithValueAndMovedDeleter)
{
	const tagged_unique_handle full{1, tagged_deleter{1}};
	EXPECT_TRUE(full);
	EXPECT_EQ(full.get(), 1);
	EXPECT_EQ(full.get_deleter().id, 1);
}

/// @brief Tests that move-constructing a unique handle transfers ownership.
TEST_F(UniqueHandleTest, MoveConstructorTransfersOwnership)
{
	stateless_unique_handle handle1{5};
	const stateless_unique_handle handle2{std::move(handle1)};
	EXPECT_FALSE(handle1);
	ASSERT_TRUE(handle2);
	EXPECT_EQ(handle2.get(), 5);
}

/// @brief Tests that the destructor of a unique handle calls the deleter, but only if not empty.
TEST_F(UniqueHandleTest, DestructorCallsDeleterIfNonEmpty)
{
	/* Empty unique handle scope */ {
		stateless_unique_handle empty;
	}
	EXPECT_EQ(stateless_deleter::deleted_count, 0);

	/* Full unique handle scope */ {
		stateless_unique_handle full{5};
	}
	EXPECT_EQ(stateless_deleter::deleted_count, 1);
	EXPECT_EQ(stateless_deleter::last_deleted_handle, 5);
}

/// @brief Tests that move-assigning a unique handle deletes the previous value and transfers ownership.
TEST_F(UniqueHandleTest, MoveAssignmentDeletesValueAndTransfersOwnership)
{
	stateless_unique_handle handle1{5};
	stateless_unique_handle handle2{10};
	handle2 = std::move(handle1);
	EXPECT_FALSE(handle1);
	ASSERT_TRUE(handle2);
	EXPECT_EQ(handle2.get(), 5);
	EXPECT_EQ(stateless_deleter::deleted_count, 1);
	EXPECT_EQ(stateless_deleter::last_deleted_handle, 10);
}

/// @brief Tests that releasing a unique handle returns the value without deleting it and empties the handle.
TEST_F(UniqueHandleTest, ReleaseReturnsValueWithoutDeletingAndEmpties)
{
	stateless_unique_handle unique_handle{5};
	const int released_handle{unique_handle.release()};
	EXPECT_FALSE(unique_handle);
	EXPECT_EQ(stateless_deleter::deleted_count, 0);
	EXPECT_EQ(released_handle, 5);
}

/// @brief Tests that resetting a unique handle calls the deleter and empties the handle.
TEST_F(UniqueHandleTest, ResetCallsDeleterAndEmpties)
{
	stateless_unique_handle handle{5};

	handle.reset(6);
	ASSERT_TRUE(handle);
	EXPECT_EQ(handle.get(), 6);
	EXPECT_EQ(stateless_deleter::deleted_count, 1);

	handle.reset();
	EXPECT_FALSE(handle);
	EXPECT_EQ(stateless_deleter::deleted_count, 2);

	handle.reset(7);
	ASSERT_TRUE(handle);
	EXPECT_EQ(handle.get(), 7);
	EXPECT_EQ(stateless_deleter::deleted_count, 2);
}