#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/audio_source.hpp"
#include "../../include/tr/audio/audio_system.hpp"
#include "../../include/tr/audio/impl.hpp"
#include "../../include/tr/utility/macro.hpp"
#include "../../include/tr/utility/ranges.hpp"

using namespace std::chrono_literals;

namespace tr {
	// The global audio device.
	ALCdevice* audio_device{nullptr};
	// The global audio context.
	ALCcontext* audio_context{nullptr};
} // namespace tr

tr::audio_system_init_error::audio_system_init_error(std::string_view description)
	: description_str{description}
{
}

std::string_view tr::audio_system_init_error::name() const
{
	return "Audio system initialization error";
}

std::string_view tr::audio_system_init_error::description() const
{
	return description_str;
}

std::string_view tr::audio_system_init_error::details() const
{
	return {};
}

void tr::audio_system::initialize()
{
	TR_ASSERT(audio_context == nullptr, "Tried to initialize an already initialized audio system.");

	if ((audio_device = alcOpenDevice(nullptr)) == nullptr) {
		throw audio_system_init_error{"Failed to open audio device."};
	}
	if ((audio_context = alcCreateContext(audio_device, nullptr)) == nullptr || !alcMakeContextCurrent(audio_context)) {
		alcDestroyContext(audio_context);
		alcCloseDevice(audio_device);
		throw audio_system_init_error{"Failed to create audio context."};
	}
	int max_sources;
	alcGetIntegerv(audio_device, ALC_MONO_SOURCES, 1, &max_sources);
	max_audio_sources = static_cast<std::size_t>(max_sources);
	audio_gains.fill(1);
}

bool tr::audio_system::active()
{
	return audio_context != nullptr;
}

void tr::audio_system::shut_down()
{
	if (audio_thread.joinable()) {
		audio_thread.request_stop();
		audio_thread.join();
	}

	audio_commands.clear();

#ifdef TR_ENABLE_ASSERTS
	for (const std::shared_ptr<base_audio_source>& ptr : audio_sources) {
		TR_ASSERT(ptr.use_count() == 1, "Tried to shut down audio system while one or more audio sources still exists.");
	}
#endif
	audio_sources.clear();

	for (auto& [buffer, cullable] : audio_buffers) {
		TR_ASSERT(cullable, "Tried to shut down audio system while one or more audio buffers still exists.");
		TR_AL_CALL(alDeleteBuffers, 1, &buffer);
	}
	audio_buffers.clear();

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(audio_context);
	alcCloseDevice(audio_device);

	audio_context = nullptr;
}

//

float tr::audio_system::master_gain()
{
	TR_ASSERT(active(), "Tried to get master gain before initializing the audio system.");

	float gain;
	TR_AL_CALL(alGetListenerf, AL_GAIN, &gain);
	return gain;
}

float tr::audio_system::class_gain(int id)
{
	TR_ASSERT(active(), "Tried to get class gain before initializing the audio system.");

	return audio_gains[id];
}

void tr::audio_system::set_master_gain(float gain)
{
	TR_ASSERT(active(), "Tried to set master gain before initializing the audio system.");
	TR_ASSERT(gain >= 0.0f, "Tried to set master gain to {}, while minimum allowed is 0.", gain);

	TR_AL_CALL(alListenerf, AL_GAIN, gain);
}

void tr::audio_system::set_class_gain(int id, float gain)
{
	TR_ASSERT(active(), "Tried to set class gain before initializing the audio system.");

	audio_gains[id] = gain;
	for (base_audio_source& source : deref(audio_sources)) {
		if (source.classes()[id]) {
			source.set_gain(source.gain());
		}
	}
}

//

glm::vec3 tr::audio_system::listener_pos()
{
	TR_ASSERT(active(), "Tried to get the listener position before initializing the audio system.");

	glm::vec3 pos;
	TR_AL_CALL(alGetListenerfv, AL_POSITION, value_ptr(pos));
	return pos;
}

