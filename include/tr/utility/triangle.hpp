///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a triangle datatype.                                                                                                         //
//                                                                                                                                       //
// A triangle is defined by 3 points designated .a, .b, and .c:                                                                          //
//     - tr::triangle tri{{500, 500}, {600, 600}, {700, 500}}                                                                            //
//                                                                                                                                       //
// The winding order of a triangle may be determined:                                                                                    //
//     - tri.winding_order() -> tr::winding_order::cw                                                                                    //
//                                                                                                                                       //
// Triangles may be checked for containing a point:                                                                                      //
//     - tri.contains({600, 550}) -> true                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "binary_io.hpp"
#include "polygon.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// 2D triangle.
	struct triangle {
		// The first point of the triangle.
		glm::vec2 a;
		// The second point of the triangle.
		glm::vec2 b;
		// The third point of the triangle.
		glm::vec2 c;

		// Determines the winding order of the triangle.
		winding_order winding_order() const;
		// Determines whether a point is contained inside the triangle.
		bool contains(glm::vec2 point) const;
	};
} // namespace tr

// Triangle binary reader.
template <> struct tr::binary_reader<tr::triangle> : tr::raw_binary_reader<tr::triangle> {};
// Triangle binary writer.
template <> struct tr::binary_writer<tr::triangle> : tr::raw_binary_writer<tr::triangle> {};