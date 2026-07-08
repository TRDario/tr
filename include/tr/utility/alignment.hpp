///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides alignment-related datatypes and functions.                                                                                   //
//                                                                                                                                       //
// Horizontal, vertical, and 2D alignment enumerators are provided. 2D alignments can be composed from and decomposed into components:   //
//     - tr::align align{tr::valign::top | tr::halign::left} -> tr::align::tl                                                            //
//     - tr::to_halign(tr::align::cr) -> tr::halign::right                                                                               //
//     - tr::to_valign(tr::align::cr) -> tr::valign::center                                                                              //
//                                                                                                                                       //
// tl::tl finds the top-left corner of an axis-aligned rectangle described with a center, size, and alignment:                           //
//     - tr::tl({500, 500}, {200, 200}, tr::align::cc) -> {400, 400} (top-left corner of a rectangle described with anchor+size+align)   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "enum.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Horizontal alignment.
	enum class halign : u8 {
		left,   // Left horizontal alignment.
		center, // Center horizontal alignment.
		right   // Right horizontal alignment.
	};
	// Vertical alignment.
	enum class valign : u8 {
		top,        // Top vertical alignment.
		center = 3, // Center vertical alignment.
		bottom = 6  // Bottom vertical alignment.
	};
	// 2D alignment.
	enum class align : u8 {
		tl = to_underlying(valign::top) + to_underlying(halign::left),      // Top-left alignment.
		tc = to_underlying(valign::top) + to_underlying(halign::center),    // Top-center alignment.
		tr = to_underlying(valign::top) + to_underlying(halign::right),     // Top-right alignment.
		cl = to_underlying(valign::center) + to_underlying(halign::left),   // Center-left alignment.
		cc = to_underlying(valign::center) + to_underlying(halign::center), // Center alignment.
		cr = to_underlying(valign::center) + to_underlying(halign::right),  // Center-right alignment.
		bl = to_underlying(valign::bottom) + to_underlying(halign::left),   // Bottom-left alignment.
		bc = to_underlying(valign::bottom) + to_underlying(halign::center), // Bottom-center alignment.
		br = to_underlying(valign::bottom) + to_underlying(halign::right)   // Bottom-right alignment.
	};
	// Converts a 2D alignment to a horizontal alignment.
	constexpr halign to_halign(align align);
	// Converts a 2D alignment to a vertical alignment.
	constexpr valign to_valign(align align);
	// Combines a horizontal and vertical alignment into a 2D alignment.
	constexpr align operator|(const valign& valign, const halign& halign);
	// Combines a horizontal and vertical alignment into a 2D alignment.
	constexpr align operator|(const halign& halign, const valign& valign);

	// Computes the top-left corner of a rectangle given a position, size, and anchor point.
	template <typename Element> constexpr glm::tvec2<Element> tl(glm::tvec2<Element> pos, glm::tvec2<Element> size, align pos_anchor);
} // namespace tr

#include "impl/alignment.hpp" // IWYU pragma: export