///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements atlas.hpp.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../atlas.hpp"

/////////////////////////////////////////////////////////////// BITMAP ATLAS //////////////////////////////////////////////////////////////

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
tr::bitmap_atlas<Key, void, Hash, Pred> tr::build_bitmap_atlas(const boost::unordered_flat_map<Key, tr::bitmap, Hash, Pred>& entries)
{
	glm::ivec2 size{};
	atlas_rects<Key, void, Hash, Pred> rects;
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
		bitmap.blit(rects[k].tl, get(entries, k));
	}
	return {std::move(bitmap), std::move(rects)};
}

////////////////////////////////////////////////////////////// DYNAMIC ATLAS //////////////////////////////////////////////////////////////

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
tr::gfx::dyn_atlas<Key, Hash, Pred>::dyn_atlas(glm::ivec2 size)
	: m_tex{size, tr::gfx::mipmaps::enabled}
{
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
tr::gfx::dyn_atlas<Key, Hash, Pred>::dyn_atlas(bitmap_atlas<Key, void, Hash, Pred>&& source)
	: m_tex{source.bitmap, tr::gfx::mipmaps::enabled}, m_rects{std::move(source.rects)}
{
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
tr::gfx::dyn_atlas<Key, Hash, Pred>::operator const tr::gfx::texture&() const
{
	return m_tex;
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
tr::gfx::dyn_atlas<Key, Hash, Pred>::operator tr::gfx::texture_ref() const
{
	return m_tex;
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
void tr::gfx::dyn_atlas<Key, Hash, Pred>::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	m_tex.set_filtering(min_filter, mag_filter);
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred> glm::ivec2 tr::gfx::dyn_atlas<Key, Hash, Pred>::size() const
{
	return m_tex.size();
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
bool tr::gfx::dyn_atlas<Key, Hash, Pred>::contains(Keylike&& key) const
{
	return m_rects.contains(std::forward<Keylike>(key));
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
tr::usize tr::gfx::dyn_atlas<Key, Hash, Pred>::entries() const
{
	return m_rects.entries();
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
tr::frect2 tr::gfx::dyn_atlas<Key, Hash, Pred>::operator[](Keylike&& key) const
{
	frect2 rect{unnormalized(std::forward<Keylike>(key))};
	rect.tl /= m_tex.size();
	rect.size /= m_tex.size();
	return rect;
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
template <tr::hash_keylike<Key, Hash, Pred> Keylike>
tr::irect2 tr::gfx::dyn_atlas<Key, Hash, Pred>::unnormalized(Keylike&& key) const
{
	TR_ASSERT(contains(key), "Tried to get nonexistent dynamic atlas entry.");

	return m_rects[std::forward<Keylike>(key)];
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
void tr::gfx::dyn_atlas<Key, Hash, Pred>::reserve(glm::ivec2 capacity)
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

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
void tr::gfx::dyn_atlas<Key, Hash, Pred>::add(Key&& key, const sub_bitmap& bitmap)
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

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred> void tr::gfx::dyn_atlas<Key, Hash, Pred>::clear()
{
	if (size() != glm::ivec2{0}) {
		m_tex.clear({});
		m_rects.clear();
	}
}

#ifdef TR_ENABLE_ASSERTS
template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
std::string tr::gfx::dyn_atlas<Key, Hash, Pred>::label() const
{
	return m_tex.label();
}

template <typename Key, tr::hasher<Key> Hash, tr::equality_predicate<Key> Pred>
void tr::gfx::dyn_atlas<Key, Hash, Pred>::set_label(std::string_view label)
{
	m_tex.set_label(label);
}
#endif
