/// @file
/// @brief Implements audio_stream.hpp.

#include "internal/ogg_audio_stream.hpp"
#include <tr/audio/audio_stream.hpp>
#include <tr/audio/exception.hpp>
#include <vorbis/vorbisfile.h>

//

tr::audio_stream::audio_stream() noexcept
	: m_looping{false}
	, m_loop_start{0}
	, m_loop_end{unknown_loop_point}
{
}

//

std::span<tr::i16> tr::audio_stream::read(std::span<i16> buffer)
{
	if (m_looping) {
		std::span<i16> remaining_buffer{buffer};
		while (true) {
			const usize samples_until_loop{(loop_end() - tell()) * channels()};
			if (samples_until_loop < remaining_buffer.size()) {
				raw_read(remaining_buffer.subspan(0, samples_until_loop));
				remaining_buffer = remaining_buffer.subspan(samples_until_loop);
				seek(loop_start());
			}
			else {
				raw_read(remaining_buffer);
				return buffer;
			}
		}
	}
	else {
		buffer = buffer.subspan(0, std::min((length() - tell()) * channels(), buffer.size()));
		raw_read(buffer);
		return buffer;
	}
}

//

bool tr::audio_stream::looping() const
{
	return m_looping;
}

void tr::audio_stream::set_looping(bool looping)
{
	m_looping = looping;
	if (looping && tell() >= loop_end()) {
		seek(loop_start());
	}
}

//

tr::usize tr::audio_stream::loop_start() const
{
	return m_loop_start;
}

void tr::audio_stream::set_loop_start(usize loop_start)
{
	m_loop_start = std::clamp(loop_start, 0uz, loop_end() - 1);
}

//

tr::usize tr::audio_stream::loop_end() const
{
	if (m_loop_end == unknown_loop_point) {
		m_loop_end = length();
	}
	return m_loop_end;
}

void tr::audio_stream::set_loop_end(usize loop_end)
{
	m_loop_end = std::clamp(loop_end, loop_start() + 1, length());
	if (looping() && tell() >= m_loop_end) {
		seek(loop_start());
	}
}

//

std::unique_ptr<tr::audio_stream> tr::open_audio_file(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path)) {
		throw audio_file_open_error{std::format("File not found: '{}'", path.string())};
	}

	const std::string extension{path.extension().string()};
	if (extension == ".ogg") {
		return std::make_unique<ogg_audio_stream>(path);
	}
	else {
		throw audio_file_open_error{std::format("Unsupported audio file extension '{}'", extension)};
	}
}