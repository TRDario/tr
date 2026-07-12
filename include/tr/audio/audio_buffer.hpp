///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio buffer class.                                                                                                       //
//                                                                                                                                       //
// Audio buffers are attached to audio sources and may store 16-bit mono or stereo audio data. They may be constructed with initial data //
// or have it set later with .set(). Buffers are constructed through tr::create_audio_buffer():                                          //
//     - std::shared_ptr<tr::audio::buffer> buf{tr::create_audio_buffer(context, data, tr::audio_format::mono16, 44100)}                 //
//       -> equivalent to tr::create_audio_buffer(context); buf->set(data, tr::audio_format::mono16, 44100)                              //
// All audio buffer instances are passed in shared pointers as they may be kept alive by the context until culled.                       //
// Leaving audio buffers alive after their context is erroneous.                                                                         //
//                                                                                                                                       //
// Audio buffers can also directly be loaded from .ogg files using tr::load_audio_file:                                                  //
//     - std::shared_ptr<tr::audio::buffer> buf{tr::load_audio_file("sound.ogg")} -> loads audio from "sound.ogg"                        //
//                                                                                                                                       //
// The size (in samples), length (in seconds), sample rate and number of channels in the buffer's audio can be queried using .size(),    //
// .length(), .sample_rate() and .channels() respectively:                                                                               //
//     - buf->size() -> 44100                                                                                                            //
//     - buf->length() -> 1.0s                                                                                                           //
//     - buf->sample_rate() -> 44100                                                                                                     //
//     - buf->channels() -> 1                                                                                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/chrono.hpp"
#include "../utility/handle.hpp"
#include "../utility/integer.hpp"

namespace tr {
	class audio_context;
}

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Audio data formats.
	enum class audio_format {
		mono16 = 0x1101,  // 16-bit mono audio.
		stereo16 = 0x1103 // 16-bit stereo audio.
	};

	// Audio data buffer.
	class audio_buffer {
	  public:
		// Audio buffers are not copyable.
		audio_buffer(const audio_buffer&) = delete;
		// Audio buffers are not movable.
		audio_buffer(audio_buffer&&) = delete;

		// Audio buffers are not copyable.
		audio_buffer& operator=(const audio_buffer&) = delete;
		// Audio buffers are not movable.
		audio_buffer& operator=(audio_buffer&&) = delete;

		// Gets the context of the audio buffer.
		audio_context& context() const;

		// Gets the size of the buffer's storage.
		usize size() const;
		// Gets the length of the buffer's audio.
		fsecs length() const;
		// Gets the sample rate of the buffer's audio.
		int sample_rate() const;
		// Gets the number of channels in the buffer's audio.
		int channels() const;

		// Sets the data of the buffer.
		void set(std::span<const i16> data, audio_format format, int frequency);

	  private:
		// Buffer destroyer.
		struct deleter {
			// Reference to the audio context the buffer is on.
			audio_context& context;

			// Destroys the audio buffer.
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenAL buffer.
		handle<unsigned int, 0, deleter> m_handle;

		friend class audio_source;
		friend std::shared_ptr<audio_buffer> create_audio_buffer(audio_context& context);

		// Constructs an empty audio buffer.
		audio_buffer(audio_context& context);

		// Gets the loop points of the buffer.
		std::pair<fsecs, fsecs> loop_points() const;
		// Sets the loop points of the buffer.
		void set_loop_points(fsecs start_point, fsecs end_point);
	};
	// Creates a shared audio buffer pointer.
	std::shared_ptr<audio_buffer> create_audio_buffer(audio_context& context);
	// Creates a shared audio buffer pointer with initial data.
	std::shared_ptr<audio_buffer> create_audio_buffer(audio_context& context, std::span<const i16> data, audio_format format,
													  int frequency);
	// Loads audio data from file into a buffer.
	// May throw: audio_file_open_error.
	std::shared_ptr<audio_buffer> load_audio_file(audio_context& context, const std::filesystem::path& path);
} // namespace tr