/// @file
/// @brief Provides `tr::circle` and related functions.

#pragma once
#include <tr/utility/arithmetic_concepts.hpp>
#include <tr/utility/default_binary_io.hpp>
#include <tr/utility/vector.hpp>

//

namespace tr
{
	/// Circle datatype.
	/// @tparam Rep Representation type of the circle.
	template <std::floating_point Rep>
	struct circle
	{
		/// Offset of the center of the circle.
		glm::tvec2<Rep> center;

		/// Radius of the circle.
		Rep radius;

		/// @name Conversion operators
		/// @{

		/// Casts the circle to a circle represented with another representation type.
		/// @tparam TargetRep Representation type to cast to.
		/// @return Cast circle value.
		template <std::floating_point TargetRep>
		[[nodiscard]] explicit constexpr operator circle<TargetRep>() const noexcept
		{
			return circle<TargetRep>{static_cast<glm::tvec2<TargetRep>>(center), static_cast<TargetRep>(radius)};
		}

		/// @}
		/// @name Contains
		/// @{

		/// Determines whether a point is contained inside the circle.
		/// @tparam PointRep Representation type of the point.
		/// @param point Point to check.
		/// @return `true` if `point` is inside the circle, `false` otherwise.
		template <arithmetic PointRep>
		[[nodiscard]] constexpr bool contains(glm::tvec2<PointRep> point) const noexcept
		{
			return distance2(center, point) <= sqr(radius);
		}

		/// @}
	};

	/// @name Intersection
	/// @{

	/// Determines if two circles intersect.
	/// @tparam LhsRep, RhsRep Representation types of the circles.
	/// @param lhs, rhs Circles to check for intersection.
	/// @return `true` if the circles intersect, `false` otherwise.
	template <std::floating_point LhsRep, std::floating_point RhsRep>
	[[nodiscard]] constexpr bool intersecting(const circle<LhsRep>& lhs, const circle<RhsRep>& rhs) noexcept
	{
		return distance2(lhs.center, rhs.center) <= sqr(lhs.radius + rhs.radius);
	}

	/// @}
} // namespace tr

//

/// Enables default binary IO for circles.
template <std::floating_point Rep>
inline constexpr bool tr::enable_default_binary_io<tr::circle<Rep>>{true};