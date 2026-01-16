///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides utilities for packing textures into an atlas.                                                                                //
//                                                                                                                                       //
// tr::atlas_packer serves as an implementation of the skyline packing algorithm; it is almost always used in tandem with something to   //
// store the calculated rectangles (like tr::atlas_rects). Inserting a new rectangle is done with the .try_insert() method, which        //
// returns the position of top-left corner of the rectangle, or std::nullopt if no suitable spot was found for it. The packer can be     //
// cleared with the .clear() method:                                                                                                     //
//     - tr::atlas_packer packer{} -> creates an empty packer                                                                            //
//     - packer.try_insert({32, 32}, {256, 256})                                                                                         //
//       -> inserts a 32x32 rectangle into the packer (within a 256x256 limit) and returns the top-left corner of the rectangle          //
//     - packer.try_insert({512, 256}, {256, 256}) -> std::nullopt                                                                       //
//     - packer.clear() -> packer is now empty again                                                                                     //
// NOTE: the packer assumes the texture is of constant size or growing.                                                                  //
//                                                                                                                                       //
// tr::atlas_rects<Key> extends the atlas packer by storing the rectangles inserted with .try_insert() in a map with an arbitrary key.   //
// This map can be queried, or cleared with the rest of the packer:                                                                      //
//     - tr::atlas_rects<int> packer{} -> creates an empty packer                                                                        //
//     - packer.try_insert(10, {32, 32}, {256, 256}) -> inserts a rectangle with the key '10' into the packer                            //
//     - packer.contains(10) -> true                                                                                                     //
//     - packer.entries() -> 1                                                                                                           //
//     - packer[10] -> {{0, 0}, {32, 32}}                                                                                                //
//     - packer.clear() -> packer is now empty again                                                                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "geometry.hpp"
#include "macro.hpp"

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

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
		template <class T> std::optional<glm::u16vec2> try_insert(T&& key, glm::u16vec2 size, glm::u16vec2 texture_size);

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
template <class T>
std::optional<glm::u16vec2> tr::atlas_rects<Key, Hash, Pred>::try_insert(T&& key, glm::u16vec2 size, glm::u16vec2 texture_size)
{
	TR_ASSERT(!contains(key), "Tried to insert a rect with the same key as an existing rect into an atlas packer.");

	std::optional<glm::u16vec2> packing_result{m_packer.try_insert(size, texture_size)};
	if (packing_result.has_value()) {
		m_rects.emplace(std::forward<T>(key), rect2<u16>{*packing_result, size});
	}
	return packing_result;
}