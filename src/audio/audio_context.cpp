///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of audio_context.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/audio/audio_context.hpp"
#include "../../include/tr/audio/audio_device.hpp"
#include "../../include/tr/audio/audio_source.hpp"
#include "../../include/tr/utility/ranges.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

///////////////////////////////////////////////////////// AUDIO CONTEXT INIT ERROR ////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////// OPENAL FUNCTIONS ////////////////////////////////////////////////////////////

// Wrapper around a void pointer that automatically casts it to a function pointer type.
struct loaded_al_function_proxy {
	void* ptr;

	template <typename Return, typename... Args> using function_pointer = Return (*)(Args...);
	template <typename Return, typename... Args> operator function_pointer<Return, Args...>()
	{
		return reinterpret_cast<function_pointer<Return, Args...>>(ptr);
	}
};

// Wraps SDL_GL_GetProcAddress to return an OpenAL function proxy.
static loaded_al_function_proxy al_function_address(ALCdevice* device, const char* name)
{
	return {alcGetProcAddress(device, name)};
}

tr::audio_context::al_functions::al_functions(ALCdevice* device)
	: delete_buffers{al_function_address(device, "alDeleteBuffersDirect")}
	, delete_sources{al_function_address(device, "alDeleteSourcesDirect")}
	, generate_buffers{al_function_address(device, "alGenBuffersDirect")}
	, generate_sources{al_function_address(device, "alGenSourcesDirect")}
	, get_buffer_property_i{al_function_address(device, "alGetBufferiDirect")}
	, get_buffer_property_iv{al_function_address(device, "alGetBufferivDirect")}
	, get_error{al_function_address(device, "alGetErrorDirect")}
	, get_listener_property_f{al_function_address(device, "alGetListenerfDirect")}
	, get_listener_property_fv{al_function_address(device, "alGetListenerfvDirect")}
	, get_source_property_f{al_function_address(device, "alGetSourcefDirect")}
	, get_source_property_fv{al_function_address(device, "alGetSourcefvDirect")}
	, get_source_property_i{al_function_address(device, "alGetSourceiDirect")}
	, set_buffer_data{al_function_address(device, "alBufferDataDirect")}
	, set_buffer_property_iv{al_function_address(device, "alBufferivDirect")}
	, set_listener_property_f{al_function_address(device, "alListenerfDirect")}
	, set_listener_property_fv{al_function_address(device, "alListenerfvDirect")}
	, set_source_property_f{al_function_address(device, "alSourcefDirect")}
	, set_source_property_fv{al_function_address(device, "alSourcefvDirect")}
	, set_source_property_i{al_function_address(device, "alSourceiDirect")}
	, source_pause{al_function_address(device, "alSourcePauseDirect")}
	, source_play{al_function_address(device, "alSourcePlayDirect")}
	, source_queue_buffers{al_function_address(device, "alSourceQueueBuffersDirect")}
	, source_stop{al_function_address(device, "alSourceStopDirect")}
	, source_unqueue_buffers{al_function_address(device, "alSourceUnqueueBuffersDirect")}
{
}

////////////////////////////////////////////////////////////// AUDIO CONTEXT //////////////////////////////////////////////////////////////

constexpr std::array<ALCint, 3> audio_context_attributes{ALC_HRTF_SOFT, ALC_FALSE, 0};

tr::audio_context::audio_context(audio_device& device)
	: m_ptr{alcCreateContext(device.m_ptr.get(), audio_context_attributes.data())}
	, m_alapi{device.m_ptr.get()}
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

void tr::audio_context::deleter::operator()(ALCcontext* context) const
{
	alcDestroyContext(context);
}

//

float tr::audio_context::master_gain() const
{
	float gain;
	m_alapi.get_listener_property_f(m_ptr.get(), AL_GAIN, &gain);
	return gain;
}

void tr::audio_context::set_master_gain(float gain)
{
	TR_ASSERT(gain >= 0.0f, "Tried to set master gain to {}, while minimum allowed is 0.", gain);

	m_alapi.set_listener_property_f(m_ptr.get(), AL_GAIN, gain);
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
	m_alapi.get_listener_property_fv(m_ptr.get(), AL_POSITION, glm::value_ptr(position));
	return position;
}

void tr::audio_context::set_listener_position(const glm::vec3& position)
{
	m_alapi.set_listener_property_fv(m_ptr.get(), AL_POSITION, glm::value_ptr(position));
}

//

glm::vec3 tr::audio_context::listener_velocity() const
{
	glm::vec3 velocity;
	m_alapi.get_listener_property_fv(m_ptr.get(), AL_VELOCITY, glm::value_ptr(velocity));
	return velocity;
}

void tr::audio_context::set_listener_velocity(const glm::vec3& velocity)
{
	m_alapi.set_listener_property_fv(m_ptr.get(), AL_VELOCITY, glm::value_ptr(velocity));
}

//

tr::orientation tr::audio_context::listener_orientation() const
{
	orientation orientation;
	m_alapi.get_listener_property_fv(m_ptr.get(), AL_ORIENTATION, &orientation.view.x);
	return orientation;
}

void tr::audio_context::set_listener_orientation(orientation orientation)
{
	m_alapi.set_listener_property_fv(m_ptr.get(), AL_ORIENTATION, &orientation.view.x);
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