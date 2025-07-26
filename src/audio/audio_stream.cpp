#include "../../include/tr/audio/audio_stream.hpp"
#include <vorbis/vorbisfile.h>

namespace tr {
	// Ogg audio file backend.
	class _ogg_audio_stream : public audio_stream {
	  public:
		_ogg_audio_stream(const std::filesystem::path& path);
		~_ogg_audio_stream() noexcept;

		std::size_t length() const noexcept override;
		int channels() const noexcept override;
		int sample_rate() const noexcept override;

		std::size_t tell() const noexcept override;
		void seek(std::size_t where) noexcept override;
		void raw_read(std::int16_t* dest, std::size_t samples) noexcept override;

	  private:
		OggVorbis_File _file{};
	};

	constexpr std::size_t UNKNOWN_LOOP_POINT{std::numeric_limits<std::size_t>::max()};
} // namespace tr

///////////////////////////////////////////////////////////// OGG AUDIO FILE //////////////////////////////////////////////////////////////

tr::_ogg_audio_stream::_ogg_audio_stream(const std::filesystem::path& path)
{
#ifdef _WIN32
	const int result{ov_fopen(path.string().c_str(), &_file)};
#else
	const int result{ov_fopen(path.c_str(), &_file)};
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

	const vorbis_comment& comments{*ov_comment(&_file, -1)};
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
		else if (comment == "LOOP=true" || comment == "LOOP=1") {
			set_looping(true);
		}
	}
}

tr::_ogg_audio_stream::~_ogg_audio_stream() noexcept
{
	ov_clear(&_file);
}

std::size_t tr::_ogg_audio_stream::length() const noexcept
{
	return static_cast<std::size_t>(ov_pcm_total(const_cast<OggVorbis_File*>(&_file), -1));
}

int tr::_ogg_audio_stream::channels() const noexcept
{
	return ov_info(const_cast<OggVorbis_File*>(&_file), -1)->channels;
}

int tr::_ogg_audio_stream::sample_rate() const noexcept
{
	return static_cast<int>(ov_info(const_cast<OggVorbis_File*>(&_file), -1)->rate);
}

std::size_t tr::_ogg_audio_stream::tell() const noexcept
{
	return static_cast<std::size_t>(ov_pcm_tell(const_cast<OggVorbis_File*>(&_file)));
}

void tr::_ogg_audio_stream::seek(std::size_t where) noexcept
{
	ov_pcm_seek(&_file, static_cast<ogg_int64_t>(where));
}

void tr::_ogg_audio_stream::raw_read(std::int16_t* dest, std::size_t samples) noexcept
{
	char* raw_dest{reinterpret_cast<char*>(dest)};
	int bytes_left{static_cast<int>(samples * sizeof(std::int16_t))};
	int cur_section;
	while (bytes_left > 0) {
		const int read_bytes{static_cast<int>(ov_read(&_file, raw_dest, bytes_left, 0, 2, 1, &cur_section))};
		if (read_bytes <= 0) {
			return;
		}
		bytes_left -= read_bytes;
		raw_dest += read_bytes;
	}
}

////////////////////////////////////////////////////////// AUDIO FILE OPEN ERROR //////////////////////////////////////////////////////////

tr::audio_file_open_error::audio_file_open_error(std::string&& description) noexcept
	: _description{std::move(description)}
{
}

std::string_view tr::audio_file_open_error::name() const noexcept
{
	return "Audio file opening error";
}

std::string_view tr::audio_file_open_error::description() const noexcept
{
	return _description;
}

std::string_view tr::audio_file_open_error::details() const noexcept
{
	return {};
}

/////////////////////////////////////////////////////////////// AUDIO FILE ////////////////////////////////////////////////////////////////

tr::audio_stream::audio_stream() noexcept
	: _looping{false}, _loop_start{0}, _loop_end{UNKNOWN_LOOP_POINT}
{
}

bool tr::audio_stream::looping() const noexcept
{
	return _looping;
}

void tr::audio_stream::set_looping(bool looping) noexcept
{
	_looping = looping;
	if (looping && tell() >= loop_end()) {
		seek(loop_start());
	}
}

std::size_t tr::audio_stream::loop_start() const noexcept
{
	return _loop_start;
}

void tr::audio_stream::set_loop_start(std::size_t loop_start) noexcept
{
	_loop_start = std::clamp(loop_start, std::size_t{0}, loop_end() - 1);
}

std::size_t tr::audio_stream::loop_end() const noexcept
{
	if (_loop_end == UNKNOWN_LOOP_POINT) {
		_loop_end = length();
	}
	return _loop_end;
}

void tr::audio_stream::set_loop_end(std::size_t loop_end) noexcept
{
	_loop_end = std::clamp(loop_end, loop_start() + 1, length());
	if (looping() && tell() >= _loop_end) {
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
		return std::make_unique<_ogg_audio_stream>(path);
	}
	else {
		throw audio_file_open_error{std::format("Unsupported audio file extension '{}'", extension)};
	}
}