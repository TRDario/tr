///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous range functionality.                                                                                           //
//                                                                                                                                       //
// Objects and ranges can be converted to spans of bytes, and conversely, byte spans can be converted to an object or spans of objects:  //
//     - glm::mat4 transform; -> tr::as_bytes(transform) -> std::span<const std::byte, 64> over 'transform'                              //
//     - glm::mat4 transform; -> tr::as_mut_bytes(transform) -> std::span<std::byte, 64> over 'transform'                                //
//     - int data[5]{0, 1, 2, 3, 4}; tr::range_bytes(data) -> std::span<const std::byte, 20> over 'data'                                 //
//     - int data[5]{0, 1, 2, 3, 4}; tr::range_mut_bytes(data) -> std::span<std::byte, 20> over 'data'                                   //
//     - std::vector<float> data; tr::range_bytes(data) -> std::span<float> over 'data'                                                  //
//     - std::span<std::byte, 64> bytes; tr::as_object<glm::mat4>(bytes) -> const glm::mat4&                                             //
//     - std::span<std::byte, 64> bytes; tr::as_mut_object<glm::mat4>(bytes) -> glm::mat4&                                               //
//     - std::span<std::byte, 64> bytes; tr::as_objects<int>(bytes) -> std::span<int, 16> over 'bytes'                                   //
//     - std::span<std::byte> bytes; tr::as_mut_objects<float>(bytes) -> std::span<float> over 'bytes'                                   //
// NOTE: using tr::as_bytes on a range object will literally get the bytes of the object, which works for in-place allocated containers, //
// but not with something like std::vector. Prefer to use tr::range_bytes over ranges, even where the former works.                      //
//                                                                                                                                       //
// tr::find_last_of finds the last element in a range that is included in another range,                                                 //
// tr::find_first_not_of finds the first element in a range that isn't included in another range,                                        //
// and tr::find_last_not_of finds the last element in a range that isn't included in another range:                                      //
//     - tr::find_last_of("abcdefg", "abc") -> iterator to 'c'                                                                           //
//     - tr::find_first_not_of("abcdefg", "abc") -> iterator to 'd'                                                                      //
//     - tr::find_last_not_of("abcdefg", "efg") -> iterator to 'd'                                                                       //
//                                                                                                                                       //
// tr::fold_left performs a left fold on the elements of a range. tr::sum is provided for the common case of summing up elements:        //
//     - tr::fold_left(std::array{1, 2, 3, 4}, 0, std::plus{}) -> 10                                                                     //
//     - tr::sum(std::array{1, 2, 3, 4}, 0) -> equivalent to the above                                                                   //
//                                                                                                                                       //
// View adaptors for projecting a class member, as well as dereferencing the objects of a range are provided:                            //
//     - std::vector<tr::rgba8> colors; for (char red : tr::project(colors, &tr::rgba8::r)) {} -> iterates over the red channels         //
//     - std::vector<tr::rgba8> colors; for (char red : colors | tr::project(&tr::rgba8::r)) {} -> iterates over the red channels        //
//     - std::vector<std::unique_ptr<int>> ptrs; for (int value : tr::deref(ptrs)) -> iterates over the dereferenced pointers            //
//     - std::vector<std::unique_ptr<int>> ptrs; for (int value : ptrs | tr::deref) -> iterates over the dereferenced pointers           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Gets a view of a contiguous range as a span of immutable bytes.
	template <borrowed_standard_layout_range Range> auto range_bytes(Range&& range);
	// Gets a view of an object as a span of immutable bytes.
	template <standard_layout Object> std::span<const std::byte, sizeof(Object)> as_bytes(const Object& object);
	// Gets a view of a contiguous range as a span of mutable bytes.
	template <borrowed_mutable_standard_layout_range Range> auto range_mut_bytes(Range&& range);
	// Gets a view of an object as a span of mutable bytes.
	template <standard_layout Object> std::span<std::byte, sizeof(Object)> as_mut_bytes(Object& object);

	// Reinterprets a range of mutable bytes as an object and returns a reference to it.
	template <standard_layout Object, borrowed_typed_contiguous_mutable_range<std::byte> Range> Object& as_mut_object(Range&& bytes);
	// Reinterprets a range of immutable bytes as an object and returns a reference to it.
	template <standard_layout Object, borrowed_typed_contiguous_const_range<std::byte> Range> const Object& as_object(Range&& bytes);
	// Reinterprets a range of mutable bytes as a span of objects.
	template <standard_layout Element, borrowed_typed_contiguous_mutable_range<std::byte> Range> auto as_mut_objects(Range&& bytes);
	// Reinterprets a range of immutable bytes as a span of objects.
	template <standard_layout Element, borrowed_typed_contiguous_const_range<std::byte> Range> auto as_objects(Range&& bytes);

	// Returns an iterator to the last element in 'searched' that matches one of the elements in 'blacklist'.
	template <std::ranges::range SearchedRange, std::ranges::forward_range WhitelistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<WhitelistRange>>)
	constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_last_of(SearchedRange&& searched, WhitelistRange&& whitelist);
	// Returns an iterator to the first element in 'searched' that doesn't match one of the elements in 'blacklist'.
	template <std::ranges::range SearchedRange, std::ranges::forward_range BlacklistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<BlacklistRange>>)
	constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_first_not_of(SearchedRange&& searched, BlacklistRange&& blacklist);
	// Returns an iterator to the last element in 'searched' that doesn't match one of the elements in 'blacklist'.
	template <std::ranges::bidirectional_range SearchedRange, std::ranges::forward_range BlacklistRange>
		requires(std::equality_comparable_with<std::ranges::range_value_t<SearchedRange>, std::ranges::range_value_t<BlacklistRange>>)
	constexpr std::ranges::borrowed_iterator_t<SearchedRange> find_last_not_of(SearchedRange&& searched, BlacklistRange&& blacklist);

	// Left-folds the elements of a range.
	template <std::ranges::range Range, typename T, std::invocable<T, std::ranges::range_value_t<Range>> BinaryOp>
	T fold_left(Range&& range, T initial_value, BinaryOp&& pred);
	// Sums the elements of a range.
	template <std::ranges::range Range, typename T> T sum(Range&& range, T initial_value);

	// Creates an adaptor for a transformed view over a range as a range of one of its members.
	template <typename Range> constexpr auto project(auto Range::* ptr);
	// Creates a transformed view over a range as a range of one of its members.
	template <std::ranges::range Range> constexpr auto project(Range&& range, auto std::ranges::range_value_t<Range>::* ptr);
	// Dereferencing range view.
	inline constexpr auto deref{std::views::transform([](auto& v) -> decltype(auto) { return *v; })};
} // namespace tr

#include "impl/ranges.hpp" // IWYU pragma: export