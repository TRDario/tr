///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests audio/audio_device.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/audio/audio_device.hpp>
#include <tr/audio/audio_device_list_view.hpp>

TEST(audio_device_test, available_audio_devices)
{
	const tr::audio_device_list_view audio_devices{tr::available_audio_devices()};
	for (tr::zstring_view device_name : audio_devices) {
		tr::audio_device device{device_name};
		EXPECT_EQ(device.name(), device_name);
	}
}