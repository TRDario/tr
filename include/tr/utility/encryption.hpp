#pragma once
#include "exception.hpp"
#include "ranges.hpp"

namespace tr {
	// Error thrown when a decryption operation fails.
	class decryption_error : public exception {
	  public:
		// Constructs an exception.
		decryption_error(std::string_view description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// Details of the decryption error.
		std::string_view description_str;
	};

	// Error thrown when an encryption operation fails.
	class encryption_error : public exception {
	  public:
		// Constructs an exception.
		encryption_error(std::string_view description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// Details of the decryption error.
		std::string_view description_str;
	};

	// Decrypts data encrypted by tr::encrypt().
	// May throw: tr::decryption_error.
	void decrypt_to(std::vector<std::byte>& out, std::vector<std::byte> encrypted);
	// Decrypts data encrypted by tr::encrypt().
	// May throw: tr::decryption_error.
	std::vector<std::byte> decrypt(std::vector<std::byte> encrypted);

	// Encrypts data.
	// May throw: tr::encryption_error.
	void encrypt_to(std::vector<std::byte>& out, std::span<const std::byte> raw, std::uint8_t key);
	// Encrypts data.
	// May throw: tr::encryption_error.
	template <std::ranges::contiguous_range Range> void encrypt_to(std::vector<std::byte>& out, Range&& range, std::uint8_t key);
	// Encrypts data.
	// May throw: tr::encryption_error.
	std::vector<std::byte> encrypt(std::span<const std::byte> raw, std::uint8_t key);
	// Encrypts data.
	// May throw: tr::encryption_error.
	template <std::ranges::contiguous_range Range> std::vector<std::byte> encrypt(Range&& range, std::uint8_t key);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range Range> void tr::encrypt_to(std::vector<std::byte>& out, Range&& range, std::uint8_t key)
{
	encrypt_to(out, std::span<const std::byte>{range_bytes(range)}, key);
}

template <std::ranges::contiguous_range Range> std::vector<std::byte> tr::encrypt(Range&& range, std::uint8_t key)
{
	return encrypt(std::span<const std::byte>{range_bytes(range)}, key);
}