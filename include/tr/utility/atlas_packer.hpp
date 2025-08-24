#pragma once
#include "geometry.hpp"
#include "macro.hpp"

namespace tr {
	// Skyline rect packer for atlas textures.
	class atlas_packer {
	  public:
		// Creates an empty packer.
		atlas_packer();

		// Clears the packer.
		void clear();
		// Attempts to insert a rectangle.
		std::optional<glm::u16vec2> try_insert(glm::u16vec2 size, glm::u16vec2 texture_size);

	  private:
		// The skyline silhouette points.
		std::vector<glm::u16vec2> m_skyline;
	};

	// Atlas packer combined with a list of rects.
	template <class Key, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>> class atlas_rects {
	  public:
		// Gets whether the atlas contains a key.
		bool contains(const auto& key) const;
		// Gets the number of entries in the atlas.
		usize entries() const;

		// Gets a rect associated with a certain key.
		rect2<u16> operator[](const auto& key) const;

		// Clears the packer.
		void clear();
		// Tries to insert a rectangle.
		std::optional<glm::u16vec2> try_insert(auto&& key, glm::u16vec2 size, glm::u16vec2 texture_size);

	  private:
		// The atlas packer.
		atlas_packer m_packer;
		// The atlas rects.
		std::unordered_map<Key, rect2<u16>, Hash, Pred> m_rects;
	};

} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class Key, class Hash, class Pred> bool tr::atlas_rects<Key, Hash, Pred>::contains(const auto& key) const
{
	return m_rects.contains(key);
}

template <class Key, class Hash, class Pred> tr::usize tr::atlas_rects<Key, Hash, Pred>::entries() const
{
	return m_rects.size();
}

template <class Key, class Hash, class Pred> tr::rect2<tr::u16> tr::atlas_rects<Key, Hash, Pred>::operator[](const auto& key) const
{
	TR_ASSERT(contains(key), "Tried to get a rect at a nonexistant key from an atlas packer.");

	return m_rects.find(key)->second;
}

template <class Key, class Hash, class Pred> void tr::atlas_rects<Key, Hash, Pred>::clear()
{
	m_packer.clear();
	m_rects.clear();
}

template <class Key, class Hash, class Pred>
std::optional<glm::u16vec2> tr::atlas_rects<Key, Hash, Pred>::try_insert(auto&& key, glm::u16vec2 size, glm::u16vec2 texture_size)
{
	TR_ASSERT(!contains(key), "Tried to insert a rect with the same key as an existing rect into an atlas packer.");

	std::optional<glm::u16vec2> packing_result{m_packer.try_insert(size, texture_size)};
	if (packing_result.has_value()) {
		m_rects.emplace(std::move(key), rect2<u16>{*packing_result, size});
	}
	return packing_result;
}