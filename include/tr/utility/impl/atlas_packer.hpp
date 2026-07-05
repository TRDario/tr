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

template <typename Key, tr::atlas_rects_value_type Value, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
bool tr::atlas_rects<Key, Value, Hash, Pred>::contains(Keylike&& key) const
{
	return m_rects.contains(std::forward<Keylike>(key));
}

template <typename Key, tr::atlas_rects_value_type Value, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
tr::usize tr::atlas_rects<Key, Value, Hash, Pred>::entries() const
{
	return m_rects.size();
}

template <typename Key, tr::atlas_rects_value_type Value, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
const Value& tr::atlas_rects<Key, Value, Hash, Pred>::operator[](Keylike&& key) const
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

template <typename Key, tr::atlas_rects_value_type Value, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
void tr::atlas_rects<Key, Value, Hash, Pred>::clear()
{
	m_packer.clear();
	m_rects.clear();
}

template <typename Key, tr::atlas_rects_value_type Value, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike, typename... Args>
	requires(std::constructible_from<Value, tr::rect2<tr::u16>, Args...>)
std::optional<glm::u16vec2> tr::atlas_rects<Key, Value, Hash, Pred>::try_insert(Keylike&& key, glm::u16vec2 size, glm::u16vec2 texture_size,
																				Args&&... extra_args)
{
	TR_ASSERT(!contains(std::forward<Keylike>(key)),
			  "Tried to insert a rect with the same key (type: '{}') as an existing rect into an atlas packer.", type_name<Key>());

	std::optional<glm::u16vec2> packing_result{m_packer.try_insert(size, texture_size)};
	if (packing_result.has_value()) {
		m_rects.emplace(std::piecewise_construct, std::forward_as_tuple(std::forward<Keylike>(key)),
						std::forward_as_tuple(rect2<u16>{*packing_result, size}, std::forward<Args>(extra_args)...));
	}
	return packing_result;
}