///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio buffer class.                                                                                                       //
//                                                                                                                                       //
// Audio buffers are attached to audio sources and may store 16-bit mono or stereo audio data. They may be constructed with initial data //
// or have it set later with .set(). Attached audio buffers can be destroyed without worry for the sources: the system will keep their   //
// data allocated until all sources have stopped using it.                                                                               //
//     - tr::audio::buffer buf{data, tr::audio::format::MONO16, 44100}                                                                   //
//       -> equivalent to tr::audio::buffer{}; buf.set(data, tr::audio::format::MONO16, 44100)                                           //
// Leaving audio buffers alive at the end of the program is erroneous and may trigger an assertion in debug builds.                      //
//                                                                                                                                       //
// Audio buffers can also directly be loaded from .ogg files using tr::audio::open_file.                                                 //
//     - tr::audio::buffer buf{tr::audio::load_file("sound.ogg")}                                                                        //
//                                                                                                                                       //
// The size (in bytes) and length (in seconds) of the buffer's audio can be queried using the .size() and .length() methods.             //
//     - buf.size() -> 88200                                                                                                             //
//     - buf.length() -> 1.0s                                                                                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/chrono.hpp"
#include "../utility/handle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::audio {
	// Audio data formats.
	enum class format {
		mono16 = 0x1101,  // 16-bit mono audio.
		stereo16 = 0x1103 // 16-bit stereo audio.
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
			empty
		};
		// Marks the buffer as cullable in the global audio manager.
		struct deleter {
			void operator()(id id) const;
		};

		// Managed audio manager buffer ID.
		handle<id, id::empty, deleter> m_id;

		friend class owning_buffer;
		friend class owning_source;
		friend class manager;
	};
	// Loads audio data from file into a buffer.
	// May throw: audio_file_open_error.
	buffer load_file(const std::filesystem::path& path);
} // namespace tr::audio