void tr::audio_system::set_listener_pos(const glm::vec3& pos)
{
	TR_ASSERT(active(), "Tried to set the listener position before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_POSITION, value_ptr(pos));
}

glm::vec3 tr::audio_system::listener_vel()
{
	TR_ASSERT(active(), "Tried to get the listener velocity before initializing the audio system.");

	glm::vec3 vel;
	TR_AL_CALL(alGetListenerfv, AL_VELOCITY, value_ptr(vel));
	return vel;
}

void tr::audio_system::set_listener_vel(const glm::vec3& vel)
{
	TR_ASSERT(active(), "Tried to set the listener velocity before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_VELOCITY, value_ptr(vel));
}

tr::orientation tr::audio_system::listener_orientation()
{
	TR_ASSERT(active(), "Tried to get the listener orientation before initializing the audio system.");

	orientation orientation;
	TR_AL_CALL(alGetListenerfv, AL_ORIENTATION, &orientation.view.x);
	return orientation;
}

void tr::audio_system::set_listener_orientation(orientation orientation)
{
	TR_ASSERT(active(), "Tried to set the listener orientation before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_ORIENTATION, &orientation.view.x);
}

bool tr::audio_system::can_allocate_audio_source(int priority)
{
	if (audio_sources.size() < max_audio_sources) {
		return true;
	}
	const auto it{std::ranges::find_if(audio_sources, [&](auto& s) { return s.use_count() == 1 && s->priority() <= priority; })};
	return it != audio_sources.end();
}

//

void tr::audio_thread_loop(std::stop_token stoken)
{
	TR_LOG(log, tr::severity::INFO, "Launched audio thread.");
	while (!stoken.stop_requested()) {
		try {
			std::lock_guard lock{audio_mutex};

			for (auto it = audio_buffers.begin(); it != audio_buffers.end();) {
				auto& [buffer, cullable]{*it};
				if (cullable && std::ranges::none_of(audio_sources, [&](auto& s) { return s->buffer() == buffer; })) {
					TR_AL_CALL(alDeleteBuffers, 1, &buffer);
					it = audio_buffers.erase(it);
				}
				else {
					++it;
				}
			}

			for (std::list<std::shared_ptr<base_audio_source>>::iterator it = audio_sources.begin(); it != audio_sources.end();) {
				base_audio_source& source{**it};

				if (it->use_count() == 1 && source.state() != audio_state::PLAYING) {
					audio_commands.remove_if([&](audio_command& command) { return command.source() == *it; });
					it = audio_sources.erase(it);
					continue;
				}

				if (!source.stream.has_value()) {
					++it;
					continue;
				}

				buffer_stream& stream{*source.stream};

				ALint nbuffers;
				TR_AL_CALL(alGetSourcei, source.id, AL_BUFFERS_PROCESSED, &nbuffers);
				if (nbuffers > 0) {
					std::array<ALuint, 4> buffers{};
					TR_AL_CALL(alSourceUnqueueBuffers, source.id, nbuffers, buffers.data());
					for (int i = 0; i < nbuffers; ++i) {
						if (!stream.stream->looping() && stream.stream->tell() == stream.stream->length()) {
							nbuffers = i;
							break;
						}
						std::ranges::find(stream.buffers, buffers[i], &buffer_stream_buffer::id)->refill(stream);
					}
					if (nbuffers > 0) {
						TR_AL_CALL(alSourceQueueBuffers, source.id, nbuffers, buffers.data());
					}
				}
				++it;
			}

			for (std::list<audio_command>::iterator it = audio_commands.begin(); it != audio_commands.end();) {
				it->execute();
				if (it->done()) {
					it = audio_commands.erase(it);
				}
				else {
					++it;
				}
			}
		}
		catch (std::exception& err) {
			TR_LOG(log, tr::severity::ERROR, "Exception in audio thread, terminating.");
			TR_LOG_CONTINUE(log, "{}", err.what());
			return;
		}
		std::this_thread::sleep_for(10ms);
	}
	TR_LOG(log, tr::severity::INFO, "Returned from audio thread.");
}