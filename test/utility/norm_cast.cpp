///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/norm_cast.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tr/utility.hpp>

using namespace tr::integer_aliases;
using tr::norm_cast;

int main()
{
	// Generate a bunch of numbers, round-trip through every kind of conversion, calculate the error and take the largest.
	tr::rng rng;
	double max_error{0};
	for (int i = 0; i < 1000000; ++i) {
		const double initial{rng.generate<double>()};
		const double final{norm_cast<double>(norm_cast<u8>(norm_cast<float>(norm_cast<u32>(norm_cast<i16>(norm_cast<i64>(initial))))))};
		max_error = std::max(max_error, std::abs(initial - final));
	}

	// Check if maximum error is outside the tolerated range.
	if (max_error > 1.0f / 255.0f + 1e-6f) {
		tr::println_error("Maximum deviation: {} (outside margin of error).", max_error);
		return EXIT_FAILURE;
	}
	else {
		tr::println("Maximum deviation: {} (within margin of error).", max_error);
		return EXIT_SUCCESS;
	}
}