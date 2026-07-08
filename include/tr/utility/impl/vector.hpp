///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr and templated parts of vector.hpp.                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../macro.hpp"
#include "../vector.hpp"

///////////////////////////////////////////////////////////// VECTOR FUNCTIONS ////////////////////////////////////////////////////////////

template <int Dimensions, typename Element> constexpr Element tr::length2(glm::vec<Dimensions, Element> v)
{
	if constexpr (Dimensions == 1) {
		return sqr(v.x);
	}
	else if constexpr (Dimensions == 2) {
		return sqr(v.x) + sqr(v.y);
	}
	else if constexpr (Dimensions == 3) {
		return sqr(v.x) + sqr(v.y) + sqr(v.z);
	}
	else if constexpr (Dimensions == 4) {
		return sqr(v.x) + sqr(v.y) + sqr(v.z) + sqr(v.w);
	}
	else {
		TR_UNREACHABLE;
	}
}

template <int Dimensions, typename ElementL, typename ElementR>
constexpr std::common_type_t<ElementL, ElementR> tr::distance2(glm::vec<Dimensions, ElementL> a, glm::vec<Dimensions, ElementR> b)
{
	return length2(glm::vec<Dimensions, std::common_type_t<ElementL, ElementR>>{b} -
				   glm::vec<Dimensions, std::common_type_t<ElementL, ElementR>>{a});
}

template <int Dimensions, tr::arithmetic Element>
constexpr glm::vec<Dimensions, float> tr::inverse(const glm::vec<Dimensions, Element>& vec)
{
	glm::vec<Dimensions, float> result;
	for (int i = 0; i < Dimensions; ++i) {
		result[i] = static_cast<float>(1.0f / vec[i]);
	}
	return result;
}

template <typename ElementL, typename ElementR>
constexpr std::common_type_t<ElementL, ElementR> tr::cross(glm::tvec2<ElementL> a, glm::tvec2<ElementR> b)
{
	return a.x * b.y - a.y * b.x;
}