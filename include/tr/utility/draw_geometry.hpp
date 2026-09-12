/// @file
/// @brief Defines functions related to generating mesh geometry.

#pragma once
#include <tr/utility/angle.hpp>
#include <tr/utility/circle.hpp>
#include <tr/utility/matrix.hpp>
#include <tr/utility/polygon.hpp>
#include <tr/utility/rectangle.hpp>
#include <tr/utility/triangle.hpp>

//

namespace tr
{
	/// @name Drawing geometry
	/// @{

	/// Calculates the number of segments needed to draw a smooth circle with pixel radius `r`.
	/// @param r Pixel radius of the circle.
	/// @return Number of segments needed to draw a smooth circle.
	[[nodiscard]] inline usize smooth_polygon_vertices(float r) noexcept
	{
		TR_ASSERT(r > 0, "Tried to pass a negative radius to smooth_polygon_vertices.");

		return std::max(usize(7 * std::pow(r, 1 / 2.4f)), 3uz);
	}

	/// Calculates the number of segments needed to draw a smooth arc with pixel radius `r`.
	/// @param r Pixel radius of the arc.
	/// @param sizeth Angle size of the arc.
	/// @return Number of segments needed to draw a smooth arc.
	[[nodiscard]] inline usize smooth_arc_vertices(float r, angle sizeth) noexcept
	{
		TR_ASSERT(r > 0, "Tried to pass a negative radius to smooth_arc_vertices.");
		TR_ASSERT(sizeth >= 0_tr && sizeth <= 1_tr, "Tried to pass an arc size of more than one turn to smooth_arc_vertices.");

		return std::max(static_cast<usize>(7 * std::pow(r, 1 / 2.4f) / (sizeth / 1_tr)), 3uz);
	}

	//

	/// Calculates the number of indices needed for a line strip.
	/// @param vertices Number of vertices in the line strip.
	/// @return Number of indices needed for a line strip.
	[[nodiscard]] constexpr usize line_strip_indices(u16 vertices) noexcept
	{
		return (vertices - 1) * 2;
	}

	/// Calculates the number of indices needed for a line loop.
	/// @param vertices Number of vertices in the line loop.
	/// @return Number of indices needed for a line loop.
	[[nodiscard]] constexpr usize line_loop_indices(u16 vertices) noexcept
	{
		return vertices * 2;
	}

	/// Calculates the number of indices needed for a simple polygon mesh with no holes.
	/// @param vertices Number of vertices in the polygon.
	/// @return Number of indices needed for a simple polygon mesh with no holes.
	[[nodiscard]] constexpr usize polygon_indices(u16 vertices) noexcept
	{
		return (vertices - 2) * 3;
	}

	/// Calculates the number of indices needed for a simple polygon mesh with no holes.
	/// @param vertices Number of vertices in the polygon outline.
	/// @return Number of indices needed for a simple polygon mesh with no holes.
	[[nodiscard]] constexpr usize polygon_outline_indices(u16 vertices) noexcept
	{
		return vertices * 6;
	}

	//

	/// Outputs indices for a line strip to an output iterator.
	/// @tparam Iterator Index output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the line strip.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `line_strip_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_line_strip_indices(Iterator out, u16 vertices, u16 base)
	{
		TR_ASSERT(base + vertices <= UINT16_MAX, "Index overflow detected in fill_line_strip_indices.");

		for (u16 i = 0; i < vertices - 1; ++i) {
			*out++ = base + i;
			*out++ = base + i + 1;
		}
		return out;
	}

	/// Outputs indices for a line loop to an output iterator.
	/// @tparam Iterator Index output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the line loop.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `line_loop_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_line_loop_indices(Iterator out, u16 vertices, u16 base)
	{
		out = fill_line_strip_indices(out, vertices, base);
		*out++ = base + vertices - 1;
		*out++ = base;
		return out;
	}

	/// Outputs indices for a convex polygon to an output iterator.
	/// @tparam Iterator Index output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the convex polygon.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `polygon_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_convex_polygon_indices(Iterator out, u16 vertices, u16 base)
	{
		TR_ASSERT(vertices >= 3, "Tried to calculate indices for {}-sided polygon.", vertices);
		TR_ASSERT(base + vertices <= UINT16_MAX, "Index overflow detected in fill_convex_polygon_indices.");

		for (u16 i = 0; i < vertices - 2; ++i) {
			*out++ = base;
			*out++ = base + i + 1;
			*out++ = base + i + 2;
		}
		return out;
	}

