///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements layered_drawing.hpp.                                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../layered_drawing.hpp"

///////////////////////////////////////////////////////////// LAYERED DRAWING /////////////////////////////////////////////////////////////

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