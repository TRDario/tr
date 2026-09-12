/// @file
/// @brief Provides `tr::rectangle_edges`.

#pragma once
#include <tr/utility/default_binary_io.hpp>
#include <tr/utility/rectangle_edges.hpp>

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
		[[nodiscard]] constexpr rectangle_edges(Element left) noexcept
			: top{left}
			, right{left}
			, bottom{left}
			, left{left}
		{
		}

		/// Constructs a set of edges with top = bottom, left = right.
		/// @param left Left and right edge value.
		/// @param top Top and bottom edge value.
		[[nodiscard]] constexpr rectangle_edges(Element left, Element top) noexcept
			: left{left}
			, top{top}
			, right{left}
			, bottom{top}
		{
		}

		/// Constructs a set of edges with top = bottom.
		/// @param left Left edge value.
		/// @param top Top and bottom edge value.
		/// @param right Right edge value.
		[[nodiscard]] constexpr rectangle_edges(Element left, Element top, Element right) noexcept
			: left{left}
			, top{top}
			, right{right}
			, bottom{top}
		{
		}

		/// Constructs a set of edges.
		/// @param left Left edge value.
		/// @param top Top edge value.
		/// @param right Right edge value.
		/// @param bottom Bottom edge value.
		[[nodiscard]] constexpr rectangle_edges(Element left, Element top, Element right, Element bottom) noexcept
			: left{left}
			, top{top}
			, right{right}
			, bottom{bottom}
		{
		}

		/// @}
		/// @name Comparison operators
		/// @{

		/// Compares two sets of edges for equality.
		/// @tparam ElementR Edge value type of the righthand-side edges.
		/// @param rhs Set of edges to compare with.
		/// @return Whether the two sets of edges are equal.
		template <typename ElementR>
		[[nodiscard]] constexpr bool operator==(const rectangle_edges<ElementR>& rhs) const noexcept
		{
			return left == rhs.left && top == rhs.top && right == rhs.right && bottom == rhs.bottom;
		}

		/// @}
	};
} // namespace tr

//

/// Enables default binary IO for rectangle edges.
template <typename Element>
inline constexpr bool tr::enable_default_binary_io<tr::rectangle_edges<Element>>{true};