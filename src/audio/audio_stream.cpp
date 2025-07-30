#include "../../include/tr/audio/audio_stream.hpp"
#include <vorbis/vorbisfile.h>

namespace tr {
	// Ogg audio file backend.
	class ogg_audio_stream : public audio_stream {
	  public:
		ogg_audio_stream(const std::filesystem::path& path);
		~ogg_audio_stream();

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
} // namespace tr

///////////////////////////////////////////////////////////// OGG AUDIO FILE //////////////////////////////////////////////////////////////

tr::ogg_audio_stream::ogg_audio_stream(const std::filesystem::path& path)
{
#ifdef _WIN32
	const int result{ov_fopen(path.string().c_str(), &file)};
#else
	const int result{ov_fopen(path.c_str(), &file)};
#endif
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

tr::ogg_audio_stream::~ogg_audio_stream()
{
	ov_clear(&file);
}

std::size_t tr::ogg_audio_stream::length() const
{
	return static_cast<std::size_t>(ov_pcm_total(const_cast<OggVorbis_File*>(&file), -1));
}

int tr::ogg_audio_stream::channels() const
{
	return ov_info(const_cast<OggVorbis_File*>(&file), -1)->channels;
}

int tr::ogg_audio_stream::sample_rate() const
{
	return static_cast<int>(ov_info(const_cast<OggVorbis_File*>(&file), -1)->rate);
}

std::size_t tr::ogg_audio_stream::tell() const
{
	return static_cast<std::size_t>(ov_pcm_tell(const_cast<OggVorbis_File*>(&file)));
}

void tr::ogg_audio_stream::seek(std::size_t where)
{
	ov_pcm_seek(&file, static_cast<ogg_int64_t>(where));
}

void tr::ogg_audio_stream::raw_read(std::span<std::int16_t> buffer)
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

tr::audio_file_open_error::audio_file_open_error(std::string&& description)
	: description_str{std::move(description)}
{
}

std::string_view tr::audio_file_open_error::name() const
{
	return "Audio file opening error";
}

std::string_view tr::audio_file_open_error::description() const
{
	return description_str;
}

std::string_view tr::audio_file_open_error::details() const
{
	return {};
}

/////////////////////////////////////////////////////////////// AUDIO FILE ////////////////////////////////////////////////////////////////

tr::audio_stream::audio_stream()
	: looping_{false}, loop_start_{0}, loop_end_{UNKNOWN_LOOP_POINT}
{
}

std::span<std::int16_t> tr::audio_stream::read(std::span<std::int16_t> buffer)
{
	if (looping_) {
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

bool tr::audio_stream::looping() const
{
	return looping_;
}

void tr::audio_stream::set_looping(bool looping)
{
	looping_ = looping;
	if (looping && tell() >= loop_end()) {
		seek(loop_start());
	}
}

std::size_t tr::audio_stream::loop_start() const
{
	return loop_start_;
}

void tr::audio_stream::set_loop_start(std::size_t loop_start)
{
	loop_start_ = std::clamp(loop_start, std::size_t{0}, loop_end() - 1);
}

std::size_t tr::audio_stream::loop_end() const
{
	if (loop_end_ == UNKNOWN_LOOP_POINT) {
		loop_end_ = length();
	}
	return loop_end_;
}

void tr::audio_stream::set_loop_end(std::size_t loop_end)
{
	loop_end_ = std::clamp(loop_end, loop_start() + 1, length());
	if (looping() && tell() >= loop_end_) {
		seek(loop_start());
	}
}

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