/// @file
/// @brief Provides `tr::rectangle` and related functions.

#pragma once
#include <tr/utility/rectangle_edges.hpp>

//

namespace tr
{
	/// Rectangle object.
	/// @tparam Rep Representation type of the rectangle.
	template <arithmetic Rep>
	struct rectangle
	{
		// Offset of the top-left corner of the rectangle.
		glm::tvec2<Rep> tl{};

		// Size of the rectangle.
		glm::tvec2<Rep> size{};

		/// @name Conversion operators
		/// @{

		template <arithmetic TargetRep>
		[[nodiscard]] explicit constexpr operator rectangle<TargetRep>() const noexcept
		{
			return rectangle<TargetRep>{static_cast<glm::tvec2<TargetRep>>(tl), static_cast<glm::tvec2<TargetRep>>(size)};
		}

		/// @}
		/// @name Contains
		/// @{

		/// Determines whether a point is contained within the rectangle.
		/// @details A point along the edges and corners of the rectangle is considered contained.
		/// @tparam PointRep Representation type of the point.
		/// @param point Point to check for being in the rectangle.
		/// @return `true` if `point` is within the rectangle, `false` otherwise.
		template <arithmetic RhsRep = Rep>
		[[nodiscard]] constexpr bool contains(glm::tvec2<RhsRep> point) const noexcept
		{
			const rectangle_edges<Rep> edges{this->edges()};
			return point.x >= edges.left && point.x <= edges.right && point.y >= edges.top && point.y <= edges.bottom;
		}

		/// Determines whether another rectangle is contained entirely within the rectangle.
		/// @tparam RhsRep Representation type of the right-hand side rectangle.
		/// @param rectangle Rectangle to check for being contained entirely within the rectangle.
		/// @return `true` if `rectangle` is contained entirely within the rectangle, `false` otherwise.
		template <arithmetic RhsRep = Rep>
		[[nodiscard]] constexpr bool contains(const rectangle<RhsRep>& rectangle) const noexcept
		{
			return contains(rectangle.tl) && contains(rectangle.tl + rectangle.size);
		}

		/// @}
		/// @name Operations
		/// @{

		/// Gets the canonical representation of the rectangle.
		/// @details The canonical representation of a rectangle has a non-negative size.
		/// @return Canonical representation of the rectangle.
		[[nodiscard]] constexpr rectangle canonical() const noexcept
		{
			if constexpr (std::unsigned_integral<Rep>) {
				return *this;
			}
			else {
				rectangle copy{*this};
				if (copy.size.x < 0) {
					copy.tl.x += copy.size.x;
					copy.size.x = -copy.size.x;
				}
				if (copy.size.y < 0) {
					copy.tl.y += copy.size.y;
					copy.size.y = -copy.size.y;
				}
				return copy;
			}
		}

		/// Gets the edges of the rectangle.
		/// @return Set of edges corresponding to the rectangle.
		[[nodiscard]] constexpr rectangle_edges<Rep> edges() const noexcept
		{
			rectangle_edges<Rep> edges{tl.x, tl.y, tl.x + size.x, tl.y + size.y};
			if constexpr (!std::unsigned_integral<Rep>) {
				if (edges.left > edges.right) {
					std::swap(edges.left, edges.right);
				}
				if (edges.top > edges.bottom) {
					std::swap(edges.top, edges.bottom);
				}
			}
			return edges;
		}

		/// @}
	};

	/// @name Rectangle comparison operators
	/// @{

	/// Compares two rectangles for equality.
	/// @tparam LhsRep, RhsRep Representation types of the vectors.
	/// @param lhs, rhs Rectangle to compare.
	/// @return Whether the rectangles are equal.
	template <arithmetic LhsRep, arithmetic RhsRep>
	[[nodiscard]] constexpr bool operator==(const rectangle<LhsRep>& lhs, const rectangle<RhsRep>& rhs) noexcept
	{
		return lhs.tl.x == rhs.tl.x && lhs.tl.y == rhs.tl.y && lhs.size.x == rhs.size.x && lhs.size.y == rhs.size.y;
	}

	/// @}
	/// @name Intersection
	/// @{

	/// Determines if two rectangles intersect.
	/// @details Rectangles that touch but don't overlap are not considered intersecting.
	/// @tparam ARep, BRep Representation types of the vectors.
	/// @param a, b Rectangles to check for an intersection.
	/// @return `true` if the rectangles intersect, `false` otherwise.
	template <arithmetic ARep, arithmetic BRep>
	[[nodiscard]] constexpr bool intersecting(const rectangle<ARep>& a, const rectangle<BRep>& b) noexcept
	{
		return intersection(a, b).has_value();
	}

	/// Computes the intersection of two rectangles.
	/// @details Rectangles that touch but don't overlap are not considered intersecting.
	/// @tparam ARep, BRep Representation types of the vectors.
	/// @param a, b Rectangles to get the intersection of.
	/// @return Intersection rectangle, if one exists.
	template <arithmetic ARep, arithmetic BRep>
	[[nodiscard]] constexpr std::optional<rectangle<std::common_type_t<ARep, BRep>>> intersection(const rectangle<ARep>& a,
																								  const rectangle<BRep>& b) noexcept
	{
		using CommonRep = std::common_type_t<ARep, BRep>;

		const rectangle<CommonRep> a_cr{static_cast<rectangle<CommonRep>>(a.canonical())};
		const rectangle<CommonRep> b_cr{static_cast<rectangle<CommonRep>>(b.canonical())};
		const glm::tvec2<CommonRep> tl{glm::max(a_cr.tl, b_cr.tl)};
		const glm::tvec2<CommonRep> br{glm::min(a_cr.tl + a_cr.size, b_cr.tl + b_cr.size)};
		return (br.x > tl.x && br.y > tl.y) ? std::make_optional<rectangle<CommonRep>>(tl, br - tl) : std::nullopt;
	}

	/// @}
} // namespace tr

//

/// Enables default binary IO for rectangles.
template <typename Element>
inline constexpr bool tr::enable_default_binary_io<tr::rectangle<Element>>{true};