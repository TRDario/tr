#pragma once
#include "../utility/exception.hpp"

namespace tr {
	// Error thrown when opening an audio file failed.
	class audio_file_open_error : public exception {
	  public:
		// Constructs an exception.
		audio_file_open_error(std::string&& description) noexcept;

		// Gets the name of the error.
		std::string_view name() const noexcept override;
		// Gets the description of the error.
		std::string_view description() const noexcept override;
		// Gets further details about the error.
		std::string_view details() const noexcept override;

	  private:
		// The description of the error.
		std::string description_str;
	};

	// Audio stream interface.
	class audio_stream {
	  public:
		audio_stream();
		virtual ~audio_stream() = default;

		// Gets the length of the stream in samples.
		virtual std::size_t length() const = 0;
		// Gets the number of channels in the stream.
		virtual int channels() const = 0;
		// Gets the sample rate of the stream.
		virtual int sample_rate() const = 0;

		// Gets the current offset within the stream.
		virtual std::size_t tell() const = 0;
		// Seeks to an offset relative to the beginning.
		virtual void seek(std::size_t where) = 0;
		// Reads samples to a destination buffer.
		virtual void raw_read(std::span<std::int16_t> buffer) = 0;
		// Reads from the stream and returns the span of the buffer that was written to.
		std::span<std::int16_t> read(std::span<std::int16_t> buffer);

		// Gets whether the stream is looping.
		bool looping() const;
		// Sets whether the stream is looping.
		void set_looping(bool looping);

		// Gets the loop start of the stream.
		std::size_t loop_start() const;
		// Sets the loop start of the stream.
		void set_loop_start(std::size_t loop_start);

		// Gets the loop end of the stream.
		std::size_t loop_end() const;
		// Sets the loop end of the stream.
		void set_loop_end(std::size_t loop_end);

	  private:
		// Whether the stream is looping.
		bool looping_;
		// The loop starting sample offset.
		std::size_t loop_start_;
		// The loop ending sample offset.
		mutable std::size_t loop_end_;
	};
	// Opens an audio stream.
	// May throw: audio_file_open_error.
	std::unique_ptr<audio_stream> open_audio_file(const std::filesystem::path& path);
} // namespace tr