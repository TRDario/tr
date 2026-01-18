///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functions for compression and rudimentary encryption of data.                                                                //
//                                                                                                                                       //
// The 'encryption' provided by these functions is not intended to be secure, just a rudimentary mechanism against the most primitive of //
// hex editing and the like.                                                                                                             //
//                                                                                                                                       //
// Encryption of data is done using tr::encrypt_to if writing to an existing vector, or tr::encrypt otherwise:                           //
//     - tr::encrypt_to(out, data) -> encrypts 'data' and outputs the result to 'out'                                                    //
//     - tr::encrypt(data) -> equivalent to creating a new vector and calling tr::encrypt_to with it as the output                       //
//                                                                                                                                       //
// Decryption of data encrypted by tr::decrypt is done using tr::decrypt_to if writing to an existing vector, or tr::decrypt otherwise:  //
//     - tr::decrypt_to(out, data) -> decrypts 'data' and outputs the result to 'out'                                                    //
//     - tr::decrypt(data) -> equivalent to creating a new vector and calling tr::decrypt_to with it as the output                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "exception.hpp"
#include "ranges.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

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
		std::string_view m_description;
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
		std::string_view m_description;
	};

	// Encrypts data.
	// May throw: tr::encryption_error.
	void encrypt_to(std::vector<std::byte>& out, std::span<const std::byte> raw);
	// Encrypts data.
	// May throw: tr::encryption_error.
	template <std::ranges::contiguous_range Range> void encrypt_to(std::vector<std::byte>& out, Range&& range);
	// Encrypts data.
	// May throw: tr::encryption_error.
	std::vector<std::byte> encrypt(std::span<const std::byte> raw);
	// Encrypts data.
	// May throw: tr::encryption_error.
	template <std::ranges::contiguous_range Range> std::vector<std::byte> encrypt(Range&& range);

	// Decrypts data encrypted by tr::encrypt().
	// May throw: tr::decryption_error.
	void decrypt_to(std::vector<std::byte>& out, std::vector<std::byte> encrypted);
	// Decrypts data encrypted by tr::encrypt().
	// May throw: tr::decryption_error.
	std::vector<std::byte> decrypt(std::vector<std::byte> encrypted);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range Range> void tr::encrypt_to(std::vector<std::byte>& out, Range&& range)
{
	encrypt_to(out, std::span<const std::byte>{range_bytes(range)});
}

template <std::ranges::contiguous_range Range> std::vector<std::byte> tr::encrypt(Range&& range)
{
	return encrypt(std::span<const std::byte>{range_bytes(range)});
}