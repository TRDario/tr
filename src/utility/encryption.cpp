#include "../../include/tr/utility/encryption.hpp"
#include "../../include/tr/utility/binary_io.hpp"
#include "../../include/tr/utility/mstream.hpp"
#include <lz4.h>

namespace tr {
	// Size of an encrypted chunk header.
	constexpr usize header_size{7};
} // namespace tr

tr::decryption_error::decryption_error(std::string_view description)
	: m_description{description}
{
}

std::string_view tr::decryption_error::name() const
{
	return "Decryption error";
}

std::string_view tr::decryption_error::description() const
{
	return m_description;
}

std::string_view tr::decryption_error::details() const
{
	return {};
}

tr::encryption_error::encryption_error(std::string_view description)
	: m_description{description}
{
}

std::string_view tr::encryption_error::name() const
{
	return "Encryption error";
}

std::string_view tr::encryption_error::description() const
{
	return m_description;
}

std::string_view tr::encryption_error::details() const
{
	return {};
}

void tr::decrypt_to(std::vector<std::byte>& out, std::vector<std::byte> encrypted)
{
	imstream header{std::views::take(encrypted, header_size)};
	const std::span<const char> compressed{(const char*)(encrypted.data() + header_size), encrypted.size() - header_size};

	if (encrypted.size() < header_size || !read_binary_magic<"tr">(header)) {
		throw decryption_error{"Invalid compressed data header."};
	}

	const u8 key{read_binary<u8>(header)};
	for (std::byte& byte : std::views::drop(encrypted, 3)) {
		byte = std::byte(u8(int(byte) + 170) ^ key);
	}

	out.resize(read_binary<u32>(header));
	const usize real_size{usize(LZ4_decompress_safe(compressed.data(), (char*)out.data(), int(compressed.size()), int(out.size())))};
	if (real_size != out.size()) {
		throw decryption_error{"Failed to decompress data after decryption."};
	}
}

std::vector<std::byte> tr::decrypt(std::vector<std::byte> encrypted)
{
	std::vector<std::byte> out;
	decrypt_to(out, std::move(encrypted));
	return out;
}

void tr::encrypt_to(std::vector<std::byte>& out, std::span<const std::byte> raw, u8 key)
{
	out.resize(LZ4_compressBound(int(raw.size())) + header_size);
	const std::span<char> compress_out{(char*)(out.data() + header_size), out.size() - header_size};

	omstream header{std::views::take(out, header_size)};
	write_binary(header, "tr");
	write_binary(header, key);
	write_binary(header, u32(raw.size()));

	const int used_size{LZ4_compress_default((const char*)raw.data(), compress_out.data(), int(raw.size()), int(compress_out.size()))};
	out.resize(used_size + header_size);

	for (std::byte& byte : std::views::drop(out, 3)) {
		byte = std::byte(int(byte ^ std::byte(key)) - 170);
	}
}

std::vector<std::byte> tr::encrypt(std::span<const std::byte> raw, u8 key)
{
	std::vector<std::byte> out;
	encrypt_to(out, raw, key);
	return out;
}