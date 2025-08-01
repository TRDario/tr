#pragma once
#include "../utility/color.hpp"
#include "../utility/geometry.hpp"

namespace tr::gfx {
	class render_target;

	// Gets the size of the backbuffer.
	glm::ivec2 backbuffer_size();

	// Gets a render target spanning the entire backbuffer.
	render_target backbuffer_render_target();
	// Gets a render target spanning a region of the backbuffer.
	render_target backbuffer_region_render_target(const irect2& rect);

	// Clears the backbuffer's color.
	void clear_backbuffer(const tr::rgbaf& color = {0, 0, 0, 0});
	// Clears the backbuffer.
	void clear_backbuffer(const tr::rgbaf& color, double depth, int stencil);
	// Clears a backbuffer region's color.
	void clear_backbuffer_region(const tr::irect2& rect, const tr::rgbaf& color = {0, 0, 0, 0});
	// Clears a backbuffer region.
	void clear_backbuffer_region(const tr::irect2& rect, const tr::rgbaf& color, double depth, int stencil);

	// Swaps the front- and backbuffer.
	void flip_backbuffer();
} // namespace tr::gfx