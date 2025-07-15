#pragma once
#include "../utility/hashmap.hpp"
#include "texture.hpp"

namespace tr {
	// Atlas bitmap with named entries.
	struct atlas_bitmap {
		// The atlas bitmap.
		bitmap bitmap;
		// The atlas entries.
		string_hash_map<irect2> entries;
	};
	// Builds an atlas bitmap.
	atlas_bitmap build_atlas_bitmap(const string_hash_map<bitmap>& bitmaps, pixel_format format = pixel_format::RGBA32) noexcept;

	// Static texture atlas.
	class static_atlas : public texture {
	  public:
		// Uploads a pre-made atlas bitmap.
		static_atlas(const atlas_bitmap& atlas_bitmap) noexcept;
		// Creates an atlas from a list of named bitmaps.
		static_atlas(const string_hash_map<bitmap>& bitmaps) noexcept;

		// Gets whether the atlas contains a texture.
		bool contains(std::string_view name) const noexcept;
		// Returns the rect associated with an entry.
		const frect2& operator[](std::string_view name) const noexcept;

	  private:
		// Normalized rects of the atlas textures.
		string_hash_map<frect2> _entries;

		using texture::clear;
		using texture::clear_region;
		using texture::copy_region;
		using texture::set_region;
	};

	// Dynamically-allocated texture atlas.
	class dyn_atlas {
	  public:
		// Creates an empty atlas.
		dyn_atlas() noexcept;
		// Creates an empty atlas with an initial size.
		dyn_atlas(glm::ivec2 size) noexcept;

		// Gets the atlas texture.
		operator const texture&() const noexcept;

		// Sets the filters used by the atlas texture sampler.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept;

		// Gets the size of the atlas texture.
		glm::ivec2 size() const noexcept;
		// Gets whether the atlas contains an entry.
		bool contains(std::string_view name) const noexcept;
		// Gets the number of entries in the atlas.
		std::size_t entries() const noexcept;

		// Returns the rect associated with an entry.
		frect2 operator[](std::string_view name) const noexcept;
		// Returns the unnormalized rect associated with an entry.
		const irect2& raw_at(std::string_view name) const noexcept;

		// Reserves a certain amount of space in the bitmap.
		void reserve(glm::ivec2 capacity) noexcept;

		// Adds an entry to the atlas.
		void add(const std::string& name, const sub_bitmap& bitmap) noexcept;
		// Adds an entry to the atlas.
		void add(std::string&& name, const sub_bitmap& bitmap) noexcept;

		// Removes an entry from the atlas.
		void remove(std::string_view name) noexcept;
		// Removes all entries from the atlas.
		void clear() noexcept;

		// Sets the debug label of the atlas texture.
		void set_label(const std::string& label) noexcept;
		// Sets the debug label of the atlas texture.
		void set_label(std::string&& label) noexcept;

	  private:
		// The atlas texture.
		texture _tex;
		// List of occupied rects.
		string_hash_map<irect2> _entries;
		// List of rects of free space.
		std::forward_list<irect2> _free_rects;
		// The texture label of the atlas.
		std::string _label;

		// Does not append new free rects unlike the exposed function.
		void _raw_reserve(glm::ivec2 capacity) noexcept;
		// Finds the predecessor to the first free rect, reallocating the atlas texture if needed until a suitable rect is available.
		std::forward_list<irect2>::iterator _find_free_rect_prev(glm::ivec2 size) noexcept;
	};
} // namespace tr