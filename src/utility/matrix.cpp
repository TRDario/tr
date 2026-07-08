///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements matrix.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/matrix.hpp"

///////////////////////////////////////////////////////////////// MATRICES ////////////////////////////////////////////////////////////////

glm::mat4 tr::ortho(const rectangle<float>& region)
{
	return glm::ortho(region.tl.x, region.tl.x + region.size.x, region.tl.y + region.size.y, region.tl.y);
}

glm::mat4 tr::scale_around(const glm::mat4& matrix, glm::vec2 center, glm::vec2 scale)
{
	return scale_around(matrix, glm::vec3{center, 0}, glm::vec3{scale, 1});
}

glm::mat4 tr::scale_around(const glm::mat4& matrix, glm::vec3 center, glm::vec3 scale)
{
	return glm::translate(glm::scale(glm::translate(matrix, center), scale), -center);
}

glm::mat4 tr::rotate_around(const glm::mat4& matrix, glm::vec2 center, angle rotation)
{
	return rotate_around(matrix, {center, 0}, rotation, z_axis);
}

glm::mat4 tr::rotate_around(const glm::mat4& matrix, glm::vec3 center, angle rotation, glm::vec3 axis)
{
	return glm::translate(glm::rotate(glm::translate(matrix, center), rotation.rads(), axis), -center);
}

//

glm::vec2 tr::matrix_operators::operator*(const glm::vec2& v, const glm::mat4& m)
{
	return glm::vec4{v, 0, 1} * m;
}

glm::vec3 tr::matrix_operators::operator*(const glm::vec3& v, const glm::mat4& m)
{
	return glm::vec4{v, 1} * m;
}

glm::vec2 tr::matrix_operators::operator*(const glm::mat4& m, const glm::vec2& v)
{
	return m * glm::vec4{v, 0, 1};
}

glm::vec3 tr::matrix_operators::operator*(const glm::mat4& m, const glm::vec3& v)
{
	return m * glm::vec4{v, 1};
}