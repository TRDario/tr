///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements mstream.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/mstream.hpp"

////////////////////////////////////////////////////////////// MEMORY BUFFER //////////////////////////////////////////////////////////////

tr::memorybuf::memorybuf(std::span<char> buffer)
	: m_buffer{buffer}
{
	setg(buffer.data(), buffer.data(), buffer.data() + buffer.size());
}

tr::memorybuf::memorybuf(std::span<unsigned char> buffer)
	: memorybuf{{(char*)buffer.data(), buffer.size()}}
{
}

tr::memorybuf::memorybuf(std::span<std::byte> buffer)
	: memorybuf{{(char*)buffer.data(), buffer.size()}}
{
}

tr::memorybuf::pos_type tr::memorybuf::seekoff(off_type offset, std::ios_base::seekdir dir, std::ios_base::openmode)
{
	switch (dir) {
	case std::ios_base::cur:
		gbump(offset);
		break;
	case std::ios_base::end:
		setg(m_buffer.data(), m_buffer.data() + m_buffer.size() + offset, m_buffer.data() + m_buffer.size());
		break;
	case std::ios_base::beg:
		setg(m_buffer.data(), m_buffer.data() + offset, m_buffer.data() + m_buffer.size());
		break;
	default:
		break;
	}

	return gptr() - eback();
}

tr::memorybuf::pos_type tr::memorybuf::seekpos(std::streampos pos, std::ios_base::openmode mode)
{
	return seekoff(pos, std::ios_base::beg, mode);
}

////////////////////////////////////////////////////////////// MEMORY STREAMS /////////////////////////////////////////////////////////////

tr::imstream::imstream(std::span<const char> buffer)
	: memorybuf{{(char*)buffer.data(), buffer.size()}}, std::istream{this}
{
}

tr::imstream::imstream(std::span<const unsigned char> buffer)
	: memorybuf{{(unsigned char*)buffer.data(), buffer.size()}}, std::istream{this}
{
}

tr::imstream::imstream(std::span<const std::byte> buffer)
	: memorybuf{{(std::byte*)buffer.data(), buffer.size()}}, std::istream{this}
{
}

tr::omstream::omstream(std::span<char> buffer)
	: memorybuf{buffer}, std::ostream{this}
{
}

tr::omstream::omstream(std::span<unsigned char> buffer)
	: memorybuf{buffer}, std::ostream{this}
{
}

tr::omstream::omstream(std::span<std::byte> buffer)
	: memorybuf{buffer}, std::ostream{this}
{
}

tr::mstream::mstream(std::span<char> buffer)
	: memorybuf{buffer}, std::iostream{this}
{
}

tr::mstream::mstream(std::span<unsigned char> buffer)
	: memorybuf{buffer}, std::iostream{this}
{
}

tr::mstream::mstream(std::span<std::byte> buffer)
	: memorybuf{buffer}, std::iostream{this}
{
}