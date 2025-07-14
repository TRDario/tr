#pragma once
#include "render_target.hpp"

namespace tr {
	// The display backbuffer.
	struct backbuffer {
		// Gets the size of the backbuffer.
		static glm::ivec2 size() noexcept;

		// Gets a render target spanning the entire backbuffer.
		static render_target render_target() noexcept;
		// Gets a render target spanning a region of the backbuffer.
		static tr::render_target region_render_target(const irect2& rect) noexcept;

		// Clears the backbuffer's color.
		static void clear(const tr::rgbaf& color = {0, 0, 0, 0}) noexcept;
		// Clears the backbuffer.
		static void clear(const tr::rgbaf& color, double depth, int stencil) noexcept;
		// Clears a backbuffer region's color.
		static void clear_region(const tr::irect2& rect, const tr::rgbaf& color = {0, 0, 0, 0}) noexcept;
		// Clears a backbuffer region.
		static void clear_region(const tr::irect2& rect, const tr::rgbaf& color, double depth, int stencil) noexcept;

		// Swaps the front- and backbuffer.
		static void flip() noexcept;
	};
} // namespace tr