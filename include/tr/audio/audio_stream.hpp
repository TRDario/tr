/// @file
/// @brief Provides an audio stream interface and a function to load an audio stream from file.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Interface for a 16-bit mono or stereo audio stream.
	class audio_stream
	{
	  public:
		/// @name Constructors
		/// @{

		/// Initializes an audio stream.
		[[nodiscard]] audio_stream() noexcept;

		/// Destroys the audio stream.
		virtual ~audio_stream() noexcept = default;

		/// @}
		/// @name Information
		/// @{

		/// Gets the length of the stream.
		/// @return Number of samples in the stream.
		[[nodiscard]] virtual usize length() const = 0;

		/// Gets the number of channels in the stream.
		/// @return Number of audio channels in the stream.
		[[nodiscard]] virtual int channels() const = 0;

		/// Gets the sample rate of the stream.
		/// @return Sample rate of the stream.
		[[nodiscard]] virtual int sample_rate() const = 0;

		/// @}
		/// @name IO
		/// @{

		/// Gets the current offset within the stream.
		/// @return Offset sample within the stream.
		[[nodiscard]] virtual usize tell() const = 0;

		/// Seeks to an offset relative to the beginning.
		/// @param where Offset sample within the stream.
		virtual void seek(usize where) = 0;

		/// Reads from the stream and returns the span of the buffer that was written to.
		/// @param buffer Buffer to read to.
		/// @return Written-to span of the buffer.
		std::span<i16> read(std::span<i16> buffer);

		/// @}
		/// @name Looping
		/// @{

		/// Gets whether the stream is looping.
		/// @return `true` if the stream is looping, `false` otherwise.
		[[nodiscard]] bool looping() const;

		/// Sets whether the stream is looping.
		/// @param looping `true` if the stream should loop, `false` otherwise.
		void set_looping(bool looping);

		/// Gets the loop start of the stream.
		/// @return Starting loop sample of the stream.
		[[nodiscard]] usize loop_start() const;

		/// Sets the loop start of the stream.
		/// @param loop_start Starting loop sample of the stream.
		void set_loop_start(usize loop_start);

		/// Gets the loop end of the stream.
		/// @return Ending loop sample of the stream.
		[[nodiscard]] usize loop_end() const;

		/// Sets the loop end of the stream.
		/// @param loop_end Ending loop sample of the stream (will be clamped to the length of the stream).
		void set_loop_end(usize loop_end);

		/// @}

	  protected:
		/// Sentinel representing an unknown ending loop point.
		static constexpr usize unknown_loop_point{std::numeric_limits<usize>::max()};

		//

		/// Reads samples to a destination buffer.
		/// @param buffer Buffer to read to.
		virtual void raw_read(std::span<i16> buffer) = 0;

	  private:
		/// Whether the stream is looping.
		bool m_looping;

		/// Loop starting sample offset.
		usize m_loop_start;

		/// Loop ending sample offset.
		mutable usize m_loop_end;
	};

	/// @name Factories
	/// @{

	/// Opens an audio stream from file.
	/// @note Currently, only Ogg files are supported.
	/// @note Ogg files may have embedded loop point metadata which is automatically detected and set by the opening function:
	/// @note - LOOPSTART=[SAMPLE] sets the starting loop point and enables looping.
	/// @note - LOOPEND=[SAMPLE] sets the ending loop point and enables looping.
	/// @note - LOOP enables looping.
	/// @param path Path to the audio file.
	/// @exception audio_file_open_error If opening the file failed.
	/// @return Audio file stream.
	[[nodiscard]] std::unique_ptr<audio_stream> open_audio_file(const std::filesystem::path& path);

	/// @}
} // namespace tr