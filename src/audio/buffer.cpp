///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements buffer.hpp and buffers from audio/impl.hpp.                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/audio/buffer.hpp"
#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/audio/impl.hpp"

////////////////////////////////////////////////////////////// OWNING BUFFER //////////////////////////////////////////////////////////////

tr::audio::owning_buffer::owning_buffer()
{
	ALuint al_id;
	TR_AL_CALL(alGenBuffers, 1, &al_id);
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
	m_handle.reset(id{al_id});
}

tr::audio::owning_buffer::operator id() const
{
	return m_handle.get(tr::no_empty_handle_check);
}

void tr::audio::owning_buffer::deleter::operator()(id id) const
{
	alDeleteBuffers(1, (ALuint*)&id);
}

tr::usize tr::audio::owning_buffer::hasher::operator()(id v) const
{
	return std::hash<id>{}(v);
}

////////////////////////////////////////////////////////// BUFFERED STREAM BUFFER /////////////////////////////////////////////////////////

void tr::audio::buffered_stream::buffer::refill_from(audio::stream& source)
{
	std::array<i16, 16384> data;

	start_offset = source.tell();
	const std::span<const i16> used_data{source.read(data)};
	const ALenum format{source.channels() == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16};
	const ALsizei size{ALsizei(used_data.size_bytes()) - ALsizei(used_data.size_bytes()) % 4};
	TR_AL_CALL(alBufferData, ALuint(owning_buffer::id(*this)), format, used_data.data(), size, source.sample_rate());
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer reallocation"};
	}
}

////////////////////////////////////////////////////////////////// BUFFER /////////////////////////////////////////////////////////////////

tr::audio::buffer::buffer()
	: m_id{g_manager.allocate_buffer()}
{
}

tr::audio::buffer::buffer(std::span<const i16> data, format format, int frequency)
	: buffer{}
{
	set(data, format, frequency);
}

void tr::audio::buffer::deleter::operator()(id id) const
{
	g_manager.mark_buffer_as_cullable(id);
}

tr::usize tr::audio::buffer::size() const
{
	ALint size;
	TR_AL_CALL(alGetBufferi, ALuint(m_id.get()), AL_SIZE, &size);
	return size;
}

tr::fsecs tr::audio::buffer::length() const
{
	ALint sample_rate;
	TR_AL_CALL(alGetBufferi, ALuint(m_id.get()), AL_FREQUENCY, &sample_rate);
	return sample_rate == 0 ? fsecs::zero() : fsecs{double(size()) / sample_rate};
}

void tr::audio::buffer::set(std::span<const i16> data, format format, int frequency)
{
	const ALsizei size{ALsizei(data.size_bytes()) - ALsizei(data.size_bytes()) % 4};
	TR_AL_CALL(alBufferData, ALuint(m_id.get()), ALenum(format), data.data(), size, ALsizei(frequency));
	if (alGetError() == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
}

tr::audio::buffer tr::audio::load_file(const std::filesystem::path& path)
{
	std::unique_ptr<stream> file{open_file(path)};
	std::vector<i16> data(file->length() * file->channels());
	file->read(data);
	return buffer{data, file->channels() == 2 ? format::stereo16 : format::mono16, file->sample_rate()};
}