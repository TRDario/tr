///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests audio/audio_context.hpp.                                                                                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/audio/audio_context.hpp>
#include <tr/audio/audio_device.hpp>

// Audio context test fixture.
class audio_context_test : public testing::Test {
  protected:
	audio_context_test()
		: device{}
		, context{device}
	{
	}

	// Device the context is created on.
	tr::audio_device device;
	// Audio context being tested.
	tr::audio_context context;
};

TEST_F(audio_context_test, master_gain)
{
	context.set_master_gain(2.0f);
	EXPECT_EQ(context.master_gain(), 2.0f);
}

TEST_F(audio_context_test, class_gain)
{
	EXPECT_EQ(context.class_gain(0), 1.0f);

	context.set_class_gain(0, 0.5f);
	EXPECT_EQ(context.class_gain(0), 0.5f);

	context.set_class_gain(1, 1.5f);
	EXPECT_EQ(context.class_gain(1), 1.5f);
}

TEST_F(audio_context_test, listener_position)
{
	const glm::vec3 position{100, 100, 100};

	context.set_listener_position(position);
	EXPECT_EQ(context.listener_position(), position);
}

TEST_F(audio_context_test, listener_velocity)
{
	const glm::vec3 velocity{100, 100, 100};

	context.set_listener_velocity(velocity);
	EXPECT_EQ(context.listener_velocity(), velocity);
}

TEST_F(audio_context_test, listener_orientation)
{
	const tr::orientation orientation{{1, 0, 0}, {0, 1, 0}};

	context.set_listener_orientation(orientation);
	EXPECT_EQ(context.listener_orientation(), orientation);
}