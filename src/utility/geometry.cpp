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
	return cross2(b - a, c - a) >= 0 ? winding_order::CCW : winding_order::CW;
}

bool tr::triangle::contains(glm::vec2 p) const
{
	const glm::vec2 r{p - a};
	const bool sign = (b.x - a.x) * r.y - (b.y - a.y) * r.x > 0;

	if ((c.x - a.x) * r.y - (c.y - a.y) * r.x > 0 == sign || (c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x) > 0 != sign) {
		return false;
	}
	else {
		return true;
	}
}

bool tr::circle::contains(glm::vec2 point) const
{
	const float dx{point.x - c.x};
	const float dy{point.y - c.y};
	return dx * dx + dy * dy <= r * r;
}

bool tr::intersecting(const circle& c1, const circle& c2)
{
	return glm::distance(c1.c, c2.c) <= (c1.r + c2.r);
}

bool tr::collinear(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	const float tolerance{std::abs(std::max({a.x, a.y, b.x, b.y, c.x, c.y})) * 0.000001f};
	return std::abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) < tolerance;
}

glm::vec2 tr::closest_point(glm::vec2 p, glm::vec2 a, glm::vec2 b)
{
	const glm::vec2 pp{p - a};
	const glm::vec2 bp{b - a};
	return a + bp * std::clamp(glm::dot(pp, bp) / glm::dot(bp, bp), 0.0f, 1.0f);
}

std::optional<glm::vec2> tr::segment_intersect(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2)
{
	const glm::vec2 r{b2 - a2};
	const glm::vec2 n{b1.y - a1.y, -b1.x + a1.x};
	const float d{glm::dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	const float t{glm::dot(a1 - a2, n) / d};
	if (t < 0 || t > 1) {
		return std::nullopt;
	}
	const glm::vec2 result{a2 + r * t};
	if ((result.x < std::min(a1.x, b1.x) || result.x > std::max(a1.x, b1.x)) ||
		(result.x - a1.x == 0 && (result.y < std::min(a1.y, b1.y) || result.y > std::max(a1.y, b1.y)))) {
		return std::nullopt;
	}
	else {
		return result;
	}
}

std::optional<glm::vec2> tr::segment_intersect(glm::vec2 a1, angle th1, glm::vec2 a2, glm::vec2 b2)
{
	const glm::vec2 r{b2 - a2};
	const glm::vec2 n{th1.sin(), -th1.cos()};
	const float d{glm::dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	const float t{glm::dot(a1 - a2, n) / d};
	if (t >= 0 && t <= 1) {
		return a2 + r * t;
	}
	else {
		return std::nullopt;
	}
}

std::optional<glm::vec2> tr::intersect(glm::vec2 a1, angle th1, glm::vec2 a2, angle th2)
{
	const glm::vec2 r{th2.cos(), th2.sin()};
	const glm::vec2 n{th1.sin(), -th1.cos()};
	const float d{glm::dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	const float t{glm::dot(a1 - a2, n) / d};
	return a2 + r * t;
}

std::optional<glm::vec2> tr::intersect(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2)
{
	const glm::vec2 r{b2 - a2};
	const glm::vec2 n{b1.y - a1.y, -b1.x + a1.x};
	const float d{glm::dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	const float t{glm::dot(a1 - a2, n) / d};
	return a2 + r * t;
}

std::optional<glm::vec2> tr::intersect(glm::vec2 a1, angle th1, glm::vec2 a2, glm::vec2 b2)
{
	const glm::vec2 r{b2 - a2};
	const glm::vec2 n{th1.sin(), -th1.cos()};
	const float d{glm::dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	const float t{glm::dot(a1 - a2, n) / d};
	return a2 + r * t;
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

bool tr::self_intersecting(std::span<const glm::vec2> vertices)
{
	enum class endpoint_type {
		LEFT,
		RIGHT
	};
	struct endpoint {
		int index;
		endpoint_type type;
	};
	const auto endpoint_comparator{[&](endpoint l, endpoint r) {
		glm::vec2 lv{vertices[l.index]};
		glm::vec2 rv{vertices[r.index]};
		return lv.x < rv.x || (lv.x == rv.x && lv.y < rv.y);
	}};
	struct segment {
		glm::vec2 a;
		glm::vec2 b;

		bool operator==(const segment& r) const = default;

		float y_at(float x) const
		{
			if (a.x == a.x) {
				return a.y;
			}
			else {
				return a.y + ((x - a.x) / (b.x - a.x)) * (b.y - a.y);
			}
		}
	};

	if (vertices.size() <= 3) {
		return false;
	}

	std::vector<segment> sweep_line;
	std::vector<endpoint> queue;
	for (usize i = 0; i < vertices.size() - 1; ++i) {
		queue.push_back({int(i), endpoint_type::LEFT});
		queue.push_back({int(i + 1), endpoint_type::RIGHT});
	}
	std::ranges::sort(queue, endpoint_comparator);

	for (usize i = 0; i < queue.size(); ++i) {
		const endpoint ep{queue[i]};
		const auto comparator{[x = vertices[ep.index].x](const segment& l, const segment& r) { return l.y_at(x) < r.y_at(x); }};

		std::ranges::sort(sweep_line, comparator);
		switch (ep.type) {
		case endpoint_type::LEFT: {
			const segment segment{vertices[ep.index], vertices[ep.index + 1]};
			const auto it{sweep_line.insert(std::ranges::upper_bound(sweep_line, segment, comparator), segment)};
			if (it < std::prev(sweep_line.end())) {
				const auto next{std::next(it)};
				if (segment_intersect(it->a, it->b, next->a, next->b).has_value()) {
					return true;
				}
			}
			if (it > sweep_line.begin()) {
				const auto prev{std::prev(it)};
				if (segment_intersect(it->a, it->b, prev->a, prev->b).has_value()) {
					return true;
				}
			}
		} break;
		case endpoint_type::RIGHT: {
			const segment segment{vertices[ep.index - 1], vertices[ep.index]};
			const auto it{std::ranges::find(sweep_line, segment)};
			if (it > sweep_line.begin() && it < std::prev(sweep_line.end())) {
				const auto prev{std::prev(it)};
				const auto next{std::next(it)};
				if (segment_intersect(prev->a, prev->b, next->a, next->b).has_value()) {
					return true;
				}
			}
			if (it != sweep_line.end()) {
				sweep_line.erase(it);
			}
		} break;
		}
	}
	return false;
}

bool tr::point_in_polygon(glm::vec2 p, std::span<const glm::vec2> vertices)
{
	const auto ray_intersecting{[](glm::vec2 p, glm::vec2 a, glm::vec2 b) {
		std::optional<glm::vec2> intersection{segment_intersect(p, 0_deg, a, b)};
		return intersection.has_value() && intersection->x >= p.x;
	}};

	int hits{0};
	for (usize i = 0; i < vertices.size() - 1; ++i) {
		hits += ray_intersecting(p, vertices[i], vertices[i + 1]);
	}
	hits += ray_intersecting(p, vertices.back(), vertices.front());
	return bool(hits % 2);
}