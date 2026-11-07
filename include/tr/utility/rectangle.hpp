/// @file
/// @brief Provides `tr::rectangle` and related functionality.

/// @ingroup math
/// @defgroup rectangle Rectangles
/// @brief Contains `tr::rectangle` and related functionality.

#pragma once
#include <tr/utility/rectangle_edges.hpp>

//

namespace tr
{
	/// @ingroup rectangle
	/// @brief Rectangle in 2D space defined with a top-left corner and size.
	/// @tparam Rep Representation type of the rectangle.
	template <arithmetic Rep>
	struct rectangle
	{
		/// @brief Offset of the top-left corner of the rectangle.
		glm::tvec2<Rep> tl;

		/// @brief Size of the rectangle.
		glm::tvec2<Rep> size;

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a zero-size rectangle anchored at (0, 0).
		[[nodiscard]] constexpr rectangle() noexcept
			: tl{0, 0}
			, size{0, 0}
		{
		}

		/// @brief Constructs a rectangle anchored at (0, 0).
		/// @param width Width of the rectangle.
		/// @param height Height of the rectangle.
		[[nodiscard]] constexpr rectangle(Rep width, Rep height) noexcept
			: tl{0, 0}
			, size{width, height}
		{
		}

		/// @brief Constructs a rectangle anchored at (0, 0).
		/// @param size Size of the rectangle.
		[[nodiscard]] explicit constexpr rectangle(glm::tvec2<Rep> size) noexcept
			: tl{0, 0}
			, size{size}
		{
		}

		/// @brief Constructs a rectangle.
		/// @param left X position of the left edge of the rectangle.
		/// @param top Y position of the top edge of the rectangle.
		/// @param width Width of the rectangle.
		/// @param height Height of the rectangle.
		[[nodiscard]] constexpr rectangle(Rep left, Rep top, Rep width, Rep height) noexcept
			: tl{left, top}
			, size{width, height}
		{
		}

		/// @brief Constructs a rectangle.
		/// @param tl Position of the top-left corner of the rectangle.
		/// @param size Size of the rectangle.
		[[nodiscard]] constexpr rectangle(glm::tvec2<Rep> tl, glm::tvec2<Rep> size) noexcept
			: tl{tl}
			, size{size}
		{
		}

		/// @brief Constructs a rectangle by copying from a rectangle of a different representation type.
		/// @tparam RhsRep Representation type of the right-hand side rectangle.
		/// @param rhs Rectangle to copy from.
		template <arithmetic RhsRep>
		[[nodiscard]] constexpr rectangle(const rectangle<RhsRep>& rhs) noexcept
			: tl{static_cast<glm::tvec2<Rep>>(rhs.tl)}
			, size{static_cast<glm::tvec2<Rep>>(rhs.size)}
		{
		}

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether a point is contained within the rectangle.
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

		/// @brief Checks whether another rectangle is contained entirely within the rectangle.
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

		/// @brief Returns the canonical representation of the rectangle.
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

		/// @brief Returns the edges of the rectangle.
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

	//

	/// @ingroup rectangle
	/// @brief Checks two rectangles for equality.
	/// @tparam LhsRep, RhsRep Representation types of the vectors.
	/// @param lhs, rhs Rectangle to compare.
	/// @return `true` if the rectangles are equal, `false` otherwise.
	template <arithmetic LhsRep, arithmetic RhsRep>
	[[nodiscard]] constexpr bool operator==(const rectangle<LhsRep>& lhs, const rectangle<RhsRep>& rhs) noexcept
	{
		return lhs.tl.x == rhs.tl.x && lhs.tl.y == rhs.tl.y && lhs.size.x == rhs.size.x && lhs.size.y == rhs.size.y;
	}

	//

	/// @ingroup rectangle
	/// @brief Checks whether two rectangles intersect.
	/// @details Rectangles that touch but don't overlap are not considered intersecting.
	/// @tparam ARep, BRep Representation types of the vectors.
	/// @param a, b Rectangles to check for an intersection.
	/// @return `true` if the rectangles intersect, `false` otherwise.
	template <arithmetic ARep, arithmetic BRep>
	[[nodiscard]] constexpr bool intersecting(const rectangle<ARep>& a, const rectangle<BRep>& b) noexcept
	{
		return intersection(a, b).has_value();
	}

	/// @ingroup rectangle
	/// @brief Returns the intersection of two rectangles.
	/// @details Rectangles that touch but don't overlap are not considered intersecting.
	/// @tparam ARep, BRep Representation types of the vectors.
	/// @param a, b Rectangles to get the intersection of.
	/// @return Intersection rectangle, if one exists.
	template <arithmetic ARep, arithmetic BRep>
	[[nodiscard]] constexpr std::optional<rectangle<std::common_type_t<ARep, BRep>>> intersection(const rectangle<ARep>& a,
																								  const rectangle<BRep>& b) noexcept
	{
		using CommonRep = std::common_type_t<ARep, BRep>;

		const rectangle<CommonRep> a_cr{a.canonical()};
		const rectangle<CommonRep> b_cr{b.canonical()};
		const glm::tvec2<CommonRep> tl{glm::max(a_cr.tl, b_cr.tl)};
		const glm::tvec2<CommonRep> br{glm::min(a_cr.tl + a_cr.size, b_cr.tl + b_cr.size)};
		return (br.x > tl.x && br.y > tl.y) ? std::make_optional<rectangle<CommonRep>>(tl, br - tl) : std::nullopt;
	}
} // namespace tr

//

/// @brief Enables default binary IO for rectangles.
template <typename Rep>
inline constexpr bool tr::enable_default_binary_io<tr::rectangle<Rep>>{true};