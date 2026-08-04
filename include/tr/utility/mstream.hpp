/// @file
/// @brief Provides a stream interface over memory buffers.

#pragma once
#include "common.hpp"

//

namespace tr
{
	/// iostream buffer operating on a memory buffer.
	class memorybuf : public std::streambuf
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a memory buffer from a buffer.
		/// @param buffer Buffer to construct the memory buffer over.
		explicit memorybuf(std::span<char> buffer);

		/// Constructs a memory buffer from a buffer.
		/// @param buffer Buffer to construct the memory buffer over.
		explicit memorybuf(std::span<unsigned char> buffer);

		/// Constructs a memory buffer from a buffer.
		/// @param buffer Buffer to construct the memory buffer over.
		explicit memorybuf(std::span<std::byte> buffer);

		/// @}

	  private:
		/// Buffer the stream operates upon.
		std::span<char> m_buffer;

		//

		pos_type seekoff(off_type offset, std::ios_base::seekdir dir, std::ios_base::openmode) override final;

		pos_type seekpos(std::streampos pos, std::ios_base::openmode mode) override final;
	};

	/// Input memory stream.
	class imstream : private memorybuf, public std::istream
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an imstream from a buffer.
		/// @param buffer Buffer to construct the imstream over.
		explicit imstream(std::span<const char> buffer);

		/// Constructs an imstream from a buffer.
		/// @param buffer Buffer to construct the imstream over.
		explicit imstream(std::span<const unsigned char> buffer);

		/// Constructs an imstream from a buffer.
		/// @param buffer Buffer to construct the imstream over.
		explicit imstream(std::span<const std::byte> buffer);

		/// @}
	};

	/// Output memory stream.
	class omstream : private memorybuf, public std::ostream
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an omstream from a buffer.
		/// @param buffer Buffer to construct the omstream over.
		explicit omstream(std::span<char> buffer);

		/// Constructs an omstream from a buffer.
		/// @param buffer Buffer to construct the omstream over.
		explicit omstream(std::span<unsigned char> buffer);

		/// Constructs an omstream from a buffer.
		/// @param buffer Buffer to construct the omstream over.
		explicit omstream(std::span<std::byte> buffer);

		/// @}
	};

	/// Input-output memory stream.
	class mstream : private memorybuf, public std::iostream
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an mstream from a buffer.
		/// @param buffer Buffer to construct the mstream over.
		explicit mstream(std::span<char> buffer);

		/// Constructs an mstream from a buffer.
		/// @param buffer Buffer to construct the mstream over.
		explicit mstream(std::span<unsigned char> buffer);

		/// Constructs an mstream from a buffer.
		/// @param buffer Buffer to construct the mstream over.
		explicit mstream(std::span<std::byte> buffer);

		/// @}
	};
} // namespace tr