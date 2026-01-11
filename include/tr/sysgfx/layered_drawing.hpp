///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides facilities for using multiple layered renderers in tandem.                                                                   //
//                                                                                                                                       //
// A layered renderer is a renderer that groups objects into discrete layers. In order to be able to define helper functions for working //
// with multiple layered renderers at once efficiently, a few standard interfaces are defined:                                           //
// A layer-drawable renderer is one which defines .draw_layer(layer, target). Such a renderer can be used with tr::gfx::draw_layer.      //
// A layer range-drawable renderer is one which defines .prepare_staggered_draw_range(min, max) that returns a staggered draw manager.   //
// Such a renderer can be used with tr::gfx::draw_layer_range.                                                                           //
// A staggered draw manager is a class which defines .draw_layer(layer, target). Renderers may batch data from multiple layers together  //
// as an optimization, which would be difficult to reconcile with having to call .draw_layer() one at a time.                            //
// Thus, tr::gfx::draw_layer_range instead pulls the staggered draw managers from every class, allowing them to batch the data knowing   //
// which layers will be drawn in advance, then iterates on the layers using those managers.                                              //
//                                                                                                                                       //
// tr::gfx::draw_layer is used to draw a single layer from multiple renderers (in the order they're passed):                             //
//     - tr::gfx::draw_layer(4, basic_renderer, circle_renderer) -> draws layer 4 of the basic renderer, then the circle renderer        //
//                                                                                                                                       //
// tr::gfx::draw_layer_range is used to draw layers [min_layer, max_layer] from multiple renderers (in the order they're passed):        //
//     - tr::gfx::draw_layer_range(0, 5, basic_renderer, circle_renderer)                                                                //
//       -> draws layer 0 of the basic renderer, then layer 0 of the circle renderer, then layer 1 of the basic renderer, and so on...   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "render_target.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Concept denoting a renderer which can draw a layer.
	template <class T>
	concept layer_drawable_renderer = requires(T renderer, int layer, const render_target& target) { renderer.draw_layer(layer, target); };
	// Concept denoting a staggered drawing manager used in draw_layer_range.
	template <class T>
	concept staggered_draw_manager = requires(T obj, int layer, const render_target& target) { obj.draw_layer(layer, target); };
	// Concept denoting a renderer capable of preparing a staggered draw manager.
	template <class T>
	concept layer_range_drawable_renderer = requires(T renderer) {
		{ renderer.prepare_staggered_draw_range(0, 0) } -> staggered_draw_manager;
	};

	// Draws a layer using multiple renderers to a target.
	template <layer_drawable_renderer... Renderers> void draw_layer(int layer, const render_target& target, Renderers&&... renderers);
	// Draws a range of layers using multiple renderers to a target.
	template <layer_range_drawable_renderer... Renderers>
	void draw_layer_range(int min_layer, int max_layer, const render_target& target, Renderers&&... renderers);
} // namespace tr::gfx

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <tr::gfx::layer_drawable_renderer... Renderers>
void tr::gfx::draw_layer(int layer, const render_target& target, Renderers&&... renderers)
{
	(renderers.draw_layer(layer, target), ...);
}

template <tr::gfx::layer_range_drawable_renderer... Renderers>
void tr::gfx::draw_layer_range(int min_layer, int max_layer, const render_target& target, Renderers&&... renderers)
{
	TR_ASSERT(max_layer >= min_layer, "Tried to draw invalid layer range [{}, {}].", min_layer, max_layer);

	std::tuple managers{renderers.prepare_staggered_draw_range(min_layer, max_layer)...};
	for (int layer = min_layer; layer <= max_layer; ++layer) {
		std::apply([&, layer](auto&... managers) { (managers.draw_layer(layer, target), ...); }, managers);
	}
}