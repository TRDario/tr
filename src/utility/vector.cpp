/// @file
/// @brief Implements vector.hpp.

#include <tr/utility/vector.hpp>

//

glm::vec2 tr::normal(angle<float> th) noexcept
{
	return glm::vec2{th.cos(), th.sin()};
}

glm::vec2 tr::magth(float mag, angle<float> th) noexcept
{
	return normal(th) * mag;
}