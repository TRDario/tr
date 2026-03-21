///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/encryption.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tr/utility.hpp>

int encryption(int, char**)
{
	tr::rng rng;
	std::array<double, 1024> data;
	for (double& value : data) {
		value = rng.generate<double>();
	}

	try {
		const std::vector<std::byte> encrypted{tr::encrypt(data)};
		const std::vector<std::byte> decrypted{tr::decrypt(std::move(encrypted))};
		if (tr::as_object<std::array<double, 1024>>(tr::range_bytes(decrypted)) != data) {
			tr::println_error("Encryption did not round-trip correctly.");
			return EXIT_FAILURE;
		}
	}
	catch (tr::decryption_error& err) {
		tr::println_error("Decryption error: {}", err.description());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}