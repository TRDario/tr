#pragma once
#include "render_target.hpp"

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
	std::tuple managers{(renderers.prepare_staggered_draw_range(min_layer, max_layer), ...)};
	for (int layer = min_layer; layer <= max_layer; ++layer) {
		std::apply([&, layer](auto&... managers) { (managers.draw_layer(layer, target), ...); }, managers);
	}
}