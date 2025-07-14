#include "../../include/tr/audio/audio_buffer.hpp"
#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"
#include <tr/sysgfx.hpp>

tr::audio_buffer::audio_buffer() noexcept
{
	ALuint id;
	TR_AL_CALL(alGenBuffers, 1, &id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		TR_TERMINATE("Out of memory", "Exception occurred during audio buffer allocation.");
	}
	_id.reset(id);
	_audio_buffers_cullable.emplace(id, false);
	if (!_audio_thread.joinable()) {
		_audio_thread = std::jthread{_audio_thread_loop};
	}
}

tr::audio_buffer::audio_buffer(std::span<const std::int16_t> data, audio_format format, int frequency) noexcept
	: audio_buffer{}
{
	set(data, format, frequency);
}

void tr::audio_buffer::_deleter::operator()(unsigned int id) const noexcept
{
	_audio_buffers_cullable.at(id) = true;
}

std::size_t tr::audio_buffer::size() const noexcept
{
	ALint size;
	TR_AL_CALL(alGetBufferi, _id.get(), AL_SIZE, &size);
	return size;
}

tr::fsecs tr::audio_buffer::length() const noexcept
{
	ALint sample_rate;
	TR_AL_CALL(alGetBufferi, _id.get(), AL_FREQUENCY, &sample_rate);
	return sample_rate == 0 ? fsecs::zero() : fsecs{static_cast<double>(size()) / sample_rate};
}

void tr::audio_buffer::set(std::span<const std::int16_t> data, audio_format format, int frequency) noexcept
{
	TR_AL_CALL(alBufferData, _id.get(), static_cast<ALenum>(format), data.data(), static_cast<ALsizei>(data.size_bytes()),
			   static_cast<ALsizei>(frequency));
	if (alGetError() == AL_OUT_OF_MEMORY) {
		TR_TERMINATE("Out of memory", "Exception occurred during audio buffer allocation.");
	}
}

tr::audio_buffer tr::load_audio_file(const std::filesystem::path& path)
{
	try {
		std::unique_ptr<audio_stream> file{open_audio_file(path)};
		std::vector<std::int16_t> data(file->length());
		file->read(data.data(), data.size());
		return audio_buffer{data, file->channels() == 2 ? audio_format::STEREO16 : audio_format::MONO16, file->sample_rate()};
	}
	catch (std::bad_alloc&) {
		TR_TERMINATE("Out of memory", "Exception occurred during audio file loading.");
	}
}