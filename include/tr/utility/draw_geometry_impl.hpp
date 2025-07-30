#pragma once
#include "draw_geometry.hpp"

inline std::size_t tr::smooth_poly_vtx(float r, float scale)
{
	return std::max(static_cast<std::size_t>(6 * scale * std::sqrt(std::abs(r))), std::size_t{3});
}

inline std::size_t tr::smooth_arc_vtx(float r, angle sizeth, float scale)
{
	return std::max(static_cast<std::size_t>(6 * scale * std::sqrt(std::abs(r)) / (360_deg / (sizeth % 360_deg))), std::size_t{3});
}

constexpr std::size_t tr::poly_idx(std::uint16_t vtx)
{
	return (vtx - 2) * 3;
}

constexpr std::size_t tr::poly_outline_idx(std::uint16_t vtx)
{
	return vtx * 6;
}

template <std::output_iterator<std::uint16_t> It> constexpr It tr::fill_poly_idx(It out, std::uint16_t vtx, std::uint16_t base)
{
	for (std::uint16_t i = 0; i < vtx - 2; ++i) {
		*out++ = base;
		*out++ = base + i + 1;
		*out++ = base + i + 2;
	}
	return out;
}

template <tr::sized_output_range<std::uint16_t> Range>
constexpr std::ranges::iterator_t<Range> tr::fill_poly_idx(Range&& out, std::uint16_t vtx, std::uint16_t base)
{
	TR_ASSERT(std::size(out) >= poly_idx(vtx), "Tried to write {} indices into a range of size {}.", poly_idx(vtx), std::size(out));

	return fill_poly_idx(std::begin(out), vtx, base);
}

