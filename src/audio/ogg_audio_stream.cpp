/// @file
/// @brief Implements internal/ogg_audio_stream.hpp.

#include "internal/ogg_audio_stream.hpp"
#include <tr/audio/exception.hpp>

//

tr::ogg_audio_stream::ogg_audio_stream(const std::filesystem::path& path)
{
	const int result{ov_fopen(TR_PATH_CSTR(path), &m_file)};
	if (result != 0) {
		switch (result) {
		case OV_EREAD:
			throw audio_file_open_error{std::format("Failed to read .ogg file from '{}'.", path.string())};
		case OV_ENOTVORBIS:
			throw audio_file_open_error{std::format("Invalid .ogg Vorbis file '{}'.", path.string())};
		case OV_EVERSION:
			throw audio_file_open_error{std::format(".ogg Vorbis version mismatch in '{}'.", path.string())};
		case OV_EBADHEADER:
			throw audio_file_open_error{std::format("Invalid .ogg Vorbis header in '{}'.", path.string())};
		case OV_EFAULT:
			throw audio_file_open_error{std::format("An internal error in Vorbis occurred while loading '{}'.", path.string())};
		}
	}

	const vorbis_comment& comments{*ov_comment(&m_file, -1)};
	for (int i = 0; i < comments.comments; ++i) {
		const std::string_view comment{comments.user_comments[i], tr::usize(comments.comment_lengths[i])};
		if (comment.starts_with("LOOPSTART=")) {
			tr::usize loop_start{unknown_loop_point};
			std::from_chars(comment.data() + 10, comment.data() + comment.size(), loop_start);
			if (loop_start != unknown_loop_point) {
				set_looping(true);
				set_loop_start(loop_start);
			}
		}
		else if (comment.starts_with("LOOPEND=")) {
			tr::usize loop_end{unknown_loop_point};
			std::from_chars(comment.data() + 8, comment.data() + comment.size(), loop_end);
			if (loop_end != unknown_loop_point) {
				set_looping(true);
				set_loop_end(loop_end);
			}
		}
		else if (comment.starts_with("LOOP=")) {
			set_looping(true);
		}
	}
}

tr::ogg_audio_stream::~ogg_audio_stream() noexcept
{
	ov_clear(&m_file);
}

//

tr::usize tr::ogg_audio_stream::length() const
{
	return ov_pcm_total(&m_file, -1);
}

int tr::ogg_audio_stream::channels() const
{
	return ov_info(&m_file, -1)->channels;
}

int tr::ogg_audio_stream::sample_rate() const
{
	return ov_info(&m_file, -1)->rate;
}

//

tr::usize tr::ogg_audio_stream::tell() const
{
	return ov_pcm_tell(&m_file);
}

void tr::ogg_audio_stream::seek(tr::usize where)
{
	ov_pcm_seek(&m_file, where);
}

//

void tr::ogg_audio_stream::raw_read(std::span<tr::i16> buffer)
{
	char* raw_dest{reinterpret_cast<char*>(buffer.data())};
	int bytes_left{static_cast<int>(buffer.size_bytes())};
	int cur_section;
	while (bytes_left > 0) {
		const long read_bytes{ov_read(&m_file, raw_dest, bytes_left, 0, 2, 1, &cur_section)};
		if (read_bytes <= 0) {
			return;
		}
		bytes_left -= read_bytes;
		raw_dest += read_bytes;
	}
}