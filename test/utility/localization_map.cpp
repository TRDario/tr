/// @file
/// @brief Tests utility/localization_map.hpp.

#include <gtest/gtest.h>
#include <tr/utility/localization_map.hpp>

//

namespace
{
	/// @brief Downgrades an iterator to a strictly input iterator.
	/// @tparam Base Base iterator type.
	template <std::input_iterator Base>
	class strict_input_iterator
	{
	  public:
		/// @brief Value type used by the iterator.
		using value_type = std::iterator_traits<Base>::value_type;

		/// @brief Difference type used by the iterator.
		using difference_type = std::iterator_traits<Base>::difference_type;

		/// @brief Reference type used by the iterator.
		using reference = std::iterator_traits<Base>::reference;

		/// @brief Category of the iterator.
		using iterator_category = std::input_iterator_tag;

		/// @brief Concept of the iterator.
		using iterator_concept = std::input_iterator_tag;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a singular iterator.
		[[nodiscard]] explicit strict_input_iterator() noexcept = default;

		/// @brief Wraps an input iterator.
		/// @param base Base iterator
		[[nodiscard]] explicit strict_input_iterator(Base base) noexcept
			: m_base{base}
		{
		}

		/// @}
		/// @name Operators
		/// @{

		/// @brief Compares two iterators for equality.
		/// @param a, b Iterators to compare.
		/// @return `true` if the iterators are equal, `false` otherwise.
		[[nodiscard]] friend bool operator==(strict_input_iterator a, strict_input_iterator b) noexcept = default;

		/// @brief Dereferences the iterator.
		/// @return Dereferenced iterator value.
		[[nodiscard]] reference operator*() const noexcept
		{
			return *m_base;
		}

		/// @brief Increments the iterator.
		/// @return Reference to `*this`.
		strict_input_iterator& operator++() noexcept
		{
			++m_base;
			return *this;
		}

		/// @brief Increments the iterator.
		/// @return Previous value of `*this`.
		[[nodiscard]] strict_input_iterator operator++(int) noexcept
		{
			strict_input_iterator prev{*this};
			++m_base;
			return prev;
		}

	  private:
		/// @brief Base iterator value.
		Base m_base;
	};
} // namespace

//

/// @brief Tests that a localization map default-constructs empty.
TEST(LocalizationMapTest, DefaultConstructsEmpty)
{
	tr::localization_map map;
	EXPECT_EQ(map.size(), 0);
}

/// @brief Tests constructing a localization map from an iterator pair.
TEST(LocalizationMapTest, ConstructsFromIteratorPair)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> data{{{"first", "First"}, {"second", "Second"}}};

	const tr::localization_map map{data.begin(), data.end()};
	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains("first"));
	EXPECT_TRUE(map.contains("second"));
	EXPECT_FALSE(map.contains("third"));
}

/// @brief Tests constructing a localization map from a range.
TEST(LocalizationMapTest, ConstructsFromRange)
{
	const tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	EXPECT_EQ(map.size(), 2);
	EXPECT_TRUE(map.contains("first"));
	EXPECT_TRUE(map.contains("second"));
	EXPECT_FALSE(map.contains("third"));
}

#if defined(TR_ENABLE_ASSERTS) && defined(GTEST_HAS_DEATH_TEST)
/// @brief Tests that constructing a localization map with duplicate keys triggers an assertion.
TEST(LocalizationMapTest, ConstructingWithDuplicateKeysTriggersAssertion)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> data{{{"first", "First"}, {"first", "Second"}}};

	EXPECT_DEATH({ const tr::localization_map map{data}; }, "duplicate keys");
}
#endif

/// @brief Tests that copy-constructing a localization map copies values from the original map.
TEST(LocalizationMapTest, CopyConstructorCopiesValues)
{
	const tr::localization_map original{{{"first", "First"}, {"second", "Second"}}};
	const tr::localization_map copy{original};
	EXPECT_EQ(original.size(), copy.size());
	EXPECT_EQ(original["first"], copy["first"]);
	EXPECT_NE(original["first"].data(), copy["first"].data());
}

