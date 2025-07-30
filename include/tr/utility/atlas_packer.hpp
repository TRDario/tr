#pragma once
#include "geometry.hpp"
#include "macro.hpp"

namespace tr {
	// Skyline rect packer for atlas textures.
	class atlas_packer {
	  public:
		// Creates an empty packer.
		atlas_packer(glm::u16vec2 size);

		// Gets the size of the packer texture.
		glm::u16vec2 size() const;

		// Clears the packer.
		void clear();
		// Resizes the packer texture.
		void resize(glm::u16vec2 size);
		// Attempts to insert a rectangle.
		std::optional<glm::u16vec2> try_insert(glm::u16vec2 size);

	  private:
		// The skyline silhouette points.
		std::vector<glm::u16vec2> skyline;
		// The size of the texture.
		glm::u16vec2 texture_size;
	};

	// Atlas packer combined with a list of rects.
	template <class Key, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>> class atlas_rects {
	  public:
		// Creates an empty packer.
		atlas_rects(glm::u16vec2 size);

		// Gets whether the atlas contains a key.
		bool contains(const auto& key) const;
		// Gets a rect associated with a certain key.
		glm::vec2 operator[](const auto& key) const;
		// Gets the unnormalized rect associated with a certain key.
		glm::u16vec2 unnormalized(const auto& key) const;

		// Clears the packer.
		void clear();
		// Resizes the packer.
		void resize(glm::u16vec2 size);
		// Tries to insert a rectangle.
		bool try_insert(auto&& key, glm::u16vec2 size);

	  private:
		// The atlas packer.
		atlas_packer packer;
		// The atlas rects.
		std::unordered_map<Key, rect2<std::uint16_t>, Hash, Pred> rects;
	};

} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class Key, class Hash, class Pred>
tr::atlas_rects<Key, Hash, Pred>::atlas_rects(glm::u16vec2 size)
	: packer{size}
{
}

template <class Key, class Hash, class Pred> bool tr::atlas_rects<Key, Hash, Pred>::contains(const auto& key) const
{
	return rects.contains(key);
}

template <class Key, class Hash, class Pred> glm::vec2 tr::atlas_rects<Key, Hash, Pred>::operator[](const auto& key) const
{
	return unnormalized(key) / glm::vec2{packer.size()};
}

template <class Key, class Hash, class Pred> glm::u16vec2 tr::atlas_rects<Key, Hash, Pred>::unnormalized(const auto& key) const
{
	TR_ASSERT(contains(key), "Tried to get a rect at a nonexistant key from an atlas packer.");

	return *rects.find(key);
}

template <class Key, class Hash, class Pred> void tr::atlas_rects<Key, Hash, Pred>::clear()
{
	packer.clear();
	rects.clear();
}

template <class Key, class Hash, class Pred> void tr::atlas_rects<Key, Hash, Pred>::resize(glm::u16vec2 size)
{
	packer.resize(size);
}

template <class Key, class Hash, class Pred> bool tr::atlas_rects<Key, Hash, Pred>::try_insert(auto&& key, glm::u16vec2 size)
{
	TR_ASSERT(!contains(key), "Tried to insert a rect with the same key as an existing rect into an atlas packer.");

	std::optional<glm::u16vec2> packing_result{packer.try_insert(size)};
	if (packing_result.has_value()) {
		rects.emplace(std::move(key), size);
		return true;
	}
	else {
		return false;
	}
}