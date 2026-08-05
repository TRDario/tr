/// @file
/// @brief Provides an audio buffer class.

#pragma once
#include "../utility/chrono.hpp"
#include "../utility/handle.hpp"
#include "../utility/integer.hpp"

namespace tr
{
	class audio_context;
}

//

namespace tr
{
	/// Audio data formats.
	enum class audio_format
	{
		/// 16-bit mono audio.
		mono16 = 0x1101,
		/// 16-bit stereo audio.
		stereo16 = 0x1103
	};

	/// Audio data buffer.
	/// @details Audio buffers are attached to audio sources and may store 16-bit mono or stereo audio data.
	class audio_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Audio buffers are not copyable.
		audio_buffer(const audio_buffer&) = delete;

		/// Audio buffers are not movable.
		audio_buffer(audio_buffer&&) = delete;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Audio buffers are not copyable.
		audio_buffer& operator=(const audio_buffer&) = delete;

		/// Audio buffers are not movable.
		audio_buffer& operator=(audio_buffer&&) = delete;

		/// @}
		/// @name Getters
		/// @{

		/// Gets the context of the audio buffer.
		/// @return Reference to the audio context.
		audio_context& context() const;

		/// Gets the size of the buffer's storage.
		/// @return Size of the buffer's storage in samples.
		usize size() const;

		/// Gets the length of the buffer's audio.
		/// @return Length of the buffer's audio in seconds.
		fsecs length() const;

		/// Gets the sample rate of the buffer's audio.
		/// @return Sample rate of the buffer's audio.
		int sample_rate() const;

		/// Gets the number of channels in the buffer's audio.
		/// @return Number of channels in the buffer's audio.
		int channels() const;

		/// @}
		/// @name Setters
		/// @{

		// Sets the data of the buffer.
		void set(std::span<const i16> data, audio_format format, int frequency);

		/// @}

	  private:
		/// Buffer destroyer.
		struct deleter
		{
			/// Reference to the audio context the buffer is on.
			audio_context& context;

			/// Destroys the audio buffer.
			/// @param id OpenAL audio buffer ID.
			void operator()(unsigned int id) const;
		};

		//

		/// Handle to the OpenAL buffer.
		handle<unsigned int, 0, deleter> m_handle;

		//

		// Constructs an empty audio buffer.
		audio_buffer(audio_context& context);

		//

		/// Gets the loop points of the buffer.
		/// @return Pair containing the start and end points of the buffer.
		std::pair<fsecs, fsecs> loop_points() const;

		/// Sets the loop points of the buffer.
		/// @param start_point Starting loop point sample.
		/// @param end_point Ending loop point sample.
		void set_loop_points(fsecs start_point, fsecs end_point);

		//

		// Accesses the raw OpenAL buffer ID.
		friend class audio_source;

		// Uses the private constructor.
		friend std::shared_ptr<audio_buffer> create_audio_buffer(audio_context& context);
	};

	/// @name Factories
	/// @{

	/// Creates a shared audio buffer pointer.
	/// @param context Audio context to create the buffer on.
	/// @return Shared pointer to a new audio buffer.
	std::shared_ptr<audio_buffer> create_audio_buffer(audio_context& context);

	/// Creates a shared audio buffer pointer with initial data.
	/// @param context Audio context to create the buffer on.
	/// @param data Data to create the buffer with.
	/// @param format Format of the audio data.
	/// @param frequency Frequency of the audio data.
	/// @return Shared pointer to a new audio buffer.
	std::shared_ptr<audio_buffer> create_audio_buffer(audio_context& context, std::span<const i16> data, audio_format format,
													  int frequency);

	/// Loads audio data from file into a buffer.
	/// @param context Audio context to create the buffer on.
	/// @param path Path to the audio file. open_audio_file is used to open the files.
	/// @exception audio_file_open_error If loading the audio file failed.
	/// @return Shared pointer to a new audio buffer.
	std::shared_ptr<audio_buffer> load_audio_file(audio_context& context, const std::filesystem::path& path);

	/// @}
} // namespace tr