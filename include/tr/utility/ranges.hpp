#pragma once
#include "concepts.hpp"
#include "macro.hpp"

namespace tr {
	/** @ingroup utility
	 *  @defgroup ranges Ranges
	 *  Custom range functionality.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Gets a view of a contiguous range as a span of immutable bytes.
	 *
	 * @tparam T A type that fulfills <em>standard_layout_range</em>.
	 *
	 * @param[in] range The range to get a byte view of.
	 *
	 * @return A span over the range's bytes.
	 ******************************************************************************************************************/
	template <standard_layout_range T> auto range_bytes(const T& range) noexcept;

	/******************************************************************************************************************
	 * Gets a view of an object as a span of immutable bytes.
	 *
	 * @tparam T A standard layout type.
	 *
	 * @param[in] object The object to get a byte view of.
	 *
	 * @return A span over the object's bytes.
	 ******************************************************************************************************************/
	template <standard_layout T> std::span<const std::byte, sizeof(T)> as_bytes(const T& object) noexcept;

	/******************************************************************************************************************
	 * Gets a view of a contiguous range as a span of mutable bytes.
	 *
	 * @tparam T A type that fulfills <em>standard_layout_range</em>.
	 *
	 * @param[in] range The range to get a byte view of.
	 *
	 * @return A mutable span over the range's bytes.
	 ******************************************************************************************************************/
	template <standard_layout_range T> auto range_mut_bytes(T& range) noexcept;

	/******************************************************************************************************************
	 * Gets a view of an object as a span of mutable bytes.
	 *
	 * @tparam T A standard layout type.
	 *
	 * @param[in] object The object to get a byte view of.
	 *
	 * @return A mutable span over the object's bytes.
	 ******************************************************************************************************************/
	template <standard_layout T> std::span<std::byte, sizeof(T)> as_mut_bytes(T& object) noexcept;

	/******************************************************************************************************************
	 * Reinterprets a span of mutable bytes as a span of objects.
	 *
	 * @pre The span must be an integer multiple of the size of the object being cast into. If the span has a static
	 *      length, the function will fail to compile, and if the span has a dynamic length, a failed assertion may be
	 *      triggered.
	 *
	 * @param[in] bytes The byte span to reinterpret.
	 *
	 * @return A mutable span over objects.
	 ******************************************************************************************************************/
	template <standard_layout T, std::size_t S> auto as_mut_objects(std::span<std::byte, S> bytes) noexcept;

	/******************************************************************************************************************
	 * Reinterprets a span of immutable bytes as a span of const objects.
	 *
	 * @pre The span must be an integer multiple of the size of the object being cast into. If the span has a static
	 *      length, the function will fail to compile, and if the span has a dynamic length, a failed assertion may be
	 *      triggered.
	 *
	 * @param[in] bytes The byte span to reinterpret.
	 *
	 * @return A span over const objects.
	 ******************************************************************************************************************/
	template <standard_layout T, std::size_t S> auto as_objects(std::span<const std::byte, S> bytes) noexcept;

	/******************************************************************************************************************
	 * Creates an adaptor for a transformed view over a range as a range of one of its members.
	 *
	 * @param[in] ptr A class member pointer.
	 *
	 * @return An adaptor for the projection transformer.
	 ******************************************************************************************************************/
	template <class R> constexpr auto project(auto R::* ptr) noexcept;

	/******************************************************************************************************************
	 * Creates a transformed view over a range as a range of one of its members.
	 *
	 * @tparam R A range type.
	 *
	 * @param[in] range The range to transform.
	 * @param[in] ptr A class member pointer.
	 *
	 * @return The transformed view.
	 ******************************************************************************************************************/
	template <std::ranges::range R> constexpr auto project(R&& range, auto std::ranges::range_value_t<R>::* ptr) noexcept;

	/******************************************************************************************************************
	 * Dereferencing range view.
	 ******************************************************************************************************************/
	inline constexpr auto deref{std::views::transform([](auto& v) -> auto& { return *v; })};

	/// @}
} // namespace tr

/// @cond IMPLEMENTATION

template <tr::standard_layout_range T> auto tr::range_bytes(const T& range) noexcept
{
	return std::as_bytes(std::span{range});
}

template <tr::standard_layout_range T> auto tr::range_mut_bytes(T& range) noexcept
{
	return std::as_writable_bytes(std::span{range});
}

template <tr::standard_layout T> std::span<const std::byte, sizeof(T)> tr::as_bytes(const T& object) noexcept
{
	return std::as_bytes(std::span<const T, 1>{std::addressof(object), 1});
}

template <tr::standard_layout T> std::span<std::byte, sizeof(T)> tr::as_mut_bytes(T& object) noexcept
{
	return std::as_writable_bytes(std::span<T, 1>{std::addressof(object), 1});
}

template <tr::standard_layout T, std::size_t S> auto tr::as_mut_objects(std::span<std::byte, S> bytes) noexcept
{
	if constexpr (S != std::dynamic_extent) {
		static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span<T, S / sizeof(T)>{(T*)(bytes.data())};
	}
	else {
		TR_ASSERT(bytes.size() % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span{(T*)(bytes.data()), bytes.size() / sizeof(T)};
	}
}

template <tr::standard_layout T, std::size_t S> auto tr::as_objects(std::span<const std::byte, S> bytes) noexcept
{
	if constexpr (S != std::dynamic_extent) {
		static_assert(S % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span<const T, S / sizeof(T)>{reinterpret_cast<const T*>(bytes.data())};
	}
	else {
		TR_ASSERT(bytes.size() % sizeof(T) == 0, "Cannot reinterpret byte span due to size / sizeof(T) not being an integer.");
		return std::span{(const T*)(bytes.data()), bytes.size() / sizeof(T)};
	}
}

template <class R> constexpr auto tr::project(auto R::* ptr) noexcept
{
	return std::views::transform([=](auto&& val) -> auto&& { return val.*ptr; });
}

template <std::ranges::range R> constexpr auto tr::project(R&& range, auto std::ranges::range_value_t<R>::* ptr) noexcept
{
	return std::views::transform(std::forward<R&&>(range), [=](auto&& val) -> auto&& { return val.*ptr; });
}

/// @endcond
