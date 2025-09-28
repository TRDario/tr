#pragma once
#include "draw_geometry.hpp"

inline tr::usize tr::smooth_polygon_vertices(float r)
{
	TR_ASSERT(r > 0, "Tried to pass a negative radius to smooth_poly_vtx.");

	return std::max(usize(7 * std::pow(r, 1 / 2.4f)), 3_uz);
}

inline tr::usize tr::smooth_arc_vertices(float r, angle sizeth)
{
	TR_ASSERT(r > 0, "Tried to pass a negative radius to smooth_poly_vtx.");
	TR_ASSERT(sizeth >= 0_turns && sizeth <= 1_turns, "Tried to pass an arc size of more than one turn to smooth_arc_vtx.");

	return std::max(usize(7 * std::pow(r, 1 / 2.4f) / (sizeth / 1_turns)), 3_uz);
}

constexpr tr::usize tr::polygon_indices(u16 vtx)
{
	return (vtx - 2) * 3;
}

constexpr tr::usize tr::polygon_outline_indices(u16 vtx)
{
	return vtx * 6;
}

template <std::output_iterator<tr::u16> O> constexpr O tr::fill_polygon_indices(u16 vtx, u16 base, O out)
{
	TR_ASSERT(vtx >= 3, "Tried to calculate indices for {}-sided polygon.", vtx);
	TR_ASSERT(base + polygon_indices(vtx) <= UINT16_MAX, "Index overflow detected in fill_poly_idx.");

	for (u16 i = 0; i < vtx - 2; ++i) {
		*out++ = base;
		*out++ = base + i + 1;
		*out++ = base + i + 2;
	}
	return out;
}

template <std::output_iterator<tr::u16> O> constexpr O tr::fill_polygon_outline_indices(u16 vtx, u16 base, O out)
{
	TR_ASSERT(vtx >= 3, "Tried to calculate indices for {}-sided polygon outline.", vtx);
	TR_ASSERT(base + polygon_outline_indices(vtx) <= UINT16_MAX, "Index overflow detected in fill_poly_outline_idx.");

	for (u16 i = 0; i < vtx - 1; ++i) {
		*out++ = base + i;
		*out++ = base + i + 1;
		*out++ = base + i + vtx;
		*out++ = base + i + 1;
		*out++ = base + i + vtx;
		*out++ = base + i + vtx + 1;
	}
	*out++ = base + vtx - 1;
	*out++ = base;
	*out++ = base + 2 * vtx - 1;
	*out++ = base;
	*out++ = base + 2 * vtx - 1;
	*out++ = base + vtx;
	return out;
}

template <std::output_iterator<glm::vec2> O> constexpr O tr::fill_rectangle_vertices(const frect2& rect, O out)
{
	*out++ = rect.tl;
	*out++ = glm::vec2{rect.tl.x, rect.tl.y + rect.size.y};
	*out++ = rect.tl + rect.size;
	*out++ = glm::vec2{rect.tl.x + rect.size.x, rect.tl.y};
	return out;
}

template <std::output_iterator<glm::vec2> O> constexpr O tr::fill_rectangle_vertices(const frect2& rect, const glm::mat4& mat, O out)
{
	*out++ = mat * rect.tl;
	*out++ = mat * glm::vec2{rect.tl.x, rect.tl.y + rect.size.y};
	*out++ = mat * (rect.tl + rect.size);
	*out++ = mat * glm::vec2{rect.tl.x + rect.size.x, rect.tl.y};
	return out;
}

template <std::output_iterator<glm::vec2> O>
O tr::fill_rectangle_vertices(glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, O out)
{
	if (rotation == 0_rad) {
		return fill_rectangle_vertices({pos - anchor, size}, out);
	}
	else {
		return fill_rectangle_vertices({pos - anchor, size}, rotate_around(glm::mat4{}, pos, rotation), out);
	}
}

