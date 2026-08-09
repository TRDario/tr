/// @file
/// @brief Implements rectangle.hpp.

#pragma once
#include "../rectangle.hpp"

//

/// Enables default binary IO for rectangles.
template <typename Element>
inline constexpr bool tr::enable_default_binary_io<tr::rectangle<Element>>{true};

//

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
constexpr bool tr::rectangle<Element>::operator==(const rectangle<ElementR>& rhs) const
{
	return tl.x == rhs.tl.x && tl.y == rhs.tl.y && size.x == rhs.size.x && size.y == rhs.size.y;
}

//

template <typename Element>
template <typename ElementR>
constexpr bool tr::rectangle<Element>::contains(glm::tvec2<ElementR> point) const
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

template <typename Element>
constexpr tr::rectangle_edges<Element> tr::rectangle<Element>::edges() const
{
	return {tl.x, tl.y, tl.x + size.x, tl.y + size.y};
}

//

template <typename ElementL, typename ElementR>
constexpr bool tr::intersecting(rectangle<ElementL> lhs, rectangle<ElementR> rhs)
{
	return lhs.contains(rhs.tl) || lhs.contains(rhs.tl + rhs.size) || rhs.contains(lhs.tl + glm::tvec2<ElementL>{lhs.size.x, 0}) ||
		   rhs.contains(lhs.tl + glm::tvec2<ElementL>{0, lhs.size.y});
}

template <typename Element>
constexpr std::optional<tr::rectangle<Element>> tr::intersection(rectangle<Element> lhs, rectangle<Element> rhs)
{
	if constexpr (!std::unsigned_integral<Element>) {
		if (lhs.size.x < 0) {
			lhs.tl.x += lhs.size.x;
			lhs.size.x = -lhs.size.x;
		}
		if (lhs.size.y < 0) {
			lhs.tl.y += lhs.size.y;
			lhs.size.y = -lhs.size.y;
		}
		if (rhs.size.x < 0) {
			rhs.tl.x += rhs.size.x;
			rhs.size.x = -rhs.size.x;
		}
		if (rhs.size.y < 0) {
			rhs.tl.y += rhs.size.y;
			rhs.size.y = -rhs.size.y;
		}
	}

	const glm::tvec2<Element> tl{glm::max(lhs.tl, rhs.tl)};
	const glm::tvec2<Element> br{glm::min(lhs.tl + lhs.size, rhs.tl + rhs.size)};
	return (br.x > tl.x && br.y > tl.y) ? std::make_optional<rectangle<Element>>(tl, br - tl) : std::nullopt;
}