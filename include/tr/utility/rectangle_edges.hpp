/// @file
/// @brief Provides `tr::rectangle_edges`.

#pragma once

//

namespace tr
{
	/// Set of rectangle edges.
	/// @tparam Element Edge value type.
	template <typename Element>
	struct rectangle_edges
	{
		/// Left edge value.
		Element left{0};

		/// Top edge value.
		Element top{0};

		/// Right edge value.
		Element right{0};

		/// Bottom edge value.
		Element bottom{0};

		/// @name Constructors
		/// @{

		/// Constructs a set of edges with value 0.
		[[nodiscard]] constexpr rectangle_edges() noexcept = default;

		/// Constructs a set of edges with the same value.
		/// @param left Uniform edge value.
		[[nodiscard]] constexpr rectangle_edges(Element left) noexcept;

		/// Constructs a set of edges with top = bottom, left = right.
		/// @param left Left and right edge value.
		/// @param top Top and bottom edge value.
		[[nodiscard]] constexpr rectangle_edges(Element left, Element top) noexcept;

		/// Constructs a set of edges with top = bottom.
		/// @param left Left edge value.
		/// @param top Top and bottom edge value.
		/// @param right Right edge value.
		[[nodiscard]] constexpr rectangle_edges(Element left, Element top, Element right) noexcept;

		/// Constructs a set of edges.
		/// @param left Left edge value.
		/// @param top Top edge value.
		/// @param right Right edge value.
		/// @param bottom Bottom edge value.
		[[nodiscard]] constexpr rectangle_edges(Element left, Element top, Element right, Element bottom) noexcept;

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two sets of edges for equality.
		/// @tparam ElementR Edge value type of the righthand-side edges.
		/// @param rhs Set of edges to compare with.
		/// @return Whether the two sets of edges are equal.
		template <typename ElementR>
		[[nodiscard]] constexpr bool operator==(const rectangle_edges<ElementR>& rhs) const noexcept;

		/// @}
	};
} // namespace tr

#include "impl/rectangle_edges.hpp" // IWYU pragma: export