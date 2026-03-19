///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides atlas textures.                                                                                                              //
//                                                                                                                                       //
// A hash map of bitmaps can be stitched together into a single atlased bitmap using tr::build_bitmap_atlas:                             //
//     - tr::string_flat_map<tr::bitmap> bitmaps{{"a", tr::load_bitmap_file("a.bmp")}, {"b", tr::load_bitmap_file("b.bmp")}}             //
//       tr::bitmap_atlas atlas{tr::build_bitmap_atlas(bitmaps)}                                                                         //
//       -> stitches all bitmaps in 'bitmaps' into a single bitmap contained in atlas.bitmap, with information on where the constituents //
//          are located in atlas.rects                                                                                                   //
//                                                                                                                                       //
// tr::gfx::dyn_atlas abstracts over a tr::gfx::texture to provide an atlas interface, automatically handling insertion, removal,        //
// resizing, and so on of the underlying texture. A dynamic atlas can be created empty, with an initial reserved size, or using a        //
// pre-assembled bitmap atlas as a source. The .reserve() method can also be called at any time to reserve texture space:                //
//     - tr::gfx::dyn_atlas{} -> creates an empty atlas                                                                                  //
//     - tr::gfx::dyn_atlas{{512, 512}} -> creates an empty atlas with a pre-allocated 512x512 texture                                   //
//     - tr::gfx::dyn_atlas atlas{}; atlas.reserve({512, 512}) -> equivalent to the above                                                //
//     - tr::gfx::dyn_atlas{bitmaps} -> uploads the 'bitmaps' atlas into a texture and takes its rect information                        //
//                                                                                                                                       //
// The underlying bitmap can be accessed with an implicit conversion, but only in a read-only manner. Filtering can be set with          //
// .set_filtering(), as in a regular texture.                                                                                            //
//                                                                                                                                       //
// Entries in the atlas can be checked for and accessed: using operator[] gets the normalized uv of the entry, while .unnormalized()     //
// gets the unnormalized region in the atlas. The total number of entries in the atlas can be obtained with .entries(), while the        //
// size of the atlas texture in pixels can be obtained with .size(). An entry is added into the atlas with .add(), and the atlas can be  //
// cleared with the .clear() method:                                                                                                     //
//     - atlas.size() -> {512, 512}                                                                                                      //
//     - atlas.add("C", tr::load_bitmap_file("c.bmp")) -> adds "C" to the atlas                                                          //
//     - atlas.entries() -> 1                                                                                                            //
//     - atlas.contains("C") -> true                                                                                                     //
//     - atlas["C"] -> gets the normalized UV rect of "C"                                                                                //
//     - atlas.unnormalized("C") -> gets the unnormalized UV rect of "C"                                                                 //
//     - atlas.clear() -> atlas is now empty again, but retains the reserved space                                                       //
//                                                                                                                                       //
// The label of an atlas can be set with TR_SET_LABEL(atlas, label):                                                                     //
//     - TR_SET_LABEL(atlas, "Example atlas") -> 'atlas' is now labelled "Example atlas"                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/atlas_packer.hpp"
#include "texture.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Basic bitmap atlas structure.
	template <typename Key, typename Hash = boost::hash<Key>, typename Pred = std::equal_to<>> struct bitmap_atlas {
		// The atlas bitmap.
		tr::bitmap bitmap;
		// The atlas entries.
		atlas_rects<Key, Hash, Pred> rects;
	};
	// Builds a bitmap atlas from individual bitmaps.
	template <typename Key, typename Hash = boost::hash<Key>, typename Pred = std::equal_to<>>
	bitmap_atlas<Key, Hash, Pred> build_bitmap_atlas(const boost::unordered_flat_map<Key, tr::bitmap, Hash, Pred>& entries);
} // namespace tr

namespace tr::gfx {
	// Dynamically-allocated texture atlas.
	template <typename Key, typename Hash = boost::hash<Key>, typename Pred = std::equal_to<>> class dyn_atlas {
	  public:
		// Creates an empty atlas.
		dyn_atlas() = default;
		// Creates an empty atlas with an initial size.
		dyn_atlas(glm::ivec2 size);
		// Uploads a bitmap atlas.
		dyn_atlas(bitmap_atlas<Key, Hash, Pred>&& source);

		// Gets the atlas texture.
		operator const texture&() const;
		// Gets a reference to the atlas texture.
		operator texture_ref() const;

		// Sets the filters used by the atlas texture sampler.
		void set_filtering(min_filter min_filter, mag_filter mag_filter);

		// Gets the size of the atlas texture.
		glm::ivec2 size() const;
		// Gets whether the atlas contains an entry.
		bool contains(const auto& key) const;
		// Gets the number of entries in the atlas.
		usize entries() const;

		// Returns the rect associated with an entry.
		frect2 operator[](const auto& key) const;
		// Returns the unnormalized rect associated with an entry.
		irect2 unnormalized(const auto& key) const;

		// Reserves a certain amount of space in the bitmap.
		void reserve(glm::ivec2 capacity);

		// Adds an entry to the atlas.
		void add(auto&& key, const sub_bitmap& bitmap);

		// Removes all entries from the atlas.
		void clear();

#ifdef TR_ENABLE_ASSERTS
		// Gets the debug label of the atlas.
		std::string label() const;
		// Sets the debug label of the atlas.
		void set_label(std::string_view label);
#endif

	  private:
		// The atlas texture.
		texture m_tex;
		// The atlas entries.
		atlas_rects<Key, Hash, Pred> m_rects;
	};
} // namespace tr::gfx

#include "impl/atlas.hpp" // IWYU pragma: export