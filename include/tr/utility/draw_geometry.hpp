/// @file
/// @brief Defines functions related to generating mesh geometry.

#pragma once
#include "angle.hpp"
#include "circle.hpp"
#include "rectangle.hpp"

//

namespace tr
{
	/// @name Drawing geometry
	/// @{

	/// Calculates the number of segments needed to draw a smooth circle with pixel radius `r`.
	/// @param r Pixel radius of the circle.
	/// @return Number of segments needed to draw a smooth circle.
	inline usize smooth_polygon_vertices(float r);

	/// Calculates the number of segments needed to draw a smooth arc with pixel radius `r`.
	/// @param r Pixel radius of the arc.
	/// @param sizeth Angle size of the arc.
	/// @return Number of segments needed to draw a smooth arc.
	inline usize smooth_arc_vertices(float r, angle sizeth);

	//

	/// Calculates the number of indices needed for a line strip.
	/// @param vertices Number of vertices in the line strip.
	/// @return Number of indices needed for a line strip.
	constexpr usize line_strip_indices(u16 vertices);

	/// Calculates the number of indices needed for a line loop.
	/// @param vertices Number of vertices in the line loop.
	/// @return Number of indices needed for a line loop.
	constexpr usize line_loop_indices(u16 vertices);

	/// Calculates the number of indices needed for a simple polygon mesh with no holes.
	/// @param vertices Number of vertices in the polygon.
	/// @return Number of indices needed for a simple polygon mesh with no holes.
	constexpr usize polygon_indices(u16 vertices);

	/// Calculates the number of indices needed for a simple polygon mesh with no holes.
	/// @param vertices Number of vertices in the polygon outline.
	/// @return Number of indices needed for a simple polygon mesh with no holes.
	constexpr usize polygon_outline_indices(u16 vertices);

	//

	/// Outputs indices for a line strip to an output iterator.
	/// @tparam Iterator Index output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the line strip.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `line_strip_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_line_strip_indices(Iterator out, u16 vertices, u16 base);

	/// Outputs indices for a line loop to an output iterator.
	/// @tparam Iterator Index output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the line loop.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `line_loop_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_line_loop_indices(Iterator out, u16 vertices, u16 base);

	/// Outputs indices for a convex polygon to an output iterator.
	/// @tparam Iterator Index output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the convex polygon.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `polygon_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_convex_polygon_indices(Iterator out, u16 vertices, u16 base);

	/// Outputs indices for a convex polygon outline to an output iterator.
	/// @note `vertices` is the number of vertices in the polygon, not the mesh.
	/// @tparam Iterator Index output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the convex polygon outline.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `polygon_outline_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_convex_polygon_outline_indices(Iterator out, u16 vertices, u16 base);

	/// Outputs indices for a simple polygon to an output iterator.
	/// @param out Output iterator.
	/// @param vertices Vertices of the polygon.
	/// @param base Base index to count up from.
	/// @pre `out` needs to have space for `polygon_indices(vertices)` indices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<u16> Iterator>
	constexpr Iterator fill_simple_polygon_indices(Iterator out, std::span<const glm::vec2> vertices, u16 base);

	//

	/// Outputs rectangle vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param rectangle Rectangle to output vertices for.
	/// @pre `out` needs to have space for 4 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	constexpr Iterator fill_rectangle_vertices(Iterator out, rectangle<float> rectangle);

	/// Outputs rectangle vertices to a range of 4 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param rectangle Rectangle to output vertices for.
	/// @pre `out` must have a size of 4.
	template <sized_output_range<glm::vec2> Range>
	constexpr void fill_rectangle_vertices(Range&& out, rectangle<float> rectangle);

	/// Outputs transformed rectangle vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param rectangle Rectangle to output vertices for.
	/// @param mat Transformation to apply on the vertices.
	/// @pre `out` needs to have space for 4 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	constexpr Iterator fill_rectangle_vertices(Iterator out, rectangle<float> rectangle, const glm::mat4& mat);

	/// Outputs transformed rectangle vertices to a range of 4 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param rectangle Rectangle to output vertices for.
	/// @param mat Transformation to apply on the vertices.
	/// @pre `out` must have a size of 4.
	template <sized_output_range<glm::vec2> Range>
	constexpr void fill_rectangle_vertices(Range&& out, rectangle<float> rectangle, const glm::mat4& mat);

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
	Iterator fill_rectangle_vertices(Iterator out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation);

	/// Outputs transformed rectangle vertices to a range of 4 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param pos Position of the rectangle's anchor.
	/// @param anchor Position of the anchor point within the rectangle.
	/// @param size Size of the rectangle.
	/// @param rotation Rotation of the rectangle.
	/// @pre `out` must have a size of 4.
	template <sized_output_range<glm::vec2> Range>
	void fill_rectangle_vertices(Range&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation);

