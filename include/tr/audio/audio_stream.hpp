#pragma once
#include <tr/utility.hpp>

namespace tr {
	// Error thrown when opening an audio file failed.
	class audio_file_open_error : public exception {
	  public:
		// Constructs an exception.
		audio_file_open_error(std::string_view location, std::string&& description) noexcept;

		// Gets the name of the error.
		std::string_view name() const noexcept override;
		// Gets the description of the error.
		std::string_view description() const noexcept override;
		// Gets further details about the error.
		std::string_view details() const noexcept override;

	  private:
		// The description of the error.
		std::string _description;
	};

	// Concept denoting an iterator suitable for audio_stream::read.
	template <class It>
	concept audio_stream_output_iterator = std::contiguous_iterator<It> && std::output_iterator<It, std::int16_t>;

	// Audio stream interface.
	class audio_stream {
	  public:
		audio_stream() noexcept;
		virtual ~audio_stream() noexcept = default;

		// Gets the length of the stream in samples.
		virtual std::size_t length() const noexcept = 0;
		// Gets the number of channels in the stream.
		virtual int channels() const noexcept = 0;
		// Gets the sample rate of the stream.
		virtual int sample_rate() const noexcept = 0;

		// Gets the current offset within the stream.
		virtual std::size_t tell() const noexcept = 0;
		// Seeks to an offset relative to the beginning.
		virtual void seek(std::size_t where) noexcept = 0;
		// Reads samples to a destination buffer.
		virtual void raw_read(std::int16_t* dest, std::size_t samples) noexcept = 0;
		// Reads from the stream and returns the number of read samples.
		template <audio_stream_output_iterator It> std::size_t read(It it, std::size_t samples) noexcept;

		// Gets whether the stream is looping.
		bool looping() const noexcept;
		// Sets whether the stream is looping.
		void set_looping(bool looping) noexcept;

		// Gets the loop start of the stream.
		std::size_t loop_start() const noexcept;
		// Sets the loop start of the stream.
		void set_loop_start(std::size_t loop_start) noexcept;

		// Gets the loop end of the stream.
		std::size_t loop_end() const noexcept;
		// Sets the loop end of the stream.
		void set_loop_end(std::size_t loop_end) noexcept;

	  private:
		// Whether the stream is looping.
		bool _looping;
		// The loop starting sample offset.
		std::size_t _loop_start;
		// The loop ending sample offset.
		mutable std::size_t _loop_end;
	};
	// Opens an audio stream.
	// May throw: audio_file_open_error.
	std::unique_ptr<audio_stream> open_audio_file(const std::filesystem::path& path);
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::audio_stream_output_iterator It> std::size_t tr::audio_stream::read(It it, std::size_t samples) noexcept
{
	if (_looping) {
		while (samples > 0) {
			const std::size_t samples_until_loop{loop_end() - tell()};
			const std::size_t samples_to_read{std::min(samples_until_loop, samples)};
			raw_read(std::to_address(it), samples_to_read);
			if (samples > 0) {
				seek(loop_start());
				samples -= samples_to_read;
			}
		}
		return samples;
	}
	else {
		const std::size_t samples_to_read{std::min(length(), samples)};
		raw_read(std::to_address(it), samples_to_read);
		return samples_to_read;
	}
}