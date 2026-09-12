/// @file
/// @brief Provides `tr::rectangle` and related functions.

#pragma once
#include <tr/utility/rectangle_edges.hpp>

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
		[[nodiscard]] constexpr rectangle() noexcept = default;

		/// Constructs a rectangle from a top-left corner and size.
		/// @param tl Offset of the top-left corner of the rectangle.
		/// @param size Size of the rectangle.
		[[nodiscard]] constexpr rectangle(glm::tvec2<Element> tl, glm::tvec2<Element> size) noexcept
			: tl{tl}
			, size{size}
		{
		}

		/// Constructs a rectangle with the top-left corner at the origin.
		/// @param size Size of the rectangle.
		[[nodiscard]] constexpr rectangle(glm::tvec2<Element> size) noexcept
			: tl{}
			, size{size}
		{
		}

		/// Copy-constructs a rectangle.
		/// @tparam ElementR Element type of the righthand-side rectangle vectors.
		/// @param rectangle Rectangle to copy.
		template <typename ElementR>
		[[nodiscard]] constexpr rectangle(const rectangle<ElementR>& rectangle) noexcept
			: tl{rectangle.tl}
			, size{rectangle.size}
		{
		}

		/// @}

		/// Compares two rectangles for equality.
		/// @tparam ElementR Element type of the righthand-side rectangle vectors.
		/// @param rhs Rectangle to compare with.
		/// @return Whether the rectangles are equal.
		template <typename ElementR>
		[[nodiscard]] constexpr bool operator==(const rectangle<ElementR>& rhs) const noexcept
		{
			return tl.x == rhs.tl.x && tl.y == rhs.tl.y && size.x == rhs.size.x && size.y == rhs.size.y;
		}

		/// @name Contains
		/// @{

		/// Determines whether a point is contained within the rectangle.
		/// @tparam ElementR Element type of the point.
		/// @param point Point to check for being in the rectangle.
		/// @return `true` if `point` is within the rectangle, `false` otherwise.
		template <typename ElementR = Element>
		[[nodiscard]] constexpr bool contains(glm::tvec2<ElementR> point) const noexcept
		{
			for (int i = 0; i < 2; ++i) {
				if (point[i] < tl[i] || point[i] > tl[i] + size[i]) {
					return false;
				}
			}
			return true;
		}

		/// Determines whether another rectangle is contained entirely within the rectangle.
		/// @tparam ElementR Element type of the righthand-side rectangle vectors.
		/// @param rectangle Rectangle to check for being contained entirely within the rectangle.
		/// @return `true` if `rectangle` is contained entirely within the rectangle, `false` otherwise.
		template <typename ElementR = Element>
		[[nodiscard]] constexpr bool contains(const rectangle<ElementR>& rectangle) const noexcept
		{
			return contains(rectangle.tl) && contains(rectangle.tl + rectangle.size);
		}

		/// @}
		/// @name Edges
		/// @{

		/// Gets the edges of the rectangle.
		/// @return Set of edges corresponding to the rectangle.
		[[nodiscard]] constexpr rectangle_edges<Element> edges() const noexcept
		{
			return {tl.x, tl.y, tl.x + size.x, tl.y + size.y};
		}

		/// @}
	};

	/// @name Intersection
	/// @{

	/// Determines if two rectangles intersect.
	/// @tparam ElementL, ElementR Element types of the rectangle vectors.
	/// @param lhs, rhs Rectangles to check for an intersection.
	/// @return `true` if the rectangles intersect, `false` otherwise.
	template <typename ElementL, typename ElementR>
	[[nodiscard]] constexpr bool intersecting(rectangle<ElementL> lhs, rectangle<ElementR> rhs) noexcept
	{
		return lhs.contains(rhs.tl) || lhs.contains(rhs.tl + rhs.size) || rhs.contains(lhs.tl + glm::tvec2<ElementL>{lhs.size.x, 0}) ||
			   rhs.contains(lhs.tl + glm::tvec2<ElementL>{0, lhs.size.y});
	}

	/// Computes the intersection of two rectangles.
	/// @tparam Element Element type of the rectangle vectors.
	/// @param lhs, rhs Rectangles to get the intersection of.
	/// @return Intersection rectangle, if one exists.
	template <typename Element>
	[[nodiscard]] constexpr std::optional<rectangle<Element>> intersection(rectangle<Element> lhs, rectangle<Element> rhs) noexcept
	{
		if constexpr (!std::unsigned_integral<Element>) {
			if (lhs.size.x < 0) {
				lhs.tl.x += lhs.size.x;
				lhs.size.x = -lhs.size.x;
			}
			if (lhs.size.y < 0) {
				lhs.tl.y += lhs.size.y;
				lhs.size.y = -lhs.size.y;
			}
			if (rhs.size.x < 0) {
				rhs.tl.x += rhs.size.x;
				rhs.size.x = -rhs.size.x;
			}
			if (rhs.size.y < 0) {
				rhs.tl.y += rhs.size.y;
				rhs.size.y = -rhs.size.y;
			}
		}

		const glm::tvec2<Element> tl{glm::max(lhs.tl, rhs.tl)};
		const glm::tvec2<Element> br{glm::min(lhs.tl + lhs.size, rhs.tl + rhs.size)};
		return (br.x > tl.x && br.y > tl.y) ? std::make_optional<rectangle<Element>>(tl, br - tl) : std::nullopt;
	}

	/// @}
} // namespace tr

//

/// Enables default binary IO for rectangles.
template <typename Element>
inline constexpr bool tr::enable_default_binary_io<tr::rectangle<Element>>{true};