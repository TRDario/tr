#pragma once
#include "../utility/chrono.hpp"
#include "../utility/handle.hpp"

namespace tr {
	// Audio data formats.
	enum class audio_format {
		MONO16 = 0x1101,  // 16-bit mono audio.
		STEREO16 = 0x1103 // 16-bit stereo audio.
	};

	// Audio data buffer.
	class audio_buffer {
	  public:
		// Constructs an empty audio buffer.
		audio_buffer() noexcept;
		// Constructs an audio buffer and immediately sets it.
		audio_buffer(std::span<const std::int16_t> data, audio_format format, int frequency) noexcept;

		// Gets the size of the buffer's storage.
		std::size_t size() const noexcept;
		// Gets the length of the buffer's audio.
		fsecs length() const noexcept;

		// Sets the data of the buffer.
		void set(std::span<const std::int16_t> data, audio_format format, int frequency) noexcept;

	  private:
		struct _deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenAL ID.
		handle<unsigned int, 0, _deleter> _id;

		friend class _audio_source;
	};
	// Loads audio data from file into a buffer.
	// May throw: audio_file_open_error.
	audio_buffer load_audio_file(const std::filesystem::path& path);
} // namespace tr