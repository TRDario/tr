/// @file
/// @brief Provides `tr::rectangle_edges`.

/// @ingroup math
/// @defgroup rectangle_edges Rectangle edges
/// @brief Contains `tr::rectangle_edges`.

#pragma once
#include <tr/utility/arithmetic_concepts.hpp>
#include <tr/utility/integer.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/specialization_of.hpp>

//

namespace tr
{
	/// @ingroup rectangle_edges
	/// @brief Set of rectangle edges.
	/// @tparam Rep Rectangle edges representation type.
	template <arithmetic Rep>
	struct rectangle_edges
	{
		/// @brief Left edge value.
		Rep left{0};

		/// @brief Top edge value.
		Rep top{0};

		/// @brief Right edge value.
		Rep right{0};

		/// @brief Bottom edge value.
		Rep bottom{0};

		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a set of rectangle edges with value 0.
		[[nodiscard]] constexpr rectangle_edges() noexcept = default;

		/// @brief Constructs a set of rectangle edges with the same value.
		/// @param left Uniform edge value.
		[[nodiscard]] constexpr rectangle_edges(Rep left) noexcept
			: left{left}
			, top{left}
			, right{left}
			, bottom{left}
		{
		}

		/// @brief Constructs a set of rectangle edges with top = bottom, left = right.
		/// @param left Left and right edge value.
		/// @param top Top and bottom edge value.
		[[nodiscard]] constexpr rectangle_edges(Rep left, Rep top) noexcept
			: left{left}
			, top{top}
			, right{left}
			, bottom{top}
		{
		}

		/// @brief Constructs a set of rectangle edges with top = bottom.
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

		/// @brief Constructs a set of rectangle edges.
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

		/// @brief Constructs a set of rectangle edges by copying from another set of rectangle edges of a different representation type.
		/// @tparam RhsRep Right-hand side rectangle edges representation type.
		/// @param rhs Set of rectangle edges to copy from.
		template <arithmetic RhsRep>
		[[nodiscard]] constexpr rectangle_edges(const rectangle_edges<RhsRep>& rhs) noexcept
			: left{static_cast<Rep>(rhs.left)}
			, top{static_cast<Rep>(rhs.top)}
			, right{static_cast<Rep>(rhs.right)}
			, bottom{static_cast<Rep>(rhs.bottom)}
		{
		}

		/// @}
	};

	//

	/// @ingroup rectangle_edges
	/// @brief Checks two sets of edges for equality.
	/// @tparam LhsRep, RhsRep Representation types of the edges.
	/// @param lhs, rhs Rectangle edges to compare.
	/// @return `true` if the two sets of edges are equal, `false` otherwise.
	template <typename LhsRep, typename RhsRep>
	[[nodiscard]] constexpr bool operator==(const rectangle_edges<LhsRep>& lhs, const rectangle_edges<RhsRep>& rhs) noexcept
	{
		return lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom;
	}
} // namespace tr

//

/// @ingroup rectangle_edges
/// @brief Specialization of `std::tuple_size` for `tr::rectangle_edges`.
template <typename Rep>
struct std::tuple_size<tr::rectangle_edges<Rep>> : std::integral_constant<std::size_t, 4>
{
};

/// @ingroup rectangle_edges
/// @brief Specialization of `std::tuple_element` for `tr::rectangle_edges`.
/// @tparam Index Index of the element.
template <std::size_t Index, typename Rep>
struct std::tuple_element<Index, tr::rectangle_edges<Rep>> : std::tuple_element<Index, std::tuple<Rep, Rep, Rep, Rep>>
{
};

namespace tr
{
	/// @ingroup rectangle_edges
	/// @brief Returns an element from a set of rectangle edges.
	/// @tparam Index Index of the element to return.
	/// @tparam RectangleEdges Potentially cvref-qualified instance of `tr::rectangle_edges`.
	/// @param edges Edges to return an element from.
	/// @return Element from `edges`.
	template <usize Index, typename RectangleEdges>
		requires cvref_specialization_of<RectangleEdges, rectangle_edges>
	[[nodiscard]] decltype(auto) get(RectangleEdges&& edges) noexcept
	{
		if constexpr (Index == 0) {
			return std::forward_like<RectangleEdges>(edges.left);
		}
		else if constexpr (Index == 1) {
			return std::forward_like<RectangleEdges>(edges.top);
		}
		else if constexpr (Index == 2) {
			return std::forward_like<RectangleEdges>(edges.right);
		}
		else if constexpr (Index == 3) {
			return std::forward_like<RectangleEdges>(edges.bottom);
		}
		else {
			TR_UNREACHABLE;
		}
	}
} // namespace tr