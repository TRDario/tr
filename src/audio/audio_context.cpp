/// @file
/// @brief Implements the non-templated parts of audio_context.hpp.

#include "../../include/tr/audio/audio_context.hpp"
#include "../../include/tr/audio/audio_device.hpp"
#include "../../include/tr/audio/audio_source.hpp"
#include "../../include/tr/utility/ranges.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

//

tr::audio_context_init_error::audio_context_init_error(ALCdevice* device)
	: m_description{alcGetString(device, alcGetError(device))}
{
}

//

std::string_view tr::audio_context_init_error::name() const
{
	return "Audio context initialization error";
}

std::string_view tr::audio_context_init_error::description() const
{
	return m_description;
}

std::string_view tr::audio_context_init_error::details() const
{
	return {};
}

//

constexpr std::array<ALCint, 3> audio_context_attributes{ALC_HRTF_SOFT, ALC_FALSE, 0};

tr::audio_context::audio_context(audio_device& device)
	: m_ptr{alcCreateContext(device.m_ptr.get(), audio_context_attributes.data())}
	, m_al_api{device.m_ptr.get()}
{
	if (m_ptr == nullptr) {
		throw audio_context_init_error{device.m_ptr.get()};
	}

	int al_max_sources;
	alcGetIntegerv(device.m_ptr.get(), ALC_MONO_SOURCES, 1, &al_max_sources);
	m_max_sources = al_max_sources;

	m_class_gains.fill(1.0f);

	try {
		m_thread = std::jthread{&audio_context::thread_loop, this};
	}
	catch (...) {
	}
}

void tr::audio_context::deleter::operator()(ALCcontext* context)
{
	alcDestroyContext(context);
}

//

float tr::audio_context::master_gain() const
{
	float gain;
	m_al_api.get_listener_property_f(m_ptr.get(), AL_GAIN, &gain);
	return gain;
}

void tr::audio_context::set_master_gain(float gain)
{
	TR_ASSERT(gain >= 0.0f, "Tried to set master gain to {}, while minimum allowed is 0.", gain);

	m_al_api.set_listener_property_f(m_ptr.get(), AL_GAIN, gain);
}

//

float tr::audio_context::class_gain(audio_class_id id) const
{
	return m_class_gains[id];
}

void tr::audio_context::set_class_gain(audio_class_id id, float gain)
{
	m_class_gains[id] = gain;
	for (audio_source& source : deref(m_sources)) {
		if (source.class_mask()[id]) {
			source.set_gain(source.gain());
		}
	}
}

//

glm::vec3 tr::audio_context::listener_position() const
{
	glm::vec3 position;
	m_al_api.get_listener_property_fv(m_ptr.get(), AL_POSITION, glm::value_ptr(position));
	return position;
}

void tr::audio_context::set_listener_position(glm::vec3 position)
{
	m_al_api.set_listener_property_fv(m_ptr.get(), AL_POSITION, glm::value_ptr(position));
}

//

glm::vec3 tr::audio_context::listener_velocity() const
{
	glm::vec3 velocity;
	m_al_api.get_listener_property_fv(m_ptr.get(), AL_VELOCITY, glm::value_ptr(velocity));
	return velocity;
}

void tr::audio_context::set_listener_velocity(glm::vec3 velocity)
{
	m_al_api.set_listener_property_fv(m_ptr.get(), AL_VELOCITY, glm::value_ptr(velocity));
}

//

tr::orientation tr::audio_context::listener_orientation() const
{
	orientation orientation;
	m_al_api.get_listener_property_fv(m_ptr.get(), AL_ORIENTATION, &orientation.view.x);
	return orientation;
}

void tr::audio_context::set_listener_orientation(orientation orientation)
{
	m_al_api.set_listener_property_fv(m_ptr.get(), AL_ORIENTATION, &orientation.view.x);
}

//

void tr::audio_context::thread_loop(std::stop_token stoken)
{
	while (!stoken.stop_requested()) {
		try {
			std::lock_guard context_lock{m_mutex};
			erase_if(m_buffers, [](const auto& ptr) { return ptr.use_count() == 1; });
			erase_if(m_sources, [](const auto& ptr) { return ptr.use_count() == 1 && ptr->state() != audio_source::state::playing; });
			for (audio_source& source : deref(m_sources)) {
				source.refill_if_needed();
			}
			erase_if(m_commands, [](auto& c) { return std::visit([]<typename T>(T& c) { return c.execute() == T::status::done; }, c); });
		}
		catch (std::exception& err) {
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds{10});
	}
}