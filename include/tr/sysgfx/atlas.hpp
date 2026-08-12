/// @file
/// @brief Provides atlas textures.

#pragma once
#include "../utility/atlas_packer.hpp"
#include "texture.hpp"

//

namespace tr
{
	/// Basic bitmap atlas structure.
	/// @tparam Key Atlas key type.
	/// @tparam Value Atlas value type.
	/// @tparam Hash Atlas key hasher.
	/// @tparam Pred Atlas key equality predicate.
	template <typename Key, atlas_entries_value_type Value, hasher<Key> Hash = boost::hash<Key>,
			  equality_predicate<Key> Pred = std::equal_to<Key>>
	struct bitmap_atlas
	{
		/// Atlas bitmap.
		bitmap bitmap;

		/// Atlas entries.
		atlas_entries<Key, Value, Hash, Pred> rectangles;
	};

	/// @name Bitmap atlas
	/// @{

	/// Builds a bitmap atlas from individual bitmaps.
	/// @tparam Key Atlas key type.
	/// @tparam Hash Atlas key hasher.
	/// @tparam Pred Atlas key equality predicate.
	/// @param entries Map holding the bitmaps to stitch together.
	/// @return Bitmap atlas built from the individual bitmaps.
	template <typename Key, hasher<Key> Hash = boost::hash<Key>, equality_predicate<Key> Pred = std::equal_to<Key>>
	bitmap_atlas<Key, void, Hash, Pred> build_bitmap_atlas(const boost::unordered_flat_map<Key, bitmap, Hash, Pred>& entries);

	/// @}

	/// Dynamically-allocated texture atlas.
	/// @tparam Key Atlas key type.
	/// @tparam Value Atlas value type.
	/// @tparam Hash Atlas key hasher.
	/// @tparam Pred Atlas key equality predicate.
	template <typename Key, atlas_entries_value_type Value, hasher<Key> Hash = boost::hash<Key>,
			  equality_predicate<Key> Pred = std::equal_to<Key>>
	class dyn_atlas
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an empty atlas.
		/// @param context Graphics context to create the atlas on.
		dyn_atlas(graphics_context& context);

		/// Creates an empty atlas with an initial size.
		/// @param context Graphics context to create the atlas on.
		/// @param size Initial size of the atlas.
		dyn_atlas(graphics_context& context, glm::ivec2 size);

		/// Uploads a bitmap atlas.
		/// @param context Graphics context to create the atlas on.
		/// @param source Bitmap atlas to use as a source.
		dyn_atlas(graphics_context& context, bitmap_atlas<Key, Value, Hash, Pred>&& source);

		/// @}
		/// @name Conversion operators
		/// @{

		/// Gets the atlas texture.
		/// @return Reference to the atlas texture.
		operator const texture&() const;

		/// Gets a reference to the atlas texture.
		/// @return View to the atlas texture.
		operator texture_view() const;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the atlas is on.
		/// @return Reference to the graphics context the atlas is on.
		graphics_context& context() const;

		/// @}
		/// @name Attributes
		/// @{

		/// Sets the filters used by the atlas texture sampler.
		/// @param min_filter Minifying filter to use.
		/// @param mag_filter Magnifying filter to use.
		void set_filtering(min_filter min_filter, mag_filter mag_filter);

		/// @}
		/// @name Information
		/// @{

		/// Gets the size of the atlas texture.
		/// @return Size of the atlas texture.
		glm::ivec2 size() const;

		/// Gets whether the atlas contains an entry.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @return `true` if a value associated with `key` existed, `false` otherwise.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		bool contains(Keylike&& key) const;

		/// Gets the number of entries in the atlas.
		/// @return Number of entries in the atlas.
		usize entries() const;

		/// @}
		/// @name Access
		/// @{

		/// Gets a rectangle associated with a certain key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre `key` must be present in the atlas.
		/// @return Constant reference to the value associated with `key`.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		rectangle<float> operator[](Keylike&& key) const;

		/// Gets the raw value associated with a certain key.
		/// @tparam Keylike Type compatible with the atlas hasher and key equality predicate.
		/// @param key Key to look up in the atlas.
		/// @pre `key` must be present in the atlas.
		/// @return Constant reference to the raw value associated with `key`.
		template <hash_keylike<Key, Hash, Pred> Keylike>
		const Value& raw(Keylike&& key) const;

		/// @}
		/// @name Manipulation
		/// @{

		/// Reserves a certain amount of space in the bitmap.
		/// @param capacity New size of the atlas bitmap.
		void reserve(glm::ivec2 capacity);

		/// Adds an entry to the atlas.
		/// @tparam Args Extra argument types.
		/// @param key Key to associate with the entry.
		/// @param bitmap Bitmap to copy into the atlas.
		/// @param args Extra arguments associated with the entry.
		template <typename... Args>
			requires(std::constructible_from<Value, rectangle<u16>, Args...>)
		void add(Key key, sub_bitmap bitmap, Args&&... args);

		/// Removes all entries from the atlas.
		void clear();

		/// @}
		/// @name Label
		/// @{

		/// Gets the debug label of the atlas.
		/// @return Label of the atlas.
		std::string label() const;

		/// Sets the debug label of the atlas.
		/// @param label Label of the atlas.
		void set_label(std::string_view label);

		/// @}

	  private:
		/// Atlas texture.
		texture m_tex;

		/// Atlas entries.
		atlas_entries<Key, Value, Hash, Pred> m_entries;
	};
} // namespace tr

#include "impl/atlas.hpp" // IWYU pragma: export