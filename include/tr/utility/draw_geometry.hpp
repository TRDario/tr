#pragma once
#include "geometry.hpp"

namespace tr {
	/** @ingroup utility
	 *  @defgroup drawing Drawing
	 *  Helper functionality for drawing.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Calculates the number of segments needed to draw a smooth circle with pixel radius r.
	 *
	 * @param[in] r The radius of the circle.
	 * @param[in] scale The field scale parameter to apply to the calculation.
	 *
	 * @return The number of segments needed to draw a smooth circle with pixel radius r.
	 ******************************************************************************************************************/
	inline std::size_t smooth_poly_vtx(float r, float scale = 1.0f) noexcept;

	/******************************************************************************************************************
	 * Calculates the number of segments needed to draw a smooth arc.
	 *
	 * @param[in] r The radius of the arc circle.
	 * @param[in] sizeth The angular size of the arc.
	 * @param[in] scale The field scale parameter to apply to the calculation.
	 *
	 * @return The number of segments needed to draw a smooth arc.
	 ******************************************************************************************************************/
	inline std::size_t smooth_arc_vtx(float r, angle sizeth, float scale = 1.0f) noexcept;

	/******************************************************************************************************************
	 * Calculates the number of indices needed for a convex polygon mesh.
	 *
	 * @param[in] vtx The number of vertices in the polygon.
	 *
	 * @return The number of indices needed for the polygon mesh.
	 *****************************************************************************************************************/
	constexpr std::size_t poly_idx(std::uint16_t vtx) noexcept;

