///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements layered_multidrawer.hpp.                                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../layered_multidrawer.hpp"

///////////////////////////////////////////////////////////// LAYERED DRAWING /////////////////////////////////////////////////////////////

namespace tr {
	// When passed a renderer stores its drawer type in ::type, otherwise stores T in ::type.
	template <layered_renderer_or_drawer T> struct add_drawer {
		using type = T;
	};
	template <layered_renderer Renderer> struct add_drawer<Renderer> {
		using type = decltype([](Renderer renderer) { return renderer.create_drawer(); });
	};
	// When passed a renderer stores gets its drawer type, otherwise is equal to T.
	template <layered_renderer_or_drawer T> using add_drawer_t = add_drawer<T>::type;

	// Gets the drawer of a renderer.
	template <layered_renderer Renderer> add_drawer_t<Renderer> get_drawer(Renderer&& renderer)
	{
		return renderer.create_drawer();
	}
	// Gets the drawer of a renderer given an explicit range of layers.
	template <layered_renderer Renderer> add_drawer_t<Renderer> get_drawer(Renderer&& renderer, int min_layer, int max_layer)
	{
		return renderer.create_drawer(min_layer, max_layer);
	}
	// Forwards a drawer.
	template <layered_renderer_drawer Drawer> Drawer&& get_drawer(Drawer&& drawer)
	{
		return std::forward(drawer);
	}
	// Forwards a drawer.
	template <layered_renderer_drawer Drawer> Drawer&& get_drawer(Drawer&& drawer, int, int)
	{
		return std::forward(drawer);
	}

	// Multidrawer deduction guide.
	template <layered_renderer_or_drawer... DrawersAndRenderers>
	layered_multidrawer(DrawersAndRenderers&&...) -> layered_multidrawer<add_drawer_t<DrawersAndRenderers>...>;
	// Multidrawer deduction guide.
	template <layered_renderer_or_drawer... DrawersAndRenderers>
	layered_multidrawer(int, int, DrawersAndRenderers&&...) -> layered_multidrawer<add_drawer_t<DrawersAndRenderers>...>;
} // namespace tr

//

template <tr::layered_renderer_drawer... Drawers>
template <tr::layered_renderer_or_drawer... DrawersAndRenderers>
tr::layered_multidrawer<Drawers...>::layered_multidrawer(DrawersAndRenderers&&... drawers_and_renderers)
	: m_drawers{get_drawer(std::forward<DrawersAndRenderers>(drawers_and_renderers))...}
{
}

template <tr::layered_renderer_drawer... Drawers>
template <tr::layered_renderer_or_drawer... DrawersAndRenderers>
tr::layered_multidrawer<Drawers...>::layered_multidrawer(int min_layer, int max_layer, DrawersAndRenderers&&... drawers_and_renderers)
	: m_drawers{get_drawer(std::forward<DrawersAndRenderers>(drawers_and_renderers), min_layer, max_layer)...}
{
}

//

template <tr::layered_renderer_drawer... Drawers>
void tr::layered_multidrawer<Drawers...>::draw_layer(int layer, const render_target& target)
{
	std::apply([layer, &target](Drawers&... drawers) { (drawers.draw_layer(layer, target), ...); });
}

template <tr::layered_renderer_drawer... Drawers>
void tr::layered_multidrawer<Drawers...>::draw_layer_range(int min_layer, int max_layer, const render_target& target)
{
	for (int layer = min_layer; layer <= max_layer; ++layer) {
		draw_layer(layer, target);
	}
}

template <tr::layered_renderer_drawer... Drawers> void tr::layered_multidrawer<Drawers...>::draw(const render_target& target)
{
	draw_layer_range(min_layer(), max_layer(), target);
}

//

template <tr::layered_renderer_drawer... Drawers> int tr::layered_multidrawer<Drawers...>::min_layer() const
{
	return std::apply([](const Drawers&... drawers) { return std::min({drawers.min_layer()...}); });
}

template <tr::layered_renderer_drawer... Drawers> int tr::layered_multidrawer<Drawers...>::max_layer() const
{
	return std::apply([](const Drawers&... drawers) { return std::min({drawers.max_layer()...}); });
}