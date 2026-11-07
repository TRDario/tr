/// @file
/// @brief Provides range concepts.

#pragma once
#include <tr/utility/iterator_concepts.hpp>

//

namespace tr
{
	/// @ingroup meta
	/// @brief Concept checking for a sized output range type.
	/// @tparam Target Type to check for being a sized output range.
	/// @tparam Element Required output type.
	template <typename Target, typename Element>
	concept sized_output_range = std::ranges::sized_range<Target> && std::output_iterator<std::ranges::iterator_t<Target>, Element>;

	/// @ingroup meta
	/// @brief Concept checking for an input range to a type convertible to `Element`.
	/// @tparam Target Type to check for being an input range to `To`.
	/// @tparam Element Type the range's values must be convertible to.
	template <typename Target, typename Element>
	concept input_range_to = std::ranges::input_range<Target> && std::convertible_to<std::ranges::range_reference_t<Target>, Element>;

	/// @ingroup meta
	/// @brief Concept checking for a forward range to a type convertible to `Element`.
	/// @tparam Target Type to check for being a forward range to `To`.
	/// @tparam Element Type the range's values must be convertible to.
	template <typename Target, typename Element>
	concept forward_range_to = std::ranges::forward_range<Target> && std::convertible_to<std::ranges::range_reference_t<Target>, Element>;

	/// @ingroup meta
	/// @brief Concept checking for a forward range to a type convertible to `Element` that does not throw.
	/// @tparam Target Type to check for being a non-throwing forward range to `To`.
	/// @tparam Element Type the range's values must be convertible to.
	template <typename Target, typename Element>
	concept nothrow_forward_range_to = forward_range_to<Target, Element> && nothrow_iterator<std::ranges::iterator_t<Target>>;
} // namespace tr