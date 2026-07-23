///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functions pertaining to geometric vectors.                                                                                   //
//                                                                                                                                       //
// A normalized vector can be created from an angle value using tr::normal() and a vector of any magnitude using tr::magth():            //
//     - tr::normal(90_deg) -> {0, 1} (normalized vector corresponding to the angle)                                                     //
//     - tr::magth(5, 90_deg) -> {0, 5} (vector constructed through magnitude + angle)                                                   //
//                                                                                                                                       //
// Miscellaneous other vector operations are provided, including squared length and distance, creating an inverse, and computing a       //
// 2D cross product of two vectors:                                                                                                      //
//     - tr::length2({10, 10}) -> 200.0f (squared length of the vector)                                                                  //
//     - tr::distance2({10, 10}, {20, 20}) -> 200.0f (squared distance between vectors)                                                  //
//     - tr::inverse({100, 100}) -> {0.01, 0.01} (inverse vector)                                                                        //
//     - tr::cross({10, 10}, {5, 20}) -> 100 (2D cross product)                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "angle.hpp"

namespace tr {
	// Computes a normalized vector from an angle value.
	glm::vec2 normal(angle th);
	// Creates a vector from a magnitude and angle.
	glm::vec2 magth(float mag, angle th);

	// Gets the squared length of a vector.
	template <int Dimensions, typename Element> constexpr Element length2(glm::vec<Dimensions, Element> a);
	// Gets the squared distance between two points.
	template <int Dimensions, typename ElementL, typename ElementR>
	constexpr std::common_type_t<ElementL, ElementR> distance2(glm::vec<Dimensions, ElementL> a, glm::vec<Dimensions, ElementR> b);
	// Gets the inverse of a vector.
	template <int Dimensions, arithmetic Element> constexpr glm::vec<Dimensions, float> inverse(const glm::vec<Dimensions, Element>& vec);
	// Calculates the 2D cross product of two vectors.
	template <typename ElementL, typename ElementR>
	constexpr std::common_type_t<ElementL, ElementR> cross(glm::tvec2<ElementL> a, glm::tvec2<ElementR> b);
} // namespace tr

#include "impl/vector.hpp" // IWYU pragma: export