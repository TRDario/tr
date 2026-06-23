///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of encryption.hpp.                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/encryption.hpp"
#include "../../include/tr/utility/binary_io.hpp"
#include "../../include/tr/utility/mstream.hpp"
#include "../../include/tr/utility/rng.hpp"
#include <lz4.h>

namespace {
	// Size of an encrypted chunk header.
	constexpr tr::usize header_size{7};
} // namespace

////////////////////////////////////////////////////////////////// ERRORS /////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////// ENCRYPTION/DECRYPTION //////////////////////////////////////////////////////////

void tr::encrypt_to(std::vector<std::byte>& out, std::span<const std::byte> raw)
{
	const std::byte key{static_cast<std::byte>(generate_random_seed())};

	out.resize(LZ4_compressBound(raw.size()) + header_size);

	omstream header{std::views::take(out, header_size)};
	write_binary_magic(header, "tr");
	write_binary(header, key, static_cast<u32>(raw.size()));

	const std::span<char> compressed{reinterpret_span<char>(std::span{out}).subspan(header_size)};
	const int used_size{LZ4_compress_default(reinterpret_cast<const char*>(raw.data()), compressed.data(), raw.size(), compressed.size())};
	out.resize(used_size + header_size);

	for (std::byte& byte : std::views::drop(out, 3)) {
		byte = static_cast<std::byte>(static_cast<u8>(byte ^ key) - 170);
	}
}

std::vector<std::byte> tr::encrypt(std::span<const std::byte> raw)
{
	std::vector<std::byte> out;
	encrypt_to(out, raw);
	return out;
}

void tr::decrypt_to(std::vector<std::byte>& out, std::vector<std::byte> encrypted)
{
	imstream header{std::views::take(encrypted, header_size)};
	const std::span<const char> compressed{reinterpret_span<const char>(std::span{encrypted}).subspan(header_size)};

	if (encrypted.size() < header_size || !read_binary_magic(header, "tr")) {
		throw decryption_error{"Invalid compressed data header."};
	}

	const std::byte key{read_binary<std::byte>(header)};
	for (std::byte& byte : std::views::drop(encrypted, 3)) {
		byte = static_cast<std::byte>(static_cast<u8>(byte) + 170) ^ key;
	}

	out.resize(read_binary<u32>(header));
	const int real_size{LZ4_decompress_safe(compressed.data(), reinterpret_cast<char*>(out.data()), compressed.size(), out.size())};
	if (static_cast<usize>(real_size) != out.size()) {
		throw decryption_error{"Failed to decompress data after decryption."};
	}
}

std::vector<std::byte> tr::decrypt(std::vector<std::byte> encrypted)
{
	std::vector<std::byte> out;
	decrypt_to(out, std::move(encrypted));
	return out;
}