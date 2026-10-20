/// @file
/// @brief Tests utility/encryption.hpp.

#include <gtest/gtest.h>
#include <tr/utility/encryption.hpp>

//

/// Tests the various encryption overloads and asserts they produce the same result.
TEST(EncryptionTest, Encrypts)
{
	const std::array<char, 4> data{'t', 'e', 's', 't'};
	std::vector<std::byte> out0;
	tr::encrypt_to(out0, std::as_bytes(std::span{data}));
	std::vector<std::byte> out1;
	tr::encrypt_to(out1, data);
	const std::vector<std::byte> out2{tr::encrypt(std::as_bytes(std::span{data}))};
	const std::vector<std::byte> out3{tr::encrypt(data)};
	EXPECT_EQ(out0.size(), out1.size());
	EXPECT_EQ(out0.size(), out2.size());
	EXPECT_EQ(out0.size(), out3.size());
}

/// Tests rejecting the decryption of an input with a truncated header.
TEST(EncryptionTest, RejectsDecryptingTruncatedHeader)
{
	EXPECT_THROW((void)tr::decrypt(std::vector<std::byte>{std::byte{'t'}, std::byte{'r'}}), tr::decryption_error);
}

/// Tests rejecting the decryption of an input with invalid header magic.
TEST(EncryptionTest, RejectsDecryptingInvalidHeaderMagic)
{
	EXPECT_THROW((void)tr::decrypt(std::vector<std::byte>{std::byte{'t'}, std::byte{'j'}, std::byte{0}, std::byte{83}, std::byte{0},
														  std::byte{0}, std::byte{0}, std::byte{0}}),
				 tr::decryption_error);
}

/// Tests rejecting the decryption of an input with an invalid header key.
TEST(EncryptionTest, RejectsDecryptingInvalidHeaderKey)
{
	EXPECT_THROW((void)tr::decrypt(std::vector<std::byte>{std::byte{'t'}, std::byte{'r'}, std::byte{0}, std::byte{84}, std::byte{0},
														  std::byte{0}, std::byte{0}, std::byte{0}}),
				 tr::decryption_error);
}

/// Tests rejecting the decryption of an input with an invalid payload.
TEST(EncryptionTest, RejectsDecryptingInvalidPayload)
{
	EXPECT_THROW((void)tr::decrypt(std::vector<std::byte>{std::byte{'t'}, std::byte{'r'}, std::byte{0}, std::byte{83}, std::byte{0},
														  std::byte{0}, std::byte{0}, std::byte{100}, std::byte{1}, std::byte{2}}),
				 tr::decryption_error);
}

/// Tests the various decryption overloads and asserts they produce the same result.
TEST(EncryptionTest, Decrypts)
{
	const std::vector<std::byte> input{
		std::byte{'t'}, std::byte{'r'}, std::byte{0},   std::byte{83},  std::byte{90},  std::byte{86},  std::byte{86},
		std::byte{86},  std::byte{150}, std::byte{202}, std::byte{187}, std::byte{201}, std::byte{202},
	};

	std::vector<std::byte> out0;
	tr::decrypt_to(out0, input);
	const std::vector<std::byte> out1{tr::decrypt(input)};
	const std::string_view out1_string{reinterpret_cast<const char*>(out1.data()), out1.size()};
	EXPECT_EQ(out0, out1);
	EXPECT_EQ(out1_string, "test");
}

/// Tests encrypting and decrypting an empty payload.
TEST(EncryptionTest, RoundTripsEmptyPayload)
{
	EXPECT_TRUE(tr::decrypt(tr::encrypt({})).empty());
}

/// Tests encrypting and decrypting data.
TEST(EncryptionTest, RoundTripsData)
{
	const std::array<char, 4> data{'t', 'e', 's', 't'};
	const std::vector<std::byte> decrypted{tr::decrypt(tr::encrypt(data))};
	const std::string_view decrypted_string{reinterpret_cast<const char*>(decrypted.data()), decrypted.size()};
	EXPECT_EQ(decrypted_string, (std::string_view{data.data(), data.size()}));
}