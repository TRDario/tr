///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of atlas_packer.hpp.                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../atlas_packer.hpp"
#include "../hash_map.hpp"
#include "../macro.hpp"

/////////////////////////////////////////////////////////////// ATLAS RECTS ///////////////////////////////////////////////////////////////

template <typename Key, typename Extra, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
bool tr::atlas_rects<Key, Extra, Hash, Pred>::contains(Keylike&& key) const
{
	return m_rects.contains(std::forward<Keylike>(key));
}

template <typename Key, typename Extra, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
tr::usize tr::atlas_rects<Key, Extra, Hash, Pred>::entries() const
{
	return m_rects.size();
}

template <typename Key, typename Extra, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
tr::rect2<tr::u16> tr::atlas_rects<Key, Extra, Hash, Pred>::operator[](Keylike&& key) const
{
	if constexpr (valid_format_string_for<"{}", Keylike>) {
		TR_ASSERT(contains(std::forward<Keylike>(key)), "Tried to get a rect at nonexistant key '{}' from an atlas packer.", key);
	}
	else {
		TR_ASSERT(contains(std::forward<Keylike>(key)), "Tried to get a rect at a nonexistant key (type: '{}') from an atlas packer.",
				  type_name<Key>());
	}

	if constexpr (std::same_as<Extra, void>) {
		return tr::get(m_rects, std::forward<Keylike>(key));
	}
	else {
		return tr::get(m_rects, std::forward<Keylike>(key)).rect;
	}
}

template <typename Key, typename Extra, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
	requires(!std::same_as<Extra, void>)
const tr::atlas_rects<Key, Extra, Hash, Pred>::value_type& tr::atlas_rects<Key, Extra, Hash, Pred>::get_with_extra(Keylike&& key) const
{
	if constexpr (valid_format_string_for<"{}", Keylike>) {
		TR_ASSERT(contains(std::forward<Keylike>(key)), "Tried to get a rect at nonexistant key '{}' from an atlas packer.", key);
	}
	else {
		TR_ASSERT(contains(std::forward<Keylike>(key)), "Tried to get a rect at a nonexistant key (type: '{}') from an atlas packer.",
				  type_name<Key>());
	}

	return tr::get(m_rects, std::forward<Keylike>(key));
}

template <typename Key, typename Extra, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
void tr::atlas_rects<Key, Extra, Hash, Pred>::clear()
{
	m_packer.clear();
	m_rects.clear();
}

template <typename Key, typename Extra, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
	requires(std::same_as<Extra, void>)
std::optional<glm::u16vec2> tr::atlas_rects<Key, Extra, Hash, Pred>::try_insert(Keylike&& key, glm::u16vec2 size, glm::u16vec2 texture_size)
{
	TR_ASSERT(!contains(std::forward<Keylike>(key)),
			  "Tried to insert a rect with the same key (type: '{}') as an existing rect into an atlas packer.", type_name<Key>());

	std::optional<glm::u16vec2> packing_result{m_packer.try_insert(size, texture_size)};
	if (packing_result.has_value()) {
		m_rects.emplace(std::forward<Keylike>(key), rect2<u16>{*packing_result, size});
	}
	return packing_result;
}

template <typename Key, typename Extra, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike, typename... Args>
	requires(std::constructible_from<Extra, Args...>)
std::optional<glm::u16vec2> tr::atlas_rects<Key, Extra, Hash, Pred>::try_insert(Keylike&& key, glm::u16vec2 size, glm::u16vec2 texture_size,
																				Args&&... extra_args)
{
	TR_ASSERT(!contains(std::forward<Keylike>(key)),
			  "Tried to insert a rect with the same key (type: '{}') as an existing rect into an atlas packer.", type_name<Key>());

	std::optional<glm::u16vec2> packing_result{m_packer.try_insert(size, texture_size)};
	if (packing_result.has_value()) {
		m_rects.emplace(std::forward<Keylike>(key),
						expanded_atlas_rect<Extra>{rect2<u16>{*packing_result, size}, Extra{std::forward<Args>(extra_args)...}});
	}
	return packing_result;
}