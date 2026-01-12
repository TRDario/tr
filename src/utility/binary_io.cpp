#include "../../include/tr/utility/binary_io.hpp"

void tr::binary_reader<std::string>::operator()(std::istream& is, std::string& out) const
{
	out.resize(read_binary<u32>(is));
	is.read(out.data(), out.size());
}

std::vector<std::byte> tr::flush_binary(std::istream& is)
{
	std::vector<std::byte> out;
	flush_binary(is, std::back_inserter(out));
	out.shrink_to_fit();
	return out;
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

void tr::binary_writer<std::span<const std::byte>>::operator()(std::ostream& os, const std::span<const std::byte>& in) const
{
	os.write((const char*)in.data(), in.size());
}