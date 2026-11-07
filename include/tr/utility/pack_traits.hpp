/// @file
/// @brief Provides traits and concepts relating to template packs.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// @ingroup meta
	/// @brief Returns the type at offset `I` of parameter pack `Ts`.
	/// @tparam Index Type offset within the parameter pack.
	/// @tparam Pack Parameter pack to get a type from.
	template <usize Index, typename... Pack>
		requires(sizeof...(Pack) > Index)
	using pack_index_t = std::tuple_element_t<Index, std::tuple<Pack...>>;

	/// @ingroup meta
	/// @brief Returns the type at the front of of parameter pack `Pack`.
	/// @tparam Pack Parameter pack to get the front type from.
	template <typename... Pack>
		requires(sizeof...(Pack) >= 1)
	using pack_front_t = pack_index_t<0, Pack...>;

	//

	/// @ingroup meta
	/// @brief Concept checking that `Target` is contained within `Accepted`.
	/// @tparam Target Type to check for in `Accepted`.
	/// @tparam Accepted List of accepted types.
	template <typename Target, typename... Accepted>
	concept one_of = (std::same_as<Target, Accepted> || ...);

	/// @ingroup meta
	/// @brief Concept checking that every element of `Targets` is the same type.
	/// @tparam Targets Types to check for equality.
	template <typename... Targets>
	concept all_same = (std::same_as<pack_front_t<Targets...>, Targets> && ...);
} // namespace tr