	/// Outputs indices for a convex polygon outline to an output iterator.
	/// @note `vertices` is the number of vertices in the polygon, not the mesh.
	/// @tparam Iterator Index output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the convex polygon outline.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `polygon_outline_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_convex_polygon_outline_indices(Iterator out, u16 vertices, u16 base)
	{
		TR_ASSERT(vertices >= 3, "Tried to calculate indices for {}-sided polygon outline.", vertices);
		TR_ASSERT(base + vertices <= UINT16_MAX, "Index overflow detected in fill_convex_polygon_outline_indices.");

		for (u16 i = 0; i < vertices - 1; ++i) {
			*out++ = base + i;
			*out++ = base + i + 1;
			*out++ = base + i + vertices;
			*out++ = base + i + 1;
			*out++ = base + i + vertices;
			*out++ = base + i + vertices + 1;
		}
		*out++ = base + vertices - 1;
		*out++ = base;
		*out++ = base + 2 * vertices - 1;
		*out++ = base;
		*out++ = base + 2 * vertices - 1;
		*out++ = base + vertices;
		return out;
	}

	/// Outputs indices for a simple polygon to an output iterator.
	/// @param out Output iterator.
	/// @param vertices Vertices of the polygon.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `polygon_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_simple_polygon_indices(Iterator out, std::span<const glm::vec2> vertices, u16 base)
	{
		TR_ASSERT(vertices.size() >= 3, "Tried to calculate indices for {}-sided polygon.", vertices.size());
		TR_ASSERT(base + vertices.size() <= UINT16_MAX, "Index overflow detected in fill_simple_polygon_indices.");

		const winding_order winding_order{polygon_winding_order(vertices)};

		std::vector<u16> indices(vertices.size());
		std::iota(indices.begin(), indices.end(), 0_u16);
		while (indices.size() >= 3) {
			for (usize i = 0; i < indices.size(); ++i) {
				const u16 left{static_cast<u16>(i == 0 ? indices.size() - 1 : i - 1)};
				const u16 right{static_cast<u16>(i == indices.size() - 1 ? 0 : i + 1)};

				if (indices.size() > 3) {
					const triangle tri{vertices[indices[left]], vertices[indices[i]], vertices[indices[right]]};
					const auto is_in_tri{[&](usize j) { return j != left && j != i && j != right && tri.contains(vertices[indices[j]]); }};
					if (tri.winding_order() != winding_order || std::ranges::any_of(std::views::iota(0uz, indices.size()), is_in_tri)) {
						continue;
					}
				}

				*out++ = indices[left] + base;
				*out++ = indices[i] + base;
				*out++ = indices[right] + base;
				indices.erase(indices.begin() + i--);
				break;
			}
		}

		return out;
	}

	//

	/// Outputs rectangle vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param rectangle Rectangle to output vertices for.
	/// @pre `out` needs to have space for 4 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	constexpr Iterator fill_rectangle_vertices(Iterator out, rectangle<float> rectangle)
	{
		*out++ = rectangle.tl;
		*out++ = glm::vec2{rectangle.tl.x, rectangle.tl.y + rectangle.size.y};
		*out++ = rectangle.tl + rectangle.size;
		*out++ = glm::vec2{rectangle.tl.x + rectangle.size.x, rectangle.tl.y};
		return out;
	}

	/// Outputs rectangle vertices to a range of 4 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param rectangle Rectangle to output vertices for.
	/// @pre `out` must have a size of 4.
	template <sized_output_range<glm::vec2> Range>
	constexpr void fill_rectangle_vertices(Range&& out, rectangle<float> rectangle)
	{
		TR_ASSERT(std::size(out) == 4, "Tried to fill a range of size {} with rectangle vertices", std::size(out));

		fill_rectangle_vertices(std::begin(out), rectangle);
	}

	/// Outputs transformed rectangle vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param rectangle Rectangle to output vertices for.
	/// @param mat Transformation to apply on the vertices.
	/// @pre `out` needs to have space for 4 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	constexpr Iterator fill_rectangle_vertices(Iterator out, rectangle<float> rectangle, const glm::mat4& mat)
	{
		*out++ = mat * rectangle.tl;
		*out++ = mat * glm::vec2{rectangle.tl.x, rectangle.tl.y + rectangle.size.y};
		*out++ = mat * (rectangle.tl + rectangle.size);
		*out++ = mat * glm::vec2{rectangle.tl.x + rectangle.size.x, rectangle.tl.y};
		return out;
	}

	/// Outputs transformed rectangle vertices to a range of 4 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param rectangle Rectangle to output vertices for.
	/// @param mat Transformation to apply on the vertices.
	/// @pre `out` must have a size of 4.
	template <sized_output_range<glm::vec2> Range>
	constexpr void fill_rectangle_vertices(Range&& out, rectangle<float> rectangle, const glm::mat4& mat)
	{
		TR_ASSERT(std::size(out) == 4, "Tried to fill a range of size {} with rectangle vertices", std::size(out));

		fill_rectangle_vertices(std::begin(out), rectangle, mat);
	}

