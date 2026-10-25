/// @file
/// @brief Implements polygon.hpp.

#include <tr/utility/line.hpp>
#include <tr/utility/polygon.hpp>
#include <tr/utility/triangle.hpp>
#include <tr/utility/vector.hpp>

//

tr::winding_order tr::polygon_winding_order(polygon_view vertices) noexcept
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

bool tr::simple_polygon(polygon_view vertices) noexcept
{
	for (usize i = 0; i < vertices.size(); ++i) {
		const usize ni{(i + 1) % vertices.size()};
		const line_segment<float> a{vertices[i], vertices[ni]};
		for (usize j = 0; j < vertices.size(); ++j) {
			const usize nj{(j + 1) % vertices.size()};
			const line_segment<float> b{vertices[j], vertices[nj]};
			if (i != j && ni != j && nj != i && intersecting(a, b)) {
				return false;
			}
		}
	}
	return true;
}

bool tr::point_in_polygon(glm::vec2 p, polygon_view vertices) noexcept
{
	bool inside{false};
	for (usize i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
		const glm::dvec2 a{vertices[j]};
		const glm::dvec2 b{vertices[i]};
		const glm::dvec2 min{glm::min(a, b)};
		const glm::dvec2 max{glm::max(a, b)};
		const glm::dvec2 edge_delta{b - a};
		const double cross{tr::cross(edge_delta, glm::dvec2{p} - a)};

		if (cross == 0 && p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y) {
			return true;
		}

		if ((a.y > p.y) != (b.y > p.y) && ((cross > 0) == (edge_delta.y > 0))) {
			inside = !inside;
		}
	}
	return inside;
}

bool tr::intersecting(polygon_view a, polygon_view b) noexcept
{
	if (std::ranges::any_of(a, [&](glm::vec2 p) { return point_in_polygon(p, b); })) {
		return true;
	}
	if (std::ranges::any_of(b, [&](glm::vec2 p) { return point_in_polygon(p, a); })) {
		return true;
	}

	for (usize i = 0; i < a.size(); ++i) {
		const line_segment<float> la{a[i], a[(i + 1) % a.size()]};
		for (usize j = 0; j < b.size(); ++j) {
			const line_segment<float> lb{b[j], b[(j + 1) % b.size()]};
			if (intersecting(la, lb)) {
				return true;
			}
		}
	}

	return false;
}