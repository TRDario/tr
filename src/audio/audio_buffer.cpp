#include "../../include/tr/audio/audio_buffer.hpp"
#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"

tr::audio_buffer::audio_buffer()
{
	ALuint id;
	TR_AL_CALL(alGenBuffers, 1, &id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
	this->id.reset(id);
	audio_buffers.emplace(id, false);
	if (!audio_thread.joinable()) {
		audio_thread = std::jthread{audio_thread_loop};
	}
}

tr::audio_buffer::audio_buffer(std::span<const std::int16_t> data, audio_format format, int frequency)
	: audio_buffer{}
{
	set(data, format, frequency);
}

void tr::audio_buffer::deleter::operator()(unsigned int id) const
{
	audio_buffers.at(id) = true;
}

std::size_t tr::audio_buffer::size() const
{
	ALint size;
	TR_AL_CALL(alGetBufferi, id.get(), AL_SIZE, &size);
	return size;
}

tr::fsecs tr::audio_buffer::length() const
{
	ALint sample_rate;
	TR_AL_CALL(alGetBufferi, id.get(), AL_FREQUENCY, &sample_rate);
	return sample_rate == 0 ? fsecs::zero() : fsecs{static_cast<double>(size()) / sample_rate};
}

void tr::audio_buffer::set(std::span<const std::int16_t> data, audio_format format, int frequency)
{
	const ALsizei size{static_cast<ALsizei>(data.size_bytes()) - static_cast<ALsizei>(data.size_bytes()) % 4};
	TR_AL_CALL(alBufferData, id.get(), static_cast<ALenum>(format), data.data(), size, static_cast<ALsizei>(frequency));
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
}

tr::audio_buffer tr::load_audio_file(const std::filesystem::path& path)
{
	std::unique_ptr<audio_stream> file{open_audio_file(path)};
	std::vector<std::int16_t> data(file->length() * file->channels());
	file->read(data);
	return audio_buffer{data, file->channels() == 2 ? audio_format::STEREO16 : audio_format::MONO16, file->sample_rate()};
}