///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides polygon-related datatypes and functions.                                                                                     //
//                                                                                                                                       //
// A winding order enumerator is provided (with values cw and ccw).                                                                      //
//                                                                                                                                       //
// Polygon views can be checked for their winding order, whether they're simple, whether a point is contained inside then, or whether    //
// two polygons intersect:                                                                                                               //
//     - tr::polygon_winding_order(vertices) -> tr::winding_order::cw                                                                    //
//     - tr::simple_polygon(vertices) -> true                                                                                            //
//     - tr::point_in_polygon(vertices, {0, 0}) -> false                                                                                 //
//     - tr::intersecting(vertices1, vertices2) -> true                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Alias for a view to a polygon.
	using polygon_view = std::span<const glm::vec2>;

	// Polygon winding order.
	enum class winding_order : bool {
		cw, // Clockwise winding order.
		ccw // Counter-clockwise winding order.
	};

	// Finds the winding order of a polygon.
	winding_order polygon_winding_order(polygon_view vertices);
	// Determines whether a polygon is simple (not self-intersecting).
	bool simple_polygon(polygon_view vertices);
	// Determines whether a point is contained in a polygon.
	bool point_in_polygon(glm::vec2 p, polygon_view vertices);
	// Determines whether two polygons are intersecting.
	bool intersecting(polygon_view a, polygon_view b);
} // namespace tr