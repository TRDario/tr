///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous range functionality.                                                                                           //
//                                                                                                                                       //
// Objects and ranges can be converted to spans of bytes, and conversely, byte spans can be converted to spans of objects:               //
//     - glm::mat4 transform; -> tr::as_bytes(transform) -> std::span<const std::byte, 64> over 'transform'                              //
//     - glm::mat4 transform; -> tr::as_mut_bytes(transform) -> std::span<std::byte, 64> over 'transform'                                //
//     - int data[5]{0, 1, 2, 3, 4}; tr::range_bytes(data) -> std::span<const std::byte, 20> over 'data'                                 //
//     - int data[5]{0, 1, 2, 3, 4}; tr::range_mut_bytes(data) -> std::span<std::byte, 20> over 'data'                                   //
//     - std::vector<float> data; tr::range_bytes(data) -> std::span<float> over 'data'                                                  //
//     - std::span<std::byte, 64> bytes; tr::as_objects<int>(bytes) -> std::span<int, 16> over 'bytes'                                   //
//     - std::span<std::byte> bytes; tr::as_mut_objects<float>(bytes) -> std::span<float> over 'bytes'                                   //
// NOTE: using tr::as_bytes on a range object will literally get the bytes of the object, which works for in-place allocated containers, //
// but not with something like std::vector. Prefer to use tr::range_bytes over ranges, even where the former works.                      //
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
#include "macro.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Gets a view of a contiguous range as a span of immutable bytes.
	template <standard_layout_range T> auto range_bytes(const T& range);
	// Gets a view of an object as a span of immutable bytes.
	template <standard_layout T> std::span<const std::byte, sizeof(T)> as_bytes(const T& object);
	// Gets a view of a contiguous range as a span of mutable bytes.
	template <standard_layout_range T> auto range_mut_bytes(T& range);
	// Gets a view of an object as a span of mutable bytes.
	template <standard_layout T> std::span<std::byte, sizeof(T)> as_mut_bytes(T& object);

	// Reinterprets a span of mutable bytes as a span of objects.
	template <standard_layout T, usize S> auto as_mut_objects(std::span<std::byte, S> bytes);
	// Reinterprets a span of immutable bytes as a span of const objects.
	template <standard_layout T, usize S> auto as_objects(std::span<const std::byte, S> bytes);

	// Creates an adaptor for a transformed view over a range as a range of one of its members.
	template <class R> constexpr auto project(auto R::* ptr);
	// Creates a transformed view over a range as a range of one of its members.
	template <std::ranges::range R> constexpr auto project(R&& range, auto std::ranges::range_value_t<R>::* ptr);
	// Dereferencing range view.
	inline constexpr auto deref{std::views::transform([](auto& v) -> decltype(auto) { return *v; })};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::standard_layout_range T> auto tr::range_bytes(const T& range)
{
	return std::as_bytes(std::span{range});
}

template <tr::standard_layout_range T> auto tr::range_mut_bytes(T& range)
{
	return std::as_writable_bytes(std::span{range});
}

template <tr::standard_layout T> std::span<const std::byte, sizeof(T)> tr::as_bytes(const T& object)
{
	return std::as_bytes(std::span<const T, 1>{std::addressof(object), 1});
}

template <tr::standard_layout T> std::span<std::byte, sizeof(T)> tr::as_mut_bytes(T& object)
{
	return std::as_writable_bytes(std::span<T, 1>{std::addressof(object), 1});
}

template <tr::standard_layout T, tr::usize S> auto tr::as_mut_objects(std::span<std::byte, S> bytes)
{
	if constexpr (S != std::dynamic_extent) {
		static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span<T, S / sizeof(T)>{(T*)bytes.data()};
	}
	else {
		TR_ASSERT(bytes.size() % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span{(T*)bytes.data(), bytes.size() / sizeof(T)};
	}
}

template <tr::standard_layout T, tr::usize S> auto tr::as_objects(std::span<const std::byte, S> bytes)
{
	if constexpr (S != std::dynamic_extent) {
		static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span<const T, S / sizeof(T)>{(const T*)bytes.data()};
	}
	else {
		TR_ASSERT(bytes.size() % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span{(const T*)bytes.data(), bytes.size() / sizeof(T)};
	}
}

template <class R> constexpr auto tr::project(auto R::* ptr)
{
	return std::views::transform([=](auto&& val) -> auto&& { return val.*ptr; });
}

template <std::ranges::range R> constexpr auto tr::project(R&& range, auto std::ranges::range_value_t<R>::* ptr)
{
	return std::views::transform(std::forward<R&&>(range), [=](auto&& val) -> auto&& { return val.*ptr; });
}