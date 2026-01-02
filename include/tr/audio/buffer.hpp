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
		buffer(std::span<const i16> data, format format, int frequency);

		// Gets the size of the buffer's storage.
		usize size() const;
		// Gets the length of the buffer's audio.
		fsecs length() const;

		// Sets the data of the buffer.
		void set(std::span<const i16> data, format format, int frequency);

	  private:
		// The ID of the buffer in the audio manager.
		enum class id : unsigned int {
			// Special ID denoting no held buffer.
			EMPTY
		};
		// Marks the buffer as cullable in the global audio manager.
		struct deleter {
			void operator()(id id) const;
		};

		// Managed audio manager buffer ID.
		handle<id, id::EMPTY, deleter> m_id;

		friend class buffer_base;
		friend class source_base;
		friend class manager;
	};
	// Loads audio data from file into a buffer.
	// May throw: audio_file_open_error.
	buffer load_file(const std::filesystem::path& path);
} // namespace tr::audio