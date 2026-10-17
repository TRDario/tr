/// @file
/// @brief Implements triangle.hpp.

#include <tr/utility/triangle.hpp>
#include <tr/utility/vector.hpp>

//

tr::winding_order tr::triangle::winding_order() const noexcept
{
	return cross(b - a, c - a) <= 0 ? winding_order::ccw : winding_order::cw;
}

bool tr::triangle::contains(glm::vec2 p) const noexcept
{
	const float dx{p.x - c.x};
	const float dy{p.y - c.y};
	const float dx_cb{c.x - b.x};
	const float dy_bc{b.y - c.y};
	const float d{dy_bc * (a.x - c.x) + dx_cb * (a.y - c.y)};
	const float s{dy_bc * dx + dx_cb * dy};
	const float t{(c.y - a.y) * dx + (a.x - c.x) * dy};
	return d < 0 ? (s <= 0 && t <= 0 && s + t >= d) : (s >= 0 && t >= 0 && s + t <= d);
}