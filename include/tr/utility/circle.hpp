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
#include "default_binary_io.hpp"

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

// Enables default binary IO for circles.
template <> inline constexpr bool tr::enable_default_binary_io<tr::circle>{true};