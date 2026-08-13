/// @file
/// @brief Provides an efficient circle renderer.

#include "../utility/circle.hpp"
#include "../utility/reference.hpp"
#include "blending.hpp"
#include "graphics_context.hpp"
#include "render_target.hpp"
#include "shader_pipeline.hpp"

//

namespace tr
{
	/// Efficient circle renderer.
	class circle_renderer
	{
	  public:
		/// Drawer class to which the circle renderer delegates the calling of draw commands.
		class drawer;

		/// @name Constructors
		/// @{

		/// Initializes the circle renderer.
		/// @param context Graphics context to create the renderer on.
		/// @param render_scale Rendering scale hint for the renderer.
		circle_renderer(graphics_context& context, float render_scale = 1.0f);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the renderer is on.
		/// @return Reference to the graphics context the renderer is on.
		graphics_context& context() const;

		/// @}
		/// @name Properties
		/// @{

		/// Sets the render scale hint for the renderer.
		/// @param render_scale Rendering scale hint for the renderer.
		void set_render_scale(float render_scale);

		/// Sets the default transformation matrix used by circles on any layer without its own default transform.
		/// @param mat Transformation matrix to use as a global default.
		void set_default_transform(const glm::mat4& mat);

		/// Sets the transformation matrix used by circles on a layer.
		/// @param layer Layer to set the default for.
		/// @param mat Transformation matrix to use as a layer default.
		void set_layer_transform(int layer, const glm::mat4& mat);

		/// Sets the blending mode used by circles on a layer.
		/// @param layer Layer to set the default for.
		/// @param blend_mode Blending mode to use as a layer default.
		void set_layer_blend_mode(int layer, const blend_mode& blend_mode);

		/// @}
		/// @name Adding circles
		/// @{

		/// Adds a filled circle to the renderer.
		/// @param layer Layer to add the circle to.
		/// @param circle Circle to add to the renderer.
		/// @param color Color of the circle fill.
		void add_circle(int layer, circle circle, rgba8 color);

		/// Adds a circle outline to the renderer.
		/// @param layer Layer to add the circle outline to.
		/// @param circle Circle to add to the renderer.
		/// @param outline_thickness Thickness of the circle outline.
		/// @param color Color of the circle outline.
		void add_circle_outline(int layer, circle circle, float outline_thickness, rgba8 color);

		/// Adds an outlined circle to the renderer.
		/// @param layer Layer to add the circle to.
		/// @param circle Circle to add to the renderer.
		/// @param outline_thickness Thickness of the circle outline.
		/// @param fill_color Color of the circle fill.
		/// @param outline_color Color of the circle outline.
		void add_outlined_circle(int layer, circle circle, float outline_thickness, rgba8 fill_color, rgba8 outline_color);

		/// @}
		/// @name Drawing
		/// @{

		/// Creates a drawer for all layers in a range.
		/// @note The renderer is "locked" after this operation and can't be interacted with while the drawer exists.
		/// @param min_layer Minimum included layer.
		/// @param max_layer Maximum included layer.
		/// @return Drawer for the layer range [`min_layer`, `max_layer`].
		drawer create_drawer(int min_layer, int max_layer);

		/// Creates a drawer for all layers in the renderer.
		/// @note The renderer is "locked" after this operation and can't be interacted with while the drawer exists.
		/// @return Drawer for all of the circles added to the renderer.
		drawer create_drawer();

		/// Draws all added circles to a rendering target.
		/// @param target Rendering target.
		void draw(const render_target& target);

		/// @}

	  private:
		/// Circle information.
		struct circle
		{
			/// Position of the circle's center.
			glm::vec2 position;

			/// Radius of the filled region of the circle.
			float fill_radius;

			/// Thickness of the outline of the circle.
			float outline_thickness;

			/// Color of the filled region of the circle.
			rgba8 fill_color;

			/// Color of the outline of the circle.
			rgba8 outline_color;

