#include "../../include/tr/utility/encryption.hpp"
#include "../../include/tr/utility/binary_io.hpp"
#include <lz4.h>

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
	std::span<const std::byte> span{encrypted};
	if (encrypted.size() < 7 || !binary_read_magic<"tr">(span)) {
		throw decryption_error{"Invalid compressed data header."};
	}
	const u8 key{binary_read<u8>(span)};
	for (std::byte& byte : std::views::drop(encrypted, 3)) {
		byte = std::byte(u8(int(byte) + 170) ^ key);
	}

	out.resize(binary_read<u32>(span));
	const int real_size{
		LZ4_decompress_safe((const char*)(encrypted.data() + 7), (char*)out.data(), int(encrypted.size() - 7), int(out.size()))};
	if (u32(real_size) != out.size()) {
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
	out.resize(LZ4_compressBound(int(raw.size()) + 7ULL));

	const int used_size{LZ4_compress_default((const char*)raw.data(), (char*)(out.data() + 7), int(raw.size()), int(out.size() - 7))};
	out.resize(used_size + 7);
	std::span<std::byte> span{out};
	span = binary_write(span, "tr");
	span = binary_write(span, key);
	binary_write(span, u32(raw.size()));
	for (std::byte& byte : span) {
		byte = std::byte(int(byte ^ std::byte(key)) - 170);
	}
}

std::vector<std::byte> tr::encrypt(std::span<const std::byte> raw, u8 key)
{
	std::vector<std::byte> out;
	encrypt_to(out, raw, key);
	return out;
}