///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements rectangle.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../rectangle.hpp"

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <typename Element>
constexpr tr::rectangle<Element>::rectangle(glm::tvec2<Element> tl, glm::tvec2<Element> size)
	: tl{tl}
	, size{size}
{
}

template <typename Element>
constexpr tr::rectangle<Element>::rectangle(glm::tvec2<Element> size)
	: tl{}
	, size{size}
{
}

template <typename Element>
template <typename ElementR>
constexpr tr::rectangle<Element>::rectangle(const rectangle<ElementR>& rectangle)
	: tl{rectangle.tl}
	, size{rectangle.size}
{
}

//

template <typename Element>
template <typename ElementR>
constexpr bool tr::rectangle<Element>::operator==(const rectangle<ElementR>& r) const
{
	return tl == r.tl && size == r.size;
}

//

template <typename Element> template <typename ElementR> constexpr bool tr::rectangle<Element>::contains(glm::tvec2<ElementR> point) const
{
	for (int i = 0; i < 2; ++i) {
		if (point[i] < tl[i] || point[i] > tl[i] + size[i]) {
			return false;
		}
	}
	return true;
}

template <typename Element>
template <typename ElementR>
constexpr bool tr::rectangle<Element>::contains(const rectangle<ElementR>& rectangle) const
{
	return contains(rectangle.tl) && contains(rectangle.tl + rectangle.size);
}

//

template <typename Element> constexpr tr::rectangle_edges<Element> tr::rectangle<Element>::edges() const
{
	return {tl.x, tl.y, tl.x + size.x, tl.y + size.y};
}

//

template <typename ElementL, typename ElementR>
constexpr bool tr::intersecting(const rectangle<ElementL>& r1, const rectangle<ElementR>& r2)
{
	return r1.contains(r2.tl) || r1.contains(r2.tl + r2.size) || r2.contains(r1.tl + glm::tvec2<ElementL>{r1.size.x, 0}) ||
		   r2.contains(r1.tl + glm::tvec2<ElementL>{0, r1.size.y});
}

template <typename Element> constexpr std::optional<tr::rectangle<Element>> tr::intersection(rectangle<Element> l, rectangle<Element> r)
{
	if constexpr (!std::unsigned_integral<Element>) {
		if (l.size.x < 0) {
			l.tl.x += l.size.x;
			l.size.x = -l.size.x;
		}
		if (l.size.y < 0) {
			l.tl.y += l.size.y;
			l.size.y = -l.size.y;
		}
		if (r.size.x < 0) {
			r.tl.x += r.size.x;
			r.size.x = -r.size.x;
		}
		if (r.size.y < 0) {
			r.tl.y += r.size.y;
			r.size.y = -r.size.y;
		}
	}

	const glm::tvec2<Element> tl{glm::max(l.tl, r.tl)};
	const glm::tvec2<Element> br{glm::min(l.tl + l.size, r.tl + r.size)};
	return (br.x > tl.x && br.y > tl.y) ? std::make_optional<rectangle<Element>>(tl, br - tl) : std::nullopt;
}