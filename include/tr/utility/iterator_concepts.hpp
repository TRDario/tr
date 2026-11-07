/// @file
/// @brief Provides iterator concepts.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr
{
	/// @ingroup meta
	/// @brief Concept checking for an iterator that does not throw.
	/// @tparam Target Type to check for being a non-throwing iterator.
	template <typename Target>
	concept nothrow_iterator = std::input_or_output_iterator<Target> && noexcept(++std::declval<Target&>()) &&
							   noexcept(std::declval<const Target&>() != std::declval<const Target&>()) &&
							   noexcept(*std::declval<Target&>());

	/// @ingroup meta
	/// @brief Concept checking for an input iterator to a type convertible to `Element`.
	/// @tparam Target Type to check for being an input iterator to `Element`.
	/// @tparam Element Type the iterator's dereferenced value must be convertible to.
	template <typename Target, typename Element>
	concept input_iterator_to = std::input_iterator<Target> && std::convertible_to<std::iter_reference_t<Target>, Element>;

	/// @ingroup meta
	/// @brief Concept checking for a forward iterator to a type convertible to `Element`.
	/// @tparam Target Type to check for being a forward iterator to `Element`.
	/// @tparam Element Type the iterator's dereferenced value must be convertible to.
	template <typename Target, typename Element>
	concept forward_iterator_to = std::forward_iterator<Target> && std::convertible_to<std::iter_reference_t<Target>, Element>;

	/// @ingroup meta
	/// @brief Concept checking for a forward iterator to a type convertible to `Element` that does not throw.
	/// @tparam Target Type to check for being a non-throwing forward iterator to `Element`.
	/// @tparam Element Type the iterator's dereferenced value must be convertible to.
	template <typename Target, typename Element>
	concept nothrow_forward_iterator_to = forward_iterator_to<Target, Element> && nothrow_iterator<Target>;
} // namespace tr