/// @file
/// @brief Provides functions for compression and rudimentary encryption of data.
/// @details
/// The 'encryption' provided by these functions is not intended to be secure, just a rudimentary mechanism against the most primitive of
/// hex editing and the like.

#pragma once
#include <tr/utility/exception.hpp>
#include <tr/utility/ranges.hpp>

//

namespace tr
{
	/// Error thrown when a decryption operation fails.
	class decryption_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param description Decryption error description.
		[[nodiscard]] decryption_error(std::string_view description) noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return "Decryption error".
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the decryption error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Details of the decryption error.
		std::string_view m_description;
	};

	/// @name Encryption
	/// @{

	/// Encrypts data to a vector.
	/// @param out Output vector.
	/// @param raw Raw source data.
	void encrypt_to(std::vector<std::byte>& out, std::span<const std::byte> raw);

	/// Encrypts data to a vector.
	/// @tparam Contiguous range type.
	/// @param out Output vector.
	/// @param range Raw source range.
	template <std::ranges::contiguous_range Range>
	void encrypt_to(std::vector<std::byte>& out, Range&& range)
	{
		encrypt_to(out, std::span<const std::byte>{range_bytes(range)});
	}

	/// Encrypts data.
	/// @param raw Raw source data.
	/// @return Vector containing encrypted data.
	[[nodiscard]] std::vector<std::byte> encrypt(std::span<const std::byte> raw);

	/// Encrypts data.
	/// @param range Raw source range.
	/// @return Vector containing encrypted data.
	template <std::ranges::contiguous_range Range>
	[[nodiscard]] std::vector<std::byte> encrypt(Range&& range)
	{
		return encrypt(std::span<const std::byte>{range_bytes(range)});
	}

	/// @}
	/// @name Decryption
	/// @{

	/// Decrypts data encrypted by `encrypt()` to a vector.
	/// @param out Output vector.
	/// @param encrypted Encrypted data.
	/// @exception decryption_error If decryption failed.
	void decrypt_to(std::vector<std::byte>& out, std::vector<std::byte> encrypted);

	/// Decrypts data encrypted by `encrypt()`.
	/// @param encrypted Encrypted data.
	/// @exception decryption_error If decryption failed.
	/// @return Vector containing decrypted data.
	[[nodiscard]] std::vector<std::byte> decrypt(std::vector<std::byte> encrypted);

	/// @}
} // namespace tr