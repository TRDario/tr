///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/handle.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/handle.hpp>

// Default-constructible, stateless deleter that keeps track of deletions.
struct stateless_deleter {
	inline static int deleted_count{0};
	inline static int last_deleted_value{0};

	void operator()(int value) const
	{
		++deleted_count;
		last_deleted_value = value;
	}
};
using stateless_deleter_handle = tr::handle<int, 0, stateless_deleter>;

// Non-default-constructible deleter with an integer tag.
struct tagged_deleter {
	int id;

	tagged_deleter(int id)
		: id{id}
	{
	}

	void operator()(int value) const {}
};
using tagged_deleter_handle = tr::handle<int, 0, tagged_deleter>;

// Handle text fixture.
class handle_test : public testing::Test {
  protected:
	void SetUp() override
	{
		stateless_deleter::deleted_count = 0;
	}
};

static_assert(tr::handle_deleter<stateless_deleter, int>);
static_assert(tr::handle_deleter<tagged_deleter, int>);
static_assert(tr::handle_deleter<decltype([](int) {}), int>);
static_assert(tr::handle_deleter<void (*)(int), int>);

static_assert(tr::default_constructible_handle_deleter<stateless_deleter>);
static_assert(!tr::default_constructible_handle_deleter<tagged_deleter>);
static_assert(!tr::default_constructible_handle_deleter<void (*)(int)>);

static_assert(sizeof(tr::handle<int, 0, stateless_deleter>) == sizeof(int));

TEST_F(handle_test, default_constructor)
{
	const stateless_deleter_handle empty;
	EXPECT_FALSE(empty);
	EXPECT_FALSE(empty.has_value());
	EXPECT_EQ(empty.get(tr::no_empty_handle_check), 0);
}

TEST_F(handle_test, default_constructor_with_deleter)
{
	const tagged_deleter_handle empty{tagged_deleter{1}};
	EXPECT_FALSE(empty);
	EXPECT_FALSE(empty.has_value());
	EXPECT_EQ(empty.get(tr::no_empty_handle_check), 0);
	EXPECT_EQ(empty.get_deleter().id, 1);
}

TEST_F(handle_test, value_constructor)
{
	const stateless_deleter_handle full{1};
	EXPECT_TRUE(full);
	EXPECT_TRUE(full.has_value());
	EXPECT_EQ(full.get(), 1);
}

TEST_F(handle_test, value_constructor_with_deleter)
{
	const tagged_deleter_handle full{1, tagged_deleter{1}};
	EXPECT_TRUE(full);
	EXPECT_TRUE(full.has_value());
	EXPECT_EQ(full.get(), 1);
	EXPECT_EQ(full.get_deleter().id, 1);
}

TEST_F(handle_test, empty_value_constructor)
{
	const stateless_deleter_handle empty{0, tr::no_empty_handle_check};
	EXPECT_FALSE(empty);
	EXPECT_FALSE(empty.has_value());
	EXPECT_EQ(empty.get(tr::no_empty_handle_check), 0);
}

TEST_F(handle_test, empty_value_constructor_with_deleter)
{
	const tagged_deleter_handle empty{0, tagged_deleter{1}, tr::no_empty_handle_check};
	EXPECT_FALSE(empty);
	EXPECT_FALSE(empty.has_value());
	EXPECT_EQ(empty.get(tr::no_empty_handle_check), 0);
	EXPECT_EQ(empty.get_deleter().id, 1);
}

TEST_F(handle_test, move_constructor)
{
	stateless_deleter_handle handle1{5};
	const stateless_deleter_handle handle2{std::move(handle1)};
	EXPECT_FALSE(handle1.has_value());
	EXPECT_TRUE(handle2.has_value());
	EXPECT_EQ(handle2.get(), 5);
}

TEST_F(handle_test, destructor)
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

TEST_F(handle_test, move_assignment)
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

TEST_F(handle_test, release)
{
	stateless_deleter_handle handle{5};
	int released_value{handle.release()};
	EXPECT_FALSE(handle.has_value());
	EXPECT_EQ(stateless_deleter::deleted_count, 0);
	EXPECT_EQ(released_value, 5);
}

TEST_F(handle_test, reset)
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

	handle.reset(0, tr::no_empty_handle_check);
	EXPECT_FALSE(handle.has_value());
	EXPECT_EQ(stateless_deleter::deleted_count, 3);
}

static void set_c_handle(int* handle, int value)
{
	*handle = value;
}

TEST_F(handle_test, out_handle)
{
	stateless_deleter_handle handle;

	set_c_handle(tr::out_handle(handle), 10);
	EXPECT_TRUE(handle.has_value());
	EXPECT_EQ(handle.get(), 10);

	set_c_handle(tr::out_handle(handle, tr::no_empty_handle_check), 0);
	EXPECT_FALSE(handle.has_value());
	EXPECT_EQ(stateless_deleter::deleted_count, 1);
	EXPECT_EQ(stateless_deleter::last_deleted_value, 10);
}