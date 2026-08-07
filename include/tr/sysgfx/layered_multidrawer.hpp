/// @file
/// @brief Provides a facility for drawing using multiple layered renderers.

#pragma once
#include "../utility/function_traits.hpp"
#include "render_target.hpp"

//

namespace tr
{
	/// Drawer of a layered renderer.
	/// @details
	/// A layered renderer drawer is an object that is capable of drawing individual graphical layers using .draw_layer(layer, target).
	///
	/// A drawer must provide .min_layer() and .max_layer() functions for querying the minimum and maximum available layers.
	///
	/// In case of no layers being available, it is recommended to return INT_MAX for .min_layer() and INT_MIN for .max_layer().
	///
	/// A drawer must treat calling .draw_layer(layer, target) for an unavailable layer as a no-op.
	template <typename T>
	concept layered_renderer_drawer = requires(T drawer, const render_target& target) {
		{ std::as_const(drawer).min_layer() } -> std::same_as<int>;
		{ std::as_const(drawer).max_layer() } -> std::same_as<int>;
		drawer.draw_layer(0, target);
	};

	/// Layered renderer.
	template <typename T>
	concept layered_renderer = requires(T renderer) {
		{ renderer.create_drawer() } -> layered_renderer_drawer;
		{ renderer.create_drawer(0, 1) } -> layered_renderer_drawer;
		requires std::same_as<return_type_t<decltype([](T renderer) { return renderer.create_drawer(); })>,
							  return_type_t<decltype([](T renderer) { return renderer.create_drawer(0, 1); })>>;
	};

	/// Layered renderer or a drawer of one.
	template <typename T>
	concept layered_renderer_or_drawer = layered_renderer<T> || layered_renderer_drawer<T>;

	//

	/// Interface for drawing multiple layered renderers in proper order.
	/// @tparam Drawers List of renderer drawer types.
	template <layered_renderer_drawer... Drawers>
	class layered_multidrawer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates a multidrawer out of a list of renderers and drawers.
		/// @tparam DrawersAndRenderers Drawer and renderer types.
		/// @param drawers_and_renderers Drawers and renderers from which drawers will be taken.
		template <layered_renderer_or_drawer... DrawersAndRenderers>
		layered_multidrawer(DrawersAndRenderers&&... drawers_and_renderers);

		/// Creates a multidrawer out of a list of renderers and drawers given an explicit layer range for renderers.
		/// @param min_layer Lower bound of drawn layers.
		/// @param max_layer Upper bound of drawn layers.
		/// @param drawers_and_renderers Drawers and renderers from which drawers will be taken.
		template <layered_renderer_or_drawer... DrawersAndRenderers>
		layered_multidrawer(int min_layer, int max_layer, DrawersAndRenderers&&... drawers_and_renderers);

		/// @}
		/// @name Drawing
		/// @{

		/// Draws a layer to a target.
		/// @param layer Layer to draw.
		/// @param target Render target to draw to.
		void draw_layer(int layer, const render_target& target);

		/// Draws a range of layers to a target.
		/// @param min_layer Lower bound of drawn layers.
		/// @param max_layer Upper bound of drawn layers.
		/// @param target Render target to draw to.
		void draw_layer_range(int min_layer, int max_layer, const render_target& target);

		/// Draws all layers to a target.
		/// @param target Render target to draw to.
		void draw(const render_target& target);

		/// @}

	  private:
		/// Base drawers.
		std::tuple<Drawers...> m_drawers;

		//

		/// Determines the minimum available layer.
		/// @return Minimum available layer.
		int min_layer() const;

		/// Determines the maximum available layer.
		/// @return Maximum available layer.
		int max_layer() const;
	};
} // namespace tr

#include "impl/layered_multidrawer.hpp" // IWYU pragma: export