/// @file
/// @brief Provides iterator concepts.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// Iterator that does not throw.
	template <typename T>
	concept nothrow_iterator = std::input_or_output_iterator<T> && noexcept(++std::declval<T&>()) &&
							   noexcept(std::declval<const T&>() != std::declval<const T&>()) && noexcept(*std::declval<T&>());

	/// Input iterator to a type convertible to `Element`.
	/// @tparam Element Type the iterator's dereferenced value must be convertible to.
	template <typename T, typename Element>
	concept input_iterator_to = std::input_iterator<T> && std::convertible_to<std::iter_reference_t<T>, Element>;

	/// Forward iterator to a type convertible to `Element`.
	/// @tparam Element Type the iterator's dereferenced value must be convertible to.
	template <typename T, typename Element>
	concept forward_iterator_to = std::forward_iterator<T> && std::convertible_to<std::iter_reference_t<T>, Element>;

	/// Forward iterator to a type convertible to `Element` that does not throw.
	/// @tparam Element Type the iterator's dereferenced value must be convertible to.
	template <typename T, typename Element>
	concept nothrow_forward_iterator_to = forward_iterator_to<T, Element> && nothrow_iterator<T>;
} // namespace tr