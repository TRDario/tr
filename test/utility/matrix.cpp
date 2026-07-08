///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/matrix.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <tr/utility/matrix.hpp>

using namespace tr::angle_literals;
using namespace tr::matrix_operators;

TEST(matrix_test, operators)
{
	const glm::mat4 transform{tr::rotate_around(glm::mat4{1.0f}, {100, 100, 100}, 45_deg, tr::y_axis)};

	const glm::vec4 regular_operator0{transform * glm::vec4{250, 250, 0, 1}};
	const glm::vec2 custom_operator0{transform * glm::vec2{250, 250}};
	EXPECT_EQ(glm::vec2{regular_operator0}, custom_operator0);

	const glm::vec4 regular_operator1{transform * glm::vec4{250, 250, 50, 1}};
	const glm::vec3 custom_operator1{transform * glm::vec3{250, 250, 50}};
	EXPECT_EQ(glm::vec3{regular_operator1}, custom_operator1);
}

TEST(matrix_test, ortho)
{
	const glm::mat4 transform{tr::ortho({{0, 0}, {500, 500}})};

	const glm::vec2 transformed0{transform * glm::vec2{500, 250}};
	EXPECT_FLOAT_EQ(transformed0.x, 1.0f);
	EXPECT_FLOAT_EQ(transformed0.y, 0.0f);

	const glm::vec2 transformed1{transform * glm::vec2{0, 375}};
	EXPECT_FLOAT_EQ(transformed1.x, -1.0f);
	EXPECT_FLOAT_EQ(transformed1.y, -0.5f);
}

TEST(matrix_test, scale_around2)
{
	const glm::mat4 transform{tr::scale_around(glm::mat4{1.0f}, {100, 100}, {2.0f, 3.0f})};

	const glm::vec2 transformed0{transform * glm::vec2{200, 100}};
	EXPECT_FLOAT_EQ(transformed0.x, 300.0f);
	EXPECT_FLOAT_EQ(transformed0.y, 100.0f);

	const glm::vec2 transformed1{transform * glm::vec2{95, 50}};
	EXPECT_FLOAT_EQ(transformed1.x, 90.0f);
	EXPECT_FLOAT_EQ(transformed1.y, -50.0f);
}

TEST(matrix_test, scale_around3)
{
	const glm::mat4 transform{tr::scale_around(glm::mat4{1.0f}, {50, 50, 50}, {2.0f, 3.0f, 1.5f})};

	const glm::vec3 transformed0{transform * glm::vec3{100, 100, 100}};
	EXPECT_FLOAT_EQ(transformed0.x, 150.0f);
	EXPECT_FLOAT_EQ(transformed0.y, 200.0f);
	EXPECT_FLOAT_EQ(transformed0.z, 125.0f);

	const glm::vec3 transformed1{transform * glm::vec3{30, 40, 50}};
	EXPECT_FLOAT_EQ(transformed1.x, 10.0f);
	EXPECT_FLOAT_EQ(transformed1.y, 20.0f);
	EXPECT_FLOAT_EQ(transformed1.z, 50.0f);
}

TEST(matrix_test, rotate_around2)
{
	const glm::mat4 transform{tr::rotate_around(glm::mat4{1.0f}, {250, 250}, 90_deg)};

	const glm::vec2 transformed0{transform * glm::vec2{200, 250}};
	EXPECT_FLOAT_EQ(transformed0.x, 250);
	EXPECT_FLOAT_EQ(transformed0.y, 200);

	const glm::vec2 transformed1{transform * glm::vec2{250, 100}};
	EXPECT_FLOAT_EQ(transformed1.x, 400);
	EXPECT_FLOAT_EQ(transformed1.y, 250);
}

TEST(matrix_test, rotate_around3)
{
	const glm::mat4 transform{tr::rotate_around(glm::mat4{1.0f}, {100, 100, 100}, 180_deg, tr::x_axis)};

	const glm::vec3 transformed0{transform * glm::vec3{100, 100, 200}};
	EXPECT_FLOAT_EQ(transformed0.x, 100);
	EXPECT_FLOAT_EQ(transformed0.y, 100);
	EXPECT_NEAR(transformed0.z, 0, 1e-3);

	const glm::vec3 transformed1{transform * glm::vec3{100, 200, 100}};
	EXPECT_FLOAT_EQ(transformed1.x, 100);
	EXPECT_NEAR(transformed1.y, 0, 1e-3);
	EXPECT_FLOAT_EQ(transformed1.z, 100);
}