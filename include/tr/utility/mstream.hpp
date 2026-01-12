///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a stream interface over memory buffers.                                                                                      //
//                                                                                                                                       //
// tr::memorybuf is provided as a memory-backed stream buffer, while the mstream family (imstream, omstream, mstream) is provided as a   //
// set of streams using said memory buffer. This allows traversing arbitrary buffers as if they were streams (for example with           //
// tr::write_binary or tr::read_binary):                                                                                                 //
//     - tr::imstream in{buffer}; tr::read_binary(in, x); tr::read_binary(in, y) -> reads the binary data of x and y from the buffer     //
//     - tr::omstream out{buffer}; tr::write_binary(out, x); tr::write_binary(out, y) -> writes the binary data of x and y to the buffer //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// iostream buffer operating on a memory buffer.
	class memorybuf : public std::streambuf {
	  public:
		// Constructs a memory buffer from a buffer.
		explicit memorybuf(std::span<char> buffer);
		// Constructs a memory buffer from a buffer.
		explicit memorybuf(std::span<unsigned char> buffer);
		// Constructs a memory buffer from a buffer.
		explicit memorybuf(std::span<std::byte> buffer);

	  private:
		// The buffer the stream operates upon.
		std::span<char> m_buffer;

		pos_type seekoff(off_type offset, std::ios_base::seekdir dir, std::ios_base::openmode) override final;
		pos_type seekpos(std::streampos pos, std::ios_base::openmode mode) override final;
	};

	// Input memory stream.
	class imstream : private memorybuf, public std::istream {
	  public:
		// Constructs an imstream from a buffer.
		explicit imstream(std::span<const char> buffer);
		// Constructs an imstream from a buffer.
		explicit imstream(std::span<const unsigned char> buffer);
		// Constructs an imstream from a buffer.
		explicit imstream(std::span<const std::byte> buffer);
	};

	// Output memory stream.
	class omstream : private memorybuf, public std::ostream {
	  public:
		// Constructs an omstream from a buffer.
		explicit omstream(std::span<char> buffer);
		// Constructs an omstream from a buffer.
		explicit omstream(std::span<unsigned char> buffer);
		// Constructs an omstream from a buffer.
		explicit omstream(std::span<std::byte> buffer);
	};

	// Input-output memory stream.
	class mstream : private memorybuf, public std::iostream {
	  public:
		// Constructs an mstream from a buffer.
		explicit mstream(std::span<char> buffer);
		// Constructs an mstream from a buffer.
		explicit mstream(std::span<unsigned char> buffer);
		// Constructs an mstream from a buffer.
		explicit mstream(std::span<std::byte> buffer);
	};
} // namespace tr