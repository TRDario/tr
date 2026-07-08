///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements alignment.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../alignment.hpp"
#include "../macro.hpp"

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

constexpr tr::halign tr::to_halign(align align)
{
	return static_cast<halign>(to_underlying(align) % 3);
}

constexpr tr::valign tr::to_valign(align align)
{
	return static_cast<valign>(to_underlying(align) - to_underlying(align) % 3);
}

constexpr tr::align tr::operator|(const valign& valign, const halign& halign)
{
	return static_cast<align>(to_underlying(halign) + to_underlying(valign));
}

constexpr tr::align tr::operator|(const halign& halign, const valign& valign)
{
	return static_cast<align>(to_underlying(halign) + to_underlying(valign));
}

//

template <typename Element> constexpr glm::tvec2<Element> tr::tl(glm::tvec2<Element> pos, glm::tvec2<Element> size, tr::align pos_anchor)
{
	switch (pos_anchor) {
	case align::tl:
		return pos;
	case align::tc:
		return {pos.x - size.x / 2, pos.y};
	case align::tr:
		return {pos.x - size.x, pos.y};
	case align::cl:
		return {pos.x, pos.y - size.y / 2};
	case align::cc:
		return pos - size / Element{2};
	case align::cr:
		return {pos.x - size.x, pos.y - size.y / 2};
	case align::bl:
		return {pos.x, pos.y - size.y};
	case align::bc:
		return {pos.x - size.x / 2, pos.y - size.y};
	case align::br:
		return pos - size;
	default:
		TR_UNREACHABLE;
	}
}