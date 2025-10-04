#pragma once
#include "texture.hpp"

namespace tr::gfx {
	// Basic bitmap atlas structure.
	template <class Key, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>> struct bitmap_atlas {
		// The atlas bitmap.
		tr::bitmap bitmap;
		// The atlas entries.
		atlas_rects<Key, Hash, Pred> rects;
	};
	// Builds a bitmap atlas from individual bitmaps.
	template <class Key, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>>
	bitmap_atlas<Key, Hash, Pred> build_bitmap_atlas(const std::unordered_map<Key, tr::bitmap, Hash, Pred>& entries);

	// Dynamically-allocated texture atlas.
	template <class Key, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>> class dyn_atlas {
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

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class Key, class Hash, class Pred>
tr::gfx::bitmap_atlas<Key, Hash, Pred> tr::gfx::build_bitmap_atlas(const std::unordered_map<Key, tr::bitmap, Hash, Pred>& entries)
{
	glm::ivec2 size{};
	atlas_rects<Key, Hash, Pred> rects;
	for (auto& [key, entry] : entries) {
		std::optional<glm::u16vec2> packing_result{rects.try_insert(std::move(key), entry.size(), size)};
		if (!packing_result.has_value()) {
			if (size == glm::ivec2{}) {
				const glm::uvec2 usize{entry.size()};
				size = {std::bit_ceil(usize.x + 1), std::bit_ceil(usize.y + 1)};
				packing_result = rects.try_insert(std::move(key), usize, size);
			}
			else {
				do {
					size.y < size.x ? size.y *= 2 : size.x *= 2;
				} while (!(packing_result = rects.try_insert(std::move(key), entry.size(), size)).has_value());
			}
		}
	}
	tr::bitmap bitmap{size};
	for (const Key& k : std::views::keys(entries)) {
		bitmap.blit(rects[k].tl, entries.at(k));
	}
	return {std::move(bitmap), std::move(rects)};
}

template <class Key, class Hash, class Pred>
tr::gfx::dyn_atlas<Key, Hash, Pred>::dyn_atlas(glm::ivec2 size)
	: m_tex{size, true}
{
}

template <class Key, class Hash, class Pred>
tr::gfx::dyn_atlas<Key, Hash, Pred>::dyn_atlas(bitmap_atlas<Key, Hash, Pred>&& source)
	: m_tex{source.bitmap, true}, m_rects{std::move(source.rects)}
{
}

template <class Key, class Hash, class Pred> tr::gfx::dyn_atlas<Key, Hash, Pred>::operator const tr::gfx::texture&() const
{
	return m_tex;
}

template <class Key, class Hash, class Pred> tr::gfx::dyn_atlas<Key, Hash, Pred>::operator tr::gfx::texture_ref() const
{
	return m_tex;
}

template <class Key, class Hash, class Pred>
void tr::gfx::dyn_atlas<Key, Hash, Pred>::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	m_tex.set_filtering(min_filter, mag_filter);
}

template <class Key, class Hash, class Pred> glm::ivec2 tr::gfx::dyn_atlas<Key, Hash, Pred>::size() const
{
	return m_tex.size();
}

template <class Key, class Hash, class Pred> bool tr::gfx::dyn_atlas<Key, Hash, Pred>::contains(const auto& key) const
{
	return m_rects.contains(key);
}

template <class Key, class Hash, class Pred> tr::usize tr::gfx::dyn_atlas<Key, Hash, Pred>::entries() const
{
	return m_rects.entries();
}

template <class Key, class Hash, class Pred> tr::frect2 tr::gfx::dyn_atlas<Key, Hash, Pred>::operator[](const auto& key) const
{
	frect2 rect{unnormalized(key)};
	rect.tl /= m_tex.size();
	rect.size /= m_tex.size();
	return rect;
}

template <class Key, class Hash, class Pred> tr::irect2 tr::gfx::dyn_atlas<Key, Hash, Pred>::unnormalized(const auto& key) const
{
	TR_ASSERT(contains(key), "Tried to get nonexistent dynamic atlas entry.");

	return m_rects[key];
}

template <class Key, class Hash, class Pred> void tr::gfx::dyn_atlas<Key, Hash, Pred>::reserve(glm::ivec2 capacity)
{
	if (capacity.x < size().x && capacity.y < size().y) {
		return;
	}
	else if (m_tex.size() == glm::ivec2{}) {
		m_tex.reallocate(capacity);
		m_tex.clear({});
	}
	else {
		const texture old_tex{m_tex.reallocate(capacity)};
		m_tex.clear({});
		m_tex.copy_region({}, old_tex, {{}, old_tex.size()});
	}
}

template <class Key, class Hash, class Pred> void tr::gfx::dyn_atlas<Key, Hash, Pred>::add(auto&& key, const sub_bitmap& bitmap)
{
	std::optional<glm::u16vec2> packing_result{m_rects.try_insert(std::move(key), bitmap.size(), size())};
	if (!packing_result.has_value()) {
		glm::u16vec2 new_size;
		if (m_tex.size() == glm::ivec2{}) {
			const glm::uvec2 usize{bitmap.size()};
			new_size = {std::bit_ceil(usize.x + 1), std::bit_ceil(usize.y + 1)};
			packing_result = m_rects.try_insert(std::move(key), usize, new_size);
		}
		else {
			new_size = size();
			do {
				new_size.y < new_size.x ? new_size.y *= 2 : new_size.x *= 2;
			} while (!(packing_result = m_rects.try_insert(std::move(key), bitmap.size(), new_size)).has_value());
		}
		reserve(new_size);
	}
	m_tex.set_region(*packing_result, bitmap);
}

template <class Key, class Hash, class Pred> void tr::gfx::dyn_atlas<Key, Hash, Pred>::clear()
{
	if (size() != glm::ivec2{0}) {
		m_tex.clear({});
		m_rects.clear();
	}
}

#ifdef TR_ENABLE_ASSERTS
template <class Key, class Hash, class Pred> std::string tr::gfx::dyn_atlas<Key, Hash, Pred>::label() const
{
	return m_tex.label();
}

template <class Key, class Hash, class Pred> void tr::gfx::dyn_atlas<Key, Hash, Pred>::set_label(std::string_view label)
{
	m_tex.set_label(label);
}
#endif
