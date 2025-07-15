#pragma once
#include "geometry.hpp"
#include "math.hpp"

constexpr tr::halign tr::to_halign(align align) noexcept
{
	return static_cast<tr::halign>(static_cast<int>(align) % 3);
}

constexpr tr::valign tr::to_valign(align align) noexcept
{
	return static_cast<tr::valign>(static_cast<int>(align) / 3);
}

constexpr tr::align tr::operator|(const valign& valign, const halign& halign) noexcept
{
	return static_cast<align>(static_cast<int>(halign) + static_cast<int>(valign));
}

constexpr tr::align tr::operator|(const halign& halign, const valign& valign) noexcept
{
	return static_cast<align>(static_cast<int>(halign) + static_cast<int>(valign));
}

template <int S, class T>
constexpr tr::rect<S, T>::rect(glm::vec<S, T> tl, glm::vec<S, T> size) noexcept
	: tl{tl}, size{size}
{
}

template <int S, class T>
constexpr tr::rect<S, T>::rect(glm::vec<S, T> size) noexcept
	: tl{}, size{size}
{
}

template <int S, class T>
template <class T1>
constexpr tr::rect<S, T>::rect(const rect<S, T1>& rect) noexcept
	: tl{static_cast<glm::vec<S, T>>(rect.tl)}, size{static_cast<glm::vec<S, T>>(rect.size)}
{
}

template <int S, class T> template <class T1> constexpr bool tr::rect<S, T>::operator==(const rect<S, T1>& r) const noexcept
{
	return tl == r.tl && size == r.size;
}

template <int S, class T> template <class T1> constexpr bool tr::rect<S, T>::contains(glm::vec<S, T1> point) const noexcept
{
	for (int i = 0; i < S; ++i) {
		if (point[i] < tl[i] || point[i] > tl[i] + size[i]) {
			return false;
		}
	}
	return true;
}

template <class T>
constexpr tr::circle<T>::circle(glm::tvec2<T> c, T r) noexcept
	: c{c}, r{r}
{
}

template <class T>
template <class T1>
constexpr tr::circle<T>::circle(const circle<T1>& circle) noexcept
	: c{static_cast<glm::tvec2<T>>(circle.c)}, r{static_cast<T>(circle.r)}
{
}

template <class T> template <class T1> constexpr bool tr::circle<T>::operator==(const circle<T1>& r) const noexcept
{
	return c == r.c && r == r.r;
}

template <class T> template <class T1> constexpr bool tr::circle<T>::contains(glm::tvec2<T1> point) const noexcept
{
	const auto dx{point.x - c.x};
	const auto dy{point.y - c.y};
	return dx * dx + dy * dy <= r * r;
}

template <class T> inline glm::tvec2<T> tr::magth(T mag, tr::angle<T> th) noexcept
{
	return glm::tvec2<T>{mag * th.cos(), mag * th.sin()};
}

template <class T> constexpr glm::tvec2<T> tr::tl(glm::tvec2<T> pos, glm::tvec2<T> size, tr::align pos_anchor) noexcept
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

template <class T1, class T2, class T3> constexpr bool tr::collinear(glm::tvec2<T1> a, glm::tvec2<T2> b, glm::tvec2<T3> c) noexcept
{
	auto tol{std::abs(std::max<std::common_type_t<T1, T2, T3>>({a.x, a.y, b.x, b.y, c.x, c.y})) * 0.000001f};
	return std::abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) < tol;
}

template <class T1, class T2> bool tr::intersecting(const rect2<T1>& r1, const rect2<T2>& r2) noexcept
{
	return r1.contains(r2.tl) || r1.contains(r2.tl + r2.size) || r2.contains(r1.tl + glm::tvec2<T1>{r1.size.x, 0}) ||
		   r2.contains(r1.tl + glm::tvec2<T1>{0, r1.size.y});
}

