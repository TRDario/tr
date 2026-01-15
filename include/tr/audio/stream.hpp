///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an audio stream interface and a function to load an audio stream from file.                                                  //
//                                                                                                                                       //
// tr::audio::stream provides an interface for a 16-bit mono or stereo audio stream. Currently, the only way to get an audio stream is   //
// to open a file using tr::audio::open_file, which creates an audio stream using data from an .ogg file.                                //
//     - std::unique_ptr<tr::audio::stream> stream{tr::audio::open_file("audio.ogg")}                                                    //
// Ogg files may have embedded loop point metadata which is automatically detected and set by the opening function:                      //
//   LOOPSTART=[SAMPLE] sets the starting loop point and enables looping.                                                                //
//   LOOPEND=[SAMPLE] sets the ending loop point and enables looping.                                                                    //
//   LOOP enables looping.                                                                                                               //
//                                                                                                                                       //
// The length, number of channels, and sample rate of the stream can be queried using methods:                                           //
//     - stream.length() -> 2205000                                                                                                      //
//     - stream.channels() -> 2                                                                                                          //
//     - stream.sample_rate() -> 44100                                                                                                   //
//                                                                                                                                       //
// The current position of the stream can be queried or set, and data can be read to a buffer:                                           //
//     - stream.tell() -> 0                                                                                                              //
//     - stream.seek(88200) -> stream is now at 2.0s                                                                                     //
//     - std::array<i16, 8192> buffer; stream.read(buffer) -> reads data into buffer, returns a span to the region that was written to   //
//                                                                                                                                       //
// A stream can be looped between arbitrary loop points (by default the beginning and end). These parameters can also be queried:        //
//     - stream.looping() -> false                                                                                                       //
//     - stream.loop_start() -> 0                                                                                                        //
//     - stream.loop_end() -> 2205000                                                                                                    //
//     - stream.set_looping(true); stream.set_loop_start(88200) -> stream now loops in the interval [2.0s, 50.0s]                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/exception.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::audio {
	// Error thrown when opening an audio file failed.
	class file_open_error : public exception {
	  public:
		// Constructs an exception.
		file_open_error(std::string&& description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string m_description;
	};

	// Audio stream interface.
	class stream {
	  public:
		stream();
		virtual ~stream() = default;

		// Gets the length of the stream in samples.
		virtual usize length() const = 0;
		// Gets the number of channels in the stream.
		virtual int channels() const = 0;
		// Gets the sample rate of the stream.
		virtual int sample_rate() const = 0;

		// Gets the current offset within the stream.
		virtual usize tell() const = 0;
		// Seeks to an offset relative to the beginning.
		virtual void seek(usize where) = 0;
		// Reads samples to a destination buffer.
		virtual void raw_read(std::span<i16> buffer) = 0;
		// Reads from the stream and returns the span of the buffer that was written to.
		std::span<i16> read(std::span<i16> buffer);

		// Gets whether the stream is looping.
		bool looping() const;
		// Sets whether the stream is looping.
		void set_looping(bool looping);

		// Gets the loop start of the stream.
		usize loop_start() const;
		// Sets the loop start of the stream.
		void set_loop_start(usize loop_start);

		// Gets the loop end of the stream.
		usize loop_end() const;
		// Sets the loop end of the stream.
		void set_loop_end(usize loop_end);

	  protected:
		// Sentinel representing an unknown ending loop point.
		static constexpr usize unknown_loop_point{std::numeric_limits<usize>::max()};

	  private:
		// Whether the stream is looping.
		bool m_looping;
		// The loop starting sample offset.
		usize m_loop_start;
		// The loop ending sample offset.
		mutable usize m_loop_end;
	};
	// Opens an audio stream.
	// May throw: file_open_error.
	std::unique_ptr<stream> open_file(const std::filesystem::path& path);
} // namespace tr::audio