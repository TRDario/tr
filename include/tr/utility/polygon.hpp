/// @file
/// @brief Provides polygon-related datatypes and functions.

#pragma once
#include "common.hpp"

//

namespace tr
{
	/// Alias for a view to a polygon's vertices.
	using polygon_view = std::span<const glm::vec2>;

	/// Polygon winding order.
	enum class winding_order : bool
	{
		/// Clockwise winding order.
		cw,
		/// Counter-clockwise winding order.
		ccw
	};

	/// @name Polygons
	/// @{

	/// Finds the winding order of a polygon.
	/// @param polygon View to a polygon.
	/// @return Winding order of `polygon`.
	winding_order polygon_winding_order(polygon_view polygon);

	/// Determines whether a polygon is simple (not self-intersecting).
	/// @param polygon View to a polygon.
	/// @return `true` if `polygon` is simple, `false` otherwise.
	bool simple_polygon(polygon_view polygon);

	/// Determines whether a point is contained in a polygon.
	/// @param p Point vector.
	/// @param polygon View to a polygon.
	/// @return `true` if `p` is within `polygon`, `false` otherwise.
	bool point_in_polygon(glm::vec2 p, polygon_view polygon);

	/// Determines whether two polygons are intersecting.
	/// @param a, b Views to polygons.
	/// @return `true` if `a` and `b` intersect, `false` otherwise.
	bool intersecting(polygon_view a, polygon_view b);

	/// @}
} // namespace tr