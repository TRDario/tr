///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides mirror repeat mappings.                                                                                                      //
//                                                                                                                                       //
// A mirror repeat mapping function is provided, defined for arithmetic-arithmetic, vector-vector, or vector-rectangle combinations:     //
//     - tr::mirror_repeat(9, 0, 5) -> 1                                                                                                 //
//     - tr::mirror_repeat({1020, 970}, {0, 0}, {1000, 1000}) -> {980, 970}                                                              //
//     - tr::mirror_repeat({1020, 970}, tr::frectangle{{1000, 1000}}) -> {980, 970}                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "rectangle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Performs a mirror repeat mapping.
	template <arithmetic Number> constexpr Number mirror_repeat(Number v, Number min, Number max);
	// Performs a mirror repeat mapping.
	template <int Dimensions, arithmetic Element>
	constexpr glm::vec<Dimensions, Element> mirror_repeat(glm::vec<Dimensions, Element> v, glm::vec<Dimensions, Element> min,
														  glm::vec<Dimensions, Element> max);
	// Performs a mirror repeat mapping of a vector on a rectangle.
	template <int Dimensions, arithmetic Element>
	constexpr glm::vec<Dimensions, Element> mirror_repeat(glm::vec<Dimensions, Element> v, const rectangle<Element>& rectangle);
} // namespace tr

#include "impl/mirror_repeat.hpp" // IWYU pragma: export