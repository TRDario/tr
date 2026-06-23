///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements audio_buffer.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/audio/audio_buffer.hpp"
#include "../../include/tr/audio/audio_context.hpp"
#include "../../include/tr/audio/audio_stream.hpp"
#include "../../include/tr/utility/exception.hpp"
#include <AL/al.h>
#include <AL/alext.h>

/////////////////////////////////////////////////////////////// AUDIO BUFFER //////////////////////////////////////////////////////////////

tr::audio_buffer::audio_buffer(audio_context& context)
	: m_handle{{context}}
{
	context.m_alapi.generate_buffers(context.m_ptr.get(), 1, out_handle(m_handle));
}

void tr::audio_buffer::deleter::operator()(unsigned int id) const
{
	context.m_alapi.delete_buffers(context.m_ptr.get(), 1, &id);
}

//

std::shared_ptr<tr::audio_buffer> tr::create_audio_buffer(audio_context& context)
{
	const std::lock_guard lock{context.m_mutex};
	return context.m_buffers.emplace_back(new tr::audio_buffer{context});
}

std::shared_ptr<tr::audio_buffer> tr::create_audio_buffer(audio_context& context, std::span<const i16> data, audio_format format,
														  int frequency)
{
	std::shared_ptr<tr::audio_buffer> buffer{create_audio_buffer(context)};
	buffer->set(data, format, frequency);
	return buffer;
}

std::shared_ptr<tr::audio_buffer> tr::load_audio_file(audio_context& context, const std::filesystem::path& path)
{
	std::unique_ptr<audio_stream> file{open_audio_file(path)};
	std::vector<i16> data(file->length() * file->channels());
	file->read(data);
	return create_audio_buffer(context, data, file->channels() == 2 ? audio_format::stereo16 : audio_format::mono16, file->sample_rate());
}

//

tr::audio_context& tr::audio_buffer::context() const
{
	return m_handle.get_deleter().context;
}

//

tr::usize tr::audio_buffer::size() const
{
	ALint size;
	audio_context& ctx{context()};
	ctx.m_alapi.get_buffer_property_i(ctx.m_ptr.get(), m_handle.get(), AL_SIZE, &size);
	return size / sizeof(i16);
}

tr::fsecs tr::audio_buffer::length() const
{
	const int sample_rate{this->sample_rate()};
	return sample_rate == 0 ? fsecs::zero() : fsecs{static_cast<double>(size()) / sample_rate / channels()};
}

int tr::audio_buffer::sample_rate() const
{
	ALint sample_rate;
	audio_context& ctx{context()};
	ctx.m_alapi.get_buffer_property_i(ctx.m_ptr.get(), m_handle.get(), AL_FREQUENCY, &sample_rate);
	return sample_rate;
}

int tr::audio_buffer::channels() const
{
	ALint channels;
	audio_context& ctx{context()};
	ctx.m_alapi.get_buffer_property_i(ctx.m_ptr.get(), m_handle.get(), AL_CHANNELS, &channels);
	return channels;
}

//

void tr::audio_buffer::set(std::span<const i16> data, audio_format format, int frequency)
{
	audio_context& ctx{context()};
	const ALsizei data_size{static_cast<ALsizei>(data.size_bytes() - data.size_bytes() % 4)};
	ctx.m_alapi.set_buffer_data(ctx.m_ptr.get(), m_handle.get(), static_cast<ALenum>(format), data.data(), data_size, frequency);
	if (ctx.m_alapi.get_error(ctx.m_ptr.get()) == AL_OUT_OF_MEMORY) {
		throw out_of_memory{"audio buffer allocation"};
	}
}

//

std::pair<tr::fsecs, tr::fsecs> tr::audio_buffer::loop_points() const
{
	const int sample_rate{this->sample_rate()};
	const int channels{this->channels()};
	const float samples_per_second{static_cast<float>(sample_rate * channels)};

	std::array<ALint, 2> loop_point_offsets{};
	audio_context& ctx{context()};
	ctx.m_alapi.get_buffer_property_iv(ctx.m_ptr.get(), m_handle.get(), AL_LOOP_POINTS_SOFT, loop_point_offsets.data());

	return {fsecs{loop_point_offsets[0] / samples_per_second}, fsecs{loop_point_offsets[1] / samples_per_second}};
}

void tr::audio_buffer::set_loop_points(fsecs start_point, fsecs end_point)
{
	const int sample_rate{this->sample_rate()};
	const int channels{this->channels()};
	const std::array<ALint, 2> loop_points{static_cast<ALint>(start_point.count() * sample_rate * channels),
										   static_cast<ALint>(end_point.count() * sample_rate * channels)};
	audio_context& ctx{context()};
	ctx.m_alapi.set_buffer_property_iv(ctx.m_ptr.get(), m_handle.get(), AL_LOOP_POINTS_SOFT, loop_points.data());
}