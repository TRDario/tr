#pragma once
#include "geometry.hpp"

namespace tr {
	// Calculates the number of segments needed to draw a smooth circle with pixel radius r.
	inline usize smooth_polygon_vertices(float r);
	// Calculates the number of segments needed to draw a smooth arc with pixel radius r.
	inline usize smooth_arc_vertices(float r, angle sizeth);
	// Calculates the number of indices needed for a simple polygon mesh with no holes.
	constexpr usize polygon_indices(u16 vtx);
	// Calculates the number of indices needed for a simple polygon mesh with no holes.
	constexpr usize polygon_outline_indices(u16 vtx);

	// Outputs indices for a convex polygon to an output iterator.
	// out needs to have space for poly_idx(vtx) indices.
	template <std::output_iterator<u16> O> constexpr O fill_convex_polygon_indices(O out, u16 vtx, u16 base);
	// Outputs indices for a convex polygon outline to an output iterator.
	// out needs to have space for poly_outline_idx(vtx) indices.
	// vtx is the number of vertices in the polygon, not the mesh.
	template <std::output_iterator<u16> O> constexpr O fill_convex_polygon_outline_indices(O out, u16 vtx, u16 base);
	// Outputs indices for a simple polygon to an output iterator.
	// out needs to have space for poly_idx(vtx) indices.
	template <std::output_iterator<u16> O> constexpr void fill_simple_polygon_indices(O out, std::span<const glm::vec2> vertices, u16 base);

	// Outputs rectangle vertices to an output iterator.
	// out needs to have space for 4 vertices.
	template <std::output_iterator<glm::vec2> O> constexpr O fill_rectangle_vertices(O out, const frect2& rect);
	// Outputs rectangle vertices to a range of 4 vertices.
	template <sized_output_range<glm::vec2> R> constexpr void fill_rectangle_vertices(R&& out, const frect2& rect);
	// Outputs transformed rectangle vertices to an output iterator.
	// out needs to have space for 4 vertices.
	template <std::output_iterator<glm::vec2> O> constexpr O fill_rectangle_vertices(O out, const frect2& rect, const glm::mat4& mat);
	// Outputs transformed rectangle vertices to a range of 4 vertices.
	template <sized_output_range<glm::vec2> R> constexpr void fill_rectangle_vertices(R&& out, const frect2& rect, const glm::mat4& mat);
	// Outputs rotated rectangle vertices to an output iterator.
	// out needs to have space for 4 vertices.
	template <std::output_iterator<glm::vec2> O>
	O fill_rectangle_vertices(O out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation);
	// Outputs transformed rectangle vertices to a range of 4 vertices.
	template <sized_output_range<glm::vec2> R>
	void fill_rectangle_vertices(R&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation);

	// Outputs rectangle outline vertices to an output iterator.
	// out needs to have space for 8 vertices.
	template <std::output_iterator<glm::vec2> O> constexpr O fill_rectangle_outline_vertices(O out, const frect2& rect, float thickness);
	// Outputs rectangle outline vertices to a range of 8 vertices.
	template <sized_output_range<glm::vec2> R> constexpr void fill_rectangle_outline_vertices(R&& out, const frect2& rect, float thickness);
	// Outputs transformed rectangle outline vertices to an output iterator.
	// out needs to have space for 8 vertices.
	template <std::output_iterator<glm::vec2> O>
	constexpr O fill_rectangle_outline_vertices(O out, const frect2& rect, float thickness, const glm::mat4& mat);
	// Outputs transformed rectangle outline vertices to a range of 8 vertices.
	template <sized_output_range<glm::vec2> R>
	constexpr void fill_rectangle_outline_vertices(R&& out, const frect2& rect, float thickness, const glm::mat4& mat);
	// Outputs rotated rectangle outline vertices to an output iterator.
	// out needs to have space for 8 vertices.
	template <std::output_iterator<glm::vec2> O>
	O fill_rectangle_outline_vertices(O out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness);
	// Outputs rotated rectangle outline vertices to a range of 8 vertices.
	template <sized_output_range<glm::vec2> R>
	void fill_rectangle_outline_vertices(R&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness);

	// Outputs arc vertices to an output iterator.
	// out needs to have space for vtx vertices.
	template <std::output_iterator<glm::vec2> O> O fill_arc_vertices(O out, usize vtx, circle circle, angle start, angle size);
	// Outputs arc vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_arc_vertices(R&& out, circle circle, angle start, angle size);
	// Outputs regular polygon vertices to an output iterator.
	// out needs to have space for vtx vertices.
	template <std::output_iterator<glm::vec2> O> O fill_regular_polygon_vertices(O out, usize vtx, circle circle, angle rotation);
	// Outputs regular polygon vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_regular_polygon_vertices(R&& out, circle circle, angle rotation);
	// Outputs circle vertices to an output iterator.
	// out needs to have space for vtx vertices.
	template <std::output_iterator<glm::vec2> O> O fill_circle_vertices(O out, usize vtx, circle circle);
	// Outputs circle vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_circle_vertices(R&& out, circle circle);

	// Outputs regular polygon outline vertices to an output iterator.
	// out needs to have space for vtx * 2 vertices.
	template <std::output_iterator<glm::vec2> O>
	O fill_regular_polygon_outline_vertices(O out, usize vtx, circle circle, angle rotation, float thickness);
	// Outputs regular polygon outline vertices to a range.
	template <sized_output_range<glm::vec2> R>
	void fill_regular_polygon_outline_vertices(R&& out, circle circle, angle rotation, float thickness);
	// Outputs circle outline vertices to an output iterator.
	// out needs to have space for vtx * 2 vertices.
	template <std::output_iterator<glm::vec2> O> O fill_circle_outline_vertices(O out, usize vtx, circle circle, float thickness);
	// Outputs circle outline vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_circle_outline_vertices(R&& out, circle circle, float thickness);
} // namespace tr

#include "draw_geometry_impl.hpp" // IWYU pragma: keep