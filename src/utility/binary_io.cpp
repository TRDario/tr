///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-constexpr parts of binary_io.hpp.                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/binary_io.hpp"

////////////////////////////////////////////////////////////// BINARY READING /////////////////////////////////////////////////////////////

void tr::binary_reader<std::string>::operator()(std::istream& is, std::string& out) const
{
	out.resize(read_binary<u32>(is));
	is.read(out.data(), out.size());
}

bool tr::read_binary_magic(std::istream& is, std::string_view magic)
{
	for (char chr : magic) {
		if (is.get() != chr) {
			return false;
		}
	}
	return true;
}

std::vector<std::byte> tr::flush_binary(std::istream& is)
{
	std::vector<std::byte> out;
	flush_binary(is, std::back_inserter(out));
	out.shrink_to_fit();
	return out;
}

////////////////////////////////////////////////////////////// BINARY WRITING /////////////////////////////////////////////////////////////

void tr::binary_writer<const char*>::operator()(std::ostream& os, const char* in) const
{
	write_binary(os, std::string_view{in});
}

void tr::binary_writer<std::string_view>::operator()(std::ostream& os, const std::string_view& in) const
{
	write_binary(os, u32(in.size()));
	os.write(in.data(), in.size());
}

void tr::binary_writer<std::string>::operator()(std::ostream& os, const std::string& in) const
{
	write_binary(os, std::string_view{in});
}

void tr::write_binary_magic(std::ostream& os, std::string_view magic)
{
	os.write(magic.data(), magic.size());
}