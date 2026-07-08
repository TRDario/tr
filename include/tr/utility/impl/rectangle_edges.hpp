///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements rectangle_edges.hpp.                                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../rectangle_edges.hpp"

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

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
constexpr tr::rectangle_edges<Element>::rectangle_edges(Element top, Element right, Element bottom, Element left)
	: left{left}
	, top{top}
	, right{right}
	, bottom{bottom}
{
}

//

template <typename Element>
template <typename ElementR>
constexpr bool tr::rectangle_edges<Element>::operator==(const rectangle_edges<ElementR>& r) const
{
	return left == r.left && top == r.top && right == r.right && bottom == r.bottom;
}