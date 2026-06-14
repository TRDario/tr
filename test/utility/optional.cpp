#include <gtest/gtest.h>
#include <tr/utility/optional.hpp>

TEST(optional_test, transform)
{
	constexpr auto pred{[](int v) { return v * 2.5f; }};

	std::optional<int> lvalue_empty{};
	std::optional<int> lvalue{5.0f};
	ASSERT_EQ(tr::transform(lvalue_empty, pred), std::nullopt);
	ASSERT_EQ(tr::transform(lvalue, pred), 12.5f);
	ASSERT_EQ(tr::transform(std::move(lvalue_empty), pred), std::nullopt);
	ASSERT_EQ(tr::transform(std::move(lvalue), pred), 12.5f);

	const std::optional<int> clvalue_empty{};
	const std::optional<int> clvalue{5.0f};
	ASSERT_EQ(tr::transform(lvalue_empty, [](int v) { return v * 2.5f; }), std::nullopt);
	ASSERT_EQ(tr::transform(lvalue, [](int v) { return v * 2.5f; }), 12.5f);
	ASSERT_EQ(tr::transform(std::move(clvalue_empty), [](int v) { return v * 2.5f; }), std::nullopt);
	ASSERT_EQ(tr::transform(std::move(clvalue), [](int v) { return v * 2.5f; }), 12.5f);
}

TEST(optional_test, and_then)
{
	constexpr auto pred{[](int v) { return v >= 5.0f ? std::optional{v * 2.5f} : std::nullopt; }};

	std::optional<int> lvalue_empty{};
	std::optional<int> lvalue1{4.0f};
	std::optional<int> lvalue2{6.0f};
	ASSERT_EQ(tr::and_then(lvalue_empty, pred), std::nullopt);
	ASSERT_EQ(tr::and_then(lvalue1, pred), std::nullopt);
	ASSERT_EQ(tr::and_then(lvalue2, pred), 15.0f);
	ASSERT_EQ(tr::and_then(std::move(lvalue_empty), pred), std::nullopt);
	ASSERT_EQ(tr::and_then(std::move(lvalue1), pred), std::nullopt);
	ASSERT_EQ(tr::and_then(std::move(lvalue2), pred), 15.0f);

	const std::optional<int> clvalue_empty{};
	const std::optional<int> clvalue1{4.0f};
	const std::optional<int> clvalue2{6.0f};
	ASSERT_EQ(tr::and_then(clvalue_empty, pred), std::nullopt);
	ASSERT_EQ(tr::and_then(clvalue1, pred), std::nullopt);
	ASSERT_EQ(tr::and_then(clvalue2, pred), 15.0f);
	ASSERT_EQ(tr::and_then(std::move(clvalue_empty), pred), std::nullopt);
	ASSERT_EQ(tr::and_then(std::move(clvalue1), pred), std::nullopt);
	ASSERT_EQ(tr::and_then(std::move(clvalue2), pred), 15.0f);
}

TEST(optional_test, or_else)
{
	constexpr auto pred{[] { return std::optional{-INFINITY}; }};

	std::optional<float> lvalue_empty{};
	std::optional<float> lvalue{5.0f};
	ASSERT_EQ(tr::or_else(lvalue_empty, pred), -INFINITY);
	ASSERT_EQ(tr::or_else(lvalue, pred), 5.0f);

	const std::optional<float> clvalue_empty{};
	const std::optional<float> clvalue{5.0f};
	ASSERT_EQ(tr::or_else(clvalue_empty, pred), -INFINITY);
	ASSERT_EQ(tr::or_else(clvalue, pred), 5.0f);
}