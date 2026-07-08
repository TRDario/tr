///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements circle.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/circle.hpp"
#include "../../include/tr/utility/math.hpp"
#include "../../include/tr/utility/vector.hpp"

////////////////////////////////////////////////////////////////// CIRCLE /////////////////////////////////////////////////////////////////

bool tr::circle::contains(glm::vec2 point) const
{
	return distance2(center, point) <= sqr(radius);
}

//

bool tr::intersecting(const circle& c1, const circle& c2)
{
	return glm::distance(c1.center, c2.center) <= (c1.radius + c2.radius);
}