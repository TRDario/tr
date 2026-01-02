#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"
#include "../../include/tr/audio/listener.hpp"

//

float tr::audio::master_gain()
{
	float gain;
	TR_AL_CALL(alGetListenerf, AL_GAIN, &gain);
	return gain;
}

void tr::audio::set_master_gain(float gain)
{
	TR_ASSERT(gain >= 0.0f, "Tried to set master gain to {}, while minimum allowed is 0.", gain);

	TR_AL_CALL(alListenerf, AL_GAIN, gain);
}

//

float tr::audio::class_gain(int id)
{
	return g_manager.class_gain(id);
}

void tr::audio::set_class_gain(int id, float gain)
{
	g_manager.set_class_gain(id, gain);
}

//

glm::vec3 tr::audio::listener_pos()
{
	glm::vec3 pos;
	TR_AL_CALL(alGetListenerfv, AL_POSITION, value_ptr(pos));
	return pos;
}

void tr::audio::set_listener_pos(const glm::vec3& pos)
{
	TR_AL_CALL(alListenerfv, AL_POSITION, value_ptr(pos));
}

glm::vec3 tr::audio::listener_vel()
{
	glm::vec3 vel;
	TR_AL_CALL(alGetListenerfv, AL_VELOCITY, value_ptr(vel));
	return vel;
}

void tr::audio::set_listener_vel(const glm::vec3& vel)
{
	TR_AL_CALL(alListenerfv, AL_VELOCITY, value_ptr(vel));
}

tr::audio::orientation tr::audio::listener_orientation()
{
	orientation orientation;
	TR_AL_CALL(alGetListenerfv, AL_ORIENTATION, &orientation.view.x);
	return orientation;
}

void tr::audio::set_listener_orientation(orientation orientation)
{
	TR_AL_CALL(alListenerfv, AL_ORIENTATION, &orientation.view.x);
}