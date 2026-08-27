/// @file
/// @brief Provides `tr::circle` and related functions.

#pragma once
#include "default_binary_io.hpp"

//

namespace tr
{
	/// Circle datatype.
	struct circle
	{
		/// Offset of the center of the circle.
		glm::vec2 center;

		/// Radius of the circle.
		float radius;

		//

		/// Determines whether a point is contained inside the circle.
		/// @param point Point to check.
		/// @return `true` if `point` is inside the circle, `false` otherwise.
		bool contains(glm::vec2 point) const;
	};

	/// @name Intersection
	/// @{

	/// Determines if two circles intersect.
	/// @param c1, c2 Circles to check for intersection.
	/// @return `true` if the circles intersect, `false` otherwise.
	bool intersecting(const circle& c1, const circle& c2);

	/// @}
} // namespace tr

/// Enables default binary IO for circles.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::circle>{true};