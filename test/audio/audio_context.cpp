/// @file
/// @brief Tests audio/audio_context.hpp.

#include <gtest/gtest.h>
#include <tr/audio/audio_context.hpp>
#include <tr/audio/audio_device.hpp>
#include <tr/audio/orientation.hpp>

//

/// Fixture used to test audio contexts.
class AudioContextTest : public testing::Test
{
  protected:
	/// Device the context is created on.
	tr::audio_device device;

	/// Audio context being tested.
	tr::audio_context context;

	//

	/// Constructs the fixture.
	AudioContextTest()
		: device{}
		, context{device}
	{
	}
};

//

/// Tests setting and getting the master gain on an audio context.
TEST_F(AudioContextTest, ControlsMasterGain)
{
	context.set_master_gain(2.0f);
	EXPECT_EQ(context.master_gain(), 2.0f);
}

/// Tests setting and getting the class gain on an audio context.
TEST_F(AudioContextTest, ControlsClassGain)
{
	EXPECT_EQ(context.class_gain(0), 1.0f);

	context.set_class_gain(0, 0.5f);
	EXPECT_EQ(context.class_gain(0), 0.5f);

	context.set_class_gain(1, 1.5f);
	EXPECT_EQ(context.class_gain(1), 1.5f);
}

/// Tests setting and getting the listener position on an audio context.
TEST_F(AudioContextTest, ControlsListenerPosition)
{
	const glm::vec3 position{100, 100, 100};

	context.set_listener_position(position);
	EXPECT_EQ(context.listener_position(), position);
}

/// Tests setting and getting the listener velocity on an audio context.
TEST_F(AudioContextTest, ControlsListenerVelocity)
{
	const glm::vec3 velocity{100, 100, 100};

	context.set_listener_velocity(velocity);
	EXPECT_EQ(context.listener_velocity(), velocity);
}

/// Tests setting and getting the listener orientation on an audio context.
TEST_F(AudioContextTest, ControlsListenerOrientation)
{
	const tr::orientation orientation{{1, 0, 0}, {0, 1, 0}};

	context.set_listener_orientation(orientation);
	EXPECT_EQ(context.listener_orientation(), orientation);
}