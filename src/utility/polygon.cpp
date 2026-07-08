///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements polygon.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/polygon.hpp"
#include "../../include/tr/utility/line.hpp"
#include "../../include/tr/utility/triangle.hpp"

///////////////////////////////////////////////////////////////// POLYGONS ////////////////////////////////////////////////////////////////

tr::winding_order tr::polygon_winding_order(polygon_view vertices)
{
	usize min_y_index{0};
	for (usize i = 1; i < vertices.size(); ++i) {
		if (vertices[i].y < vertices[min_y_index].y) {
			min_y_index = i;
		}
	}

	const glm::vec2 a{vertices[min_y_index == 0 ? vertices.size() - 1 : min_y_index - 1]};
	const glm::vec2 b{vertices[min_y_index]};
	const glm::vec2 c{vertices[min_y_index == vertices.size() - 1 ? 0 : min_y_index + 1]};
	return triangle{a, b, c}.winding_order();
}

bool tr::simple_polygon(polygon_view vertices)
{
	for (usize i = 0; i < vertices.size(); ++i) {
		const usize ni{(i + 1) % vertices.size()};
		const line_segment a{vertices[i], vertices[ni]};
		for (usize j = 0; j < vertices.size(); ++j) {
			const usize nj{(j + 1) % vertices.size()};
			const line_segment b{vertices[j], vertices[nj]};
			if (i != j && ni != j && nj != i && intersecting(a, b)) {
				return false;
			}
		}
	}
	return true;
}

bool tr::point_in_polygon(glm::vec2 p, polygon_view vertices)
{
	usize hits{0};
	for (usize i = 0; i < vertices.size(); ++i) {
		const line_segment segment{vertices[i], vertices[(i + 1) % vertices.size()]};
		const std::optional<glm::vec2> ray_intersection{intersection(p, 0_deg, segment)};
		hits += ray_intersection.has_value() && ray_intersection->x >= p.x && *ray_intersection != segment.b;
	}
	return hits % 2 == 1;
}

bool tr::intersecting(polygon_view a, polygon_view b)
{
	if (std::ranges::any_of(a, [&](glm::vec2 p) { return point_in_polygon(p, b); })) {
		return true;
	}
	if (std::ranges::any_of(b, [&](glm::vec2 p) { return point_in_polygon(p, a); })) {
		return true;
	}

	for (usize i = 0; i < a.size(); ++i) {
		const line_segment la{a[i], a[(i + 1) % a.size()]};
		for (usize j = 0; j < b.size(); ++j) {
			const line_segment lb{b[j], b[(j + 1) % b.size()]};
			if (intersecting(la, lb)) {
				return true;
			}
		}
	}

	return false;
}