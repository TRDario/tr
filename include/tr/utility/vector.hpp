/// @file
/// Provides geometric vector utilities.

#pragma once
#include "angle.hpp"

//

namespace tr
{
	/// Creates a normalized vector from an angle value.
	/// @param th Angle defining the direction the vector will be pointing in.
	/// @return Normalized vector pointing in the direction defined by `th`.
	glm::vec2 normal(angle th);

	/// Creates a vector from a magnitude and angle.
	/// @param mag Magnitude of the vector.
	/// @param th Angle defining the direction the vector will be pointing in.
	/// @return Vector pointing in the direction defined by `th` with magnitude `mag`.
	glm::vec2 magth(float mag, angle th);

	//

	/// Computes the squared length of a vector.
	/// @tparam Dimensions Number of dimensions of the vector.
	/// @tparam Element Type of the vector elements.
	/// @param v Vector to get the square length of.
	/// @return '|v|²'.
	template <int Dimensions, arithmetic Element>
	constexpr Element length2(glm::vec<Dimensions, Element> v);

	/// Computes the squared distance between two points.
	/// @tparam Dimensions Number of dimensions of the vectors.
	/// @tparam ElementL Type of the left vector elements.
	/// @tparam ElementR Type of the right vector elements.
	/// @param a Vector to the first point.
	/// @param b Vector to the second point.
	/// @return `|b-a|²`.
	template <int Dimensions, arithmetic ElementL, arithmetic ElementR>
	constexpr std::common_type_t<ElementL, ElementR> distance2(glm::vec<Dimensions, ElementL> a, glm::vec<Dimensions, ElementR> b);

	/// Computes the inverse of a vector.
	/// @tparam Dimensions Number of dimensions of the vector.
	/// @tparam Element Type of the vector elements.
	/// @param v Vector to get the inverse of.
	/// @return `v⁻¹`.
	template <int Dimensions, arithmetic Element>
	constexpr glm::vec<Dimensions, float> inverse(glm::vec<Dimensions, Element> v);

	/// Computes the 2D cross product of two vectors.
	/// @tparam Dimensions Number of dimensions of the vectors.
	/// @tparam ElementL Type of the left vector elements.
	/// @tparam ElementR Type of the right vector elements.
	/// @param a First vector.
	/// @param b Second vector.
	/// @return `a×b`.
	template <arithmetic ElementL, arithmetic ElementR>
	constexpr std::common_type_t<ElementL, ElementR> cross(glm::tvec2<ElementL> a, glm::tvec2<ElementR> b);
} // namespace tr

#include "impl/vector.hpp" // IWYU pragma: export