template <std::output_iterator<std::uint16_t> It> constexpr It tr::fill_poly_outline_idx(It out, std::uint16_t vtx, std::uint16_t base)
{
	for (std::uint16_t i = 0; i < vtx - 1; ++i) {
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

template <tr::sized_output_range<std::uint16_t> Range>
constexpr std::ranges::iterator_t<Range> tr::fill_poly_outline_idx(Range&& out, std::uint16_t vtx, std::uint16_t base)
{
	TR_ASSERT(std::size(out) >= poly_outline_idx(vtx), "Tried to write {} indices into a range of size {}.", poly_outline_idx(vtx),
			  std::size(out));

	return fill_poly_outline_idx(std::begin(out), vtx, base);
}

template <std::output_iterator<glm::vec2> It> constexpr It tr::fill_rect_vtx(It out, const frect2& rect)
{
	*out++ = rect.tl;
	*out++ = glm::vec2{rect.tl.x, rect.tl.y + rect.size.y};
	*out++ = rect.tl + rect.size;
	*out++ = glm::vec2{rect.tl.x + rect.size.x, rect.tl.y};
	return out;
}

template <tr::sized_output_range<glm::vec2> Range>
constexpr std::ranges::iterator_t<Range> tr::fill_rect_vtx(Range&& out, const frect2& rect)
{
	TR_ASSERT(std::size(out) >= 4, "Tried to write 4 vertices into a range of size {}.", std::size(out));

	return fill_rect_vtx(std::begin(out), rect);
}

template <std::output_iterator<glm::vec2> It> constexpr It tr::fill_rect_vtx(It out, const frect2& rect, const glm::mat4& mat)
{
	*out++ = mat * rect.tl;
	*out++ = mat * glm::vec2{rect.tl.x, rect.tl.y + rect.size.y};
	*out++ = mat * (rect.tl + rect.size);
	*out++ = mat * glm::vec2{rect.tl.x + rect.size.x, rect.tl.y};
	return out;
}

template <tr::sized_output_range<glm::vec2> Range>
constexpr std::ranges::iterator_t<Range> tr::fill_rect_vtx(Range&& out, const frect2& rect, const glm::mat4& mat)
{
	TR_ASSERT(std::size(out) >= 4, "Tried to write 4 vertices into a range of size {}.", std::size(out));

	return fill_rect_vtx(std::begin(out), rect, mat);
}

template <std::output_iterator<glm::vec2> It>
inline It tr::fill_rotated_rect_vtx(It out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rot)
{
	if (rot == tr::rads(0)) {
		return fill_rect_vtx(out, {pos - anchor, size});
	}
	else {
		const glm::mat4 mat{rotate_around(glm::mat4{1}, pos, rot)};
		return fill_rect_vtx(out, {pos - anchor, size}, mat);
	}
}

template <tr::sized_output_range<glm::vec2> Range>
inline std::ranges::iterator_t<Range> tr::fill_rotated_rect_vtx(Range&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rot)
{
	TR_ASSERT(std::size(out) >= 4, "Tried to write 4 vertices into a range of size {}.", std::size(out));

	return fill_rotated_rect_vtx(std::begin(out), pos, anchor, size, rot);
}

template <std::output_iterator<glm::vec2> It> constexpr It tr::fill_rect_outline_vtx(It out, const frect2& rect, float thick)
{
	out = fill_rect_vtx(out, {rect.tl - thick / 2, rect.size + thick});
	return fill_rect_vtx(out, {rect.tl + thick / 2, rect.size - thick});
}

template <tr::sized_output_range<glm::vec2> Range>
constexpr std::ranges::iterator_t<Range> tr::fill_rect_outline_vtx(Range&& out, const frect2& rect, float thick)
{
	TR_ASSERT(std::size(out) >= 8, "Tried to write 8 vertices into a range of size {}.", std::size(out));

	return fill_rect_outline_vtx(std::begin(out), rect, thick);
}

template <std::output_iterator<glm::vec2> It>
constexpr It tr::fill_rect_outline_vtx(It out, const frect2& rect, float thick, const glm::mat4& mat)
{
	out = fill_rect_vtx(out, {rect.tl - thick / 2, rect.size + thick}, mat);
	return fill_rect_vtx(out, {rect.tl + thick / 2, rect.size - thick}, mat);
}

template <tr::sized_output_range<glm::vec2> Range>
constexpr std::ranges::iterator_t<Range> tr::fill_rect_outline_vtx(Range&& out, const frect2& rect, float thick, const glm::mat4& mat)
{
	TR_ASSERT(std::size(out) >= 8, "Tried to write 8 vertices into a range of size {}.", std::size(out));

	return fill_rect_outline_vtx(std::begin(out), rect, thick, mat);
}

template <std::output_iterator<glm::vec2> It>
inline It tr::fill_rotated_rect_outline_vtx(It out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rot, float thick)
{
	if (rot == tr::rads(0)) {
		return fill_rect_outline_vtx(out, pos - anchor, size, thick);
	}
	else {
		const glm::mat4 mat{rotate_around(glm::mat4{1}, pos, rot)};
		return fill_rect_outline_vtx(out, pos - anchor, size, thick, mat);
	}
}

template <tr::sized_output_range<glm::vec2> Range>
inline std::ranges::iterator_t<Range> tr::fill_rotated_rect_outline_vtx(Range&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size,
																		angle rot, float thick)
{
	TR_ASSERT(std::size(out) >= 8, "Tried to write 8 vertices into a range of size {}.", std::size(out));

	return fill_rotated_rect_outline_vtx(std::begin(out), pos, anchor, size, rot, thick);
}

template <std::output_iterator<glm::vec2> It> It tr::fill_arc_vtx(It out, std::size_t vtx, circle circ, angle startth, angle sizeth)
{
	angle dth{sizeth / vtx};
	float dsin{dth.sin()};
	float dcos{dth.cos()};
	glm::vec2 delta{circ.r * startth.cos(), circ.r * startth.sin()};
	for (std::size_t i = 0; i < vtx; ++i) {
		*out++ = delta + circ.c;
		delta = glm::vec2{dcos * delta.x - dsin * delta.y, dsin * delta.x + dcos * delta.y};
	}
	return out;
}

template <tr::sized_output_range<glm::vec2> Range>
inline std::ranges::iterator_t<Range> tr::fill_arc_vtx(Range&& out, std::size_t vtx, circle circ, angle startth, angle sizeth)
{
	TR_ASSERT(std::size(out) >= vtx, "Tried to write {} vertices into a range of size {}.", vtx, std::size(out));

	return fill_arc_vtx(std::begin(out), vtx, circ, startth, sizeth);
}

template <std::output_iterator<glm::vec2> It> It tr::fill_poly_vtx(It out, std::size_t vtx, circle circ, angle rot)
{
	return fill_arc_vtx(out, vtx, circ, rot, 360_deg);
}

template <tr::sized_output_range<glm::vec2> Range>
inline std::ranges::iterator_t<Range> tr::fill_poly_vtx(Range&& out, std::size_t vtx, circle circ, angle rot)
{
	return fill_poly_vtx(std::begin(out), vtx, circ, rot);
}

template <std::output_iterator<glm::vec2> It> It tr::fill_poly_outline_vtx(It out, std::size_t vtx, circle circ, angle rot, float thick)
{
	out = fill_poly_vtx(out, vtx, {circ.c, circ.r + thick / 2}, rot);
	return fill_poly_vtx(out, vtx, {circ.c, circ.r - thick / 2}, rot);
}

template <tr::sized_output_range<glm::vec2> Range>
inline std::ranges::iterator_t<Range> tr::fill_poly_outline_vtx(Range&& out, std::size_t vtx, circle circ, angle rot, float thick)
{
	TR_ASSERT(std::size(out) >= vtx * 2, "Tried to write {} vertices into a range of size {}", vtx * 2, std::size(out));

	return fill_poly_outline_vtx(std::begin(out), vtx, circ, rot, thick);
}