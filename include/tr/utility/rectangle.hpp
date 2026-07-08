///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an axis-aligned rectangle datatype and related functions.                                                                    //
//                                                                                                                                       //
// Rectangles are defined by their top-left corner and their size:                                                                       //
//     - tr::rectangle<float> rect{} -> {.tl{0, 0}, .size{0, 0}}                                                                         //
//     - tr::rectangle<float> rect{glm::vec2{500, 500}} -> {.tl{0, 0}, .size{500, 500}}                                                  //
//     - tr::rectangle<float> rect{{100, 100}, {100, 100}} -> {.tl{100, 100}, .size{100, 100}}                                           //
//                                                                                                                                       //
// Rectangles can be checked for containing a point or another rectangle:                                                                //
//     - tr::rectangle<float>{{100, 100}, {100, 100}}.contains({150, 150}) -> true                                                       //
//     - tr::rectangle<float>{{100, 100}, {100, 100}}.contains({{125, 125}, {50, 50}}) -> true                                           //
//                                                                                                                                       //
// The set of a rectangle's edges may be obtained:                                                                                       //
//     - tr::rectangle<float>{{100, 100}, {100, 100}}.edges() -> {.left{100}, .top{100}, .right{200}, .bottom{200}}                      //
//                                                                                                                                       //
// Rectangles can be checked for intersection:                                                                                           //
//     - tr::intersecting(tr::rectangle<float>{{100, 100}, {100, 100}}, tr::rectangle<float>{{50, 125}, {400, 50}})                      //
//       -> true                                                                                                                         //
//     - tr::intersection(tr::rectangle<float>{{100, 100}, {100, 100}}, tr::rectangle<float>{{50, 125}, {400, 50}})                      //
//       -> tr::frec2{{100, 125}, {100, 50}}                                                                                             //
//                                                                                                                                       //
// Rectangles are binary readable and writable.                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "rectangle_edges.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Rectangle object.
	template <typename Element> struct rectangle {
		// The offset of the top-left corner of the rectangle.
		glm::tvec2<Element> tl;
		// The size of the rectangle.
		glm::tvec2<Element> size;

		// Default-constructs a rectangle.
		constexpr rectangle() = default;
		// Constructs a rectangle from a top-left corner and size.
		constexpr rectangle(glm::tvec2<Element> tl, glm::tvec2<Element> size);
		// Constructs a rectangle with the top-left corner at the origin.
		constexpr rectangle(glm::tvec2<Element> size);
		// Copy-constructs a rectangle.
		template <typename ElementR> constexpr rectangle(const rectangle<ElementR>& rectangle);

		// Compares two rectangles for equality.
		template <typename ElementR> constexpr bool operator==(const rectangle<ElementR>&) const;

		// Determines whether a point is contained inside the rectangle.
		template <typename ElementR = Element> constexpr bool contains(glm::tvec2<ElementR> point) const;
		// Determines whether another rectangle is contained entirely inside the rectangle.
		template <typename ElementR = Element> constexpr bool contains(const rectangle<ElementR>& rectangle) const;

		// Gets the edges of the rectangle.
		constexpr rectangle_edges<Element> edges() const;
	};

	// Determines if two rectangles intersect.
	template <typename ElementL, typename ElementR>
	constexpr bool intersecting(const rectangle<ElementL>& r1, const rectangle<ElementR>& r2);
	// Computes the intersection of two rectangles.
	template <typename Element> constexpr std::optional<rectangle<Element>> intersection(rectangle<Element> l, rectangle<Element> r);
} // namespace tr

// Enables default binary IO for rectangles.
template <typename Element> inline constexpr bool tr::enable_default_binary_io<tr::rectangle<Element>>{true};

#include "impl/rectangle.hpp" // IWYU pragma: export