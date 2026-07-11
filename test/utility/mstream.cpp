///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/mstream.hpp.                                                                                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/mstream.hpp>
#include <tr/utility/ranges.hpp>
#include <tr/utility/rng.hpp>

TEST(mstream_test, omstream)
{
	tr::rng rng;

	std::array<double, 512> out_source;
	for (double& value : out_source) {
		value = rng.generate<double>();
	}

	std::array<double, 1024> out_dest{};
	tr::omstream os{tr::range_mut_bytes(out_dest)};
	os.seekp(512 * sizeof(double));
	os.write((const char*)out_source.data(), sizeof(out_source));

	EXPECT_TRUE(std::ranges::equal(std::views::drop(out_dest, 512), out_source));
}

TEST(mstream_test, imstream)
{
	tr::rng rng;

	std::array<double, 1024> in_source;
	for (double& value : in_source) {
		value = rng.generate<double>();
	}

	tr::imstream is{tr::range_bytes(in_source)};
	std::array<double, 512> in_dest;
	is.seekg(512 * sizeof(double));
	is.read((char*)in_dest.data(), sizeof(in_dest));

	EXPECT_TRUE(std::ranges::equal(std::views::drop(in_source, 512), in_dest));
}

TEST(mstream_test, mstream)
{
	tr::rng rng;

	std::array<double, 1024> buffer;
	tr::mstream ios{tr::range_mut_bytes(buffer)};

	std::array<double, 1024> source;
	for (double& value : source) {
		value = rng.generate<double>();
	}
	ios.write((const char*)source.data(), sizeof(source));

	std::array<double, 1024> dest;
	ios.read((char*)dest.data(), sizeof(dest));

	EXPECT_EQ(source, dest);
}