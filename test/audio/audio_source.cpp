/// @file
/// @brief Tests audio/audio_source.hpp.

#include <gtest/gtest.h>
#include <tr/audio/audio_context.hpp>
#include <tr/audio/audio_device.hpp>
#include <tr/audio/audio_source.hpp>
#include <tr/audio/audio_stream.hpp>

using namespace std::chrono_literals;
using namespace tr::angle_literals;

//

constexpr std::array<tr::i16, 44100> buffer_data{};

//

/// Mock audio stream used in audio source testing.
struct mock_audio_stream : public tr::audio_stream
{
	tr::usize length() const override
	{
		return 441000;
	};

	int channels() const override
	{
		return 1;
	};

	int sample_rate() const override
	{
		return 44100;
	}

	//

	tr::usize tell() const override
	{
		return m_position;
	}

	void seek(tr::usize where) override
	{
		m_position = where;
	}

  private:
	/// Current position within the stream.
	tr::usize m_position{0};

	//

	void raw_read(std::span<tr::i16> buffer) override
	{
		std::ranges::fill(buffer, INT16_MAX);
		m_position += buffer.size();
	}
};

//

/// Test fixture used to test audio sources.
class AudioSourceTests : public testing::Test
{
  protected:
	/// Device the source's context is created on.
	tr::audio_device device;

	/// Context the source is created on.
	tr::audio_context context;

	/// Audio buffer used in the source testing.
	std::shared_ptr<tr::audio_buffer> buffer;

	/// Audio stream used in the source testing.
	std::unique_ptr<tr::audio_stream> stream;

	/// Audio source being tested.
	std::shared_ptr<tr::audio_source> source;

	//

	/// Constructs the fixture.
	AudioSourceTests()
		: device{}
		, context{device}
		, buffer{tr::create_audio_buffer(context, buffer_data, tr::audio_format::mono16, 44100)}
		, stream{std::make_unique<mock_audio_stream>()}
		, source{context.create_audio_source(0)}
	{
	}
};

//

/// Tests getting the context of an audio source.
TEST_F(AudioSourceTests, GetsContext)
{
	EXPECT_EQ(&context, &source->context());
}

/// Tests getting the priority of an audio source.
TEST_F(AudioSourceTests, GetsPriority)
{
	EXPECT_EQ(source->priority(), 0);
}

/// Tests controlling the class mask of an audio source.
TEST_F(AudioSourceTests, ControlsClassMask)
{
	source->set_class_mask(0x00000080);
	EXPECT_EQ(source->class_mask(), 0x00000080);
}

/// Tests controlling the pitch of an audio source.
TEST_F(AudioSourceTests, ControlsPitch)
{
	source->set_pitch(1.5f);
	EXPECT_EQ(source->pitch(), 1.5f);

	source->set_pitch(0.5f, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->pitch(), 0.5f);
}

/// Tests controlling the gain of an audio source.
TEST_F(AudioSourceTests, ControlsGain)
{
	source->set_gain(1.5f);
	EXPECT_EQ(source->gain(), 1.5f);

	source->set_gain(0.5f, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->gain(), 0.5f);
}

/// Tests controlling the maximum distance of an audio source.
TEST_F(AudioSourceTests, ControlsMaxDistance)
{
	source->set_max_distance(1000.0f);
	EXPECT_EQ(source->max_distance(), 1000.0f);

	source->set_max_distance(100.0f, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->max_distance(), 100.0f);
}

/// Tests controlling the rolloff factor of an audio source.
TEST_F(AudioSourceTests, ControlsRolloffFactor)
{
	source->set_rolloff_factor(2.0f);
	EXPECT_EQ(source->rolloff_factor(), 2.0f);

	source->set_rolloff_factor(0.5f, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->rolloff_factor(), 0.5f);
}

/// Tests controlling the reference distance of an audio source.
TEST_F(AudioSourceTests, ControlsReferenceDistance)
{
	source->set_reference_distance(1000.0f);
	EXPECT_EQ(source->reference_distance(), 1000.0f);

	source->set_reference_distance(100.0f, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->reference_distance(), 100.0f);
}

/// Tests controlling the gain outside the cone of an audio source.
TEST_F(AudioSourceTests, ControlsGainOutsideCone)
{
	source->set_gain_outside_cone(0.5f);
	EXPECT_EQ(source->gain_outside_cone(), 0.5f);

	source->set_gain_outside_cone(1.0f, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->gain_outside_cone(), 1.0f);
}

