///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements ranges.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../macro.hpp"
#include "../ranges.hpp"

////////////////////////////////////////////////////////////////// RANGES /////////////////////////////////////////////////////////////////

template <tr::borrowed_standard_layout_range Range> auto tr::range_bytes(Range&& range)
{
	return std::as_bytes(std::span{range});
}

template <tr::borrowed_mutable_standard_layout_range Range> auto tr::range_mut_bytes(Range&& range)
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

template <tr::standard_layout Object, tr::borrowed_typed_contiguous_mutable_range<std::byte> Range> Object& tr::as_mut_object(Range&& bytes)
{
	const auto span{tr::range_mut_bytes(bytes)};

	if constexpr (decltype(span)::extent != std::dynamic_extent) {
		static_assert(decltype(span)::extent == sizeof(Object), "Cannot reinterpret byte range as object due to size != sizeof(T).");
	}
	else {
		TR_ASSERT(span.size() == sizeof(Object), "Cannot reinterpret byte range as '{}' due to size ({}) != sizeof(T) ({}).",
				  type_name<Object>(), span.size(), sizeof(Object));
	}

	return *(Object*)span.data();
}

template <tr::standard_layout Object, tr::borrowed_typed_contiguous_const_range<std::byte> Range> const Object& tr::as_object(Range&& bytes)
{
	const auto span{tr::range_bytes(bytes)};

	if constexpr (decltype(span)::extent != std::dynamic_extent) {
		static_assert(decltype(span)::extent == sizeof(Object), "Cannot reinterpret byte range as object due to size != sizeof(T).");
	}
	else {
		TR_ASSERT(span.size() == sizeof(Object), "Cannot reinterpret byte range as '{}' due to size ({}) != sizeof({}) ({}).",
				  type_name<Object>(), span.size(), type_name<Object>(), sizeof(Object));
	}

	return *(const Object*)span.data();
}

template <tr::standard_layout Element, tr::borrowed_typed_contiguous_mutable_range<std::byte> Range> auto tr::as_mut_objects(Range&& bytes)
{
	const auto span{tr::range_mut_bytes(bytes)};

	if constexpr (decltype(span)::extent != std::dynamic_extent) {
		static_assert(decltype(span)::extent % sizeof(Element) == 0,
					  "Cannot reinterpret byte range as an object span due to size / sizeof(T) not being an integer.");
		return std::span<Element, decltype(span)::extent / sizeof(Element)>{(Element*)span.data()};
	}
	else {
		TR_ASSERT(span.size() % sizeof(Element) == 0,
				  "Cannot reinterpret byte range as a span of '{}' due to size ({}) / sizeof({}) ({}) not being an integer.",
				  type_name<Element>(), span.size(), type_name<Element>(), sizeof(Element));
		return std::span{(Element*)span.data(), span.size() / sizeof(Element)};
	}
}

template <tr::standard_layout Element, tr::borrowed_typed_contiguous_const_range<std::byte> Range> auto tr::as_objects(Range&& bytes)
{
	const auto span{tr::range_bytes(bytes)};

	if constexpr (decltype(span)::extent != std::dynamic_extent) {
		static_assert(decltype(span)::extent % sizeof(Element) == 0,
					  "Cannot reinterpret byte range as an object span due to size / sizeof(T) not being an integer.");
		return std::span<Element, decltype(span)::extent / sizeof(Element)>{(Element*)span.data()};
	}
	else {
		TR_ASSERT(span.size() % sizeof(Element) == 0,
				  "Cannot reinterpret byte range as a span of '{}' due to size ({}) / sizeof({}) ({}) not being an integer.",
				  type_name<Element>(), span.size(), type_name<Element>(), sizeof(Element));
		return std::span{(Element*)span.data(), span.size() / sizeof(Element)};
	}
}

//

template <std::ranges::range SearchedRange, std::ranges::forward_range WhitelistRange>
	requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<WhitelistRange>>)
constexpr std::ranges::borrowed_iterator_t<SearchedRange> tr::find_last_of(SearchedRange&& searched, WhitelistRange&& whitelist)
{
	for (auto value_it = std::ranges::rbegin(searched); value_it != std::ranges::rend(searched); ++value_it) {
		if (std::ranges::find(whitelist, *value_it) != std::ranges::end(whitelist)) {
			return std::ranges::prev(value_it.base());
		}
	}
	return std::ranges::end(searched);
}

template <std::ranges::range SearchedRange, std::ranges::forward_range BlacklistRange>
	requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<BlacklistRange>>)
constexpr std::ranges::borrowed_iterator_t<SearchedRange> tr::find_first_not_of(SearchedRange&& searched, BlacklistRange&& blacklist)
{
	for (auto value_it = std::ranges::begin(searched); value_it != std::ranges::end(searched); ++value_it) {
		if (std::ranges::find(blacklist, *value_it) == std::ranges::end(blacklist)) {
			return value_it;
		}
	}
	return std::ranges::end(searched);
}

template <std::ranges::bidirectional_range SearchedRange, std::ranges::forward_range BlacklistRange>
	requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<BlacklistRange>>)
constexpr std::ranges::borrowed_iterator_t<SearchedRange> tr::find_last_not_of(SearchedRange&& searched, BlacklistRange&& blacklist)
{
	for (auto value_it = std::ranges::rbegin(searched); value_it != std::ranges::rend(searched); ++value_it) {
		if (std::ranges::find(blacklist, *value_it) == std::ranges::end(blacklist)) {
			return std::ranges::prev(value_it.base());
		}
	}
	return std::ranges::end(searched);
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

//

template <tr::move_assignable Element> void tr::unstable_erase(std::vector<Element>& vec, typename std::vector<Element>::iterator where)
{
	*where = std::move(vec.back());
	vec.pop_back();
}