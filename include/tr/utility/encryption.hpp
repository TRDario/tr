/// @file
/// @brief Provides functions for compression and rudimentary encryption of data.
/// @details
/// The 'encryption' provided by these functions is not intended to be secure, just a rudimentary mechanism against the most primitive of
/// hex editing and the like.

/// @ingroup utility
/// @defgroup encryption Encryption
/// @brief Contains functions for compression and rudimentary encryption of data.
/// @details
/// The 'encryption' provided by these functions is not intended to be secure, just a rudimentary mechanism against the most primitive of
/// hex editing and the like.

#pragma once
#include <tr/utility/exception.hpp>
#include <tr/utility/ranges.hpp>

//

namespace tr
{
	/// @ingroup encryption
	/// @brief Error thrown when a decryption operation fails.
	class decryption_error : public exception
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an exception.
		/// @param description Decryption error description.
		[[nodiscard]] decryption_error(std::string_view description) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the name of the error.
		/// @return "Decryption error".
		[[nodiscard]] std::string_view name() const noexcept override;

		/// @brief Returns the description of the error.
		/// @return Description of the decryption error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// @brief Returns further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// @brief Details of the decryption error.
		std::string_view m_description;
	};

	//

	/// @ingroup encryption
	/// @brief Encrypts data to a vector.
	/// @param out Output vector.
	/// @param raw Raw source data.
	/// @par Exception safety
	/// If an exception is thrown, `out` is left in a valid, but unspecified state (basic exception guarantee).
	void encrypt_to(std::vector<std::byte>& out, std::span<const std::byte> raw);

	/// @ingroup encryption
	/// @brief Encrypts data to a vector.
	/// @tparam Range Continuous range type.
	/// @param out Output vector.
	/// @param range Raw source range.
	/// @par Exception safety
	/// If an exception is thrown, `out` is left in a valid, but unspecified state (basic exception guarantee).
	template <std::ranges::contiguous_range Range>
	void encrypt_to(std::vector<std::byte>& out, Range&& range)
	{
		encrypt_to(out, std::span<const std::byte>{range_bytes(range)});
	}

	/// @ingroup encryption
	/// @brief Encrypts data to a new vector.
	/// @param raw Raw source data.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Vector containing encrypted data.
	[[nodiscard]] std::vector<std::byte> encrypt(std::span<const std::byte> raw);

	/// @ingroup encryption
	/// @brief Encrypts data to a new vector.
	/// @param range Raw source range.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Vector containing encrypted data.
	template <std::ranges::contiguous_range Range>
	[[nodiscard]] std::vector<std::byte> encrypt(Range&& range)
	{
		return encrypt(std::span<const std::byte>{range_bytes(range)});
	}

	//

	/// @ingroup encryption
	/// @brief Decrypts data encrypted by `encrypt()` to a vector.
	/// @param out Output vector.
	/// @param encrypted Encrypted data.
	/// @exception decryption_error If decryption failed.
	/// @par Exception safety
	/// If an exception is thrown, `out` is left in a valid, but unspecified state (basic exception guarantee).
	void decrypt_to(std::vector<std::byte>& out, std::vector<std::byte> encrypted);

	/// @ingroup encryption
	/// @brief Decrypts data encrypted by `encrypt()` to a new vector.
	/// @param encrypted Encrypted data.
	/// @exception decryption_error If decryption failed.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Vector containing decrypted data.
	[[nodiscard]] std::vector<std::byte> decrypt(std::vector<std::byte> encrypted);
} // namespace tr