/// Tests controlling the cone widths of an audio source.
TEST_F(AudioSourceTests, ControlsConeWidths)
{
	source->set_cone_widths(60_degf, 120_degf);
	EXPECT_EQ(source->inner_cone_width(), 60_degf);
	EXPECT_EQ(source->outer_cone_width(), 120_degf);

	source->set_cone_widths(120_degf, 240_degf);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->inner_cone_width(), 120_degf);
	EXPECT_EQ(source->outer_cone_width(), 240_degf);
}

/// Tests controlling the position of an audio source.
TEST_F(AudioSourceTests, ControlsPosition)
{
	const glm::vec3 first_position{100, 100, 100};
	const glm::vec3 second_position{200, 200, 200};

	source->set_position(first_position);
	EXPECT_EQ(source->position(), first_position);

	source->set_position(second_position, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->position(), second_position);
}

/// Tests controlling the velocity of an audio source.
TEST_F(AudioSourceTests, ControlsVelocity)
{
	const glm::vec3 first_velocity{100, 100, 100};
	const glm::vec3 second_velocity{200, 200, 200};

	source->set_velocity(first_velocity);
	EXPECT_EQ(source->velocity(), first_velocity);

	source->set_velocity(second_velocity, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->velocity(), second_velocity);
}

/// Tests controlling the direction of an audio source.
TEST_F(AudioSourceTests, ControlsDirection)
{
	const glm::vec3 first_direction{0, 1, 0};
	const glm::vec3 second_direction{1, 0, 0};

	source->set_direction(first_direction);
	EXPECT_EQ(source->direction(), first_direction);

	source->set_direction(second_direction, 0.1s);
	std::this_thread::sleep_for(0.2s);
	EXPECT_EQ(source->direction(), second_direction);
}

/// Tests controlling the origin of an audio source.
TEST_F(AudioSourceTests, ControlsOrigin)
{
	source->set_origin(tr::audio_source::origin::listener);
	EXPECT_EQ(source->origin(), tr::audio_source::origin::listener);
	source->set_origin(tr::audio_source::origin::absolute);
	EXPECT_EQ(source->origin(), tr::audio_source::origin::absolute);
}

/// Tests using an audio buffer as an attachment for an audio source.
TEST_F(AudioSourceTests, UsesBufferSource)
{
	source->use(buffer);
	EXPECT_EQ(source->length(), 1.0s);

	EXPECT_EQ(source->state(), tr::audio_source::state::initial);

	source->set_offset(0.5s);
	source->play();
	EXPECT_EQ(source->state(), tr::audio_source::state::playing);
	source->pause();
	EXPECT_EQ(source->state(), tr::audio_source::state::paused);
	EXPECT_EQ(source->offset(), 0.5s);
	source->stop();
	EXPECT_EQ(source->state(), tr::audio_source::state::stopped);

	source->set_looping(true);
	EXPECT_TRUE(source->looping());

	source->set_loop_points(0.25s, 0.75s);
	EXPECT_EQ(source->loop_start(), 0.25s);
	EXPECT_EQ(source->loop_end(), 0.75s);
}

/// Tests using an audio stream as an attachment for an audio source.
TEST_F(AudioSourceTests, UsesStreamSource)
{
	source->use(std::move(stream));
	EXPECT_EQ(source->length(), 10.0s);

	source->set_offset(0.5s);
	EXPECT_EQ(source->offset(), 0.5s);

	source->set_looping(true);
	EXPECT_TRUE(source->looping());

	source->set_loop_points(0.25s, 0.75s);
	EXPECT_EQ(source->loop_start(), 0.25s);
	EXPECT_EQ(source->loop_end(), 0.75s);

	source->set_offset(tr::audio_source::start);
	EXPECT_EQ(source->state(), tr::audio_source::state::initial);
	source->play();
	EXPECT_EQ(source->state(), tr::audio_source::state::playing);
	source->pause();
	EXPECT_EQ(source->state(), tr::audio_source::state::paused);
	source->stop();
	EXPECT_EQ(source->state(), tr::audio_source::state::stopped);
}

/// Tests clearing the attachment an audio source.
TEST_F(AudioSourceTests, ClearsAttachment)
{
	source->clear();

	EXPECT_EQ(source->length(), 0s);
	EXPECT_EQ(source->loop_start(), 0s);
	EXPECT_EQ(source->loop_end(), 0s);
	EXPECT_EQ(source->offset(), 0s);
}