	/******************************************************************************************************************
	 * Calculates the number of indices needed for a convex polygon mesh.
	 *
	 * @param[in] vtx The number of vertices in the polygon.
	 *
	 * @return The number of indices needed for the polygon mesh.
	 *****************************************************************************************************************/
	constexpr std::size_t poly_outline_idx(std::uint16_t vtx) noexcept;

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon.
	 *
	 * @tparam It An index output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for `(vtx - 2) * 3` indices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] base The "base" index offset added to every index value.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<std::uint16_t> It> constexpr It fill_poly_idx(It out, std::uint16_t vtx, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon.
	 *
	 * @tparam R A sized output range of std::uint16_t.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for `(vtx - 2) * 3` indices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] base The "base" index offset added to every index value.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<std::uint16_t> R>
	constexpr std::ranges::iterator_t<R> fill_poly_idx(R&& out, std::uint16_t vtx, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon outline.
	 *
	 * @tparam It An index output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for `vtx * 6` indices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] base The "base" index offset added to every index value.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<std::uint16_t> It> constexpr It fill_poly_outline_idx(It out, std::uint16_t vtx, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs indices for a convex polygon outline.
	 *
	 * @tparam R A sized output range of std::uint16_t.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for `vtx * 6` indices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] base The "base" index offset added to every index value.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<std::uint16_t> R>
	constexpr std::ranges::iterator_t<R> fill_poly_outline_idx(R&& out, std::uint16_t vtx, std::uint16_t base);

	/******************************************************************************************************************
	 * Outputs unrotated rectangle vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] rect The rect to produce vertices for.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> constexpr It fill_rect_vtx(It out, const frect2& rect);

	/******************************************************************************************************************
	 * Outputs unrotated rectangle vertices.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] rect The rect to produce vertices for.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R> constexpr std::ranges::iterator_t<R> fill_rect_vtx(R&& out, const frect2& rect);

	/******************************************************************************************************************
	 * Outputs rectangle vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] rect The rectangle to produce vertices for.
	 * @param[in] mat A transformation matrix to apply to the vertices.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> constexpr It fill_rect_vtx(It out, const frect2& rect, const glm::mat4& mat);

	/******************************************************************************************************************
	 * Outputs unrotated rectangle vertices.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] rect The rect to produce vertices for.
	 * @param[in] mat A transformation matrix to apply to the vertices.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R>
	constexpr std::ranges::iterator_t<R> fill_rect_vtx(R&& out, const frect2& rect, const glm::mat4& mat);

	/******************************************************************************************************************
	 * Outputs rotated rectangle vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] pos The position of the anchor point of the rectangle.
	 * @param[in] anchor The position of the anchor point within the rectangle ((0, 0) = top-left).
	 * @param[in] size The size of the rectangle.
	 * @param[in] rot The rotation of the rectangle.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	inline It fill_rotated_rect_vtx(It out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rot);

	/******************************************************************************************************************
	 * Outputs rotated rectangle vertices.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for 4 vertices.
	 * @endparblock
	 * @param[in] pos The position of the anchor point of the rectangle.
	 * @param[in] anchor The position of the anchor point within the rectangle ((0, 0) = top-left).
	 * @param[in] size The size of the rectangle.
	 * @param[in] rot The rotation of the rectangle.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R>
	inline std::ranges::iterator_t<R> fill_rotated_rect_vtx(R&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rot);

	/******************************************************************************************************************
	 * Outputs unrotated rectangle outline vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] rect The rect to produce vertices for.
	 * @param[in] thick The thickness of the outline.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> constexpr It fill_rect_outline_vtx(It out, const frect2& rect, float thick);

	/******************************************************************************************************************
	 * Outputs rectangle outline vertices.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] rect The rect to produce vertices for.
	 * @param[in] thick The thickness of the outline.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R>
	constexpr std::ranges::iterator_t<R> fill_rect_outline_vtx(R&& out, const frect2& rect, float thick);

	/******************************************************************************************************************
	 * Outputs rectangle outline vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] rect The rect to produce vertices for.
	 * @param[in] thick The thickness of the outline.
	 * @param[in] mat A transformation matrix to apply to the vertices.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	constexpr It fill_rect_outline_vtx(It out, const frect2& rect, float thick, const glm::mat4& mat);

	/******************************************************************************************************************
	 * Outputs rectangle outline vertices.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] rect The rect to produce vertices for.
	 * @param[in] thick The thickness of the outline.
	 * @param[in] mat A transformation matrix to apply to the vertices.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R>
	constexpr std::ranges::iterator_t<R> fill_rect_outline_vtx(R&& out, const frect2& rect, float thick, const glm::mat4& mat);

	/******************************************************************************************************************
	 * Outputs rotated rectangle outline vertices.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] pos The position of the anchor point of the rectangle.
	 * @param[in] anchor The position of the anchor point within the rectangle ((0, 0) = top-left).
	 * @param[in] size The size of the rectangle.
	 * @param[in] rot The rotation of the rectangle.
	 * @param[in] thick The thickness of the outline.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It>
	inline It fill_rotated_rect_outline_vtx(It out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rot, float thick);

	/******************************************************************************************************************
	 * Outputs rotated rectangle outline vertices.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for 8 vertices.
	 * @endparblock
	 * @param[in] pos The position of the anchor point of the rectangle.
	 * @param[in] anchor The position of the anchor point within the rectangle ((0, 0) = top-left).
	 * @param[in] size The size of the rectangle.
	 * @param[in] rot The rotation of the rectangle.
	 * @param[in] thick The thickness of the outline.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R>
	inline std::ranges::iterator_t<R> fill_rotated_rect_outline_vtx(R&& out, glm::vec2 pos, glm::vec2 anchor, glm::vec2 size, angle rot,
																	float thick);

	/******************************************************************************************************************
	 * Outputs vertex positions along an arc.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for all of the vertices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices on the arc.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] circ The arc circle.
	 * @param[in] startth The starting angle on the arc circle of the arc.
	 * @param[in] sizeth The angular size of the arc.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> It fill_arc_vtx(It out, std::size_t vtx, circle circ, angle startth, angle sizeth);

	/******************************************************************************************************************
	 * Outputs vertex positions along an arc.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for all of the vertices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices on the arc.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] circ The arc circle.
	 * @param[in] startth The starting angle on the arc circle of the arc.
	 * @param[in] sizeth The angular size of the arc.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R>
	std::ranges::iterator_t<R> fill_arc_vtx(R&& out, std::size_t vtx, circle circ, angle startth, angle sizeth);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for all of the vertices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] circ The tangent circle of the polygon.
	 * @param[in] rot The rotation of the polygon.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> It fill_poly_vtx(It out, std::size_t vtx, circle circ, angle rot = 0_deg);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for all of the vertices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vertices must be greater than 2.
	 * @endparblock
	 * @param[in] circ The tangent circle of the polygon.
	 * @param[in] rot The rotation of the polygon.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R>
	std::ranges::iterator_t<R> fill_poly_vtx(R&& out, std::size_t vtx, circle circ, angle rot = 0_deg);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon's outline.
	 *
	 * @tparam It A position vector output iterator type.
	 *
	 * @param[out] out
	 * @parblock
	 * The output iterator.
	 *
	 * @pre There has to be space for `vtx * 2` vertices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] circ The tangent circle of the polygon.
	 * @param[in] rot The rotation of the polygon.
	 * @param[in] thick The thickness of the polygon.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <std::output_iterator<glm::vec2> It> It fill_poly_outline_vtx(It out, std::size_t vtx, circle circ, angle rot, float thick);

	/******************************************************************************************************************
	 * Outputs vertex positions for a regular polygon's outline.
	 *
	 * @tparam R A sized output range of glm::vec2.
	 *
	 * @param[out] out
	 * @parblock
	 * The output range which will be filled from the beginning.
	 *
	 * @pre There has to be space for `vtx * 2` vertices.
	 * @endparblock
	 * @param[in] vtx
	 * @parblock
	 * The number of vertices in the polygon.
	 *
	 * @pre @em vtx must be greater than 2.
	 * @endparblock
	 * @param[in] circ The tangent circle of the polygon.
	 * @param[in] rot The rotation of the polygon.
	 * @param[in] thick The thickness of the polygon.
	 *
	 * @return An iterator to the end of the outputted sequence.
	 ******************************************************************************************************************/
	template <sized_output_range<glm::vec2> R>
	std::ranges::iterator_t<R> fill_poly_outline_vtx(R&& out, std::size_t vtx, circle circ, angle rot, float thick);

	/// @}
} // namespace tr

#include "draw_geometry_impl.hpp"