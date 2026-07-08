///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements mirror_repeat.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../math.hpp"
#include "../mirror_repeat.hpp"

////////////////////////////////////////////////////////////// MIRROR REPEAT //////////////////////////////////////////////////////////////

template <tr::arithmetic Number> constexpr Number tr::mirror_repeat(Number v, Number min, Number max)
{
	Number shift{v - min};
	Number size{max - min};
	shift = tr::mod(tr::abs(shift + (shift < 0)), (2 * size));
	return min + (shift >= size ? 2 * size - shift - 1 : shift);
}

template <int Dimensions, tr::arithmetic Element>
constexpr glm::vec<Dimensions, Element> tr::mirror_repeat(glm::vec<Dimensions, Element> v, glm::vec<Dimensions, Element> min,
														  glm::vec<Dimensions, Element> max)
{
	glm::vec<Dimensions, Element> result;
	for (int i = 0; i < Dimensions; ++i) {
		result[i] = mirror_repeat(v[i], min[i], max[i]);
	}
	return result;
}

template <int Dimensions, tr::arithmetic Element>
constexpr glm::vec<Dimensions, Element> tr::mirror_repeat(glm::vec<Dimensions, Element> v, const rectangle<Element>& rectangle)
{
	return mirror_repeat(v, rectangle.tl, rectangle.tl + rectangle.size);
}