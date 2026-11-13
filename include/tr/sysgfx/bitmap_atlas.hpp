/// @file
/// @brief Provides `tr::bitmap_atlas`.

#pragma once
#include <tr/sysgfx/bitmap.hpp>
#include <tr/utility/atlas_entries.hpp>

//

namespace tr
{
	/// @brief Bitmap atlas structure.
	/// @tparam Key Atlas key type.
	/// @tparam Value Atlas value type.
	/// @tparam Hash Atlas key hasher.
	/// @tparam Pred Atlas key equality predicate.
	template <typename Key, atlas_entries_value_type Value, hasher<Key> Hash = boost::hash<Key>,
			  equality_predicate<Key> Pred = std::equal_to<Key>>
	struct bitmap_atlas
	{
		/// @brief Atlas bitmap.
		bitmap bitmap;

		/// @brief Atlas entries.
		atlas_entries<Key, Value, Hash, Pred> rectangles;
	};

	//

	/// @brief Builds a bitmap atlas from individual bitmaps.
	/// @tparam Key Atlas key type.
	/// @tparam Hash Atlas key hasher.
	/// @tparam Pred Atlas key equality predicate.
	/// @param bitmaps Map holding the bitmaps to stitch together.
	/// @par Complexity
	/// Linear time with respect to the number of bitmaps.
	/// @par Exception safety
	/// If an exception is thrown, this function has no effect (strong exception guarantee).
	/// @return Bitmap atlas built from the individual bitmaps.
	template <typename Key, hasher<Key> Hash = boost::hash<Key>, equality_predicate<Key> Pred = std::equal_to<Key>>
	[[nodiscard]] bitmap_atlas<Key, void, Hash, Pred> build_bitmap_atlas(const boost::unordered_flat_map<Key, bitmap, Hash, Pred>& bitmaps)
	{
		glm::ivec2 size{};
		atlas_entries<Key, void, Hash, Pred> entries;
		for (auto& [key, entry] : bitmaps) {
			std::optional<glm::u16vec2> packing_result{entries.try_insert(std::move(key), entry.size(), size)};
			if (!packing_result) {
				if (size == glm::ivec2{0, 0}) {
					const glm::uvec2 usize{entry.size()};
					size = {std::bit_ceil(usize.x + 1), std::bit_ceil(usize.y + 1)};
					packing_result = entries.try_insert(std::move(key), usize, size);
				}
				else {
					do {
						size.y < size.x ? size.y *= 2 : size.x *= 2;
					} while (!(packing_result = entries.try_insert(std::move(key), entry.size(), size)));
				}
			}
		}
		bitmap bitmap{size};
		for (const Key& k : std::views::keys(bitmaps)) {
			bitmap.blit(entries[k].tl, get(bitmaps, k));
		}
		return bitmap_atlas<Key, void, Hash, Pred>{std::move(bitmap), std::move(bitmaps)};
	}
} // namespace tr