#include "../../include/tr/audio/stream.hpp"
#include <vorbis/vorbisfile.h>

namespace tr::audio {
	// Ogg audio file backend.
	class ogg_stream : public stream {
	  public:
		ogg_stream(const std::filesystem::path& path);
		~ogg_stream();

		std::size_t length() const override;
		int channels() const override;
		int sample_rate() const override;

		std::size_t tell() const override;
		void seek(std::size_t where) override;
		void raw_read(std::span<std::int16_t> buffer) override;

	  private:
		OggVorbis_File file{};
	};

	constexpr std::size_t UNKNOWN_LOOP_POINT{std::numeric_limits<std::size_t>::max()};
} // namespace tr::audio

///////////////////////////////////////////////////////////// OGG AUDIO FILE //////////////////////////////////////////////////////////////

tr::audio::ogg_stream::ogg_stream(const std::filesystem::path& path)
{
#ifdef _WIN32
	const int result{ov_fopen(path.string().c_str(), &file)};
#else
	const int result{ov_fopen(path.c_str(), &file)};
#endif
	if (result != 0) {
		switch (result) {
		case OV_EREAD:
			throw file_open_error{std::format("Failed to read .ogg file from '{}'.", path.string())};
		case OV_ENOTVORBIS:
			throw file_open_error{std::format("Invalid .ogg Vorbis file '{}'.", path.string())};
		case OV_EVERSION:
			throw file_open_error{std::format(".ogg Vorbis version mismatch in '{}'.", path.string())};
		case OV_EBADHEADER:
			throw file_open_error{std::format("Invalid .ogg Vorbis header in '{}'.", path.string())};
		case OV_EFAULT:
			throw file_open_error{std::format("An internal error in Vorbis occurred while loading '{}'.", path.string())};
		}
	}

	const vorbis_comment& comments{*ov_comment(&file, -1)};
	for (int i = 0; i < comments.comments; ++i) {
		const std::string_view comment{comments.user_comments[i], static_cast<std::size_t>(comments.comment_lengths[i])};
		if (comment.starts_with("LOOPSTART=")) {
			std::size_t loop_start{UNKNOWN_LOOP_POINT};
			std::from_chars(comment.data() + 10, comment.data() + comment.size(), loop_start);
			if (loop_start != UNKNOWN_LOOP_POINT) {
				set_looping(true);
				set_loop_start(loop_start);
			}
		}
		else if (comment.starts_with("LOOPEND=")) {
			std::size_t loop_end{UNKNOWN_LOOP_POINT};
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
	ov_clear(&file);
}

std::size_t tr::audio::ogg_stream::length() const
{
	return static_cast<std::size_t>(ov_pcm_total(const_cast<OggVorbis_File*>(&file), -1));
}

int tr::audio::ogg_stream::channels() const
{
	return ov_info(const_cast<OggVorbis_File*>(&file), -1)->channels;
}

int tr::audio::ogg_stream::sample_rate() const
{
	return static_cast<int>(ov_info(const_cast<OggVorbis_File*>(&file), -1)->rate);
}

std::size_t tr::audio::ogg_stream::tell() const
{
	return static_cast<std::size_t>(ov_pcm_tell(const_cast<OggVorbis_File*>(&file)));
}

void tr::audio::ogg_stream::seek(std::size_t where)
{
	ov_pcm_seek(&file, static_cast<ogg_int64_t>(where));
}

void tr::audio::ogg_stream::raw_read(std::span<std::int16_t> buffer)
{
	char* raw_dest{reinterpret_cast<char*>(buffer.data())};
	int bytes_left{static_cast<int>(buffer.size_bytes())};
	int cur_section;
	while (bytes_left > 0) {
		const int read_bytes{static_cast<int>(ov_read(&file, raw_dest, bytes_left, 0, 2, 1, &cur_section))};
		if (read_bytes <= 0) {
			return;
		}
		bytes_left -= read_bytes;
		raw_dest += read_bytes;
	}
}

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

/////////////////////////////////////////////////////////////// AUDIO FILE ////////////////////////////////////////////////////////////////

tr::audio::stream::stream()
	: m_looping{false}, m_loop_start{0}, m_loop_end{UNKNOWN_LOOP_POINT}
{
}

std::span<std::int16_t> tr::audio::stream::read(std::span<std::int16_t> buffer)
{
	if (m_looping) {
		std::span<std::int16_t> remaining_buffer{buffer};
		while (true) {
			const std::size_t samples_until_loop{(loop_end() - tell()) * channels()};
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

std::size_t tr::audio::stream::loop_start() const
{
	return m_loop_start;
}

void tr::audio::stream::set_loop_start(std::size_t loop_start)
{
	m_loop_start = std::clamp(loop_start, std::size_t{0}, loop_end() - 1);
}

std::size_t tr::audio::stream::loop_end() const
{
	if (m_loop_end == UNKNOWN_LOOP_POINT) {
		m_loop_end = length();
	}
	return m_loop_end;
}

void tr::audio::stream::set_loop_end(std::size_t loop_end)
{
	m_loop_end = std::clamp(loop_end, loop_start() + 1, length());
	if (looping() && tell() >= m_loop_end) {
		seek(loop_start());
	}
}

std::unique_ptr<tr::audio::stream> tr::audio::open_file(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path)) {
		throw file_open_error{std::format("File not found: '{}'", path.string())};
	}

	const std::string extension{path.extension().string()};
	if (extension == ".ogg") {
		return std::make_unique<ogg_stream>(path);
	}
	else {
		throw file_open_error{std::format("Unsupported audio file extension '{}'", extension)};
	}
}