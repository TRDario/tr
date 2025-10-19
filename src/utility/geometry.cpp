#include "../../include/tr/utility/geometry.hpp"

glm::vec2 tr::normal(angle th)
{
	return glm::vec2{th.cos(), th.sin()};
}

glm::vec2 tr::magth(float mag, angle th)
{
	return normal(th) * mag;
}

tr::winding_order tr::triangle::winding_order() const
{
	return cross(b - a, c - a) >= 0 ? winding_order::CCW : winding_order::CW;
}

bool tr::triangle::contains(glm::vec2 p) const
{
	const glm::vec2 delta{p - a};
	const bool sign{winding_order() == winding_order::CCW};
	return cross(c - a, delta) > 0 != sign && cross(c - b, p - b) > 0 == sign;
}

bool tr::circle::contains(glm::vec2 point) const
{
	const glm::vec2 delta{point - c};
	return delta.x * delta.x + delta.y * delta.y <= r * r;
}

bool tr::intersecting(const circle& c1, const circle& c2)
{
	return glm::distance(c1.c, c2.c) <= (c1.r + c2.r);
}

//

bool tr::collinear(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	const float tolerance{std::abs(std::max({a.x, a.y, b.x, b.y, c.x, c.y})) * 1e-6f};
	return std::abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) < tolerance;
}

//

glm::vec2 tr::line_segment::closest_point(glm::vec2 p) const
{
	const glm::vec2 pdelta{p - a};
	const glm::vec2 delta{b - a};
	const float length2{glm::dot(delta, delta)};
	return a + delta * std::clamp(glm::dot(pdelta, delta) / length2, 0.0f, 1.0f);
}

bool tr::intersecting(const line_segment& l, const line_segment& r)
{
	const glm::vec2 delta{r.b - r.a};
	const glm::vec2 normal{l.b.y - l.a.y, -l.b.x + l.a.x};
	const float dot{glm::dot(delta, normal)};
	if (std::abs(dot) < 1e-6f) {
		return false;
	}
	const float t{glm::dot(l.a - r.a, normal) / dot};
	if (t < 0 || t > 1) {
		return false;
	}
	const glm::vec2 result{r.a + delta * t};
	const glm::vec2 min{glm::min(l.a, l.b)};
	const glm::vec2 max{glm::max(l.a, l.b)};
	return result.x >= min.x && result.x <= max.x && (result.x != l.a.x || (result.y >= min.y && result.y <= max.y));
}

std::optional<glm::vec2> tr::intersection(const line_segment& l, const line_segment& r)
{
	const glm::vec2 delta{r.b - r.a};
	const glm::vec2 normal{l.b.y - l.a.y, -l.b.x + l.a.x};
	const float dot{glm::dot(delta, normal)};
	if (std::abs(dot) < 1e-6f) {
		return std::nullopt;
	}
	const float t{glm::dot(l.a - r.a, normal) / dot};
	if (t < 0 || t > 1) {
		return std::nullopt;
	}
	const glm::vec2 result{r.a + delta * t};
	const glm::vec2 min{glm::min(l.a, l.b)};
	const glm::vec2 max{glm::max(l.a, l.b)};
	if ((result.x < min.x || result.x > max.x) || (result.x == l.a.x && (result.y < min.y || result.y > max.y))) {
		return std::nullopt;
	}
	else {
		return result;
	}
}

std::optional<glm::vec2> tr::intersection(glm::vec2 lp, angle lth, const line_segment& r)
{
	const glm::vec2 delta{r.b - r.a};
	const glm::vec2 normal{lth.sin(), -lth.cos()};
	const float dot{glm::dot(delta, normal)};
	if (std::abs(dot) < 1e-6f) {
		return std::nullopt;
	}
	const float t{glm::dot(lp - r.a, normal) / dot};
	if (t >= 0 && t <= 1) {
		return r.a + delta * t;
	}
	else {
		return std::nullopt;
	}
}

std::optional<glm::vec2> tr::intersection(glm::vec2 a1, angle th1, glm::vec2 a2, angle th2)
{
	const glm::vec2 delta{th2.cos(), th2.sin()};
	const glm::vec2 normal{th1.sin(), -th1.cos()};
	const float dot{glm::dot(delta, normal)};
	if (std::abs(dot) < 1e-6f) {
		return std::nullopt;
	}
	const float t{glm::dot(a1 - a2, normal) / dot};
	return a2 + delta * t;
}

std::optional<glm::vec2> tr::insersection(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2)
{
	const glm::vec2 delta{b2 - a2};
	const glm::vec2 normal{b1.y - a1.y, -b1.x + a1.x};
	const float dot{glm::dot(delta, normal)};
	if (std::abs(dot) < 1e-6f) {
		return std::nullopt;
	}
	const float t{glm::dot(a1 - a2, normal) / dot};
	return a2 + delta * t;
}

std::optional<glm::vec2> tr::intersection(glm::vec2 a1, angle th1, glm::vec2 a2, glm::vec2 b2)
{
	const glm::vec2 delta{b2 - a2};
	const glm::vec2 normal{th1.sin(), -th1.cos()};
	const float dot{glm::dot(delta, normal)};
	if (std::abs(dot) < 1e-6f) {
		return std::nullopt;
	}
	const float t{glm::dot(a1 - a2, normal) / dot};
	return a2 + delta * t;
}

glm::vec2 tr::matrix_operators::operator*(const glm::vec2& v, const glm::mat4& m)
{
	return glm::vec4{v, 0, 1} * m;
}

glm::vec3 tr::matrix_operators::operator*(const glm::vec3& v, const glm::mat4& m)
{
	return glm::vec4{v, 1} * m;
}

glm::vec2 tr::matrix_operators::operator*(const glm::mat4& m, const glm::vec2& v)
{
	return m * glm::vec4{v, 0, 1};
}

glm::vec3 tr::matrix_operators::operator*(const glm::mat4& m, const glm::vec3& v)
{
	return m * glm::vec4{v, 1};
}

glm::mat4 tr::ortho(const frect2& rect)
{
	return glm::ortho(rect.tl.x, rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, rect.tl.y);
}

glm::mat4 tr::scale_around(const glm::mat4& mat, const glm::vec2& c, const glm::vec2& s)
{
	return scale_around(mat, glm::vec3{c, 0}, glm::vec3{s, 1});
}

glm::mat4 tr::scale_around(const glm::mat4& mat, const glm::vec3& c, const glm::vec3& s)
{
	return glm::translate(glm::scale(glm::translate(mat, c), s), -c);
}

glm::mat4 tr::rotate_around(const glm::mat4& mat, const glm::vec2& c, const angle& th)
{
	return rotate_around(mat, {c, 0}, th, Z_AXIS);
}

glm::mat4 tr::rotate_around(const glm::mat4& mat, const glm::vec3& c, const angle& th, const glm::vec3& axis)
{
	return glm::translate(glm::rotate(glm::translate(mat, c), th.rads(), axis), -c);
}

//

tr::winding_order tr::polygon_winding_order(std::span<const glm::vec2> vertices)
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

bool tr::simple_polygon(std::span<const glm::vec2> vertices)
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

bool tr::point_in_polygon(glm::vec2 p, std::span<const glm::vec2> vertices)
{
	usize hits{0};
	for (usize i = 0; i < vertices.size() - 1; ++i) {
		const line_segment segment{vertices[i], vertices[(i + 1) % vertices.size()]};
		const std::optional<glm::vec2> ray_intersection{intersection(p, 0_deg, segment)};
		hits += ray_intersection.has_value() && ray_intersection->x >= p.x;
	}
	return hits % 2 == 1;
}