///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests audio/audio_device_list_view.hpp.                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/audio/audio_device.hpp>
#include <tr/audio/audio_device_list_view.hpp>

TEST(audio_device_list_view_test, audio_device_list_view_iterator)
{
	constexpr const char* raw_device_list{"test1\0test2\0test3\0"};
	tr::audio_device_list_view_iterator it{raw_device_list};
	tr::audio_device_list_view_iterator it2{it};
	tr::audio_device_list_view_end_sentinel end{};

	EXPECT_EQ(it->c_str(), raw_device_list);
	EXPECT_EQ(*it, "test1");
	EXPECT_EQ(it, it2);
	EXPECT_NE(it, end);

	++it;
	EXPECT_EQ(*it, "test2");
	EXPECT_GT(it, it2);
	EXPECT_NE(it, end);

	it2 = it++;
	EXPECT_EQ(*it2, "test2");
	EXPECT_EQ(*it, "test3");
	EXPECT_NE(it, end);

	++it;
	EXPECT_TRUE(it->empty());
	EXPECT_EQ(it, end);
}