	/// Outputs rotated rectangle vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param pos Position of the rectangle's anchor.
	/// @param anchor Position of the anchor point within the rectangle.
	/// @param size Size of the rectangle.
	/// @param rotation Rotation of the rectangle.
	/// @pre `out` needs to have space for 4 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_rectangle_vertices(Iterator out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation)
	{
		if (rotation == 0_rad) {
			return fill_rectangle_vertices(out, {pos - anchor, size});
		}
		else {
			return fill_rectangle_vertices(out, {pos - anchor, size}, rotate_around(glm::mat4{1.0f}, pos, rotation));
		}
	}

	/// Outputs transformed rectangle vertices to a range of 4 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param pos Position of the rectangle's anchor.
	/// @param anchor Position of the anchor point within the rectangle.
	/// @param size Size of the rectangle.
	/// @param rotation Rotation of the rectangle.
	/// @pre `out` must have a size of 4.
	template <sized_output_range<glm::vec2> Range>
	void fill_rectangle_vertices(Range&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation)
	{
		TR_ASSERT(std::size(out) == 4, "Tried to fill a range of size {} with rectangle vertices", std::size(out));

		fill_rectangle_vertices(std::begin(out), pos, anchor, size, rotation);
	}

	//

	/// Outputs rectangle outline vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param rectangle Rectangle to output vertices for.
	/// @param thickness Thickness of the rectangle outline.
	/// @pre `out` needs to have space for 8 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	constexpr Iterator fill_rectangle_outline_vertices(Iterator out, rectangle<float> rectangle, float thickness)
	{
		out = fill_rectangle_vertices(out, {rectangle.tl - thickness / 2, rectangle.size + thickness});
		return fill_rectangle_vertices(out, {rectangle.tl + thickness / 2, rectangle.size - thickness});
	}

	/// Outputs rectangle outline vertices to a range of 8 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param rectangle Rectangle to output vertices for.
	/// @param thickness Thickness of the rectangle outline.
	/// @pre `out` must have a size of 8.
	template <sized_output_range<glm::vec2> Range>
	constexpr void fill_rectangle_outline_vertices(Range&& out, rectangle<float> rectangle, float thickness)
	{
		TR_ASSERT(std::size(out) == 8, "Tried to fill a range of size {} with rectangle outline vertices", std::size(out));

		fill_rectangle_outline_vertices(std::begin(out), rectangle, thickness);
	}

	/// Outputs transformed rectangle outline vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param rectangle Rectangle to output vertices for.
	/// @param thickness Thickness of the rectangle outline.
	/// @param mat Transformation to apply on the vertices.
	/// @pre `out` needs to have space for 8 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	constexpr Iterator fill_rectangle_outline_vertices(Iterator out, rectangle<float> rectangle, float thickness, const glm::mat4& mat)
	{
		out = fill_rectangle_vertices(out, {rectangle.tl - thickness / 2, rectangle.size + thickness}, mat);
		return fill_rectangle_vertices(out, {rectangle.tl + thickness / 2, rectangle.size - thickness}, mat);
	}

	/// Outputs transformed rectangle outline vertices to a range of 8 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param rectangle Rectangle to output vertices for.
	/// @param thickness Thickness of the rectangle outline.
	/// @param mat Transformation to apply on the vertices.
	/// @pre `out` must have a size of 8.
	template <sized_output_range<glm::vec2> Range>
	constexpr void fill_rectangle_outline_vertices(Range&& out, rectangle<float> rectangle, float thickness, const glm::mat4& mat)
	{
		TR_ASSERT(std::size(out) == 8, "Tried to fill a range of size {} with rectangle outline vertices", std::size(out));

		fill_rectangle_outline_vertices(std::begin(out), rectangle, thickness, mat);
	}

	/// Outputs rotated rectangle outline vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param pos Position of the rectangle's anchor.
	/// @param anchor Position of the anchor point within the rectangle.
	/// @param size Size of the rectangle.
	/// @param rotation Rotation of the rectangle.
	/// @param thickness Thickness of the rectangle outline.
	/// @pre `out` needs to have space for 8 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_rectangle_outline_vertices(Iterator out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness)
	{
		if (rotation == 0_rad) {
			return fill_rectangle_outline_vertices(out, pos - anchor, size, thickness);
		}
		else {
			return fill_rectangle_outline_vertices(out, pos - anchor, size, thickness, rotate_around(glm::mat4{1.0f}, pos, rotation));
		}
	}

	/// Outputs rotated rectangle outline vertices to a range of 8 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param pos Position of the rectangle's anchor.
	/// @param anchor Position of the anchor point within the rectangle.
	/// @param size Size of the rectangle.
	/// @param rotation Rotation of the rectangle.
	/// @param thickness Thickness of the rectangle outline.
	/// @pre `out` must have a size of 8.
	template <sized_output_range<glm::vec2> Range>
	void fill_rectangle_outline_vertices(Range&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness)
	{
		TR_ASSERT(std::size(out) == 8, "Tried to fill a range of size {} with rectangle outline vertices", std::size(out));

		fill_rectangle_outline_vertices(std::begin(out), pos, anchor, size, rotation, thickness);
	}

