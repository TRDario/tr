/// @file
/// Provides geometric vector utilities.

#pragma once
#include <tr/utility/angle.hpp>

//

namespace tr
{
	/// @name Vector operations
	/// @{

	/// Creates a normalized vector from an angle value.
	/// @param th Angle defining the direction the vector will be pointing in.
	/// @return Normalized vector pointing in the direction defined by `th`.
	[[nodiscard]] glm::vec2 normal(angle th) noexcept;

	/// Creates a vector from a magnitude and angle.
	/// @param mag Magnitude of the vector.
	/// @param th Angle defining the direction the vector will be pointing in.
	/// @return Vector pointing in the direction defined by `th` with magnitude `mag`.
	[[nodiscard]] glm::vec2 magth(float mag, angle th) noexcept;

	//

	/// Computes the squared length of a vector.
	/// @tparam Dimensions Number of dimensions of the vector.
	/// @tparam Element Type of the vector elements.
	/// @param v Vector to get the square length of.
	/// @return '|v|²'.
	template <int Dimensions, arithmetic Element>
	[[nodiscard]] constexpr Element length2(glm::vec<Dimensions, Element> v) noexcept
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

	/// Computes the squared distance between two points.
	/// @tparam Dimensions Number of dimensions of the vectors.
	/// @tparam ElementL Type of the left vector elements.
	/// @tparam ElementR Type of the right vector elements.
	/// @param a Vector to the first point.
	/// @param b Vector to the second point.
	/// @return `|b-a|²`.
	template <int Dimensions, arithmetic ElementL, arithmetic ElementR>
	[[nodiscard]] constexpr std::common_type_t<ElementL, ElementR> distance2(glm::vec<Dimensions, ElementL> a,
																			 glm::vec<Dimensions, ElementR> b) noexcept
	{
		return length2(glm::vec<Dimensions, std::common_type_t<ElementL, ElementR>>{b} -
					   glm::vec<Dimensions, std::common_type_t<ElementL, ElementR>>{a});
	}

	/// Computes the inverse of a vector.
	/// @tparam Dimensions Number of dimensions of the vector.
	/// @tparam Element Type of the vector elements.
	/// @param v Vector to get the inverse of.
	/// @return `v⁻¹`.
	template <int Dimensions, arithmetic Element>
	[[nodiscard]] constexpr glm::vec<Dimensions, float> inverse(glm::vec<Dimensions, Element> v) noexcept
	{
		glm::vec<Dimensions, float> result;
		for (int i = 0; i < Dimensions; ++i) {
			result[i] = static_cast<float>(1.0f / v[i]);
		}
		return result;
	}

	/// Computes the 2D cross product of two vectors.
	/// @tparam Dimensions Number of dimensions of the vectors.
	/// @tparam ElementL Type of the left vector elements.
	/// @tparam ElementR Type of the right vector elements.
	/// @param a, b Vector values.
	/// @return `a×b`.
	template <arithmetic ElementL, arithmetic ElementR>
	[[nodiscard]] constexpr std::common_type_t<ElementL, ElementR> cross(glm::tvec2<ElementL> a, glm::tvec2<ElementR> b) noexcept
	{
		return a.x * b.y - a.y * b.x;
	}

	/// @}
} // namespace tr