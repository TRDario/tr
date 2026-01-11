///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements stream.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/audio/stream.hpp"
#include <vorbis/vorbisfile.h>

////////////////////////////////////////////////////////// AUDIO FILE OPEN ERROR //////////////////////////////////////////////////////////

tr::audio::file_open_error::file_open_error(std::string&& description)
	: m_description{std::move(description)}
{
}

std::string_view tr::audio::file_open_error::name() const
{
	return "Audio file opening error";
}

std::string_view tr::audio::file_open_error::description() const
{
	return m_description;
}

std::string_view tr::audio::file_open_error::details() const
{
	return {};
}

//////////////////////////////////////////////////////////// OGG AUDIO STREAM /////////////////////////////////////////////////////////////

namespace tr::audio {
	// Ogg audio file backend.
	class ogg_stream : public stream {
	  public:
		// Loads an Ogg stream from file.
		ogg_stream(const std::filesystem::path& path);
		~ogg_stream();

		usize length() const override;
		int channels() const override;
		int sample_rate() const override;

		usize tell() const override;
		void seek(usize where) override;
		void raw_read(std::span<i16> buffer) override;

	  private:
		// A handle to the Ogg file.
		mutable OggVorbis_File m_file{};
	};
} // namespace tr::audio

tr::audio::ogg_stream::ogg_stream(const std::filesystem::path& path)
{
	const int result{ov_fopen(TR_PATH_CSTR(path), &m_file)};
	if (result != 0) {
		switch (result) {
		case OV_EREAD:
			throw file_open_error{TR_FMT::format("Failed to read .ogg file from '{}'.", path.string())};
		case OV_ENOTVORBIS:
			throw file_open_error{TR_FMT::format("Invalid .ogg Vorbis file '{}'.", path.string())};
		case OV_EVERSION:
			throw file_open_error{TR_FMT::format(".ogg Vorbis version mismatch in '{}'.", path.string())};
		case OV_EBADHEADER:
			throw file_open_error{TR_FMT::format("Invalid .ogg Vorbis header in '{}'.", path.string())};
		case OV_EFAULT:
			throw file_open_error{TR_FMT::format("An internal error in Vorbis occurred while loading '{}'.", path.string())};
		}
	}

	const vorbis_comment& comments{*ov_comment(&m_file, -1)};
	for (int i = 0; i < comments.comments; ++i) {
		const std::string_view comment{comments.user_comments[i], usize(comments.comment_lengths[i])};
		if (comment.starts_with("LOOPSTART=")) {
			usize loop_start{UNKNOWN_LOOP_POINT};
			std::from_chars(comment.data() + 10, comment.data() + comment.size(), loop_start);
			if (loop_start != UNKNOWN_LOOP_POINT) {
				set_looping(true);
				set_loop_start(loop_start);
			}
		}
		else if (comment.starts_with("LOOPEND=")) {
			usize loop_end{UNKNOWN_LOOP_POINT};
			std::from_chars(comment.data() + 8, comment.data() + comment.size(), loop_end);
			if (loop_end != UNKNOWN_LOOP_POINT) {
				set_looping(true);
				set_loop_end(loop_end);
			}
		}
		else if (comment.starts_with("LOOP=")) {
			set_looping(true);
		}
	}
}

tr::audio::ogg_stream::~ogg_stream()
{
	ov_clear(&m_file);
}

tr::usize tr::audio::ogg_stream::length() const
{
	return usize(ov_pcm_total(&m_file, -1));
}

int tr::audio::ogg_stream::channels() const
{
	return ov_info(&m_file, -1)->channels;
}

int tr::audio::ogg_stream::sample_rate() const
{
	return int(ov_info(&m_file, -1)->rate);
}

tr::usize tr::audio::ogg_stream::tell() const
{
	return usize(ov_pcm_tell(&m_file));
}

void tr::audio::ogg_stream::seek(usize where)
{
	ov_pcm_seek(&m_file, ogg_int64_t(where));
}

void tr::audio::ogg_stream::raw_read(std::span<i16> buffer)
{
	char* raw_dest{(char*)buffer.data()};
	int bytes_left{int(buffer.size_bytes())};
	int cur_section;
	while (bytes_left > 0) {
		const int read_bytes{int(ov_read(&m_file, raw_dest, bytes_left, 0, 2, 1, &cur_section))};
		if (read_bytes <= 0) {
			return;
		}
		bytes_left -= read_bytes;
		raw_dest += read_bytes;
	}
}

////////////////////////////////////////////////////////////// AUDIO STREAM ///////////////////////////////////////////////////////////////

tr::audio::stream::stream()
	: m_looping{false}, m_loop_start{0}, m_loop_end{UNKNOWN_LOOP_POINT}
{
}

std::span<tr::i16> tr::audio::stream::read(std::span<i16> buffer)
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

bool tr::audio::stream::looping() const
{
	return m_looping;
}

void tr::audio::stream::set_looping(bool looping)
{
	m_looping = looping;
	if (looping && tell() >= loop_end()) {
		seek(loop_start());
	}
}

tr::usize tr::audio::stream::loop_start() const
{
	return m_loop_start;
}

void tr::audio::stream::set_loop_start(usize loop_start)
{
	m_loop_start = std::clamp(loop_start, 0_uz, loop_end() - 1);
}

tr::usize tr::audio::stream::loop_end() const
{
	if (m_loop_end == UNKNOWN_LOOP_POINT) {
		m_loop_end = length();
	}
	return m_loop_end;
}

void tr::audio::stream::set_loop_end(usize loop_end)
{
	m_loop_end = std::clamp(loop_end, loop_start() + 1, length());
	if (looping() && tell() >= m_loop_end) {
		seek(loop_start());
	}
}

std::unique_ptr<tr::audio::stream> tr::audio::open_file(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path)) {
		throw file_open_error{TR_FMT::format("File not found: '{}'", path.string())};
	}

	const std::string extension{path.extension().string()};
	if (extension == ".ogg") {
		return std::make_unique<ogg_stream>(path);
	}
	else {
		throw file_open_error{TR_FMT::format("Unsupported audio file extension '{}'", extension)};
	}
}