///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a facility for drawing using multiple layered renderers.                                                                     //
//                                                                                                                                       //
// A layered rendere drawer is an object that is capable of drawing individual graphical layers using .draw_layer(layer, target).        //
// In addition, a drawer must provide .min_layer() and .max_layer() functions for querying the minimum and maximum available layers.     //
// In case of no layers being available, it is recommended to return INT_MAX for .min_layer() and INT_MIN for .max_layer().              //
// A drawer must treat calling .draw_layer(layer, target) for an unavailable layer as a no-op.                                           //
//                                                                                                                                       //
// A layered renderer is one that provides .create_drawer() members with and without an explicit layer range.                            //
//                                                                                                                                       //
// tr::layered_multidrawer is an interface wrapping over a collection of renderer drawers that draws them in proper order.               //
// Layered multidrawers may be constructed with any mixture of renderers (in which case .create_drawer() is invoked) and drawers.        //
// An explicit layer range can optionally be specified in the constructor:                                                               //
//     - tr::basic_renderer basic{context}; tr::circle_renderer circle{context}                                                          //
//       tr::layered_multidrawer drawer{basic.create_drawer(), circle.create_drawer()}                                                   //
//       -> creates a multidrawer using the basic and circle renderers                                                                   //
//     - tr::layered_multidrawer drawer{basic, circle}                                                                                   //
//       -> equivalent to the above                                                                                                      //
//     - tr::layered_multidrawer drawer{0, 5, basic, circle}                                                                             //
//       -> equivalent to {basic.create_drawer(0, 5), circle.create_drawer(0, 5)}, covers only layers 0-5 (inclusive)                    //
//                                                                                                                                       //
// A single layer, a subrange of layers, or the entire available range of layers may be drawn:                                           //
//     - drawer.draw_layer(0, target) -> draws layer 0 on the basic renderer, then layer 0 on the circle renderer                        //
//     - drawer.draw_layer_range(0, 5, target) -> draws layer 0 on the basic and circle renderers, then layer 1...                       //
//     - drawer.draw(target) -> draws the entire available range of layers on the basic and circle renderers                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "render_target.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Concept denoting a drawer of a layered renderer.
	template <typename T>
	concept layered_renderer_drawer = requires(T drawer, const render_target& target) {
		{ std::as_const(drawer).min_layer() } -> std::same_as<int>;
		{ std::as_const(drawer).max_layer() } -> std::same_as<int>;
		drawer.draw_layer(0, target);
	};
	// Concept denoting a layered renderer.
	template <typename T>
	concept layered_renderer = requires(T renderer) {
		{ renderer.create_drawer() } -> layered_renderer_drawer;
		{ renderer.create_drawer(0, 1) } -> layered_renderer_drawer;
		requires std::same_as<return_type_t<decltype([](T renderer) { return renderer.create_drawer(); })>,
							  return_type_t<decltype([](T renderer) { return renderer.create_drawer(0, 1); })>>;
	};
	// Concept denoting a layered renderer or a drawer of one.
	template <typename T>
	concept layered_renderer_or_drawer = layered_renderer<T> || layered_renderer_drawer<T>;

	// Interface for drawing multiple layered renderers in order.
	template <layered_renderer_drawer... Drawers> class layered_multidrawer {
	  public:
		// Creates a multidrawer out of a list of renderers and drawers.
		template <layered_renderer_or_drawer... DrawersAndRenderers> layered_multidrawer(DrawersAndRenderers&&... drawers_and_renderers);
		// Creates a multidrawer out of a list of renderers and drawers given an explicit layer range for renderers.
		template <layered_renderer_or_drawer... DrawersAndRenderers>
		layered_multidrawer(int min_layer, int max_layer, DrawersAndRenderers&&... drawers_and_renderers);

		// Draws a layer to a target.
		void draw_layer(int layer, const render_target& target);
		// Draws a range of layers to a target.
		void draw_layer_range(int min_layer, int max_layer, const render_target& target);
		// Draws all layers to a target.
		void draw(const render_target& target);

	  private:
		// Base drawers.
		std::tuple<Drawers...> m_drawers;

		// Determines the minimum available layer.
		int min_layer() const;
		// Determines the maximum available layer.
		int max_layer() const;
	};
} // namespace tr

#include "impl/layered_multidrawer.hpp" // IWYU pragma: export