///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr and templated parts of geometry.hpp.                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../geometry.hpp"
#include "../macro.hpp"
#include "../math.hpp"

//////////////////////////////////////////////////////////////// ALIGNMENT ////////////////////////////////////////////////////////////////

constexpr tr::halign tr::to_halign(align align)
{
	return halign(int(align) % 3);
}

constexpr tr::valign tr::to_valign(align align)
{
	return valign(int(align) / 3);
}

constexpr tr::align tr::operator|(const valign& valign, const halign& halign)
{
	return align(int(halign) + int(valign));
}

constexpr tr::align tr::operator|(const halign& halign, const valign& valign)
{
	return align(int(halign) + int(valign));
}

/////////////////////////////////////////////////////////////////// RECT //////////////////////////////////////////////////////////////////

template <int Dimensions, typename Element>
constexpr tr::rect<Dimensions, Element>::rect(glm::vec<Dimensions, Element> tl, glm::vec<Dimensions, Element> size)
	: tl{tl}, size{size}
{
}

template <int Dimensions, typename Element>
constexpr tr::rect<Dimensions, Element>::rect(glm::vec<Dimensions, Element> size)
	: tl{}, size{size}
{
}

template <int Dimensions, typename Element>
template <typename ElementR>
constexpr tr::rect<Dimensions, Element>::rect(const rect<Dimensions, ElementR>& rect)
	: tl{rect.tl}, size{rect.size}
{
}

template <int Dimensions, typename Element>
template <typename ElementR>
constexpr bool tr::rect<Dimensions, Element>::operator==(const rect<Dimensions, ElementR>& r) const
{
	return tl == r.tl && size == r.size;
}

template <int Dimensions, typename Element>
template <typename ElementR>
constexpr bool tr::rect<Dimensions, Element>::contains(glm::vec<Dimensions, ElementR> point) const
{
	for (int i = 0; i < Dimensions; ++i) {
		if (point[i] < tl[i] || point[i] > tl[i] + size[i]) {
			return false;
		}
	}
	return true;
}

template <int Dimensions, typename Element>
template <typename ElementR>
constexpr bool tr::rect<Dimensions, Element>::contains(const rect<Dimensions, ElementR>& rect) const
{
	return contains(rect.tl) && contains(rect.tl + rect.size);
}

template <typename ElementL, typename ElementR> constexpr bool tr::intersecting(const rect2<ElementL>& r1, const rect2<ElementR>& r2)
{
	return r1.contains(r2.tl) || r1.contains(r2.tl + r2.size) || r2.contains(r1.tl + glm::tvec2<ElementL>{r1.size.x, 0}) ||
		   r2.contains(r1.tl + glm::tvec2<ElementL>{0, r1.size.y});
}

///////////////////////////////////////////////////////////// VECTOR FUNCTIONS ////////////////////////////////////////////////////////////

template <int Dimensions, typename Element> constexpr Element tr::length2(glm::vec<Dimensions, Element> v)
{
	if constexpr (Dimensions == 1) {
		return sqr(v.x);
	}
	else if constexpr (Dimensions == 2) {
		return sqr(v.x) + sqr(v.y);
	}
	else if constexpr (Dimensions == 3) {
		return sqr(v.x) + sqr(v.y) + sqr(v.z);
	}
	else if constexpr (Dimensions == 4) {
		return sqr(v.x) + sqr(v.y) + sqr(v.z) + sqr(v.w);
	}
	else {
		TR_UNREACHABLE;
	}
}

template <int Dimensions, typename ElementL, typename ElementR>
constexpr std::common_type_t<ElementL, ElementR> tr::dist2(glm::vec<Dimensions, ElementL> a, glm::vec<Dimensions, ElementR> b)
{
	return length2(glm::vec<Dimensions, std::common_type_t<ElementL, ElementR>>{b} -
				   glm::vec<Dimensions, std::common_type_t<ElementL, ElementR>>{a});
}

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
		return pos - size / Element(2);
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

template <int Dimensions, tr::arithmetic Element>
constexpr glm::vec<Dimensions, float> tr::inverse(const glm::vec<Dimensions, Element>& vec)
{
	glm::vec<Dimensions, float> result;
	for (int i = 0; i < Dimensions; ++i) {
		result[i] = float(1.0f / vec[i]);
	}
	return result;
}

template <typename ElementL, typename ElementR>
constexpr std::common_type_t<ElementL, ElementR> tr::cross(glm::tvec2<ElementL> a, glm::tvec2<ElementR> b)
{
	return a.x * b.y - a.y * b.x;
}

template <tr::arithmetic Number> constexpr Number tr::mirror_repeat(Number v, Number min, Number max)
{
	Number shift{v - min};
	Number size{max - min};
	shift = tr::mod(tr::abs(shift + (shift < 0)), (2 * size));
	return min + (shift >= size ? 2 * size - shift - 1 : shift);
}

template <int Dimensions, tr::arithmetic Element>
constexpr glm::vec<Dimensions, Element> tr::mirror_repeat(glm::vec<Dimensions, Element> v, glm::vec<Dimensions, Element> min,
														  glm::vec<Dimensions, Element> max)
{
	glm::vec<Dimensions, Element> result;
	for (int i = 0; i < Dimensions; ++i) {
		result[i] = mirror_repeat(v[i], min[i], max[i]);
	}
	return result;
}

template <int Dimensions, tr::arithmetic Element>
constexpr glm::vec<Dimensions, Element> tr::mirror_repeat(glm::vec<Dimensions, Element> v, const rect<Dimensions, Element>& rect)
{
	return mirror_repeat(v, rect.tl, rect.tl + rect.size);
}