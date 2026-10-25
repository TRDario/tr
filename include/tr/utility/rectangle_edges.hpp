/// @file
/// @brief Provides `tr::rectangle_edges`.

#pragma once
#include <tr/utility/arithmetic_concepts.hpp>
#include <tr/utility/default_binary_io.hpp>

//

namespace tr
{
	/// Set of rectangle edges.
	/// @tparam Rep Representation type of the edges.
	template <arithmetic Rep>
	struct rectangle_edges
	{
		/// Left edge value.
		Rep left{0};

		/// Top edge value.
		Rep top{0};

		/// Right edge value.
		Rep right{0};

		/// Bottom edge value.
		Rep bottom{0};

		/// @name Constructors and destructors
		/// @{

		/// Constructs a set of edges with value 0.
		[[nodiscard]] constexpr rectangle_edges() noexcept = default;

		/// Constructs a set of edges with the same value.
		/// @param left Uniform edge value.
		[[nodiscard]] constexpr rectangle_edges(Rep left) noexcept
			: top{left}
			, right{left}
			, bottom{left}
			, left{left}
		{
		}

		/// Constructs a set of edges with top = bottom, left = right.
		/// @param left Left and right edge value.
		/// @param top Top and bottom edge value.
		[[nodiscard]] constexpr rectangle_edges(Rep left, Rep top) noexcept
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
		[[nodiscard]] constexpr rectangle_edges(Rep left, Rep top, Rep right) noexcept
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
		[[nodiscard]] constexpr rectangle_edges(Rep left, Rep top, Rep right, Rep bottom) noexcept
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
template <typename Rep>
inline constexpr bool tr::enable_default_binary_io<tr::rectangle_edges<Rep>>{true};