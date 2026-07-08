///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements line.hpp.                                                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/utility/line.hpp"
#include "../../include/tr/utility/vector.hpp"

////////////////////////////////////////////////////// LINE SEGMENT AND INTERSECTION //////////////////////////////////////////////////////

float tr::line_segment::length() const
{
	return glm::distance(a, b);
}

float tr::line_segment::length2() const
{
	return distance2(a, b);
}

//

glm::vec2 tr::line_segment::closest_point(glm::vec2 p) const
{
	const glm::vec2 pdelta{p - a};
	const glm::vec2 delta{b - a};
	const float length2{glm::dot(delta, delta)};
	return a + delta * std::clamp(glm::dot(pdelta, delta) / length2, 0.0f, 1.0f);
}

//

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

bool tr::collinear(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	const float tolerance{std::abs(std::max({a.x, a.y, b.x, b.y, c.x, c.y})) * 1e-6f};
	return std::abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) < tolerance;
}

//

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