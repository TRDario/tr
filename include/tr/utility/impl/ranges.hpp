///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements ranges.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../macro.hpp"
#include "../ranges.hpp"

////////////////////////////////////////////////////////////////// RANGES /////////////////////////////////////////////////////////////////

template <tr::standard_layout_range Range> auto tr::range_bytes(const Range& range)
{
	return std::as_bytes(std::span{range});
}

template <tr::standard_layout_range Range> auto tr::range_mut_bytes(Range& range)
{
	return std::as_writable_bytes(std::span{range});
}

template <tr::standard_layout Object> std::span<const std::byte, sizeof(Object)> tr::as_bytes(const Object& object)
{
	return std::as_bytes(std::span<const Object, 1>{std::addressof(object), 1});
}

template <tr::standard_layout Object> std::span<std::byte, sizeof(Object)> tr::as_mut_bytes(Object& object)
{
	return std::as_writable_bytes(std::span<Object, 1>{std::addressof(object), 1});
}

//

template <tr::standard_layout Object, tr::usize Size> Object& tr::as_mut_object(std::span<std::byte, Size> bytes)
{
	if constexpr (Size != std::dynamic_extent) {
		static_assert(Size == sizeof(Object), "Cannot reinterpret byte span as object due to size != sizeof(T).");
	}
	else {
		TR_ASSERT(Size == sizeof(Object), "Cannot reinterpret byte span as object due to size != sizeof(T).");
	}

	return *(Object*)bytes.data();
}

template <tr::standard_layout Object, tr::usize Size> const Object& tr::as_object(std::span<const std::byte, Size> bytes)
{
	if constexpr (Size != std::dynamic_extent) {
		static_assert(Size == sizeof(Object), "Cannot reinterpret byte span as object due to size != sizeof(T).");
	}
	else {
		TR_ASSERT(Size == sizeof(Object), "Cannot reinterpret byte span as object due to size != sizeof(T).");
	}

	return *(const Object*)bytes.data();
}

template <tr::standard_layout Element, tr::usize Size> auto tr::as_mut_objects(std::span<std::byte, Size> bytes)
{
	if constexpr (Size != std::dynamic_extent) {
		static_assert(Size % sizeof(Element) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span<Element, Size / sizeof(Element)>{(Element*)bytes.data()};
	}
	else {
		TR_ASSERT(bytes.size() % sizeof(Element) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span{(Element*)bytes.data(), bytes.size() / sizeof(Element)};
	}
}

template <tr::standard_layout Element, tr::usize Size> auto tr::as_objects(std::span<const std::byte, Size> bytes)
{
	if constexpr (Size != std::dynamic_extent) {
		static_assert(Size % sizeof(Element) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span<const Element, Size / sizeof(Element)>{(const Element*)bytes.data()};
	}
	else {
		TR_ASSERT(bytes.size() % sizeof(Element) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span{(const Element*)bytes.data(), bytes.size() / sizeof(Element)};
	}
}

//

template <std::ranges::range Range, typename T, std::invocable<T, std::ranges::range_value_t<Range>> BinaryOp>
T tr::fold_left(Range&& range, T initial_value, BinaryOp&& pred)
{
	return std::accumulate(std::ranges::begin(range), std::ranges::end(range), initial_value, std::forward<BinaryOp>(pred));
}

template <std::ranges::range Range, typename T> T tr::sum(Range&& range, T initial_value)
{
	return std::accumulate(std::ranges::begin(range), std::ranges::end(range), initial_value);
}

//

template <typename Range> constexpr auto tr::project(auto Range::* ptr)
{
	return std::views::transform([=](auto&& val) -> auto&& { return val.*ptr; });
}

template <std::ranges::range Range> constexpr auto tr::project(Range&& range, auto std::ranges::range_value_t<Range>::* ptr)
{
	return std::views::transform(std::forward<Range&&>(range), [=](auto&& val) -> auto&& { return val.*ptr; });
}