///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of atlas_packer.hpp.                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../atlas_packer.hpp"
#include "../macro.hpp"

/////////////////////////////////////////////////////////////// ATLAS RECTS ///////////////////////////////////////////////////////////////

template <typename Key, typename Hash, typename Pred> bool tr::atlas_rects<Key, Hash, Pred>::contains(const auto& key) const
{
	return m_rects.contains(key);
}

template <typename Key, typename Hash, typename Pred> tr::usize tr::atlas_rects<Key, Hash, Pred>::entries() const
{
	return m_rects.size();
}

template <typename Key, typename Hash, typename Pred> tr::rect2<tr::u16> tr::atlas_rects<Key, Hash, Pred>::operator[](const auto& key) const
{
	TR_ASSERT(contains(key), "Tried to get a rect at a nonexistant key from an atlas packer.");

	return m_rects.find(key)->second;
}

template <typename Key, typename Hash, typename Pred> void tr::atlas_rects<Key, Hash, Pred>::clear()
{
	m_packer.clear();
	m_rects.clear();
}

template <typename Key, typename Hash, typename Pred>
template <typename T>
std::optional<glm::u16vec2> tr::atlas_rects<Key, Hash, Pred>::try_insert(T&& key, glm::u16vec2 size, glm::u16vec2 texture_size)
{
	TR_ASSERT(!contains(key), "Tried to insert a rect with the same key as an existing rect into an atlas packer.");

	std::optional<glm::u16vec2> packing_result{m_packer.try_insert(size, texture_size)};
	if (packing_result.has_value()) {
		m_rects.emplace(std::forward<T>(key), rect2<u16>{*packing_result, size});
	}
	return packing_result;
}