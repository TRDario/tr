#include "../../include/tr/utility/binary_io.hpp"

void tr::binary_reader<std::string>::read_from_stream(std::istream& is, std::string& out)
{
	out.resize(binary_read<u32>(is));
	is.read(out.data(), out.size());
}

std::span<const std::byte> tr::binary_reader<std::string>::read_from_span(std::span<const std::byte> span, std::string& out)
{
	out.resize(binary_read<u32>(span));
	return tr::binary_reader<std::span<char>>::read_from_span(span, std::span{out});
}

std::vector<std::byte> tr::flush_binary(std::istream& is)
{
	std::vector<std::byte> out;
	flush_binary(is, std::back_inserter(out));
	out.shrink_to_fit();
	return out;
}

void tr::binary_writer<std::string_view>::write_to_stream(std::ostream& os, const std::string_view& in)
{
	binary_write(os, u32(in.size()));
	os.write(in.data(), in.size());
}

std::span<std::byte> tr::binary_writer<std::string_view>::write_to_span(std::span<std::byte> span, const std::string_view& in)
{
	if (span.size() < in.size() + sizeof(u32)) {
		throw std::out_of_range{"Tried to binary write a string larger than the size of the output range."};
	}
	span = binary_write(span, u32(in.size()));
	std::ranges::copy(range_bytes(in), span.begin());
	return span.subspan(in.size());
}

void tr::binary_writer<std::string>::write_to_stream(std::ostream& os, const std::string& in)
{
	binary_write(os, std::string_view{in});
}

std::span<std::byte> tr::binary_writer<std::string>::write_to_span(std::span<std::byte> span, const std::string& in)
{
	return binary_write(span, std::string_view{in});
}

void tr::binary_writer<std::span<const std::byte>>::write_to_stream(std::ostream& os, const std::span<const std::byte>& in)
{
	os.write((const char*)in.data(), in.size());
}

std::span<std::byte> tr::binary_writer<std::span<const std::byte>>::write_to_span(std::span<std::byte> span,
																				  const std::span<const std::byte>& in)
{
	if (span.size() < in.size()) {
		throw std::out_of_range{"Tried to binary write a span larger than the size of the output range."};
	}
	std::ranges::copy(in, span.begin());
	return span.subspan(in.size());
}