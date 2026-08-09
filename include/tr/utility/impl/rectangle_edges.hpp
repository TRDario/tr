/// @file
/// @brief Implements rectangle_edges.hpp.

#pragma once
#include "../default_binary_io.hpp"
#include "../rectangle_edges.hpp"

//

/// Enables default binary IO for rectangle edges.
template <typename Element>
inline constexpr bool tr::enable_default_binary_io<tr::rectangle_edges<Element>>{true};

//

template <typename Element>
constexpr tr::rectangle_edges<Element>::rectangle_edges(Element left)
	: top{left}
	, right{left}
	, bottom{left}
	, left{left}
{
}

template <typename Element>
constexpr tr::rectangle_edges<Element>::rectangle_edges(Element left, Element top)
	: left{left}
	, top{top}
	, right{left}
	, bottom{top}
{
}

template <typename Element>
constexpr tr::rectangle_edges<Element>::rectangle_edges(Element left, Element top, Element right)
	: left{left}
	, top{top}
	, right{right}
	, bottom{top}
{
}

template <typename Element>
constexpr tr::rectangle_edges<Element>::rectangle_edges(Element left, Element top, Element right, Element bottom)
	: left{left}
	, top{top}
	, right{right}
	, bottom{bottom}
{
}

//

template <typename Element>
template <typename ElementR>
constexpr bool tr::rectangle_edges<Element>::operator==(const rectangle_edges<ElementR>& rhs) const
{
	return left == rhs.left && top == rhs.top && right == rhs.right && bottom == rhs.bottom;
}