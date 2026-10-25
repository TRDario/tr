/// @file
/// @brief Tests utility/zstring_view_zspan.hpp.

#include <gtest/gtest.h>
#include <tr/utility/zstring_view_zspan.hpp>

//

/// Tests that the value of a NUL-terminated span of NUL-terminated string views is a NUL-terminated string view.
static_assert(std::same_as<tr::zstring_view_zspan::value_type, tr::zstring_view>);

/// Tests that a NUL-terminated span of NUL-terminated string views is a view.
static_assert(std::ranges::view<tr::zstring_view_zspan>);

/// Tests that a NUL-terminated span of NUL-terminated string views is a borrowed range.
static_assert(std::ranges::borrowed_range<tr::zstring_view_zspan>);

/// Tests that the value of a NUL-terminated span of NUL-terminated string views is a NUL-terminated string view.
static_assert(std::same_as<tr::basic_zstring_view_zspan<wchar_t>::value_type, tr::basic_zstring_view<wchar_t>>);

//

/// Tests that a default-constructed NUL-terminated span of NUL-terminated string views is empty.
TEST(ZstringViewZspanTest, DefaultConstructsEmpty)
{
	const tr::zstring_view_zspan span{};
	EXPECT_TRUE(span.empty());
	EXPECT_EQ(span.begin(), span.end());
	EXPECT_EQ(span.data(), nullptr);
}

/// Tests that a NUL-terminated span of NUL-terminated string views pointing to an empty list is empty.
TEST(ZstringViewZspanTest, EmptyListConstructsEmpty)
{
	const char* const empty_list[]{nullptr};
	const tr::zstring_view_zspan span{empty_list};
	EXPECT_TRUE(span.empty());
	EXPECT_EQ(span.begin(), span.end());
}

/// Tests accessing strings of a NUL-terminated span of NUL-terminated string views.
TEST(ZstringViewZspanTest, ProvidesAccess)
{
	const char* const strings[]{"first", "second", "third", nullptr};
	const tr::zstring_view_zspan span{strings};
	EXPECT_FALSE(span.empty());
	ASSERT_EQ(std::ranges::distance(span), 3);
	EXPECT_EQ(span.data(), strings);
	EXPECT_EQ(span.front(), "first");
	EXPECT_EQ(span[1], "second");
	EXPECT_EQ(span.at(2), "third");
}

/// Tests iterating through a NUL-terminated span of NUL-terminated string views.
TEST(ZstringViewZspanTest, Iterates)
{
	const char* const strings[]{"alpha", "beta", "gamma", nullptr};
	const tr::zstring_view_zspan span{strings};
	std::vector<std::string_view> values;
	for (const tr::zstring_view value : span) {
		values.emplace_back(value);
	}

	EXPECT_EQ(values, (std::vector<std::string_view>{"alpha", "beta", "gamma"}));
	ASSERT_EQ(std::ranges::distance(span), 3);
	EXPECT_EQ(span.begin()[1], "beta");
}

/// Tests a span of wide character string views.
TEST(ZstringViewZspanTest, SupportsWideCharacters)
{
	const wchar_t* const strings[]{L"wide", L"second", nullptr};
	const tr::wzstring_view_zspan span{strings};
	EXPECT_EQ(*span.begin(), L"wide");
	EXPECT_EQ(span.front(), L"wide");
	EXPECT_EQ(span[1], L"second");
}