///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/encryption.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/encryption.hpp>
#include <tr/utility/rng.hpp>

TEST(encryption_test, round_trip)
{
	tr::rng rng;
	std::array<double, 1024> data;
	for (double& value : data) {
		value = rng.generate<double>();
	}

	std::vector<std::byte> encrypted{tr::encrypt(data)};
	const std::vector<std::byte> decrypted{tr::decrypt(std::move(encrypted))};
	const std::array<double, 1024>& decrypted_object{tr::as_object<std::array<double, 1024>>(decrypted)};
	EXPECT_EQ(decrypted_object, data);
}