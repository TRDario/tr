///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests audio/audio_buffer.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/audio/audio_context.hpp>
#include <tr/audio/audio_device.hpp>

using namespace std::chrono_literals;

class audio_buffer_test : public testing::Test {
  protected:
	audio_buffer_test()
		: device{}
		, context{device}
		, buffer{tr::create_audio_buffer(context)}
	{
	}

	// Device the buffer's context is created on.
	tr::audio_device device;
	// Context the buffer is created on.
	tr::audio_context context;
	// Audio buffer being tested.
	std::shared_ptr<tr::audio_buffer> buffer;
};

TEST_F(audio_buffer_test, context)
{
	EXPECT_EQ(&context, &buffer->context());
}

TEST_F(audio_buffer_test, mono)
{
	constexpr std::array<tr::i16, 44100> data{};

	buffer->set(data, tr::audio_format::mono16, 44100);
	EXPECT_EQ(buffer->size(), 44100);
	EXPECT_EQ(buffer->length(), 1.0s);
	EXPECT_EQ(buffer->sample_rate(), 44100);
	EXPECT_EQ(buffer->channels(), 1);
}

TEST_F(audio_buffer_test, stereo)
{
	constexpr std::array<tr::i16, 88200> data{};

	buffer->set(data, tr::audio_format::stereo16, 44100);
	EXPECT_EQ(buffer->size(), 88200);
	EXPECT_EQ(buffer->length(), 1.0s);
	EXPECT_EQ(buffer->sample_rate(), 44100);
	EXPECT_EQ(buffer->channels(), 2);
}