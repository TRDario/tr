///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Defines functions related to generating mesh geometry.                                                                                //
//                                                                                                                                       //
// Functions for outputting different types of shapes and primitives and primitives are provided. Said functions take iterators to where //
// the indices will be written, as well as the base index value to start counting from, and return the end iterator of the written range.//
// To ensure you have the space to store the indices, you may use functions that determine the amount of indices needed for a shape of   //
// N vertices:                                                                                                                           //
//     - tr::line_strip_indices(20) -> 38                                                                                                //
//     - it = tr::fill_line_strip_indices(it, 20, 0)                                                                                     //
//       -> buffer now contains indices for a line strip of 20 vertices                                                                  //
//     - tr::line_loop_indices(20) -> 40                                                                                                 //
//     - tr::fill_line_loop_indices(std::back_inserter(indices), 20, 20)                                                                 //
//       -> 'indices' now contains indices for a line loop of 20 vertices, indices count up from 20                                      //
//     - tr::polygon_indices(10) -> 24                                                                                                   //
//     - tr::fill_convex_polygon_indices(std::back_inserter(indices), 10, 40)                                                            //
//       -> 'indices' now contains indices for a convex polygon of 10 vertices, indices count up from 40                                 //
//     - tr::fill_convex_polygon_indices(std::back_inserter(indices), polygon, 50)                                                       //
//       -> 'indices' now contains indices for a simple polygon triangulated from 'polygon', indices count up from 50                    //
//     - tr::polygon_outline_indices(10) -> 60                                                                                           //
//     - tr::fill_convex_polygon_outline_indices(std::back_inserter(indices), 10, 60)                                                    //
//       -> 'indices' now contains indices for an outline of a convex polygon of 10 vertices, indices count up from 60                   //
//                                                                                                                                       //
// Vertex filling functions are provided for rectangles, rectangle outlines, arcs, regular polygons/circles, and regular polygon/circle  //
// outlins. Generally, These functions accept either iterators (in which case the end iterator is returned and arcs and                  //
// regular polygons/circles take a number of vertices to fill) or ranges (rectangles and their outlines only accept ranges of 4/8        //
// vertices respectively, arcs and regular polygons/circles fill the entire range):                                                      //
//     - it = tr::fill_rectangle_vertices(it, rect)                                                                                      //
//       -> buffer now contains the 4 vertices of the rectangle                                                                          //
//     - tr::fill_rectangle_vertices(range, rect, mat)                                                                                   //
//       -> range now contains the 4 vertices of the rectangle, transformed by 'mat'                                                     //
//     - it = tr::fill_rectangle_vertices(it, pos, size / 2, size, 45_deg)                                                               //
//       -> buffer now contains the 4 vertices of a rectangle positioned and rotated around its center point by 45 degrees               //
//     - it = tr::fill_rectangle_outline_vertices(it, rect, thickness)                                                                   //
//       -> buffer now contains the 8 vertices of the rectangle outline                                                                  //
//     - tr::fill_rectangle_outline_vertices(range, rect, thickness, mat)                                                                //
//       -> range now contains the 8 vertices of the rectangle outline, transformed by 'mat'                                             //
//     - it = tr::fill_rectangle_outline_vertices(it, pos, size / 2, size, 45_deg, thickness)                                            //
//       -> buffer now contains the 8 vertices of a rectangle outline positioned and rotated around its center point by 45 degrees       //
//     - tr::fill_arc_vertices(range, circle, 45_deg, 90_deg)                                                                            //
//       -> range now contains vertices of an arc spanning from 45 degrees to 135 degrees on a circle                                    //
//     - it = tr::fill_regular_polygon_vertices(it, 20, circle, 20_deg)                                                                  //
//       -> buffer now contains the 20 vertices of a regular polygon described by a circle, rotated by 20 degrees                        //
//     - tr::fill_circle_vertices(range, circle)                                                                                         //
//       -> range now contains the vertices of a circle                                                                                  //
//     - tr::fill_regular_polygon_outline_vertices(range, circle, 20_deg, thickness)                                                     //
//       -> range now contins the vertices of a regular polygon outline describe by a circle, rotated by 20 degrees                      //
//     - it = tr::fill_circle_outline_vertices(it, 20, circle, thickness)                                                                //
//       -> range now contains the 40 (20 inner, 20 outer) vertices of a circle outline                                                  //
//                                                                                                                                       //
// Functions for calculating the number of vertices needed to draw a smooth polygon or arc are provided:                                 //
//     - tr::smooth_polygon_vertices(10.0f) -> calculates the number of vertices needed to draw a smooth circle                          //
//     - tr::smooth_arc_vertices(10.0f, 45_deg) -> calculates the number of vertices needed to draw a smooth arc of 45 degrees           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "geometry.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Calculates the number of segments needed to draw a smooth circle with pixel radius r.
	inline usize smooth_polygon_vertices(float r);
	// Calculates the number of segments needed to draw a smooth arc with pixel radius r.
	inline usize smooth_arc_vertices(float r, angle sizeth);

	// Calculates the number of indices needed for a line strip.
	constexpr usize line_strip_indices(u16 vertices);
	// Calculates the number of indices needed for a line loop.
	constexpr usize line_loop_indices(u16 vertices);
	// Calculates the number of indices needed for a simple polygon mesh with no holes.
	constexpr usize polygon_indices(u16 vertices);
	// Calculates the number of indices needed for a simple polygon mesh with no holes.
	constexpr usize polygon_outline_indices(u16 vertices);

	// Outputs indices for a line strip to an output iterator.
	// out needs to have space for line_strip_indices(vertices) indices.
	template <std::output_iterator<u16> O> constexpr O fill_line_strip_indices(O out, u16 vertices, u16 base);
	// Outputs indices for a line loop to an output iterator.
	// out needs to have space for line_loop_indices(vertices) indices.
	template <std::output_iterator<u16> O> constexpr O fill_line_loop_indices(O out, u16 vertices, u16 base);
	// Outputs indices for a convex polygon to an output iterator.
	// out needs to have space for polygon_indices(vertices) indices.
	template <std::output_iterator<u16> O> constexpr O fill_convex_polygon_indices(O out, u16 vertices, u16 base);
	// Outputs indices for a convex polygon outline to an output iterator.
	// out needs to have space for polygon_outline_indices(vertices) indices.
	// vertices is the number of vertices in the polygon, not the mesh.
	template <std::output_iterator<u16> O> constexpr O fill_convex_polygon_outline_indices(O out, u16 vertices, u16 base);
	// Outputs indices for a simple polygon to an output iterator.
	// out needs to have space for polygon_indices(vertices) indices.
	template <std::output_iterator<u16> O> constexpr O fill_simple_polygon_indices(O out, std::span<const glm::vec2> vertices, u16 base);

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
	// out needs to have space for vertices vertices.
	template <std::output_iterator<glm::vec2> O> O fill_arc_vertices(O out, usize vertices, circle circle, angle start, angle size);
	// Outputs arc vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_arc_vertices(R&& out, circle circle, angle start, angle size);
	// Outputs regular polygon vertices to an output iterator.
	// out needs to have space for vertices vertices.
	template <std::output_iterator<glm::vec2> O> O fill_regular_polygon_vertices(O out, usize vertices, circle circle, angle rotation);
	// Outputs regular polygon vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_regular_polygon_vertices(R&& out, circle circle, angle rotation);
	// Outputs circle vertices to an output iterator.
	// out needs to have space for vertices vertices.
	template <std::output_iterator<glm::vec2> O> O fill_circle_vertices(O out, usize vertices, circle circle);
	// Outputs circle vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_circle_vertices(R&& out, circle circle);

	// Outputs regular polygon outline vertices to an output iterator.
	// out needs to have space for vertices * 2 vertices.
	template <std::output_iterator<glm::vec2> O>
	O fill_regular_polygon_outline_vertices(O out, usize vertices, circle circle, angle rotation, float thickness);
	// Outputs regular polygon outline vertices to a range.
	template <sized_output_range<glm::vec2> R>
	void fill_regular_polygon_outline_vertices(R&& out, circle circle, angle rotation, float thickness);
	// Outputs circle outline vertices to an output iterator.
	// out needs to have space for vertices * 2 vertices.
	template <std::output_iterator<glm::vec2> O> O fill_circle_outline_vertices(O out, usize vertices, circle circle, float thickness);
	// Outputs circle outline vertices to a range.
	template <sized_output_range<glm::vec2> R> void fill_circle_outline_vertices(R&& out, circle circle, float thickness);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "draw_geometry_impl.hpp" // IWYU pragma: keep