template <class T> bool tr::intersecting(const circle<T>& c1, const circle<T>& c2) noexcept
{
	return glm::distance(c1.c, c2.c) <= c1.r + c2.r;
}

template <std::floating_point T> constexpr glm::tvec2<T> tr::closest_point(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept
{
	auto pp{p - a};
	auto bp{b - a};
	return a + bp * std::clamp(dot(pp, bp) / dot(bp, bp), T{0}, T{1});
}

template <std::integral T> constexpr glm::dvec2 tr::closest_point(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b) noexcept
{
	return closest_point(static_cast<glm::dvec2>(p), static_cast<glm::dvec2>(a), static_cast<glm::dvec2>(b));
}

template <std::floating_point T>
constexpr std::optional<glm::tvec2<T>> tr::segment_intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2,
															 glm::tvec2<T> b2) noexcept
{
	auto r{b2 - a2};
	glm::tvec2<T> n{b1.y - a1.y, -b1.x + a1.x};
	auto d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	if (t < 0 || t > 1) {
		return std::nullopt;
	}
	auto result{a2 + r * t};
	if ((result.x < std::min(a1.x, b1.x) || result.x > std::max(a1.x, b1.x)) ||
		(result.x - a1.x == 0 && (result.y < std::min(a1.y, b1.y) || result.y > std::max(a1.y, b1.y)))) {
		return std::nullopt;
	}
	else {
		return result;
	}
}

template <std::integral T>
constexpr std::optional<glm::dvec2> tr::segment_intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
	return segment_intersect(static_cast<glm::dvec2>(a1), static_cast<glm::dvec2>(b1), static_cast<glm::dvec2>(a2),
							 static_cast<glm::dvec2>(b2));
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::segment_intersect(glm::tvec2<T> a1, angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
	auto r{b2 - a2};
	glm::tvec2<T> n{sin(th1), -cos(th1)};
	auto d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	if (t >= 0 && t <= 1) {
		return a2 + r * t;
	}
	else {
		return std::nullopt;
	}
}

template <std::integral T>
std::optional<glm::dvec2> tr::segment_intersect(glm::tvec2<T> a1, dangle th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
	return segment_intersect(static_cast<glm::dvec2>(a1), th1, static_cast<glm::dvec2>(a2), static_cast<glm::dvec2>(b2));
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, angle<T> th1, glm::tvec2<T> a2, angle<T> th2) noexcept
{
	glm::tvec2<T> r{cos(th2), sin(th2)};
	glm::tvec2<T> n{sin(th1), -cos(th1)};
	auto d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	return a2 + r * t;
}

template <std::integral T> std::optional<glm::dvec2> tr::intersect(glm::tvec2<T> a1, dangle th1, glm::tvec2<T> a2, dangle th2) noexcept
{
	return intersect(static_cast<glm::dvec2>(a1), th1, static_cast<glm::dvec2>(a2), th2);
}

template <std::floating_point T>
constexpr std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, glm::tvec2<T> b1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
	auto r{b2 - a2};
	glm::tvec2<T> n{b1.y - a1.y, -b1.x + a1.x};
	auto d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	return a2 + r * t;
}

template <std::floating_point T>
std::optional<glm::tvec2<T>> tr::intersect(glm::tvec2<T> a1, angle<T> th1, glm::tvec2<T> a2, glm::tvec2<T> b2) noexcept
{
	auto r{b2 - a2};
	glm::tvec2<T> n{sin(th1), -cos(th1)};
	auto d{dot(r, n)};
	if (!d) {
		return std::nullopt;
	}
	auto t{dot(a1 - a2, n) / d};
	return a2 + r * t;
}

template <class T1, class T2> constexpr std::common_type_t<T1, T2> tr::cross2(glm::tvec2<T1> a, glm::tvec2<T2> b) noexcept
{
	return a.x * b.y - a.y * b.x;
}

