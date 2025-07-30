#pragma once
#include "texture.hpp"
#include "texture_unit.hpp"

namespace tr {
	// Dynamically-allocated texture atlas.
	template <class Key, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>> class dyn_atlas {
	  public:
		// Creates an empty atlas.
		dyn_atlas() = default;
		// Creates an empty atlas with an initial size.
		dyn_atlas(glm::ivec2 size);

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
		std::size_t entries() const;

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

		// Sets the debug label of the atlas texture.
		void set_label(const std::string& label);
		// Sets the debug label of the atlas texture.
		void set_label(std::string&& label);

	  private:
		// The atlas texture.
		texture tex;
		// The atlas entries.
		atlas_rects<Key, Hash, Pred> rects;
		// The texture label of the atlas.
		std::string label;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class Key, class Hash, class Pred>
tr::dyn_atlas<Key, Hash, Pred>::dyn_atlas(glm::ivec2 size)
	: tex{size, true}
{
}

template <class Key, class Hash, class Pred> tr::dyn_atlas<Key, Hash, Pred>::operator const tr::texture&() const
{
	return tex;
}

template <class Key, class Hash, class Pred> tr::dyn_atlas<Key, Hash, Pred>::operator tr::texture_ref() const
{
	return tex;
}

template <class Key, class Hash, class Pred>
void tr::dyn_atlas<Key, Hash, Pred>::set_filtering(min_filter min_filter, mag_filter mag_filter)
{
	tex.set_filtering(min_filter, mag_filter);
}

template <class Key, class Hash, class Pred> glm::ivec2 tr::dyn_atlas<Key, Hash, Pred>::size() const
{
	return tex.size();
}

template <class Key, class Hash, class Pred> bool tr::dyn_atlas<Key, Hash, Pred>::contains(const auto& key) const
{
	return rects.contains(key);
}

template <class Key, class Hash, class Pred> std::size_t tr::dyn_atlas<Key, Hash, Pred>::entries() const
{
	return rects.entries();
}

template <class Key, class Hash, class Pred> tr::frect2 tr::dyn_atlas<Key, Hash, Pred>::operator[](const auto& key) const
{
	frect2 rect{unnormalized(key)};
	rect.tl /= tex.size();
	rect.size /= tex.size();
	return rect;
}

template <class Key, class Hash, class Pred> tr::irect2 tr::dyn_atlas<Key, Hash, Pred>::unnormalized(const auto& key) const
{
	TR_ASSERT(contains(key), "Tried to get nonexistent dynamic atlas entry.");

	return rects[key];
}

template <class Key, class Hash, class Pred> void tr::dyn_atlas<Key, Hash, Pred>::reserve(glm::ivec2 capacity)
{
	if (capacity.x < size().x && capacity.y < size().y) {
		return;
	}
	else if (tex.size() == glm::ivec2{}) {
		tex.allocate(capacity, true, pixel_format::RGBA32);
	}
	else {
		texture new_tex{capacity};
		new_tex.copy_region({}, tex, {{}, tex.size()});
		replace_texture_bindings(tex, new_tex);
		tex = std::move(new_tex);
		if (!label.empty()) {
			tex.set_label(label);
		}
	}
}

template <class Key, class Hash, class Pred> void tr::dyn_atlas<Key, Hash, Pred>::add(auto&& key, const sub_bitmap& bitmap)
{
	std::optional<glm::u16vec2> packing_result{rects.try_insert(std::move(key), bitmap.size(), size())};
	if (!packing_result.has_value()) {
		glm::u16vec2 new_size;
		if (tex.size() == glm::ivec2{}) {
			const glm::uvec2 usize{static_cast<glm::u16vec2>(bitmap.size())};
			new_size = {std::bit_ceil(usize.x), std::bit_ceil(usize.y)};
			packing_result = rects.try_insert(std::move(key), usize, new_size);
		}
		else {
			new_size = size();
			do {
				new_size.y < new_size.x ? new_size.y *= 2 : new_size.x *= 2;
			} while (!(packing_result = rects.try_insert(std::move(key), static_cast<glm::u16vec2>(bitmap.size()), new_size)).has_value());
		}
		reserve(new_size);
	}
	tex.set_region(*packing_result, bitmap);
}

template <class Key, class Hash, class Pred> void tr::dyn_atlas<Key, Hash, Pred>::clear()
{
	if (size() != glm::ivec2{0}) {
		tex.clear({});
		rects.clear();
	}
}

template <class Key, class Hash, class Pred> void tr::dyn_atlas<Key, Hash, Pred>::set_label(const std::string& new_label)
{
	label = new_label;
	tex.set_label(new_label);
}

template <class Key, class Hash, class Pred> void tr::dyn_atlas<Key, Hash, Pred>::set_label(std::string&& new_label)
{
	label = std::move(new_label);
	tex.set_label(label);
}
