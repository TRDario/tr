/// @file
/// @brief Implements audio_context.hpp.

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <tr/audio/audio_context.hpp>
#include <tr/audio/audio_device.hpp>
#include <tr/audio/audio_source.hpp>
#include <tr/audio/exception.hpp>
#include <tr/audio/orientation.hpp>
#include <tr/utility/ranges.hpp>

//

namespace
{
	/// Attributes of audio contexts.
	constexpr std::array<ALCint, 3> audio_context_attributes{ALC_HRTF_SOFT, ALC_FALSE, 0};
} // namespace

tr::audio_context::audio_context(audio_device& device)
	: m_ptr{alcCreateContext(device.unwrap(), audio_context_attributes.data())}
	, m_al{device.unwrap()}
{
	if (m_ptr == nullptr) {
		throw audio_context_init_error{device.unwrap()};
	}

	int al_max_sources;
	alcGetIntegerv(device.unwrap(), ALC_MONO_SOURCES, 1, &al_max_sources);
	m_max_sources = al_max_sources;

	m_class_gains.fill(1.0f);

	try {
		m_thread = std::jthread{&audio_context::thread_loop, this};
	}
	catch (...) {
	}
}

void tr::audio_context::deleter::operator()(ALCcontext* context) noexcept
{
	alcDestroyContext(context);
}

//

float tr::audio_context::master_gain() const noexcept
{
	float gain;
	m_al.get_listener_property_f(unwrap(), AL_GAIN, &gain);
	return gain;
}

void tr::audio_context::set_master_gain(float gain) noexcept
{
	TR_ASSERT(gain >= 0.0f, "Tried to set master gain to {}, while minimum allowed is 0.", gain);

	m_al.set_listener_property_f(unwrap(), AL_GAIN, gain);
}

//

float tr::audio_context::class_gain(audio_class_id id) const noexcept
{
	return m_class_gains[id];
}

void tr::audio_context::set_class_gain(audio_class_id id, float gain) noexcept
{
	m_class_gains[id] = gain;
	for (audio_source& source : deref(m_sources)) {
		if (source.class_mask()[id]) {
			source.set_gain(source.gain());
		}
	}
}

//

glm::vec3 tr::audio_context::listener_position() const noexcept
{
	glm::vec3 position;
	m_al.get_listener_property_fv(unwrap(), AL_POSITION, glm::value_ptr(position));
	return position;
}

void tr::audio_context::set_listener_position(glm::vec3 position) noexcept
{
	m_al.set_listener_property_fv(unwrap(), AL_POSITION, glm::value_ptr(position));
}

//

glm::vec3 tr::audio_context::listener_velocity() const noexcept
{
	glm::vec3 velocity;
	m_al.get_listener_property_fv(unwrap(), AL_VELOCITY, glm::value_ptr(velocity));
	return velocity;
}

void tr::audio_context::set_listener_velocity(glm::vec3 velocity) noexcept
{
	m_al.set_listener_property_fv(unwrap(), AL_VELOCITY, glm::value_ptr(velocity));
}

//

tr::orientation tr::audio_context::listener_orientation() const noexcept
{
	orientation orientation;
	m_al.get_listener_property_fv(unwrap(), AL_ORIENTATION, &orientation.view.x);
	return orientation;
}

void tr::audio_context::set_listener_orientation(orientation orientation) noexcept
{
	m_al.set_listener_property_fv(unwrap(), AL_ORIENTATION, &orientation.view.x);
}

//

std::shared_ptr<tr::audio_buffer> tr::audio_context::create_audio_buffer()
{
	const std::lock_guard lock{m_mutex};
	return m_buffers.emplace_back(std::make_shared<audio_buffer>(*this));
}

std::shared_ptr<tr::audio_source> tr::audio_context::create_audio_source(int priority)
{
	const std::lock_guard lock{m_mutex};
	if (m_sources.size() == m_max_sources) {
		auto erasable_it{std::ranges::find_if(m_sources, [&](auto& src) { return src.use_count() == 1 && src->priority() <= priority; })};
		if (erasable_it == m_sources.end()) {
			return nullptr;
		}
		else {
			unstable_erase(m_sources, erasable_it);
		}
	}
	return m_sources.emplace_back(std::make_shared<audio_source>(*this, priority));
}

//

ALCcontext* tr::audio_context::unwrap() const noexcept
{
	return m_ptr.get();
}

//

const tr::internal::openal& tr::audio_context::al() const noexcept
{
	return m_al;
}

//

std::lock_guard<std::mutex> tr::audio_context::lock_mutex()
{
	return std::lock_guard{m_mutex};
}

//

void tr::audio_context::thread_loop(std::stop_token stoken) noexcept
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