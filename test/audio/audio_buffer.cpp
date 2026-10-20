/// @file
/// @brief Tests audio/audio_buffer.hpp.

#include <gtest/gtest.h>
#include <tr/audio/audio_buffer.hpp>
#include <tr/audio/audio_context.hpp>
#include <tr/audio/audio_device.hpp>

using namespace std::chrono_literals;

//

/// Fixture used to test audio buffers.
class AudioBufferTest : public testing::Test
{
  protected:
	/// Device the buffer's context is created on.
	tr::audio_device device;

	/// Context the buffer is created on.
	tr::audio_context context;

	/// Audio buffer being tested.
	std::shared_ptr<tr::audio_buffer> buffer;

	//

	/// Constructs the fixture.
	AudioBufferTest()
		: device{}
		, context{device}
		, buffer{context.create_audio_buffer()}
	{
	}
};

//

/// Tests getting the context of an audio buffer.
TEST_F(AudioBufferTest, HasContext)
{
	EXPECT_EQ(&context, &buffer->context());
}

/// Tests setting mono audio to an audio buffer.
TEST_F(AudioBufferTest, SetsMonoAudio)
{
	constexpr std::array<tr::i16, 44100> data{};

	buffer->set(data, tr::audio_format::mono16, 44100);
	EXPECT_EQ(buffer->size(), 44100);
	EXPECT_EQ(buffer->length(), 1.0s);
	EXPECT_EQ(buffer->sample_rate(), 44100);
	EXPECT_EQ(buffer->channels(), 1);
}

/// Tests setting stereo audio to an audio buffer.
TEST_F(AudioBufferTest, SetsStereoAudio)
{
	constexpr std::array<tr::i16, 88200> data{};

	buffer->set(data, tr::audio_format::stereo16, 44100);
	EXPECT_EQ(buffer->size(), 88200);
	EXPECT_EQ(buffer->length(), 1.0s);
	EXPECT_EQ(buffer->sample_rate(), 44100);
	EXPECT_EQ(buffer->channels(), 2);
}