			//

			/// Provided for `tr::as_vertex_attribute_list`.
			static constexpr auto as_vertex_attribute_list{tr::as_vertex_attribute_list<glm::vec2, float, float, rgba8, rgba8>};
		};

		/// Layer information.
		struct layer
		{
			/// Transformation matrix of the layer (or empty for the global default).
			std::optional<glm::mat4> transform;

			/// Blending mode of the layer.
			blend_mode blend_mode{alpha_blending};

			/// Circles to draw on this layer.
			std::vector<circle> circles;
		};

		//

		/// Bindings of the circle renderer vertex format.
		static constexpr std::array vertex_format_bindings{make_vertex_binding<glm::u8vec2>(), make_vertex_binding<circle>(1)};

		//

		/// The ID of the renderer.
		renderer_id m_id;

		/// Global default transform.
		glm::mat4 m_default_transform{1.0f};

		/// Drawing layers.
		std::map<int, layer> m_layers;

		/// The pipeline and shaders used by the renderer.
		owning_shader_pipeline m_pipeline;

		/// The circle renderer vertex format.
		vertex_format m_vertex_format;

		/// The shader circle buffer.
		dyn_vertex_buffer<circle> m_shader_circles;

		/// The vertices of the quad used to draw circles.
		static_vertex_buffer<glm::u8vec2> m_quad_vertices;

		/// Last used transform.
		glm::mat4 m_last_transform{1.0f};

		/// Last used blending mode.
		blend_mode m_last_blend_mode{alpha_blending};

#ifdef TR_ENABLE_ASSERTS
		/// Flag that is set to true when a staggered draw is ongoing.
		bool m_locked{false};
#endif
	};

	/// Drawer class to which the circle renderer delegates the calling of draw commands.
	class circle_renderer::drawer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Moves a drawer.
		/// @param rhs Drawer to move.
		drawer(drawer&& rhs) noexcept;

		/// Cleans up the drawing data and unlocks the parent renderer.
		~drawer();

		/// @}
		/// @name Assignment operators
		/// @{

		/// Moves a drawer.
		/// @param rhs Drawer to move.
		/// @return Reference to `*this`.
		drawer& operator=(drawer&& rhs) noexcept;

		/// @}
		/// @name Layers
		/// @{

		/// Gets the minimum available layer for drawing.
		/// @return Minimum available layer for drawing.
		int min_layer() const;

		/// Gets the maximum available layer for drawing.
		/// @return Maximum available layer for drawing.
		int max_layer() const;

		/// @}
		/// @name Drawing
		/// @{

		/// Draws a single layer.
		/// @param layer Layer to draw.
		/// @param target Rendering target.
		void draw_layer(int layer, const render_target& target);

		/// Draws everything.
		/// @param target Rendering target.
		void draw(const render_target& target);

		/// @}

	  private:
		/// Reference to the parent renderer.
		opt_ref<circle_renderer> m_renderer;

		/// Range of circles to draw.
		std::ranges::subrange<std::map<int, layer>::iterator> m_range;

		//

		/// Creates a drawer.
		/// @param renderer Reference to the parent renderer.
		/// @param range Range of circles to draw.
		drawer(circle_renderer& renderer, std::ranges::subrange<std::map<int, layer>::iterator> range);

		//

		/// Sets up the graphical context for drawing.
		/// @param context Reference to the graphics context.
		void setup_context(graphics_context& context);

		/// Sets up the graphical context for a specific draw call.
		/// @param context Reference to the graphics context.
		/// @param transform Transformation matrix to use.
		/// @param blend_mode Blending mode to use.
		void setup_draw_call_state(graphics_context& context, const glm::mat4& transform, const blend_mode& blend_mode);

		//

		/// Cleans up the drawing data and unlocks the parent renderer.
		void clean_up();

		//

		// Uses the private constructor.
		friend class circle_renderer;
	};
} // namespace tr