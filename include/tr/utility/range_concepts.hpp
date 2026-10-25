/// @file
/// @brief Provides range concepts.

#pragma once
#include <tr/utility/iterator_concepts.hpp>

//

namespace tr
{
	/// Sized output range type.
	/// @tparam Element Required output type.
	template <typename Target, typename Element>
	concept sized_output_range = std::ranges::sized_range<Target> && std::output_iterator<std::ranges::iterator_t<Target>, Element>;

	/// Input range to a type convertible to `Element`.
	/// @tparam Element Type the range's values must be convertible to.
	template <typename Target, typename Element>
	concept input_range_to = std::ranges::input_range<Target> && std::convertible_to<std::ranges::range_reference_t<Target>, Element>;

	/// Forward range to a type convertible to `Element`.
	/// @tparam Element Type the range's values must be convertible to.
	template <typename Target, typename Element>
	concept forward_range_to = std::ranges::forward_range<Target> && std::convertible_to<std::ranges::range_reference_t<Target>, Element>;

	/// Forward range to a type convertible to `Element` that does not throw.
	/// @tparam Element Type the range's values must be convertible to.
	template <typename Target, typename Element>
	concept nothrow_forward_range_to = forward_range_to<Target, Element> && nothrow_iterator<std::ranges::iterator_t<Target>>;
} // namespace tr