	//

	/// Outputs rectangle outline vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param rectangle Rectangle to output vertices for.
	/// @param thickness Thickness of the rectangle outline.
	/// @pre `out` needs to have space for 8 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	constexpr Iterator fill_rectangle_outline_vertices(Iterator out, rectangle<float> rectangle, float thickness);

	/// Outputs rectangle outline vertices to a range of 8 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param rectangle Rectangle to output vertices for.
	/// @param thickness Thickness of the rectangle outline.
	/// @pre `out` must have a size of 8.
	template <sized_output_range<glm::vec2> Range>
	constexpr void fill_rectangle_outline_vertices(Range&& out, rectangle<float> rectangle, float thickness);

	/// Outputs transformed rectangle outline vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param rectangle Rectangle to output vertices for.
	/// @param thickness Thickness of the rectangle outline.
	/// @param mat Transformation to apply on the vertices.
	/// @pre `out` needs to have space for 8 vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	constexpr Iterator fill_rectangle_outline_vertices(Iterator out, rectangle<float> rectangle, float thickness, const glm::mat4& mat);

	/// Outputs transformed rectangle outline vertices to a range of 8 vertices.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param rectangle Rectangle to output vertices for.
	/// @param thickness Thickness of the rectangle outline.
	/// @param mat Transformation to apply on the vertices.
	/// @pre `out` must have a size of 8.
	template <sized_output_range<glm::vec2> Range>
	constexpr void fill_rectangle_outline_vertices(Range&& out, rectangle<float> rectangle, float thickness, const glm::mat4& mat);

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
	Iterator fill_rectangle_outline_vertices(Iterator out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation,
											 float thickness);

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
	void fill_rectangle_outline_vertices(Range&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rotation, float thickness);

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
	Iterator fill_arc_vertices(Iterator out, usize vertices, circle circle, angle start, angle size);

	/// Outputs arc vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circle the arc rests on.
	/// @param start Starting angle of the arc on the circle.
	/// @param size Angle size of the arc.
	template <sized_output_range<glm::vec2> Range>
	void fill_arc_vertices(Range&& out, circle circle, angle start, angle size);

	/// Outputs regular polygon vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the polygon.
	/// @param circle Circumscribed circle of the polygon.
	/// @param rotation Rotation of the polygon.
	/// @pre `out` needs to have space for `vertices` vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_regular_polygon_vertices(Iterator out, usize vertices, circle circle, angle rotation);

	/// Outputs regular polygon vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circumscribed circle of the polygon.
	/// @param rotation Rotation of the polygon.
	template <sized_output_range<glm::vec2> Range>
	void fill_regular_polygon_vertices(Range&& out, circle circle, angle rotation);

	/// Outputs circle vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the circle.
	/// @param circle Circle to output vertex positions for.
	/// @pre `out` needs to have space for `vertices` vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_circle_vertices(Iterator out, usize vertices, circle circle);

	/// Outputs circle vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circle to output vertex positions for.
	template <sized_output_range<glm::vec2> Range>
	void fill_circle_vertices(Range&& out, circle circle);

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
	Iterator fill_regular_polygon_outline_vertices(Iterator out, usize vertices, circle circle, angle rotation, float thickness);

	/// Outputs regular polygon outline vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circumscribed circle of the polygon.
	/// @param rotation Rotation of the polygon.
	/// @param thickness Thickness of the polygon outline.
	template <sized_output_range<glm::vec2> Range>
	void fill_regular_polygon_outline_vertices(Range&& out, circle circle, angle rotation, float thickness);

	/// Outputs circle outline vertices to an output iterator.
	/// @tparam Iterator Vector output iterator type.
	/// @param out Output iterator.
	/// @param vertices Number of vertices in the circle.
	/// @param circle Circle to output vertex positions for.
	/// @param thickness Thickness of the circle outline.
	/// @pre `out` needs to have space for `vertices * 2` vertices.
	/// @return Iterator to the end of the written range.
	template <std::output_iterator<glm::vec2> Iterator>
	Iterator fill_circle_outline_vertices(Iterator out, usize vertices, circle circle, float thickness);

	/// Outputs circle outline vertices to a range.
	/// @tparam Range Vector output range type.
	/// @param out Output range.
	/// @param circle Circle to output vertex positions for.
	/// @param thickness Thickness of the circle outline.
	template <sized_output_range<glm::vec2> Range>
	void fill_circle_outline_vertices(Range&& out, circle circle, float thickness);

	/// @}
} // namespace tr

#include "impl/draw_geometry.hpp" // IWYU pragma: export