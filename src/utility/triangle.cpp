///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements triangle.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/triangle.hpp"
#include "../../include/tr/utility/vector.hpp"

///////////////////////////////////////////////////////////////// TRIANGLE ////////////////////////////////////////////////////////////////

tr::winding_order tr::triangle::winding_order() const
{
	return cross(b - a, c - a) >= 0 ? winding_order::ccw : winding_order::cw;
}

bool tr::triangle::contains(glm::vec2 p) const
{
	const float s{cross(a - c, p - c)};
	const float t{cross(b - a, p - a)};

	if ((s < 0) != (t < 0) && s != 0 && t != 0) {
		return false;
	}

	const float d{cross(c - b, p - b)};
	return d == 0 || (d < 0) == (s + t <= 0);
}