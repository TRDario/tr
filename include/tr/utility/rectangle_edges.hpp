///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a rectangle edges datatype.                                                                                                  //
//                                                                                                                                       //
// A set of 2D edges is provided as tr::rectangle_edges<T> (a few shorthands are also provided in the form tr::Trectangle_edges).        //
// Edges are binary readable and writable. Edges are constructed like in CSS, but in the order (left, top, right, bottom):               //
//     - tr::frectangle_edges{} -> {.left = 0, .top = 0, .right = 0, .bottom = 0}                                                        //
//     - tr::frectangle_edges{50} -> {.left = 50, .top = 50, .right = 50, .bottom = 50}                                                  //
//     - tr::frectangle_edges{50, 100} -> {.left = 50, .top = 100, .right = 50, .bottom = 100}                                           //
//     - tr::frectangle_edges{50, 100, 150} -> {.left = 50, .top = 100, .right = 150, .bottom = 100}                                     //
//     - tr::frectangle_edges{50, 100, 150, 200} -> {.left = 50, .top = 100, .right = 150, .bottom = 200}                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "default_binary_io.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Set of rectangle edges.
	template <typename Element> struct rectangle_edges {
		// Left edge value.
		Element left{0};
		// Top edge value.
		Element top{0};
		// Right edge value.
		Element right{0};
		// Bottom edge value.
		Element bottom{0};

		// Constructs a set of edges with value 0.
		constexpr rectangle_edges() = default;
		// Constructs a set of edges with the same value.
		constexpr rectangle_edges(Element left);
		// Constructs a set of edges with top = bottom, left = right.
		constexpr rectangle_edges(Element left, Element top);
		// Constructs a set of edges width left = right.
		constexpr rectangle_edges(Element left, Element top, Element right);
		// Constructs a set of edges width left = right.
		constexpr rectangle_edges(Element left, Element top, Element right, Element bottom);

		template <typename ElementR> constexpr bool operator==(const rectangle_edges<ElementR>&) const;
	};
	// Shorthard for int rectangle edges.
	using irectangle_edges = rectangle_edges<int>;
	// Shorthand for float rectangle edges.
	using frectangle_edges = rectangle_edges<float>;
} // namespace tr

// Enables default binary IO for rectangle edges.
template <typename Element> inline constexpr bool tr::enable_default_binary_io<tr::rectangle_edges<Element>>{true};

#include "impl/rectangle_edges.hpp" // IWYU pragma: export