/// @brief Tests that move-constructing a localization map transfers values from the original map.
TEST(LocalizationMapTest, MoveConstructorTransfersValues)
{
	tr::localization_map original{{{"first", "First"}, {"second", "Second"}}};
	const tr::localization_map moved{std::move(original)};
	EXPECT_EQ(original.size(), 0);
	EXPECT_EQ(moved.size(), 2);
	EXPECT_TRUE(moved.contains("first"));
	EXPECT_TRUE(moved.contains("second"));
}

/// @brief Tests that copy-assigning a localization map replaces its contents with copied values from the original map.
TEST(LocalizationMapTest, CopyAssignmentReplacesValuesWithCopied)
{
	tr::localization_map map1{{{"first", "First"}, {"second", "Second"}}};
	tr::localization_map map2{{{"third", "Third"}, {"fourth", "Fourth"}}};
	map1 = map2;
	EXPECT_EQ(map1.size(), map2.size());
	EXPECT_FALSE(map1.contains("first"));
	EXPECT_TRUE(map1.contains("third"));
	EXPECT_EQ(map1["third"], map2["third"]);
	EXPECT_NE(map1["third"].data(), map2["third"].data());
}

/// @brief Tests that copy-assigning a localization map replaces its contents with transferred values from the original map.
TEST(LocalizationMapTest, MoveAssignmentReplacesValuesWithTransferred)
{
	tr::localization_map map1{{{"first", "First"}, {"second", "Second"}}};
	tr::localization_map map2{{{"third", "Third"}, {"fourth", "Fourth"}}};
	map1 = std::move(map2);
	EXPECT_EQ(map1.size(), 2);
	EXPECT_EQ(map2.size(), 0);
	EXPECT_FALSE(map1.contains("first"));
	EXPECT_TRUE(map1.contains("third"));
}

/// @brief Tests that accessing a localization map with present keys returns associated values.
TEST(LocalizationMapTest, AccessWithPresentKeyReturnsValue)
{
	const tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	EXPECT_EQ(map["first"], "First");
	EXPECT_EQ(map.value_or("second", "error"), "Second");
}

/// @brief Tests that accessing a localization map with a missing key returns the passed key or a default value.
TEST(LocalizationMapTest, AccessWithMissingKeyReturnsKeyOrDefaultValue)
{
	const tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	const std::string_view missing_key{"third"};
	const std::string_view default_value{"default"};
	EXPECT_EQ(map[missing_key].data(), missing_key.data());
	EXPECT_EQ(map.value_or(missing_key, default_value), default_value);
}

/// @brief Tests that clearing a localization map erases its values.
TEST(LocalizationMapTest, ClearEmptiesMap)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	EXPECT_EQ(map.size(), 2);
	map.clear();
	EXPECT_EQ(map.size(), 0);
}

/// @brief Tests that updating a localization map with a new key adds a new value.
TEST(LocalizationMapTest, UpdateWithNewKeyAddsValue)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update("third", "Third");
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["third"], "Third");
}

/// @brief Tests that updating a localization map with a present key replaces the associated value.
TEST(LocalizationMapTest, UpdateWithPresentKeyReplacesValue)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update("first", "1st");
	EXPECT_EQ(map.size(), 2);
	EXPECT_EQ(map["first"], "1st");
}

/// @brief Tests updating a localization map with an input iterator pair to a range of key-value pairs.
TEST(LocalizationMapTest, UpdatesWithInputIteratorPair)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> extra{{{"third", "Third"}, {"first", "1st"}}};

	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update(strict_input_iterator{extra.begin()}, strict_input_iterator{extra.end()});
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["first"], "1st");
	EXPECT_EQ(map["third"], "Third");
}

/// @brief Tests updating a localization map with a forward iterator pair to a range of key-value pairs.
TEST(LocalizationMapTest, UpdatesWithForwardIteratorPair)
{
	constexpr std::array<std::pair<std::string_view, std::string_view>, 2> extra{{{"third", "Third"}, {"first", "1st"}}};

	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update(extra.begin(), extra.end());
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["first"], "1st");
	EXPECT_EQ(map["third"], "Third");
}

/// @brief Tests updating a localization map with a range of key-value pairs.
TEST(LocalizationMapTest, UpdatesWithRange)
{
	tr::localization_map map{{{"first", "First"}, {"second", "Second"}}};
	map.update({{"third", "Third"}, {"first", "1st"}});
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["first"], "1st");
	EXPECT_EQ(map["third"], "Third");
}