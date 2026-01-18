///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functions to manipulate the window backbuffer.                                                                               //
//                                                                                                                                       //
// The backbuffer can be accessed after opening the window. Its size can be queried, and render targets corresponding to it or a region  //
// within it can be obtained:                                                                                                            //
//     - tr::gfx::backbuffer_size() -> the size of the backbuffer in pixels, may not match the reported window size                      //
//     - tr::gfx::backbuffer_render_target() -> a render target spanning the entire backbuffer                                           //
//     - tr::gfx::backbuffer_region_render_target({{100, 100}, {100, 100}})                                                              //
//       -> a render target spanning from (100, 100) to (200, 200) in the backbuffer                                                     //
//                                                                                                                                       //
// The only direct way of manipulating the backbuffer's contents is by clearing it or a region of it. This can be done just for the      //
// color component, or all 3 of the backbuffer components:                                                                               //
//     - tr::gfx::clear_backbuffer() -> clears the backbuffer to transparency                                                            //
//     - tr::gfx::clear_backbuffer({255, 255, 255, 255}) -> clears the backbuffer to white                                               //
//     - tr::gfx::clear_backbuffer({255, 255, 255, 255}, 1.0f, 0) -> clears the backbuffer to white, depth 1.0f, and stencil 0           //
//     - tr::gfx::clear_backbuffer_region({{100, 100}, {100, 100}})                                                                      //
//       -> clears the backbuffer region from (100, 100) to (200, 200) to transparency                                                   //
//     - tr::gfx::clear_backbuffer_region({{100, 100}, {100, 100}}, {255, 0, 0, 255})                                                    //
//       -> clears the backbuffer region from (100, 100) to (200, 200) to red                                                            //
//     - tr::gfx::clear_backbuffer_region({{100, 100}, {100, 100}}, {255, 0, 0, 255}, 1.0f, 0)                                           //
//       -> clears the backbuffer region from (100, 100) to (200, 200) to red, depth 1.0f, and stencil 0                                 //
//                                                                                                                                       //
// To display a frame after it is drawn, the backbuffer must be flipped:                                                                 //
//     - tr::gfx::flip_backbuffer() -> swaps the front- and backbuffer                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/color.hpp"
#include "../utility/geometry.hpp"

namespace tr::gfx {
	class render_target;
}

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
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