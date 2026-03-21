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
	setp(buffer.data(), buffer.data() + buffer.size());
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

tr::memorybuf::pos_type tr::memorybuf::seekoff(off_type offset, std::ios_base::seekdir dir, std::ios_base::openmode which)
{
	pos_type result{pos_type(-1)};
	off_type ioffset{offset};
	off_type ooffset{offset};
	if (dir == std::ios_base::cur) {
		ioffset += gptr() - eback();
		ooffset += pptr() - pbase();
	}
	else if (dir == std::ios_base::end) {
		ioffset += m_buffer.size();
		ooffset = ioffset;
	}

	if (which & std::ios_base::in && ioffset >= 0 && ioffset <= off_type(m_buffer.size())) {
		setg(eback(), eback() + ioffset, egptr());
		result = pos_type(ioffset);
	}
	if (which & std::ios_base::out && ooffset >= 0 && ooffset <= off_type(m_buffer.size())) {
		setp(pbase(), epptr());
		pbump(ooffset);
		result = pos_type(ooffset);
	}

	return result;
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