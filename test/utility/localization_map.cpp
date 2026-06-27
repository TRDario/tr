///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/localization_map.hpp.                                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/localization_map.hpp>

TEST(localization_map_test, access)
{
	const tr::localization_map map{{{"key", "value"}}};
	EXPECT_TRUE(map.contains("key"));
	EXPECT_EQ(map["key"], "value");
	EXPECT_FALSE(map.contains("key2"));
	EXPECT_EQ(map["key2"], "key2");
}

TEST(localization_map_test, script)
{
	tr::localization_map map;
	map.load_script("key = \"value\"");
	EXPECT_TRUE(map.contains("key"));
	EXPECT_EQ(map["key"], "value");

	map.clear();
	map.load_script("escaped = \"\\\\This is\\n\\\"escaped\\\"!\\\\\"");
	EXPECT_TRUE(map.contains("escaped"));
	EXPECT_EQ(map["escaped"], "\\This is\n\"escaped\"!\\");

	// Check if missing a key produces an error.
	map.clear();
	EXPECT_TRUE(map.load_script("\"Blah\"").size() == 1);

	map.clear();
	map.load_script("no_delimiter \"Blah\"");
	EXPECT_FALSE(map.contains("no_delimiter"));

	map.clear();
	map.load_script("no_value = ");
	EXPECT_FALSE(map.contains("no_value"));

	map.clear();
	map.load_script("unterminated = \"");
	EXPECT_FALSE(map.contains("unterminated"));

	map.clear();
	map.load_script("unknown_escape = \"\\f\"");
	EXPECT_FALSE(map.contains("unknown_escape"));

	map.clear();
	map.load_script("extra = \"\" Foo Bar Baz");
	EXPECT_FALSE(map.contains("extra"));
}