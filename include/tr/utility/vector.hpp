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
	/// @tparam Rep Representation type of the angle and result vector.
	/// @param angle Angle defining the direction the vector will be pointing in.
	/// @return Normalized vector pointing in the direction defined by `angle`.
	template <std::floating_point Rep>
	[[nodiscard]] glm::tvec2<Rep> normal(angle<Rep> angle) noexcept
	{
		return glm::tvec2<Rep>{angle.cos(), angle.sin()};
	}

	/// Creates a vector from a magnitude and angle.
	/// @tparam Rep Representation type of the magnitude, angle and result vector.
	/// @param magnitude Magnitude of the vector.
	/// @param angle Angle defining the direction the vector will be pointing in.
	/// @return Vector pointing in the direction defined by `angle` with magnitude `magnitude`.
	template <std::floating_point Rep>
	[[nodiscard]] glm::tvec2<Rep> magth(Rep magnitude, angle<float> angle) noexcept
	{
		return normal(angle) * magnitude;
	}

	//

	/// Computes the squared length of a vector.
	/// @tparam Dimensions Number of dimensions of the vector.
	/// @tparam Rep Representation type of the vector.
	/// @param a Vector to get the square length of.
	/// @return '|a|²'.
	template <int Dimensions, arithmetic Rep>
	[[nodiscard]] constexpr Rep length2(glm::vec<Dimensions, Rep> a) noexcept
	{
		if constexpr (Dimensions == 1) {
			return sqr(a.x);
		}
		else if constexpr (Dimensions == 2) {
			return sqr(a.x) + sqr(a.y);
		}
		else if constexpr (Dimensions == 3) {
			return sqr(a.x) + sqr(a.y) + sqr(a.z);
		}
		else if constexpr (Dimensions == 4) {
			return sqr(a.x) + sqr(a.y) + sqr(a.z) + sqr(a.w);
		}
		else {
			TR_UNREACHABLE;
		}
	}

	/// Computes the squared distance between two points.
	/// @tparam Dimensions Number of dimensions of the vectors.
	/// @tparam ARep, BRep Representation types of the vectors.
	/// @param a, b Vectors to the points the distance is being measured for.
	/// @return `|b-a|²`.
	template <int Dimensions, arithmetic ARep, arithmetic BRep>
	[[nodiscard]] constexpr std::common_type_t<ARep, BRep> distance2(glm::vec<Dimensions, ARep> a, glm::vec<Dimensions, BRep> b) noexcept
	{
		return length2(glm::vec<Dimensions, std::common_type_t<ARep, BRep>>{b} - glm::vec<Dimensions, std::common_type_t<ARep, BRep>>{a});
	}

	/// Computes the inverse of a vector.
	/// @tparam Dimensions Number of dimensions of the vector.
	/// @tparam Rep Representation type of the vector.
	/// @param a Vector to get the inverse of.
	/// @return `a⁻¹`.
	template <int Dimensions, arithmetic Rep>
	[[nodiscard]] constexpr auto inverse(glm::vec<Dimensions, Rep> a) noexcept
	{
		using CommonRep = std::conditional_t<std::integral<Rep>, double, Rep>;

		glm::vec<Dimensions, CommonRep> result;
		for (int i = 0; i < Dimensions; ++i) {
			result[i] = CommonRep{1} / a[i];
		}
		return result;
	}

	/// Computes the 2D cross product of two vectors.
	/// @tparam Dimensions Number of dimensions of the vectors.
	/// @tparam ARep, BRep Representation types of the vectors.
	/// @param a, b Values to get the cross product of.
	/// @return `a×b`.
	template <arithmetic ARep, arithmetic BRep>
	[[nodiscard]] constexpr std::common_type_t<ARep, BRep> cross(glm::tvec2<ARep> a, glm::tvec2<BRep> b) noexcept
	{
		return a.x * b.y - a.y * b.x;
	}

	/// @}
} // namespace tr