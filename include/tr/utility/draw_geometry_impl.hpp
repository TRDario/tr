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

template <std::output_iterator<tr::u16> O> constexpr O tr::fill_convex_polygon_indices(O out, u16 vtx, u16 base)
{
	TR_ASSERT(vtx >= 3, "Tried to calculate indices for {}-sided polygon.", vtx);
	TR_ASSERT(base + vtx <= UINT16_MAX, "Index overflow detected in fill_poly_idx.");

	for (u16 i = 0; i < vtx - 2; ++i) {
		*out++ = base;
		*out++ = base + i + 1;
		*out++ = base + i + 2;
	}
	return out;
}

template <std::output_iterator<tr::u16> O> constexpr O tr::fill_convex_polygon_outline_indices(O out, u16 vtx, u16 base)
{
	TR_ASSERT(vtx >= 3, "Tried to calculate indices for {}-sided polygon outline.", vtx);
	TR_ASSERT(base + vtx <= UINT16_MAX, "Index overflow detected in fill_poly_outline_idx.");

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

template <std::output_iterator<glm::vec2> O> constexpr O tr::fill_rectangle_vertices(O out, const frect2& rect)
{
	*out++ = rect.tl;
	*out++ = glm::vec2{rect.tl.x, rect.tl.y + rect.size.y};
	*out++ = rect.tl + rect.size;
	*out++ = glm::vec2{rect.tl.x + rect.size.x, rect.tl.y};
	return out;
}

template <tr::sized_output_range<glm::vec2> R> constexpr void tr::fill_rectangle_vertices(R&& out, const frect2& rect)
{
	TR_ASSERT(std::size(out) == 4, "Tried to fill a range of size {} with rectangle vertices", std::size(out));

	fill_rectangle_vertices(std::begin(out), rect);
}

template <std::output_iterator<glm::vec2> O> constexpr O tr::fill_rectangle_vertices(O out, const frect2& rect, const glm::mat4& mat)
{
	*out++ = mat * rect.tl;
	*out++ = mat * glm::vec2{rect.tl.x, rect.tl.y + rect.size.y};
	*out++ = mat * (rect.tl + rect.size);
	*out++ = mat * glm::vec2{rect.tl.x + rect.size.x, rect.tl.y};
	return out;
}

template <tr::sized_output_range<glm::vec2> R> constexpr void tr::fill_rectangle_vertices(R&& out, const frect2& rect, const glm::mat4& mat)
{
	TR_ASSERT(std::size(out) == 4, "Tried to fill a range of size {} with rectangle vertices", std::size(out));

	fill_rectangle_vertices(std::begin(out), rect, mat);
}

template <std::output_iterator<glm::vec2> O>
O tr::fill_rectangle_vertices(O out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation)
{
	if (rotation == 0_rad) {
		return fill_rectangle_vertices(out, {pos - anchor, size});
	}
	else {
		return fill_rectangle_vertices(out, {pos - anchor, size}, rotate_around(glm::mat4{1.0f}, pos, rotation));
	}
}

template <tr::sized_output_range<glm::vec2> R>
void tr::fill_rectangle_vertices(R&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation)
{
	TR_ASSERT(std::size(out) == 4, "Tried to fill a range of size {} with rectangle vertices", std::size(out));

	fill_rectangle_vertices(std::begin(out), pos, anchor, size, rotation);
}

template <std::output_iterator<glm::vec2> O> constexpr O tr::fill_rectangle_outline_vertices(O out, const frect2& rect, float thickness)
{
	out = fill_rectangle_vertices(out, {rect.tl - thickness / 2, rect.size + thickness});
	return fill_rectangle_vertices(out, {rect.tl + thickness / 2, rect.size - thickness});
}

template <tr::sized_output_range<glm::vec2> R>
constexpr void tr::fill_rectangle_outline_vertices(R&& out, const frect2& rect, float thickness)
{
	TR_ASSERT(std::size(out) == 8, "Tried to fill a range of size {} with rectangle outline vertices", std::size(out));

	fill_rectangle_outline_vertices(std::begin(out), rect, thickness);
}

template <std::output_iterator<glm::vec2> O>
constexpr O tr::fill_rectangle_outline_vertices(O out, const frect2& rect, float thickness, const glm::mat4& mat)
{
	out = fill_rectangle_vertices(out, {rect.tl - thickness / 2, rect.size + thickness}, mat);
	return fill_rectangle_vertices(out, {rect.tl + thickness / 2, rect.size - thickness}, mat);
}

template <tr::sized_output_range<glm::vec2> R>
constexpr void tr::fill_rectangle_outline_vertices(R&& out, const frect2& rect, float thickness, const glm::mat4& mat)
{
	TR_ASSERT(std::size(out) == 8, "Tried to fill a range of size {} with rectangle outline vertices", std::size(out));

	fill_rectangle_outline_vertices(std::begin(out), rect, thickness, mat);
}

template <std::output_iterator<glm::vec2> O>
O tr::fill_rectangle_outline_vertices(O out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness)
{
	if (rotation == 0_rad) {
		return fill_rectangle_outline_vertices(out, pos - anchor, size, thickness);
	}
	else {
		return fill_rectangle_outline_vertices(out, pos - anchor, size, thickness, rotate_around(glm::mat4{1.0f}, pos, rotation));
	}
}

template <tr::sized_output_range<glm::vec2> R>
void tr::fill_rectangle_outline_vertices(R&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness)
{
	TR_ASSERT(std::size(out) == 8, "Tried to fill a range of size {} with rectangle outline vertices", std::size(out));

	fill_rectangle_outline_vertices(std::begin(out), pos, anchor, size, rotation, thickness);
}

template <std::output_iterator<glm::vec2> O> O tr::fill_arc_vertices(O out, usize vtx, circle circle, angle start, angle size)
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

template <tr::sized_output_range<glm::vec2> R> void tr::fill_arc_vertices(R&& out, circle circle, angle start, angle size)
{
	fill_arc_vertices(std::begin(out), std::size(out), circle, start, size);
}

template <std::output_iterator<glm::vec2> O> O tr::fill_regular_polygon_vertices(O out, usize vtx, circle circle, angle rotation)
{
	return fill_arc_vertices(out, vtx, circle, rotation, 1_turns);
}

template <tr::sized_output_range<glm::vec2> R> void tr::fill_regular_polygon_vertices(R&& out, circle circle, angle rotation)
{
	fill_regular_polygon_vertices(std::begin(out), std::size(out), circle, rotation);
}

template <std::output_iterator<glm::vec2> O> O tr::fill_circle_vertices(O out, usize vtx, circle circle)
{
	return fill_arc_vertices(out, vtx, circle, 0_turns, 1_turns);
}

template <tr::sized_output_range<glm::vec2> R> void tr::fill_circle_vertices(R&& out, circle circle)
{
	fill_circle_vertices(std::begin(out), std::size(out), circle);
}

template <std::output_iterator<glm::vec2> O>
O tr::fill_regular_polygon_outline_vertices(O out, usize vtx, circle circle, angle rotation, float thickness)
{
	out = fill_regular_polygon_vertices(out, vtx, {circle.c, circle.r + thickness / 2}, rotation);
	return fill_regular_polygon_vertices(out, vtx, {circle.c, circle.r - thickness / 2}, rotation);
}

template <tr::sized_output_range<glm::vec2> R>
void tr::fill_regular_polygon_outline_vertices(R&& out, circle circle, angle rotation, float thickness)
{
	fill_regular_polygon_outline_vertices(std::begin(out), std::size(out) / 2, circle, rotation, thickness);
}

template <std::output_iterator<glm::vec2> O> O tr::fill_circle_outline_vertices(O out, usize vtx, circle circle, float thickness)
{
	out = fill_circle_vertices(out, vtx, {circle.c, circle.r + thickness / 2});
	return fill_circle_vertices(out, vtx, {circle.c, circle.r - thickness / 2});
}

template <tr::sized_output_range<glm::vec2> R> void tr::fill_circle_outline_vertices(R&& out, circle circle, float thickness)
{
	fill_circle_outline_vertices(std::begin(out), std::size(out) / 2, circle, thickness);
}