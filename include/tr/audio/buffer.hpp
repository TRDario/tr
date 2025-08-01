#pragma once
#include "../utility/chrono.hpp"
#include "../utility/handle.hpp"

namespace tr::audio {
	// Audio data formats.
	enum class format {
		MONO16 = 0x1101,  // 16-bit mono audio.
		STEREO16 = 0x1103 // 16-bit stereo audio.
	};

	// Audio data buffer.
	class buffer {
	  public:
		// Constructs an empty audio buffer.
		buffer();
		// Constructs an audio buffer and immediately sets it.
		buffer(std::span<const std::int16_t> data, format format, int frequency);

		// Gets the size of the buffer's storage.
		std::size_t size() const;
		// Gets the length of the buffer's audio.
		fsecs length() const;

		// Sets the data of the buffer.
		void set(std::span<const std::int16_t> data, format format, int frequency);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenAL ID.
		handle<unsigned int, 0, deleter> m_id;

		friend class source_base;
	};
	// Loads audio data from file into a buffer.
	// May throw: audio_file_open_error.
	buffer load_file(const std::filesystem::path& path);
} // namespace tr::audio