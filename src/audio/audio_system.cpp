#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/audio_source.hpp"
#include "../../include/tr/audio/audio_system.hpp"
#include "../../include/tr/audio/impl.hpp"
#include "../../include/tr/utility/macro.hpp"
#include "../../include/tr/utility/ranges.hpp"

using namespace std::chrono_literals;

namespace tr {
	// The global audio device.
	ALCdevice* _device{nullptr};
	// The global audio context.
	ALCcontext* _context{nullptr};
} // namespace tr

tr::audio_system_init_error::audio_system_init_error(std::string_view location, std::string_view description) noexcept
	: exception{location}, _description{description}
{
}

std::string_view tr::audio_system_init_error::name() const noexcept
{
	return "Audio system initialization error";
}

std::string_view tr::audio_system_init_error::description() const noexcept
{
	return _description;
}

std::string_view tr::audio_system_init_error::details() const noexcept
{
	return {};
}

void tr::audio_system::initialize()
{
	TR_ASSERT(_context == nullptr, "Tried to initialize an already initialized audio system.");

	if ((_device = alcOpenDevice(nullptr)) == nullptr) {
		TR_THROW(audio_system_init_error, "Failed to open audio device.");
	}
	if ((_context = alcCreateContext(_device, nullptr)) == nullptr || !alcMakeContextCurrent(_context)) {
		alcDestroyContext(_context);
		alcCloseDevice(_device);
		TR_THROW(audio_system_init_error, "Failed to create audio context.");
	}

	_audio_gains.fill(1);
}

bool tr::audio_system::active() noexcept
{
	return _context != nullptr;
}

void tr::audio_system::shut_down() noexcept
{
	if (_audio_thread.joinable()) {
		_audio_thread.request_stop();
		_audio_thread.join();
	}

	_audio_commands.clear();

#ifdef TR_ENABLE_ASSERTS
	for (const std::shared_ptr<_audio_source>& ptr : _audio_sources) {
		TR_ASSERT(ptr.use_count() == 1, "Tried to shut down audio system while one or more audio sources still exists.");
	}
#endif
	_audio_sources.clear();

	for (auto& [buffer, cullable] : _audio_buffers_cullable) {
		TR_ASSERT(cullable, "Tried to shut down audio system while one or more audio buffers still exists.");
		alDeleteBuffers(1, &buffer);
	}
	_audio_buffers_cullable.clear();

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(_context);
	alcCloseDevice(_device);

	_context = nullptr;
}

//

float tr::audio_system::master_gain() noexcept
{
	TR_ASSERT(active(), "Tried to get master gain before initializing the audio system.");

	float gain;
	TR_AL_CALL(alGetListenerf, AL_GAIN, &gain);
	return gain;
}

float tr::audio_system::class_gain(int id) noexcept
{
	TR_ASSERT(active(), "Tried to get class gain before initializing the audio system.");

	return _audio_gains[id];
}

void tr::audio_system::set_master_gain(float gain) noexcept
{
	TR_ASSERT(active(), "Tried to set master gain before initializing the audio system.");
	TR_ASSERT(gain >= 0.0f, "Tried to set master gain to {}, while minimum allowed is 0.", gain);

	TR_AL_CALL(alListenerf, AL_GAIN, gain);
}

void tr::audio_system::set_class_gain(int id, float gain) noexcept
{
	TR_ASSERT(active(), "Tried to set class gain before initializing the audio system.");

	_audio_gains[id] = gain;
	for (_audio_source& source : deref(_audio_sources)) {
		if (source.classes()[id]) {
			source.set_gain(source.gain());
		}
	}
}

//

glm::vec3 tr::audio_system::listener_pos() noexcept
{
	TR_ASSERT(active(), "Tried to get the listener position before initializing the audio system.");

	glm::vec3 pos;
	TR_AL_CALL(alGetListenerfv, AL_POSITION, value_ptr(pos));
	return pos;
}

void tr::audio_system::set_listener_pos(const glm::vec3& pos) noexcept
{
	TR_ASSERT(active(), "Tried to set the listener position before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_POSITION, value_ptr(pos));
}

glm::vec3 tr::audio_system::listener_vel() noexcept
{
	TR_ASSERT(active(), "Tried to get the listener velocity before initializing the audio system.");

	glm::vec3 vel;
	TR_AL_CALL(alGetListenerfv, AL_VELOCITY, value_ptr(vel));
	return vel;
}

void tr::audio_system::set_listener_vel(const glm::vec3& vel) noexcept
{
	TR_ASSERT(active(), "Tried to set the listener velocity before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_VELOCITY, value_ptr(vel));
}

tr::orientation tr::audio_system::listener_orientation() noexcept
{
	TR_ASSERT(active(), "Tried to get the listener orientation before initializing the audio system.");

	orientation orientation;
	TR_AL_CALL(alGetListenerfv, AL_ORIENTATION, &orientation.view.x);
	return orientation;
}

void tr::audio_system::set_listener_orientation(orientation orientation) noexcept
{
	TR_ASSERT(active(), "Tried to set the listener orientation before initializing the audio system.");

	TR_AL_CALL(alListenerfv, AL_ORIENTATION, &orientation.view.x);
}

bool tr::audio_system::can_allocate_audio_source(int priority) noexcept
{
	if (_audio_sources.size() < 128) {
		return true;
	}
	const auto it{std::ranges::find_if(_audio_sources, [&](auto& s) { return s.use_count() == 1 && s->priority() <= priority; })};
	return it != _audio_sources.end();
}

//

void tr::_audio_thread_loop(std::stop_token stoken) noexcept
{
	while (!stoken.stop_requested()) {
		try {
			std::lock_guard lock{_audio_mutex};

			for (auto it = _audio_buffers_cullable.begin(); it != _audio_buffers_cullable.end();) {
				auto& [buffer, cullable]{*it};
				if (cullable && std::ranges::none_of(_audio_sources, [&](auto& s) { return s->buffer() == buffer; })) {
					alDeleteBuffers(1, &buffer);
					it = _audio_buffers_cullable.erase(it);
				}
				else {
					++it;
				}
			}

			for (std::list<std::shared_ptr<_audio_source>>::iterator it = _audio_sources.begin(); it != _audio_sources.end();) {
				_audio_source& source{**it};

				if (it->use_count() == 1 && source.state() != audio_state::PLAYING) {
					_audio_commands.remove_if([&](_audio_command& command) { return command.source() == *it; });
					it = _audio_sources.erase(it);
					continue;
				}

				if (!source._stream.has_value()) {
					++it;
					continue;
				}

				_buffer_stream& stream{*source._stream};

				ALint nbuffers;
				TR_AL_CALL(alGetSourcei, source._id, AL_BUFFERS_PROCESSED, &nbuffers);
				if (nbuffers > 0) {
					std::array<ALuint, 4> buffers{};
					TR_AL_CALL(alSourceUnqueueBuffers, source._id, nbuffers, buffers.data());
					for (int i = 0; i < nbuffers; ++i) {
						stream.buffers[i].refill(stream);
						if (stream.stream->tell() == stream.stream->length()) {
							nbuffers = i + 1;
						}
					}
					TR_AL_CALL(alSourceQueueBuffers, source._id, nbuffers, buffers.data());
				}
				++it;
			}

			for (std::list<_audio_command>::iterator it = _audio_commands.begin(); it != _audio_commands.end();) {
				it->execute();
				if (it->done()) {
					it = _audio_commands.erase(it);
				}
				else {
					++it;
				}
			}
		}
		catch (...) {
			// Gracefully exit.
		}
		std::this_thread::sleep_for(10ms);
	}
}