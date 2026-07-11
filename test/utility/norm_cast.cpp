///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/norm_cast.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/norm_cast.hpp>
#include <tr/utility/rng.hpp>

using namespace tr::aliases;
using tr::norm_cast;

TEST(norm_cast_test, round_trip)
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
	EXPECT_FALSE(max_error > 1.0f / 255.0f + 1e-6f) << "Maximum deviation: " << max_error << " (outside margin of error).";
}