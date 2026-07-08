///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a circle datatype and related functions.                                                                                     //
//                                                                                                                                       //
// Circles are defined by a center and radius:                                                                                           //
//     - tr::circle circle{{500, 500}, 100} -> circle with radius 100 centered at (500, 500)                                             //
//                                                                                                                                       //
// Circles can be checked for containing a point:                                                                                        //
//     - circle.contains({450, 550}) -> true                                                                                             //
//     - circle.contains({-450, -550}) -> false                                                                                          //
//                                                                                                                                       //
// Circles are binary readable and writable.                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "binary_io.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// 2D circle.
	struct circle {
		// The offset of the center of the circle.
		glm::vec2 center;
		// The radius of the circle.
		float radius;

		// Determines whether a point is contained inside the circle.
		bool contains(glm::vec2 point) const;
	};

	// Determines if two circles intersect.
	bool intersecting(const circle& c1, const circle& c2);
} // namespace tr

// Circle binary reader.
template <> struct tr::binary_reader<tr::circle> : tr::raw_binary_reader<tr::circle> {};
// Circle binary writer.
template <> struct tr::binary_writer<tr::circle> : tr::raw_binary_writer<tr::circle> {};