template <tr::arithmetic T> T tr::mirror_repeat(T v, T min, T max) noexcept
{
	T shift{v - min};
	T size{max - min};
	shift = tr::mod(std::abs(shift + (shift < 0)), (2 * size));
	return min + (shift >= size ? 2 * size - shift - 1 : shift);
}

template <int S, class T>
glm::vec<S, T> tr::mirror_repeat(const glm::vec<S, T>& v, const glm::vec<S, T>& min, const glm::vec<S, T>& max) noexcept
{
	glm::vec<S, T> result;
	for (int i = 0; i < S; ++i) {
		result[i] = mirror_repeat(v[i], min[i], max[i]);
	}
	return result;
}

template <std::size_t S, class T> glm::vec<S, T> tr::mirror_repeat(const glm::vec<S, T>& v, const rect<S, T>& rect) noexcept
{
	return mirror_repeat(v, rect.tl, rect.tl + rect.size);
}

template <std::size_t S, std::floating_point T> constexpr glm::vec<S, T> tr::inverse(const glm::vec<S, T>& vec) noexcept
{
	glm::vec<S, T> result;
	for (std::size_t i = 0; i < S; ++i) {
		result[i] = static_cast<T>(1) / vec[i];
	}
	return result;
}

template <std::size_t S, std::integral T> constexpr glm::vec<S, double> tr::inverse(const glm::vec<S, T>& vec) noexcept
{
	glm::vec<S, double> result;
	for (std::size_t i = 0; i < S; ++i) {
		result[i] = 1.0 / vec[i];
	}
	return result;
}

template <class T> constexpr glm::tvec2<T> tr::matrix_operators::operator*(const glm::tvec2<T>& v, const glm::tmat4x4<T>& m) noexcept
{
	return glm::tvec4<T>{v, 0, 1} * m;
}

template <class T> constexpr glm::tvec3<T> tr::matrix_operators::operator*(const glm::tvec3<T>& v, const glm::tmat4x4<T>& m) noexcept
{
	return glm::tvec4<T>{v, 1} * m;
}

template <class T> constexpr glm::tvec2<T> tr::matrix_operators::operator*(const glm::tmat4x4<T>& m, const glm::tvec2<T>& v) noexcept
{
	return m * glm::tvec4<T>{v, 0, 1};
}

template <class T> constexpr glm::tvec3<T> tr::matrix_operators::operator*(const glm::tmat4x4<T>& m, const glm::tvec3<T>& v) noexcept
{
	return m * glm::tvec4<T>{v, 1};
}

template <class T> glm::tmat4x4<T> tr::ortho(const glm::tvec2<T>& bl) noexcept
{
	return ortho({{}, bl});
}

template <class T> glm::tmat4x4<T> tr::ortho(const tr::rect2<T>& rect) noexcept
{
	return glm::ortho(rect.tl.x, rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, rect.tl.y);
}

template <class T> glm::tmat4x4<T> tr::scale_around(const glm::tmat4x4<T>& mat, const glm::tvec2<T>& c, const glm::tvec2<T>& s) noexcept
{
	return scale_around(mat, {c, 0}, {s, 1});
}

template <class T> glm::tmat4x4<T> tr::scale_around(const glm::tmat4x4<T>& mat, const glm::tvec3<T>& c, const glm::tvec3<T>& s) noexcept
{
	return translate(scale(translate(mat, c), s), -c);
}

template <class T> glm::tmat4x4<T> tr::rotate_around(const glm::tmat4x4<T>& mat, const glm::tvec2<T>& c, const angle<T>& th) noexcept
{
	return rotate_around(mat, {c, 0}, th, Z_AXIS);
}

template <class T>
glm::tmat4x4<T> tr::rotate_around(const glm::tmat4x4<T>& mat, const glm::tvec3<T>& c, const angle<T>& th,
								  const glm::tvec3<T>& axis) noexcept
{
	return translate(rotate(translate(mat, c), th.rads(), axis), -c);
}