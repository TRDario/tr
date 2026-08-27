/// @file
/// @brief Provides `tr::rectangle` and related functions.

#pragma once
#include "rectangle_edges.hpp"

//

namespace tr
{
	/// Rectangle object.
	/// @tparam Element Element type of the rectangle vectors.
	template <typename Element>
	struct rectangle
	{
		// Offset of the top-left corner of the rectangle.
		glm::tvec2<Element> tl;

		// Size of the rectangle.
		glm::tvec2<Element> size;

		/// @name Constructors
		/// @{

		/// Default-constructs a rectangle.
		constexpr rectangle() = default;

		/// Constructs a rectangle from a top-left corner and size.
		/// @param tl Offset of the top-left corner of the rectangle.
		/// @param size Size of the rectangle.
		constexpr rectangle(glm::tvec2<Element> tl, glm::tvec2<Element> size);

		/// Constructs a rectangle with the top-left corner at the origin.
		/// @param size Size of the rectangle.
		constexpr rectangle(glm::tvec2<Element> size);

		/// Copy-constructs a rectangle.
		/// @tparam ElementR Element type of the righthand-side rectangle vectors.
		/// @param rectangle Rectangle to copy.
		template <typename ElementR>
		constexpr rectangle(const rectangle<ElementR>& rectangle);

		/// @}

		/// Compares two rectangles for equality.
		/// @tparam ElementR Element type of the righthand-side rectangle vectors.
		/// @param rhs Rectangle to compare with.
		/// @return Whether the rectangles are equal.
		template <typename ElementR>
		constexpr bool operator==(const rectangle<ElementR>& rhs) const;

		/// @name Contains
		/// @{

		/// Determines whether a point is contained within the rectangle.
		/// @tparam ElementR Element type of the point.
		/// @param point Point to check for being in the rectangle.
		/// @return `true` if `point` is within the rectangle, `false` otherwise.
		template <typename ElementR = Element>
		constexpr bool contains(glm::tvec2<ElementR> point) const;

		/// Determines whether another rectangle is contained entirely within the rectangle.
		/// @tparam ElementR Element type of the righthand-side rectangle vectors.
		/// @param rectangle Rectangle to check for being contained entirely within the rectangle.
		/// @return `true` if `rectangle` is contained entirely within the rectangle, `false` otherwise.
		template <typename ElementR = Element>
		constexpr bool contains(const rectangle<ElementR>& rectangle) const;

		/// @}
		/// @name Edges
		/// @{

		/// Gets the edges of the rectangle.
		/// @return Set of edges corresponding to the rectangle.
		constexpr rectangle_edges<Element> edges() const;

		/// @}
	};

	/// @name Intersection
	/// @{

	/// Determines if two rectangles intersect.
	/// @tparam ElementL, ElementR Element types of the rectangle vectors.
	/// @param lhs, rhs Rectangles to check for an intersection.
	/// @return `true` if the rectangles intersect, `false` otherwise.
	template <typename ElementL, typename ElementR>
	constexpr bool intersecting(rectangle<ElementL> lhs, rectangle<ElementR> rhs);

	/// Computes the intersection of two rectangles.
	/// @tparam Element Element type of the rectangle vectors.
	/// @param lhs, rhs Rectangles to get the intersection of.
	/// @return Intersection rectangle, if one exists.
	template <typename Element>
	constexpr std::optional<rectangle<Element>> intersection(rectangle<Element> lhs, rectangle<Element> rhs);

	/// @}
} // namespace tr

#include "impl/rectangle.hpp" // IWYU pragma: export