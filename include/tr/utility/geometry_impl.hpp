#pragma once
#include "geometry.hpp"
#include "math.hpp"

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

template <int S, class T>
constexpr tr::rect<S, T>::rect(glm::vec<S, T> tl, glm::vec<S, T> size)
	: tl{tl}, size{size}
{
}

template <int S, class T>
constexpr tr::rect<S, T>::rect(glm::vec<S, T> size)
	: tl{}, size{size}
{
}

template <int S, class T>
template <class T1>
constexpr tr::rect<S, T>::rect(const rect<S, T1>& rect)
	: tl{rect.tl}, size{rect.size}
{
}

template <int S, class T> template <class T1> constexpr bool tr::rect<S, T>::operator==(const rect<S, T1>& r) const
{
	return tl == r.tl && size == r.size;
}

template <int S, class T> template <class T1> constexpr bool tr::rect<S, T>::contains(glm::vec<S, T1> point) const
{
	for (int i = 0; i < S; ++i) {
		if (point[i] < tl[i] || point[i] > tl[i] + size[i]) {
			return false;
		}
	}
	return true;
}

template <class T1, class T2> constexpr bool tr::intersecting(const rect2<T1>& r1, const rect2<T2>& r2)
{
	return r1.contains(r2.tl) || r1.contains(r2.tl + r2.size) || r2.contains(r1.tl + glm::tvec2<T1>{r1.size.x, 0}) ||
		   r2.contains(r1.tl + glm::tvec2<T1>{0, r1.size.y});
}

template <int S, class T> constexpr T tr::length2(glm::vec<S, T> v)
{
	if constexpr (S == 1) {
		return sqr(v.x);
	}
	else if constexpr (S == 2) {
		return sqr(v.x) + sqr(v.y);
	}
	else if constexpr (S == 3) {
		return sqr(v.x) + sqr(v.y) + sqr(v.z);
	}
	else if constexpr (S == 4) {
		return sqr(v.x) + sqr(v.y) + sqr(v.z) + sqr(v.w);
	}
	else {
		unreachable();
	}
}

template <int S, class T1, class T2> constexpr std::common_type_t<T1, T2> tr::dist2(glm::vec<S, T1> a, glm::vec<S, T2> b)
{
	return length2(glm::vec<S, std::common_type_t<T1, T2>>{b} - glm::vec<S, std::common_type_t<T1, T2>>{a});
}

template <class T> constexpr glm::tvec2<T> tr::tl(glm::tvec2<T> pos, glm::tvec2<T> size, tr::align pos_anchor)
{
	switch (pos_anchor) {
	case align::TOP_LEFT:
		return pos;
	case align::TOP_CENTER:
		return {pos.x - size.x / 2, pos.y};
	case align::TOP_RIGHT:
		return {pos.x - size.x, pos.y};
	case align::CENTER_LEFT:
		return {pos.x, pos.y - size.y / 2};
	case align::CENTER:
		return pos - size / T(2);
	case align::CENTER_RIGHT:
		return {pos.x - size.x, pos.y - size.y / 2};
	case align::BOTTOM_LEFT:
		return {pos.x, pos.y - size.y};
	case align::BOTTOM_CENTER:
		return {pos.x - size.x / 2, pos.y - size.y};
	case align::BOTTOM_RIGHT:
		return pos - size;
	}
}

template <int S, tr::arithmetic T> constexpr glm::vec<S, float> tr::inverse(const glm::vec<S, T>& vec)
{
	glm::vec<S, float> result;
	for (int i = 0; i < S; ++i) {
		result[i] = float(1.0f / vec[i]);
	}
	return result;
}

template <class T1, class T2> constexpr std::common_type_t<T1, T2> tr::cross(glm::tvec2<T1> a, glm::tvec2<T2> b)
{
	return a.x * b.y - a.y * b.x;
}

template <tr::arithmetic T> constexpr T tr::mirror_repeat(T v, T min, T max)
{
	T shift{v - min};
	T size{max - min};
	shift = tr::mod(tr::abs(shift + (shift < 0)), (2 * size));
	return min + (shift >= size ? 2 * size - shift - 1 : shift);
}

template <int S, tr::arithmetic T> constexpr glm::vec<S, T> tr::mirror_repeat(glm::vec<S, T> v, glm::vec<S, T> min, glm::vec<S, T> max)
{
	glm::vec<S, T> result;
	for (int i = 0; i < S; ++i) {
		result[i] = mirror_repeat(v[i], min[i], max[i]);
	}
	return result;
}

template <int S, tr::arithmetic T> constexpr glm::vec<S, T> tr::mirror_repeat(glm::vec<S, T> v, const rect<S, T>& rect)
{
	return mirror_repeat(v, rect.tl, rect.tl + rect.size);
}