	//

	/// Outputs arc vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the arc.
	/// @param circle Circle the arc rests on.
	/// @param start Starting angle of the arc on the circle.
	/// @param size Angle size of the arc.
	/// @pre `out` needs to have space for `vertices` vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_arc_vertices(Iterator out, usize vertices, circle circle, angle start, angle size)
	{
		angle dth{size / vertices};
		float dsin{dth.sin()};
		float dcos{dth.cos()};
		glm::vec2 delta{circle.radius * start.cos(), circle.radius * start.sin()};
		for (usize i = 0; i < vertices; ++i) {
			*out++ = delta + circle.center;
			delta = glm::vec2{dcos * delta.x - dsin * delta.y, dsin * delta.x + dcos * delta.y};
		}
		return out;
	}

	/// Outputs arc vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circle the arc rests on.
	/// @param start Starting angle of the arc on the circle.
	/// @param size Angle size of the arc.
	template <sized_output_range<glm::vec2> Range>
	void fill_arc_vertices(Range&& out, circle circle, angle start, angle size)
	{
		fill_arc_vertices(std::begin(out), std::size(out), circle, start, size);
	}

	/// Outputs regular polygon vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the polygon.
	/// @param circle Circumscribed circle of the polygon.
	/// @param rotation Rotation of the polygon.
	/// @pre `out` needs to have space for `vertices` vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_regular_polygon_vertices(Iterator out, usize vertices, circle circle, angle rotation)
	{
		return fill_arc_vertices(out, vertices, circle, rotation, 1_tr);
	}

	/// Outputs regular polygon vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circumscribed circle of the polygon.
	/// @param rotation Rotation of the polygon.
	template <sized_output_range<glm::vec2> Range>
	void fill_regular_polygon_vertices(Range&& out, circle circle, angle rotation)
	{
		fill_regular_polygon_vertices(std::begin(out), std::size(out), circle, rotation);
	}

	/// Outputs circle vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the circle.
	/// @param circle Circle to output vertex positions for.
	/// @pre `out` needs to have space for `vertices` vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_circle_vertices(Iterator out, usize vertices, circle circle)
	{
		return fill_arc_vertices(out, vertices, circle, 0_tr, 1_tr);
	}

	/// Outputs circle vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circle to output vertex positions for.
	template <sized_output_range<glm::vec2> Range>
	void fill_circle_vertices(Range&& out, circle circle)
	{
		fill_circle_vertices(std::begin(out), std::size(out), circle);
	}

	//

	/// Outputs regular polygon outline vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the polygon.
	/// @param circle Circumscribed circle of the polygon.
	/// @param rotation Rotation of the polygon.
	/// @param thickness Thickness of the polygon outline.
	/// @pre `out` needs to have space for `vertices * 2` vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_regular_polygon_outline_vertices(Iterator out, usize vertices, circle circle, angle rotation, float thickness)
	{
		out = fill_regular_polygon_vertices(out, vertices, {circle.center, circle.radius + thickness / 2}, rotation);
		return fill_regular_polygon_vertices(out, vertices, {circle.center, circle.radius - thickness / 2}, rotation);
	}

	/// Outputs regular polygon outline vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circumscribed circle of the polygon.
	/// @param rotation Rotation of the polygon.
	/// @param thickness Thickness of the polygon outline.
	template <sized_output_range<glm::vec2> Range>
	void fill_regular_polygon_outline_vertices(Range&& out, circle circle, angle rotation, float thickness)
	{
		fill_regular_polygon_outline_vertices(std::begin(out), std::size(out) / 2, circle, rotation, thickness);
	}

	/// Outputs circle outline vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the circle.
	/// @param circle Circle to output vertex positions for.
	/// @param thickness Thickness of the circle outline.
	/// @pre `out` needs to have space for `vertices * 2` vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_circle_outline_vertices(Iterator out, usize vertices, circle circle, float thickness)
	{
		out = fill_circle_vertices(out, vertices, {circle.center, circle.radius + thickness / 2});
		return fill_circle_vertices(out, vertices, {circle.center, circle.radius - thickness / 2});
	}

	/// Outputs circle outline vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circle to output vertex positions for.
	/// @param thickness Thickness of the circle outline.
	template <sized_output_range<glm::vec2> Range>
	void fill_circle_outline_vertices(Range&& out, circle circle, float thickness)
	{
		fill_circle_outline_vertices(std::begin(out), std::size(out) / 2, circle, thickness);
	}

	/// @}
} // namespace tr