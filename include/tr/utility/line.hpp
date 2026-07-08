///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a line segment datatype and line-related functions.                                                                          //
//                                                                                                                                       //
// A line segment is defined by two points (designated .a and .b):                                                                       //
//     - tr::line_segment line{{500, 500}, {750, 750}} -> {.a{500, 500}, .b{750, 750}}                                                   //
//                                                                                                                                       //
// The length and squared length of a line segment can be gotten, as well as the closest point to the line:                              //
//     - line.length() -> 353.553391                                                                                                     //
//     - line.length2() -> 125000                                                                                                        //
//     - line.closest_point({750, 500}) -> {625, 625}                                                                                    //
//                                                                                                                                       //
// Three points can be checked for collinearity:                                                                                         //
//     - tr::collinear({5, 5}, {10, 10}, {20, 20}) -> true                                                                               //
//                                                                                                                                       //
// The intersection point of two line segments or lines can be determined:                                                               //
//     - tr::intersecting(tr::line_segment{{100, 100}, {200, 200}}, tr::line_segment{{100, 200}, {200, 100}}) -> true                    //
//     - tr::intersection(tr::line_segment{{100, 100}, {200, 200}}, tr::line_segment{{100, 200}, {200, 100}}) -> {150, 150}              //
//     - tr::intersection(tr::line_segment{{100, 100}, {200, 100}}, {250, 50}, {250, 150}) -> std::nullopt                               //
//     - tr::intersection({100, 100}, {200, 100}, {250, 50}, {250, 150}) -> {250, 100}                                                   //
//     - tr::intersection({100, 100}, {200, 100}, {250, 50}, 90_deg) -> {250, 100}                                                       //
//     - tr::intersection({100, 100}, 0_deg, {250, 50}, 90_deg) -> {250, 100}                                                            //
//                                                                                                                                       //
// Line segments are binary readable and writable.                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "angle.hpp"
#include "default_binary_io.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// 2D line segment.
	struct line_segment {
		// The first point of the line segment.
		glm::vec2 a;
		// The second point of the line segment.
		glm::vec2 b;

		// Length of the line segment.
		float length() const;
		// Square length of the line segment.
		float length2() const;

		// Calculates the closest point to p on the line segment.
		glm::vec2 closest_point(glm::vec2 p) const;
	};

	// Determines whether 3 points are collinear.
	bool collinear(glm::vec2 a, glm::vec2 b, glm::vec2 c);
	// Determines if two line segments intersect.
	bool intersecting(const line_segment& l, const line_segment& r);

	// Calculates the intersection point of two line segments.
	std::optional<glm::vec2> intersection(const line_segment& l, const line_segment& r);
	// Calculates the intersection point of a line with a line segment.
	std::optional<glm::vec2> intersection(glm::vec2 lp, angle lth, const line_segment& r);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> intersection(glm::vec2 a1, angle th1, glm::vec2 a2, angle th2);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> insersection(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2);
	// Calculates the intersection point of two lines.
	std::optional<glm::vec2> intersection(glm::vec2 a1, angle th1, glm::vec2 a2, glm::vec2 b2);

} // namespace tr

// Enables default binary IO for line segments.
template <> inline constexpr bool tr::enable_default_binary_io<tr::line_segment>{true};