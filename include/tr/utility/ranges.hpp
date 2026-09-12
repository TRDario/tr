/// @file
/// @brief Provides miscellaneous range utilities.

#pragma once
#include <tr/utility/concepts.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/type_name.hpp>

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
	[[nodiscard]] auto reinterpret_span(std::span<From, Extent> from) noexcept
	{
		if constexpr (Extent != std::dynamic_extent) {
			static_assert(Extent * sizeof(From) % sizeof(To) == 0, "Cannot reinterpret span due to size_bytes() % sizeof(T) != 0.");
			return std::span<To, Extent * sizeof(From) / sizeof(To)>{reinterpret_cast<To*>(from.data()),
																	 Extent * sizeof(From) / sizeof(To)};
		}
		else {
			TR_ASSERT(from.size_bytes() % sizeof(To) == 0, "Cannot reinterpret span as '{}' due to size_bytes ({}) % sizeof({}) ({}) != 0.",
					  type_name<To>(), from.size_bytes(), type_name<To>(), sizeof(To));
			return std::span<To>{reinterpret_cast<To*>(from.data()), from.size_bytes() / sizeof(To)};
		}
	}

	//

	/// Gets a view of a contiguous range as a span of immutable bytes.
	/// @tparam Range Contiguous range of standard layout elements.
	/// @param range Contiguous range to reinterpret as bytes.
	/// @return `range` reinterpreted as a range of bytes.
	template <borrowed_standard_layout_range Range>
	[[nodiscard]] auto range_bytes(Range&& range) noexcept
	{
		return std::as_bytes(std::span{range});
	}

	/// Gets a view of an object as a span of immutable bytes.
	/// @tparam Standard layout object type.
	/// @param object Object to reinterpret as bytes.
	/// @return `object` reinterpreted as a range of bytes.
	template <standard_layout Object>
	[[nodiscard]] std::span<const std::byte, sizeof(Object)> as_bytes(const Object& object) noexcept
	{
		return std::as_bytes(std::span<const Object, 1>{std::addressof(object), 1});
	}

	/// Gets a view of a contiguous range as a span of mutable bytes.
	/// @tparam Range Contiguous range of standard layout elements.
	/// @param range Contiguous range to reinterpret as bytes.
	/// @return `range` reinterpreted as a range of mutable bytes.
	template <borrowed_mutable_standard_layout_range Range>
	[[nodiscard]] auto range_mut_bytes(Range&& range) noexcept
	{
		return std::as_writable_bytes(std::span{range});
	}

	/// Gets a view of an object as a span of mutable bytes.
	/// @tparam Standard layout object type.
	/// @param object Object to reinterpret as bytes.
	/// @return `object` reinterpreted as a range of mutable bytes.
	template <standard_layout Object>
	[[nodiscard]] std::span<std::byte, sizeof(Object)> as_mut_bytes(Object& object) noexcept
	{
		return std::as_writable_bytes(std::span<Object, 1>{std::addressof(object), 1});
	}

	//

	/// Reinterprets a range of mutable bytes as an object and returns a reference to it.
	/// @tparam Object Object type to reinterpret the byte range as.
	/// @tparam Range Contiguous range of mutable bytes.
	/// @param bytes Range to reinterpret.
	/// @return Reference to the reinterpreted object.
	template <standard_layout Object, borrowed_typed_contiguous_mutable_range<std::byte> Range>
	[[nodiscard]] Object& as_mut_object(Range&& bytes) noexcept
	{
		const auto span{tr::range_mut_bytes(bytes)};

		if constexpr (decltype(span)::extent != std::dynamic_extent) {
			static_assert(decltype(span)::extent == sizeof(Object), "Cannot reinterpret byte range as object due to size != sizeof(T).");
		}
		else {
			TR_ASSERT(span.size() == sizeof(Object), "Cannot reinterpret byte range as '{}' due to size ({}) != sizeof(T) ({}).",
					  type_name<Object>(), span.size(), sizeof(Object));
		}

		return *reinterpret_cast<Object*>(span.data());
	}

	/// Reinterprets a range of immutable bytes as an object and returns a reference to it.
	/// @tparam Object Object type to reinterpret the byte range as.
	/// @tparam Range Contiguous range of bytes.
	/// @param bytes Range to reinterpret.
	/// @return Reference to the reinterpreted object.
	template <standard_layout Object, borrowed_typed_contiguous_const_range<std::byte> Range>
	[[nodiscard]] const Object& as_object(Range&& bytes) noexcept
	{
		const auto span{tr::range_bytes(bytes)};

		if constexpr (decltype(span)::extent != std::dynamic_extent) {
			static_assert(decltype(span)::extent == sizeof(Object), "Cannot reinterpret byte range as object due to size != sizeof(T).");
		}
		else {
			TR_ASSERT(span.size() == sizeof(Object), "Cannot reinterpret byte range as '{}' due to size ({}) != sizeof({}) ({}).",
					  type_name<Object>(), span.size(), type_name<Object>(), sizeof(Object));
		}

		return *reinterpret_cast<const Object*>(span.data());
	}

	/// Reinterprets a range of mutable bytes as a span of objects.
	/// @tparam Element Element type to reinterpret the byte range as.
	/// @tparam Range Contiguous range of mutable bytes.
	/// @param bytes Range to reinterpret.
	/// @return Span of `Element`.
	template <standard_layout Element, borrowed_typed_contiguous_mutable_range<std::byte> Range>
	[[nodiscard]] auto as_mut_objects(Range&& bytes) noexcept
	{
		const auto span{tr::range_mut_bytes(bytes)};

		if constexpr (decltype(span)::extent != std::dynamic_extent) {
			static_assert(decltype(span)::extent % sizeof(Element) == 0,
						  "Cannot reinterpret byte range as an object span due to size / sizeof(T) not being an integer.");
			return std::span<Element, decltype(span)::extent / sizeof(Element)>{reinterpret_cast<Element*>(span.data())};
		}
		else {
			TR_ASSERT(span.size() % sizeof(Element) == 0,
					  "Cannot reinterpret byte range as a span of '{}' due to size ({}) / sizeof({}) ({}) not being an integer.",
					  type_name<Element>(), span.size(), type_name<Element>(), sizeof(Element));
			return std::span{reinterpret_cast<Element*>(span.data()), span.size() / sizeof(Element)};
		}
	}

	/// Reinterprets a range of immutable bytes as a span of objects.
	/// @tparam Element Element type to reinterpret the byte range as.
	/// @tparam Range Contiguous range of bytes.
	/// @param bytes Range to reinterpret.
	/// @return Span of `const Element`.
	template <standard_layout Element, borrowed_typed_contiguous_const_range<std::byte> Range>
	[[nodiscard]] auto as_objects(Range&& bytes) noexcept
	{
		const auto span{tr::range_bytes(bytes)};

		if constexpr (decltype(span)::extent != std::dynamic_extent) {
			static_assert(decltype(span)::extent % sizeof(Element) == 0,
						  "Cannot reinterpret byte range as an object span due to size / sizeof(T) not being an integer.");
			return std::span<Element, decltype(span)::extent / sizeof(Element)>{reinterpret_cast<Element*>(span.data())};
		}
		else {
			TR_ASSERT(span.size() % sizeof(Element) == 0,
					  "Cannot reinterpret byte range as a span of '{}' due to size ({}) / sizeof({}) ({}) not being an integer.",
					  type_name<Element>(), span.size(), type_name<Element>(), sizeof(Element));
			return std::span{reinterpret_cast<Element*>(span.data()), span.size() / sizeof(Element)};
		}
	}

	//

	/// Returns an iterator to the last element in `searched` that matches one of the elements in `whitelist`.
	/// @tparam SearchedRange Type of the searched range.
	/// @tparam WhitelistRange Type of the whitelist range.
	/// @param searched Range to search.
	/// @param whitelist List of elements to search for.
	/// @return Iterator to the first element in `searched` that matches one in `whitelist`.
	template <std::ranges::range SearchedRange, std::ranges::forward_range WhitelistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<WhitelistRange>>)
	[[nodiscard]] constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_last_of(SearchedRange&& searched,
																						 WhitelistRange&& whitelist)
	{
		for (auto value_it = std::ranges::rbegin(searched); value_it != std::ranges::rend(searched); ++value_it) {
			if (std::ranges::contains(whitelist, *value_it)) {
				return std::ranges::prev(value_it.base());
			}
		}
		return std::ranges::end(searched);
	}

	/// Returns an iterator to the first element in `searched` that doesn't match one of the elements in `blacklist`.
	/// @tparam SearchedRange Type of the searched range.
	/// @tparam BlacklistRange Type of the blacklist range.
	/// @param searched Range to search.
	/// @param blacklist List of elements to not search for.
	/// @return Iterator to the first element in `searched` that doesn't match one of the elements in `blacklist`.
	template <std::ranges::range SearchedRange, std::ranges::forward_range BlacklistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<BlacklistRange>>)
	[[nodiscard]] constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_first_not_of(SearchedRange&& searched,
																							  BlacklistRange&& blacklist)
	{
		for (auto value_it = std::ranges::begin(searched); value_it != std::ranges::end(searched); ++value_it) {
			if (!std::ranges::contains(blacklist, *value_it)) {
				return value_it;
			}
		}
		return std::ranges::end(searched);
	}

	/// Returns an iterator to the last element in `searched` that doesn't match one of the elements in `blacklist`.
	/// @tparam SearchedRange Type of the searched range.
	/// @tparam BlacklistRange Type of the blacklist range.
	/// @param searched Range to search.
	/// @param blacklist List of elements to not search for.
	/// @return Iterator to the last element in `searched` that doesn't match one of the elements in `blacklist`.
	template <std::ranges::bidirectional_range SearchedRange, std::ranges::forward_range BlacklistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<BlacklistRange>>)
	[[nodiscard]] constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_last_not_of(SearchedRange&& searched,
																							 BlacklistRange&& blacklist)
	{
		for (auto value_it = std::ranges::rbegin(searched); value_it != std::ranges::rend(searched); ++value_it) {
			if (!std::ranges::contains(blacklist, *value_it)) {
				return std::ranges::prev(value_it.base());
			}
		}
		return std::ranges::end(searched);
	}

	//

	/// Sums the elements of a range.
	/// @tparam Type of the summed range.
	/// @tparam Value Summed value type.
	/// @param range Range to sum
	/// @param initial_value Initial value to sum with.
	/// @return Sum of the range.
	template <std::ranges::range Range, typename Value>
	[[nodiscard]] Value sum(Range&& range, Value initial_value)
	{
		return std::accumulate(std::ranges::begin(range), std::ranges::end(range), initial_value);
	}

	//

	/// Vector-like container that `tr::unstable_erase` may be used on.
	template <typename T>
	concept unstable_erasable = std::ranges::contiguous_range<T> && std::movable<std::ranges::range_value_t<T>> &&
								requires(T& v) { v.pop_back(); };

	/// O(1) unstable vector-like container erase function.
	/// @details O(1) erasure is achieved by swapping the final element into the position of the erased and then popping back.
	/// @tparam Container Vector-like container type.
	/// @param container Container to erase from.
	/// @param where Element in the container to erase.
	template <unstable_erasable Container>
	void unstable_erase(Container& container, typename Container::iterator where)
	{
		const typename Container::iterator back_it{std::prev(container.end())};
		if (where != back_it) {
			*where = std::move(*back_it);
		}
		container.pop_back();
	}

	/// @}

	/// Dereferencing range view.
	inline constexpr auto deref{std::views::transform([](auto& v) -> decltype(auto) { return *v; })};
} // namespace tr