/// @file
/// @brief Provides miscellaneous range utilities.

#pragma once
#include "concepts.hpp"

//

namespace tr
{
	/// @name Ranges
	/// @{

	/// Reinterprets a span of one type to a span of another.
	/// @tparam To Element type to reinterpret as.
	/// @tparam From Source element type.
	/// @tparam Extent Extent of the source span.
	/// @param from Span to reinterpret.
	/// @return Span reinterpreted as a span of another type.
	template <standard_layout To, standard_layout From, usize Extent>
	auto reinterpret_span(std::span<From, Extent> from);

	//

	/// Gets a view of a contiguous range as a span of immutable bytes.
	/// @tparam Range Contiguous range of standard layout elements.
	/// @param range Contiguous range to reinterpret as bytes.
	/// @return `range` reinterpreted as a range of bytes.
	template <borrowed_standard_layout_range Range>
	auto range_bytes(Range&& range);

	/// Gets a view of an object as a span of immutable bytes.
	/// @tparam Standard layout object type.
	/// @param object Object to reinterpret as bytes.
	/// @return `object` reinterpreted as a range of bytes.
	template <standard_layout Object>
	std::span<const std::byte, sizeof(Object)> as_bytes(const Object& object);

	/// Gets a view of a contiguous range as a span of mutable bytes.
	/// @tparam Range Contiguous range of standard layout elements.
	/// @param range Contiguous range to reinterpret as bytes.
	/// @return `range` reinterpreted as a range of mutable bytes.
	template <borrowed_mutable_standard_layout_range Range>
	auto range_mut_bytes(Range&& range);

	/// Gets a view of an object as a span of mutable bytes.
	/// @tparam Standard layout object type.
	/// @param object Object to reinterpret as bytes.
	/// @return `object` reinterpreted as a range of mutable bytes.
	template <standard_layout Object>
	std::span<std::byte, sizeof(Object)> as_mut_bytes(Object& object);

	//

	/// Reinterprets a range of mutable bytes as an object and returns a reference to it.
	/// @tparam Object Object type to reinterpret the byte range as.
	/// @tparam Range Contiguous range of mutable bytes.
	/// @param bytes Range to reinterpret.
	/// @return Reference to the reinterpreted object.
	template <standard_layout Object, borrowed_typed_contiguous_mutable_range<std::byte> Range>
	Object& as_mut_object(Range&& bytes);

	/// Reinterprets a range of immutable bytes as an object and returns a reference to it.
	/// @tparam Object Object type to reinterpret the byte range as.
	/// @tparam Range Contiguous range of bytes.
	/// @param bytes Range to reinterpret.
	/// @return Reference to the reinterpreted object.
	template <standard_layout Object, borrowed_typed_contiguous_const_range<std::byte> Range>
	const Object& as_object(Range&& bytes);

	/// Reinterprets a range of mutable bytes as a span of objects.
	/// @tparam Element Element type to reinterpret the byte range as.
	/// @tparam Range Contiguous range of mutable bytes.
	/// @param bytes Range to reinterpret.
	/// @return Span of `Element`.
	template <standard_layout Element, borrowed_typed_contiguous_mutable_range<std::byte> Range>
	auto as_mut_objects(Range&& bytes);

	/// Reinterprets a range of immutable bytes as a span of objects.
	/// @tparam Element Element type to reinterpret the byte range as.
	/// @tparam Range Contiguous range of bytes.
	/// @param bytes Range to reinterpret.
	/// @return Span of `const Element`.
	template <standard_layout Element, borrowed_typed_contiguous_const_range<std::byte> Range>
	auto as_objects(Range&& bytes);

	//

	/// Returns an iterator to the last element in `searched` that matches one of the elements in `whitelist`.
	/// @tparam SearchedRange Type of the searched range.
	/// @tparam WhitelistRange Type of the whitelist range.
	/// @param searched Range to search.
	/// @param whitelist List of elements to search for.
	/// @return Iterator to the first element in `searched` that matches one in `whitelist`.
	template <std::ranges::range SearchedRange, std::ranges::forward_range WhitelistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<WhitelistRange>>)
	constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_last_of(SearchedRange&& searched, WhitelistRange&& whitelist);

	/// Returns an iterator to the first element in `searched` that doesn't match one of the elements in `blacklist`.
	/// @tparam SearchedRange Type of the searched range.
	/// @tparam BlacklistRange Type of the blacklist range.
	/// @param searched Range to search.
	/// @param blacklist List of elements to not search for.
	/// @return Iterator to the first element in `searched` that doesn't match one of the elements in `blacklist`.
	template <std::ranges::range SearchedRange, std::ranges::forward_range BlacklistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<BlacklistRange>>)
	constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_first_not_of(SearchedRange&& searched, BlacklistRange&& blacklist);

	/// Returns an iterator to the last element in `searched` that doesn't match one of the elements in `blacklist`.
	/// @tparam SearchedRange Type of the searched range.
	/// @tparam BlacklistRange Type of the blacklist range.
	/// @param searched Range to search.
	/// @param blacklist List of elements to not search for.
	/// @return Iterator to the last element in `searched` that doesn't match one of the elements in `blacklist`.
	template <std::ranges::bidirectional_range SearchedRange, std::ranges::forward_range BlacklistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<BlacklistRange>>)
	constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_last_not_of(SearchedRange&& searched, BlacklistRange&& blacklist);

	//

	/// Sums the elements of a range.
	/// @tparam Type of the summed range.
	/// @tparam Value Summed value type.
	/// @param range Range to sum
	/// @param initial_value Initial value to sum with.
	/// @return Sum of the range.
	template <std::ranges::range Range, typename Value>
	Value sum(Range&& range, Value initial_value);

	//

	/// O(1) unstable vector erase function.
	/// @details O(1) erasure is achieved by swapping the final element into the position of the erased and then popping back.
	/// @tparam Element Vector element type.
	/// @param vec Vector to erase from.
	/// @param where Element in the vector to erase.
	template <move_assignable Element>
	void unstable_erase(std::vector<Element>& vec, typename std::vector<Element>::iterator where);

	/// @}

	/// Dereferencing range view.
	inline constexpr auto deref{std::views::transform([](auto& v) -> decltype(auto) { return *v; })};
} // namespace tr

#include "impl/ranges.hpp" // IWYU pragma: export