#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"
#include "../../include/tr/audio/source.hpp"
#include "../../include/tr/audio/system.hpp"
#include "../../include/tr/utility/macro.hpp"
#include "../../include/tr/utility/ranges.hpp"
#include <AL/alext.h>

using namespace std::chrono_literals;

namespace tr::audio {
	// The global audio device.
	ALCdevice* device{nullptr};
	// The global audio context.
	ALCcontext* context{nullptr};

	// OpenAL context attributes.
	constexpr std::array<ALCint, 3> CONTEXT_ATTRIBUTES{ALC_HRTF_SOFT, ALC_FALSE, 0};
} // namespace tr::audio

tr::audio::init_error::init_error(std::string_view description)
	: m_description{description}
{
}

std::string_view tr::audio::init_error::name() const
{
	return "Audio system initialization error";
}

std::string_view tr::audio::init_error::description() const
{
	return m_description;
}

std::string_view tr::audio::init_error::details() const
{
	return {};
}

void tr::audio::initialize()
{
	TR_ASSERT(context == nullptr, "Tried to initialize an already initialized audio system.");

	if ((device = alcOpenDevice(nullptr)) == nullptr) {
		throw init_error{"Failed to open audio device."};
	}
	if ((context = alcCreateContext(device, CONTEXT_ATTRIBUTES.data())) == nullptr || !alcMakeContextCurrent(context)) {
		alcDestroyContext(context);
		alcCloseDevice(device);
		throw init_error{"Failed to create audio context."};
	}
	int al_max_sources;
	alcGetIntegerv(device, ALC_MONO_SOURCES, 1, &al_max_sources);
	max_sources = usize(al_max_sources);
	gains.fill(1);
}

bool tr::audio::active()
{
	return context != nullptr;
}

void tr::audio::shut_down()
{
	if (thread.joinable()) {
		thread.request_stop();
		thread.join();
	}

	commands.clear();

#ifdef TR_ENABLE_ASSERTS
	for (const std::shared_ptr<source_base>& ptr : sources) {
		TR_ASSERT(ptr.use_count() == 1, "Tried to shut down audio system while one or more audio sources still exists.");
	}
#endif
	sources.clear();
#ifdef TR_ENABLE_ASSERTS
	for (auto& [buffer, cullable] : buffers) {
		TR_ASSERT(cullable, "Tried to shut down audio system while one or more audio buffers still exists.");
		TR_AL_CALL(alDeleteBuffers, 1, &buffer);
	}
#endif
	buffers.clear();

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);

	context = nullptr;
}

//

float tr::audio::master_gain()
{
	TR_ASSERT(active(), "Tried to get master gain before initializing the audio system.");

	float gain;
	TR_AL_CALL(alGetListenerf, AL_GAIN, &gain);
	return gain;
}

float tr::audio::class_gain(int id)
{
	TR_ASSERT(active(), "Tried to get class gain before initializing the audio system.");

	return gains[id];
}

void tr::audio::set_master_gain(float gain)
{
	TR_ASSERT(active(), "Tried to set master gain before initializing the audio system.");
	TR_ASSERT(gain >= 0.0f, "Tried to set master gain to {}, while minimum allowed is 0.", gain);

	TR_AL_CALL(alListenerf, AL_GAIN, gain);
}

void tr::audio::set_class_gain(int id, float gain)
{
	TR_ASSERT(active(), "Tried to set class gain before initializing the audio system.");

	gains[id] = gain;
	for (source_base& source : deref(sources)) {
		if (source.classes()[id]) {
			source.set_gain(source.gain());
		}
	}
}

//

glm::vec3 tr::audio::listener_pos()
{
	TR_ASSERT(active(), "Tried to get the listener position before initializing the audio system.");

	glm::vec3 pos;
	TR_AL_CALL(alGetListenerfv, AL_POSITION, value_ptr(pos));
	return pos;
}

void tr::audio::set_listener_pos(const glm::vec3& pos)
{
	TR_ASSERT(active(), "Tried to set the listener position before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_POSITION, value_ptr(pos));
}

glm::vec3 tr::audio::listener_vel()
{
	TR_ASSERT(active(), "Tried to get the listener velocity before initializing the audio system.");

	glm::vec3 vel;
	TR_AL_CALL(alGetListenerfv, AL_VELOCITY, value_ptr(vel));
	return vel;
}

void tr::audio::set_listener_vel(const glm::vec3& vel)
{
	TR_ASSERT(active(), "Tried to set the listener velocity before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_VELOCITY, value_ptr(vel));
}

tr::audio::orientation tr::audio::listener_orientation()
{
	TR_ASSERT(active(), "Tried to get the listener orientation before initializing the audio system.");

	orientation orientation;
	TR_AL_CALL(alGetListenerfv, AL_ORIENTATION, &orientation.view.x);
	return orientation;
}

void tr::audio::set_listener_orientation(orientation orientation)
{
	TR_ASSERT(active(), "Tried to set the listener orientation before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_ORIENTATION, &orientation.view.x);
}

//

void tr::audio::thread_fn(std::stop_token stoken)
{
	TR_LOG(log, tr::severity::INFO, "Launched audio thread.");
	while (!stoken.stop_requested()) {
		try {
			std::lock_guard lock{mutex};

			for (auto buffer_it = buffers.begin(); buffer_it != buffers.end();) {
				auto& [buffer, cullable]{*buffer_it};
				const auto using_buffer{[&](auto& s) { return s->buffer() == buffer; }};

				if (cullable && std::ranges::none_of(sources, using_buffer)) {
					TR_AL_CALL(alDeleteBuffers, 1, &buffer);
					buffer_it = buffers.erase(buffer_it);
				}
				else {
					++buffer_it;
				}
			}

			for (std::list<std::shared_ptr<source_base>>::iterator it = sources.begin(); it != sources.end();) {
				source_base& source{**it};

				if (it->use_count() == 1 && source.state() != state::PLAYING) {
					const auto using_source{[&](command& command) { return command.source() == *it; }};

					commands.remove_if(using_source);
					it = sources.erase(it);
					continue;
				}

				if (!source.m_stream.has_value()) {
					++it;
					continue;
				}

				buffer_stream& stream{*source.m_stream};

				ALint nbuffers;
				TR_AL_CALL(alGetSourcei, source.m_id, AL_BUFFERS_PROCESSED, &nbuffers);
				if (nbuffers > 0) {
					std::array<ALuint, 4> ids{};
					TR_AL_CALL(alSourceUnqueueBuffers, source.m_id, nbuffers, ids.data());
					for (int i = 0; i < nbuffers; ++i) {
						if (!stream.stream->looping() && stream.stream->tell() == stream.stream->length()) {
							nbuffers = i;
							break;
						}
						refill(*stream.stream, *std::ranges::find(stream.buffers, ids[i], &buffer_stream_buffer::id));
					}
					if (nbuffers > 0) {
						TR_AL_CALL(alSourceQueueBuffers, source.m_id, nbuffers, ids.data());
					}
				}
				++it;
			}

			for (std::list<command>::iterator it = commands.begin(); it != commands.end();) {
				it->execute();
				if (it->done()) {
					it = commands.erase(it);
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