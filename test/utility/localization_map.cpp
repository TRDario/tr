#include <gtest/gtest.h>
#include <tr/utility/localization_map.hpp>

TEST(localization_map_test, access)
{
	const tr::localization_map map{{{"key", "value"}}};
	ASSERT_TRUE(map.contains("key"));
	ASSERT_EQ(map["key"], "value");
	ASSERT_FALSE(map.contains("key2"));
	ASSERT_EQ(map["key2"], "key2");
}

TEST(localization_map_test, script)
{
	tr::localization_map map;
	map.load_script("key = \"value\"");
	ASSERT_TRUE(map.contains("key"));
	ASSERT_EQ(map["key"], "value");

	map.clear();
	map.load_script("escaped = \"\\\\This is\\n\\\"escaped\\\"!\\\\\"");
	ASSERT_TRUE(map.contains("escaped"));
	ASSERT_EQ(map["escaped"], "\\This is\n\"escaped\"!\\");

	// Check is missing a key produces an error.
	map.clear();
	ASSERT_TRUE(map.load_script("\"Blah\"").size() == 1);

	map.clear();
	map.load_script("no_delimiter \"Blah\"");
	ASSERT_FALSE(map.contains("no_delimiter"));

	map.clear();
	map.load_script("no_value = ");
	ASSERT_FALSE(map.contains("no_value"));

	map.clear();
	map.load_script("unterminated = \"");
	ASSERT_FALSE(map.contains("unterminated"));

	map.clear();
	map.load_script("unknown_escape = \"\\f\"");
	ASSERT_FALSE(map.contains("unknown_escape"));

	map.clear();
	map.load_script("extra = \"\" Foo Bar Baz");
	ASSERT_FALSE(map.contains("extra"));
}