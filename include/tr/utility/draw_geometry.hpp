#pragma once
#include "geometry.hpp"

namespace tr {
	// Calculates the number of segments needed to draw a smooth circle with pixel radius r.
	inline usize smooth_polygon_vertices(float r);
	// Calculates the number of segments needed to draw a smooth arc with pixel radius r.
	inline usize smooth_arc_vertices(float r, angle sizeth);
	// Calculates the number of indices needed for a convex polygon mesh.
	constexpr usize polygon_indices(u16 vtx);
	// Calculates the number of indices needed for a convex polygon mesh.
	constexpr usize polygon_outline_indices(u16 vtx);

	// Outputs indices for a convex polygon to an output iterator.
	// out needs to have space for poly_idx(vtx) indices.
	template <std::output_iterator<u16> O> constexpr O fill_polygon_indices(u16 vtx, u16 base, O out);
	// Outputs indices for a convex polygon outline to an output iterator.
	// out needs to have space for poly_outline_idx(vtx) indices.
	// vtx is the number of vertices in the polygon, not the mesh.
	template <std::output_iterator<u16> O> constexpr O fill_polygon_outline_indices(u16 vtx, u16 base, O out);

	// Outputs rectangle vertices to an output iterator.
	// out needs to have space for 4 vertices.
	template <std::output_iterator<glm::vec2> O> constexpr O fill_rectangle_vertices(const frect2& rect, O out);
	// Outputs transformed rectangle vertices to an output iterator.
	// out needs to have space for 4 vertices.
	template <std::output_iterator<glm::vec2> O> constexpr O fill_rectangle_vertices(const frect2& rect, const glm::mat4& mat, O out);
	// Outputs rotated rectangle vertices to an output iterator.
	// out needs to have space for 4 vertices.
	template <std::output_iterator<glm::vec2> O>
	O fill_rectangle_vertices(glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, O out);

	// Outputs rectangle outline vertices to an output iterator.
	// out needs to have space for 8 vertices.
	template <std::output_iterator<glm::vec2> O> constexpr O fill_rectangle_outline_vertices(const frect2& rect, float thickness, O out);
	// Outputs transformed rectangle outline vertices to an output iterator.
	// out needs to have space for 8 vertices.
	template <std::output_iterator<glm::vec2> O>
	constexpr O fill_rectangle_outline_vertices(const frect2& rect, float thickness, const glm::mat4& mat, O out);
	// Outputs rotated rectangle outline vertices to an output iterator.
	// out needs to have space for 8 vertices.
	template <std::output_iterator<glm::vec2> O>
	O fill_rectangle_outline_vertices(glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness, O out);

	// Outputs arc vertices to an output iterator.
	// out needs to have space for vtx vertices.
	template <std::output_iterator<glm::vec2> O> O fill_arc_vertices(usize vtx, circle circle, angle start, angle size, O out);
	// Outputs arc vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_arc_vertices(circle circle, angle start, angle size, R&& out);
	// Outputs regular polygon vertices to an output iterator.
	// out needs to have space for vtx vertices.
	template <std::output_iterator<glm::vec2> O> O fill_polygon_vertices(usize vtx, circle circle, angle rotation, O out);
	// Outputs regular polygon vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_polygon_vertices(circle circle, angle rotation, R&& out);
	// Outputs circle vertices to an output iterator.
	// out needs to have space for vtx vertices.
	template <std::output_iterator<glm::vec2> O> O fill_circle_vertices(usize vtx, circle circle, O out);
	// Outputs circle vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_circle_vertices(circle circle, R&& out);

	// Outputs regular polygon outline vertices to an output iterator.
	// out needs to have space for vtx * 2 vertices.
	template <std::output_iterator<glm::vec2> O>
	O fill_polygon_outline_vertices(usize vtx, circle circle, angle rotation, float thickness, O out);
	// Outputs regular polygon outline vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_polygon_outline_vertices(circle circle, angle rotation, float thickness, R&& out);
	// Outputs circle outline vertices to an output iterator.
	// out needs to have space for vtx * 2 vertices.
	template <std::output_iterator<glm::vec2> O> O fill_circle_outline_vertices(usize vtx, circle circle, float thickness, O out);
	// Outputs circle outline vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_circle_outline_vertices(circle circle, float thickness, R&& out);
} // namespace tr

#include "draw_geometry_impl.hpp" // IWYU pragma: keep