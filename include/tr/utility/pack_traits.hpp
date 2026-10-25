/// @file
/// @brief Provides traits and concepts relating to template packs.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Gets the type at offset `I` of parameter pack `Ts`.
	/// @tparam I Type offset within the parameter pack.
	/// @tparam Ts Parameter pack to get a type from.
	template <usize I, typename... Ts>
		requires(sizeof...(Ts) > I)
	using pack_index_t = std::tuple_element_t<I, std::tuple<Ts...>>;

	/// Gets the type at the front of of parameter pack `Ts`.
	/// @tparam Ts Parameter pack to get the front type from.
	template <typename... Ts>
		requires(sizeof...(Ts) >= 1)
	using pack_front_t = pack_index_t<0, Ts...>;

	//

	/// Type contained within a list of types.
	/// @tparam Ts List of accepted types.
	template <typename T, typename... Ts>
	concept one_of = (std::same_as<T, Ts> || ...);

	/// Concept denoting a parameter pack of identical types.
	template <typename... Ts>
	concept all_same = (std::same_as<pack_front_t<Ts...>, Ts> && ...);
} // namespace tr