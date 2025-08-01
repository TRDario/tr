#include "../../include/tr/audio/buffer.hpp"
#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"

tr::audio::buffer::buffer()
{
	ALuint id;
	TR_AL_CALL(alGenBuffers, 1, &id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
	m_id.reset(id);
	buffers.emplace(id, false);
	if (!thread.joinable()) {
		thread = std::jthread{thread_fn};
	}
}

tr::audio::buffer::buffer(std::span<const std::int16_t> data, format format, int frequency)
	: buffer{}
{
	set(data, format, frequency);
}

void tr::audio::buffer::deleter::operator()(unsigned int id) const
{
	buffers.at(id) = true;
}

std::size_t tr::audio::buffer::size() const
{
	ALint size;
	TR_AL_CALL(alGetBufferi, m_id.get(), AL_SIZE, &size);
	return size;
}

tr::fsecs tr::audio::buffer::length() const
{
	ALint sample_rate;
	TR_AL_CALL(alGetBufferi, m_id.get(), AL_FREQUENCY, &sample_rate);
	return sample_rate == 0 ? fsecs::zero() : fsecs{static_cast<double>(size()) / sample_rate};
}

void tr::audio::buffer::set(std::span<const std::int16_t> data, format format, int frequency)
{
	const ALsizei size{static_cast<ALsizei>(data.size_bytes()) - static_cast<ALsizei>(data.size_bytes()) % 4};
	TR_AL_CALL(alBufferData, m_id.get(), static_cast<ALenum>(format), data.data(), size, static_cast<ALsizei>(frequency));
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
}

tr::audio::buffer tr::audio::load_file(const std::filesystem::path& path)
{
	std::unique_ptr<stream> file{open_file(path)};
	std::vector<std::int16_t> data(file->length() * file->channels());
	file->read(data);
	return buffer{data, file->channels() == 2 ? format::STEREO16 : format::MONO16, file->sample_rate()};
}