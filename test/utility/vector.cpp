/// @file
/// @brief Tests utility/vector.hpp.

#include <gtest/gtest.h>
#include <tr/utility/vector.hpp>

using namespace tr::angle_literals;

//

/// Tests creating a normalized vector from an angle.
TEST(VectorTest, CreatesNormalVector)
{
	const glm::vec2 right{tr::normal(0_radf)};
	const glm::vec2 up{tr::normal(0.25_trf)};
	EXPECT_FLOAT_EQ(right.x, 1.0f);
	EXPECT_FLOAT_EQ(right.y, 0.0f);
	EXPECT_NEAR(up.x, 0.0f, 1e-6f);
	EXPECT_FLOAT_EQ(up.y, 1.0f);
}

/// Tests creating a vector from a magnitude and angle.
TEST(VectorTest, CreatesVectorFromMagnitudeAndAngle)
{
	const glm::vec2 vector{tr::magth(3.0f, 0.25_trf)};
	EXPECT_NEAR(vector.x, 0.0f, 1e-6f);
	EXPECT_FLOAT_EQ(vector.y, 3.0f);
}

/// Tests computing squared lengths in one through four dimensions.
TEST(VectorTest, ComputesSquaredLength)
{
	EXPECT_EQ(tr::length2(glm::vec<1, int>{3}), 9);
	EXPECT_EQ(tr::length2(glm::ivec2{3, 4}), 25);
	EXPECT_EQ(tr::length2(glm::ivec3{1, 2, 2}), 9);
	EXPECT_EQ(tr::length2(glm::ivec4{1, 2, 2, 4}), 25);
	EXPECT_DOUBLE_EQ(tr::length2(glm::dvec2{1.5, 2.0}), 6.25);
}

/// Tests computing squared distance and promoting mixed representation types.
TEST(VectorTest, ComputesSquaredDistance)
{
	const glm::ivec2 a{1, 2};
	const glm::dvec2 b{4.0, 6.0};
	EXPECT_DOUBLE_EQ(tr::distance2(a, b), 25.0);
	EXPECT_EQ(tr::distance2(glm::ivec3{1, 2, 3}, glm::ivec3{1, 2, 3}), 0);
}

/// Tests computing the reciprocal of every vector component.
TEST(VectorTest, ComputesInverse)
{
	const glm::dvec3 inverse{tr::inverse(glm::ivec3{2, 4, -5})};
	EXPECT_DOUBLE_EQ(inverse.x, 0.5);
	EXPECT_DOUBLE_EQ(inverse.y, 0.25);
	EXPECT_DOUBLE_EQ(inverse.z, -0.2);
}

/// Tests the signed 2D cross product for same and mixed representation types.
TEST(VectorTest, ComputesCrossProduct)
{
	EXPECT_EQ(tr::cross(glm::ivec2{1, 0}, glm::ivec2{0, 1}), 1);
	EXPECT_EQ(tr::cross(glm::ivec2{0, 1}, glm::ivec2{1, 0}), -1);
	EXPECT_DOUBLE_EQ(tr::cross(glm::ivec2{2, 0}, glm::dvec2{0.0, 3.0}), 6.0);
}