/// @file
/// @brief Provides a line segment datatype and line-related functions.

#pragma once
#include "angle.hpp"
#include "default_binary_io.hpp"

//

namespace tr
{
	/// 2D line segment datatype.
	struct line_segment
	{
		/// The first point of the line segment.
		glm::vec2 a;

		/// The second point of the line segment.
		glm::vec2 b;

		/// @name Length
		/// @{

		/// Gets the length of the line segment.
		/// @return `|b-a|`.
		float length() const;

		/// Gets the square length of the line segment.
		/// @return `|b-a|²`.
		float length2() const;

		/// @}
		/// @name Points
		/// @{

		/// Calculates the closest point to `p` on the line segment.
		/// @param p Point to project onto the line.
		/// @return Projection of `p` onto the line.
		glm::vec2 closest_point(glm::vec2 p) const;

		/// @}
	};

	/// @name Intersection
	/// @{

	/// Determines whether 3 points are collinear.
	/// @param a, b, c Points to check for collinearity.
	/// @return `true` if the points are collinear, `false` otherwise.
	bool collinear(glm::vec2 a, glm::vec2 b, glm::vec2 c);

	/// Determines if two line segments intersect.
	/// @param l, r Line segments to check for intersection.
	/// @return `true` if the lines intersect, `false` otherwise.
	bool intersecting(const line_segment& l, const line_segment& r);

	/// Calculates the intersection point of two line segments.
	/// @param l, r Line segments to check for intersection.
	/// @return Intersection point of the lines, if one exists.
	std::optional<glm::vec2> intersection(const line_segment& l, const line_segment& r);

	/// Calculates the intersection point of a line with a line segment.
	/// @param lp Point on the first line.
	/// @param lth Angle of the first line.
	/// @param r Line segment to check for intersection.
	/// @return Intersection point of the lines, if one exists.
	std::optional<glm::vec2> intersection(glm::vec2 lp, angle lth, const line_segment& r);

	/// Calculates the intersection point of two lines.
	/// @param lp Point on the first line.
	/// @param lth Angle of the first line.
	/// @param rp Point on the second line.
	/// @param rth Angle of the second line.
	/// @return Intersection point of the lines, if one exists.
	std::optional<glm::vec2> intersection(glm::vec2 lp, angle lth, glm::vec2 rp, angle rth);

	/// Calculates the intersection point of two lines.
	/// @param lp Point on the first line.
	/// @param lth Angle of the first line.
	/// @param ra, rb Points on the second line.
	/// @return Intersection point of the lines, if one exists.
	std::optional<glm::vec2> intersection(glm::vec2 lp, angle lth, glm::vec2 ra, glm::vec2 rb);

	/// Calculates the intersection point of two lines.
	/// @param la, lb Points on the first line.
	/// @param ra, rb Points on the second line.
	/// @return Intersection point of the lines, if one exists.
	std::optional<glm::vec2> insersection(glm::vec2 la, glm::vec2 lb, glm::vec2 ra, glm::vec2 rb);

	/// @}
} // namespace tr

/// Enables default binary IO for line segments.
template <>
inline constexpr bool tr::enable_default_binary_io<tr::line_segment>{true};