template <std::output_iterator<glm::vec2> O> constexpr O tr::fill_rectangle_outline_vertices(const frect2& rect, float thickness, O out)
{
	out = fill_rectangle_vertices({rect.tl - thickness / 2, rect.size + thickness}, out);
	return fill_rectangle_vertices({rect.tl + thickness / 2, rect.size - thickness}, out);
}

template <std::output_iterator<glm::vec2> O>
constexpr O tr::fill_rectangle_outline_vertices(const frect2& rect, float thickness, const glm::mat4& mat, O out)
{
	out = fill_rectangle_vertices({rect.tl - thickness / 2, rect.size + thickness}, mat, out);
	return fill_rectangle_vertices({rect.tl + thickness / 2, rect.size - thickness}, mat, out);
}

template <std::output_iterator<glm::vec2> O>
O tr::fill_rectangle_outline_vertices(glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness, O out)
{
	if (rotation == 0_rad) {
		return fill_rectangle_outline_vertices(pos - anchor, size, thickness, out);
	}
	else {
		return fill_rectangle_outline_vertices(pos - anchor, size, thickness, rotate_around(glm::mat4{}, pos, rotation), out);
	}
}

template <std::output_iterator<glm::vec2> O> O tr::fill_arc_vertices(usize vtx, circle circle, angle start, angle size, O out)
{
	angle dth{size / vtx};
	float dsin{dth.sin()};
	float dcos{dth.cos()};
	glm::vec2 delta{circle.r * start.cos(), circle.r * start.sin()};
	for (usize i = 0; i < vtx; ++i) {
		*out++ = delta + circle.c;
		delta = glm::vec2{dcos * delta.x - dsin * delta.y, dsin * delta.x + dcos * delta.y};
	}
	return out;
}

template <tr::sized_output_range<glm::vec2> R> void tr::fill_arc_vertices(circle circle, angle start, angle size, R&& out)
{
	fill_arc_vertices(std::size(out), circle, start, size, std::begin(out));
}

template <std::output_iterator<glm::vec2> O> O tr::fill_polygon_vertices(usize vtx, circle circle, angle rotation, O out)
{
	return fill_arc_vertices(out, vtx, circle, rotation, 1_turns);
}

template <tr::sized_output_range<glm::vec2> R> void tr::fill_polygon_vertices(circle circle, angle rotation, R&& out)
{
	fill_polygon_vertices(std::size(out), circle, rotation, std::begin(out));
}

template <std::output_iterator<glm::vec2> O> O tr::fill_circle_vertices(usize vtx, circle circle, O out)
{
	return fill_arc_vertices(out, vtx, circle, 0_turns, 1_turns);
}

template <tr::sized_output_range<glm::vec2> R> void tr::fill_circle_vertices(circle circle, R&& out)
{
	fill_circle_vertices(std::size(out), circle, std::begin(out));
}

template <std::output_iterator<glm::vec2> O>
O tr::fill_polygon_outline_vertices(usize vtx, circle circle, angle rotation, float thickness, O out)
{
	out = fill_polygon_vertices(vtx, {circle.c, circle.r + thickness / 2}, rotation, out);
	return fill_polygon_vertices(vtx, {circle.c, circle.r - thickness / 2}, rotation, out);
}

template <tr::sized_output_range<glm::vec2> R>
void tr::fill_polygon_outline_vertices(circle circle, angle rotation, float thickness, R&& out)
{
	fill_polygon_outline_vertices(std::size(out), circle, rotation, thickness, std::begin(out));
}

template <std::output_iterator<glm::vec2> O> O tr::fill_circle_outline_vertices(usize vtx, circle circle, float thickness, O out)
{
	out = fill_circle_vertices(vtx, {circle.c, circle.r + thickness / 2}, out);
	return fill_circle_vertices(vtx, {circle.c, circle.r - thickness / 2}, out);
}

template <tr::sized_output_range<glm::vec2> R> void tr::fill_circle_outline_vertices(circle circle, float thickness, R&& out)
{
	fill_circle_outline_vertices(std::size(out), circle, thickness, std::begin(out));
}