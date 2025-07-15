#include "../../include/tr/utility/encryption.hpp"
#include "../../include/tr/utility/binary_io.hpp"
#include <lz4.h>

tr::decryption_error::decryption_error(std::string_view description) noexcept
	: _description{description}
{
}

std::string_view tr::decryption_error::name() const noexcept
{
	return "Decryption error";
}

std::string_view tr::decryption_error::description() const noexcept
{
	return _description;
}

std::string_view tr::decryption_error::details() const noexcept
{
	return {};
}

tr::encryption_error::encryption_error(std::string_view description) noexcept
	: _description{description}
{
}

std::string_view tr::encryption_error::name() const noexcept
{
	return "Encryption error";
}

std::string_view tr::encryption_error::description() const noexcept
{
	return _description;
}

std::string_view tr::encryption_error::details() const noexcept
{
	return {};
}

void tr::decrypt_to(std::vector<std::byte>& out, std::vector<std::byte> encrypted)
{
	std::span<const std::byte> span{encrypted};
	if (encrypted.size() < 7 || !binary_read_magic<"tr">(span)) {
		throw decryption_error{"Invalid compressed data header."};
	}
	const std::uint8_t key{binary_read<std::uint8_t>(span)};
	for (std::byte& byte : std::views::drop(encrypted, 3)) {
		byte = static_cast<std::byte>(static_cast<std::uint8_t>(static_cast<int>(byte) + 170) ^ key);
	}

	out.resize(binary_read<std::uint32_t>(span));
	const int real_size{LZ4_decompress_safe(reinterpret_cast<const char*>(encrypted.data() + 7), reinterpret_cast<char*>(out.data()),
											static_cast<int>(encrypted.size() - 7), static_cast<int>(out.size()))};
	if (static_cast<std::uint32_t>(real_size) != out.size()) {
		throw decryption_error{"Failed to decompress data after decryption."};
	}
}

std::vector<std::byte> tr::decrypt(std::vector<std::byte> encrypted)
{
	std::vector<std::byte> out;
	decrypt_to(out, std::move(encrypted));
	return out;
}

void tr::encrypt_to(std::vector<std::byte>& out, std::span<const std::byte> raw, std::uint8_t key)
{
	try {
		out.resize(LZ4_compressBound(static_cast<int>(raw.size()) + 7ULL));
	}
	catch (std::bad_alloc&) {
		throw encryption_error{"Failed to allocate memory for decompressed data."};
	}

	const int used_size{LZ4_compress_default(reinterpret_cast<const char*>(raw.data()), reinterpret_cast<char*>(out.data() + 7),
											 static_cast<int>(raw.size()), static_cast<int>(out.size() - 7))};
	out.resize(used_size + 7);
	std::span<std::byte> span{out};
	span = binary_write(span, "tr");
	span = binary_write(span, key);
	binary_write(span, static_cast<std::uint32_t>(raw.size()));
	for (std::byte& byte : span) {
		byte = static_cast<std::byte>(static_cast<int>(byte ^ std::byte{key}) - 170);
	}
}

std::vector<std::byte> tr::encrypt(std::span<const std::byte> raw, std::uint8_t key)
{
	std::vector<std::byte> out;
	encrypt_to(out, raw, key);
	return out;
}