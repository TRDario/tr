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
// By default, only the rects are stored in the object, but if Extra is set to non-void, that extra data is stored alongside the rect.   //
// This map can be queried, or cleared with the rest of the packer:                                                                      //
//     - tr::atlas_rects<int> packer{} -> creates an empty packer                                                                        //
//     - packer.try_insert(10, {32, 32}, {256, 256}) -> inserts a rectangle with the key '10' into the packer                            //
//     - packer.contains(10) -> true                                                                                                     //
//     - packer.entries() -> 1                                                                                                           //
//     - packer[10] -> {{0, 0}, {32, 32}}                                                                                                //
//     - packer.clear() -> packer is now empty again                                                                                     //
//     - tr::atlas_rects<int, char> packer2{}; packer2.try_insert(5, {32, 32}, {256, 256}, 'A'); packer2.get_with_extra(5).extra -> 'A'  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "geometry.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

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

	// Expanded atlas rect value.
	template <typename Extra> struct expanded_atlas_rect {
		// The rect itself.
		rect2<u16> rect;
		// Extra data associated with the rect.
		Extra extra;
	};

	// Atlas packer combined with a list of rects.
	template <typename Key, typename Extra = void, hasher<Key> Hash = boost::hash<Key>, equality_predicate<Key> Pred = std::equal_to<Key>>
	class atlas_rects {
	  public:
		using value_type = std::conditional_t<std::same_as<Extra, void>, rect2<u16>, expanded_atlas_rect<Extra>>;

		// Gets whether the atlas contains a key.
		template <hash_keylike<Key, Hash, Pred> Keylike> bool contains(Keylike&& key) const;
		// Gets the number of entries in the atlas.
		usize entries() const;

		// Gets a rect associated with a certain key.
		template <hash_keylike<Key, Hash, Pred> Keylike> rect2<u16> operator[](Keylike&& key) const;
		// Gets a rect associated with a certain key along with its associated extra data.
		template <hash_keylike<Key, Hash, Pred> Keylike>
			requires(!std::same_as<Extra, void>)
		const value_type& get_with_extra(Keylike&& key) const;

		// Clears the packer.
		void clear();
		// Tries to insert a rectangle.
		template <hash_keylike<Key, Hash, Pred> Keylike>
			requires(std::same_as<Extra, void>)
		std::optional<glm::u16vec2> try_insert(Keylike&& key, glm::u16vec2 size, glm::u16vec2 texture_size);
		// Tries to insert a rectangle.
		template <hash_keylike<Key, Hash, Pred> Keylike, typename... Args>
			requires(std::constructible_from<Extra, Args...>)
		std::optional<glm::u16vec2> try_insert(Keylike&& key, glm::u16vec2 size, glm::u16vec2 texture_size, Args&&... extra_args);

	  private:
		// The atlas packer.
		atlas_packer m_packer;
		// The atlas rects.
		boost::unordered_flat_map<Key, value_type, Hash, Pred> m_rects;
	};
} // namespace tr

#include "impl/atlas_packer.hpp" // IWYU pragma: export