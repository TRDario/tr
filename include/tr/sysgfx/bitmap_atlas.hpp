/// @file
/// @brief Provides `tr::bitmap_atlas`.

#pragma once
#include <tr/sysgfx/bitmap.hpp>
#include <tr/utility/atlas_entries.hpp>

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
	/// @param bitmaps Map holding the bitmaps to stitch together.
	/// @return Bitmap atlas built from the individual bitmaps.
	template <typename Key, hasher<Key> Hash = boost::hash<Key>, equality_predicate<Key> Pred = std::equal_to<Key>>
	[[nodiscard]] bitmap_atlas<Key, void, Hash, Pred> build_bitmap_atlas(const boost::unordered_flat_map<Key, bitmap, Hash, Pred>& bitmaps)
	{
		glm::ivec2 size{};
		atlas_entries<Key, void, Hash, Pred> entries;
		for (auto& [key, entry] : bitmaps) {
			std::optional<glm::u16vec2> packing_result{entries.try_insert(std::move(key), entry.size(), size)};
			if (!packing_result.has_value()) {
				if (size == glm::ivec2{}) {
					const glm::uvec2 usize{entry.size()};
					size = {std::bit_ceil(usize.x + 1), std::bit_ceil(usize.y + 1)};
					packing_result = entries.try_insert(std::move(key), usize, size);
				}
				else {
					do {
						size.y < size.x ? size.y *= 2 : size.x *= 2;
					} while (!(packing_result = entries.try_insert(std::move(key), entry.size(), size)).has_value());
				}
			}
		}
		bitmap bitmap{size};
		for (const Key& k : std::views::keys(bitmaps)) {
			bitmap.blit(entries[k].tl, get(bitmaps, k));
		}
		return {std::move(bitmap), std::move(bitmaps)};
	}

	/// @}
} // namespace tr