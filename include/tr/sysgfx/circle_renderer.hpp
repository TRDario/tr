///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an efficient circle renderer.                                                                                                //
//                                                                                                                                       //
// The circle renderer is constructed with an initial render scale (by default 1.0f). The render scale is used to determine the ratio    //
// between logical pixels and physical pixels on the render target. The render scale can be modified with the .set_render_scale() method //
// at any point afterwards:                                                                                                              //
//     - tr::gfx::circle_renderer{}                                                                                                      //
//       -> creates a circle render with render scale 1.0f: a circle with radius 5.0f will have a real radius of 5px                     //
//     - tr::gfx::circle_renderer{2.0f}                                                                                                  //
//       -> creates a circle render with render scale 2.0f: a circle with radius 5.0f will have a real radius of 10px                    //
//     - tr::gfx::circle_renderer circle{}; circle.set_render_scale(2.0f)                                                                //
//       -> equivalent to the above                                                                                                      //
//                                                                                                                                       //
// The circle renderer is a layer-based renderer, compatible with the utilities provided in layered_drawing.hpp. Each layer has its own  //
// transformation matrix (falls back to the global default if not provided) and blending mode (falls back to alpha blending if not       //
// provided) that can be set. The global default transformation matrix can also be set:                                                  //
//     - circle.set_default_transform(tr::ortho(tr::frect2{{1000, 1000}})) -> sets the global transformation matrix                      //
//     - circle.set_layer_transform(1, tr::ortho(tr::frect2{{500, 500}})) -> sets the transformation matrix for layer 1                  //
//     - circle.set_layer_blend_mode(1, tr::gfx::premultiplied_alpha_blending) -> sets the blending mode for layer 1                     //
//                                                                                                                                       //
// Circles are appended to the drawing list of the circle renderer one-by-one. Each circle can be filled, outlined, or both:             //
//     - circle.add_circle(0, {{500, 500}, 10}, "FFFFFF"_rgba8)                                                                          //
//       -> adds a white circle of radius 10 centered at (500, 500) to layer 0                                                           //
//     - circle.add_circle_outline(0, {{200, 200}, 20}, 5, "FF0000"_rgba8)                                                               //
//       -> adds a red circle outline of radius 20 and thickness 5 centered at (200, 200) to layer 0                                     //
//     - circle.add_outlined_circle(0, {{100, 250}, 15}, 4, "0000FF"_rgba8, "00FF00"_rgba8)                                              //
//       -> adds a blue circle of radius 15 centered at (100, 250)  with a green outline of thickness 4 to layer 0                       //
//                                                                                                                                       //
// Added circles are not drawn until a call to one of the drawing functions. Aside from supporting the functions in layered_drawing.hpp, //
// the circle renderer can be drawn alone. Drawn circles are erased from the renderer:                                                   //
//     - circle.draw_layer(0, target) -> draws layer 0 to the target                                                                     //
//     - circle.draw_layer_range(0, 10, target) -> draws layers 0-10 to the target                                                       //
//     - circle.draw(target) -> draws all layers to the target                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "backbuffer.hpp"
#include "blending.hpp"
#include "graphics_context.hpp"
#include "render_target.hpp"
#include "shader_pipeline.hpp"

///////////////////////////////////////////////////////////// CIRCLE RENDERER /////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Efficient circle renderer.
	class circle_renderer {
	  public:
		class staggered_draw_manager;

		// Initializes the circle renderer.
		circle_renderer(float render_scale = 1.0f);

		// Sets the render scale hint for the renderer.
		void set_render_scale(float render_scale);
		// Sets the default transformation matrix used by circles on any layer without its own default transform.
		void set_default_transform(const glm::mat4& mat);
		// Sets the transformation matrix used by circles on a layer.
		void set_layer_transform(int layer, const glm::mat4& mat);
		// Sets the blending mode used by circles on a layer.
		void set_layer_blend_mode(int layer, const blend_mode& blend_mode);

		// Adds a filled circle to the renderer.
		void add_circle(int layer, const circle& circle, rgba8 color);
		// Adds a circle outline to the renderer.
		void add_circle_outline(int layer, const circle& circle, float outline_thickness, rgba8 color);
		// Adds an outlined circle to the renderer.
		void add_outlined_circle(int layer, const circle& circle, float outline_thickness, rgba8 fill_color, rgba8 outline_color);

		// Prepares a staggered draw manager for all layers in a priority range. The renderer is "locked" and can't be interacted with while
		// this manager exists.
		staggered_draw_manager prepare_staggered_draw_range(int min_layer, int max_layer);
		// Prepares a staggered draw manager. The renderer is "locked" and can't be interacted with while this manager exists.
		staggered_draw_manager prepare_staggered_draw();
		// Draws a layer to a rendering target.
		void draw_layer(int layer, const render_target& target = backbuffer_render_target());
		// Draws all layers in a priority range to a rendering target.
		void draw_layer_range(int min_layer, int max_layer, const render_target& target = backbuffer_render_target());
		// Draws all added circles to a rendering target.
		void draw(const render_target& target = backbuffer_render_target());

	  private:
		// Circle information.
		struct circle {
			// The position of the circle's center.
			glm::vec2 position;
			// The radius of the filled region of the circle.
			float fill_radius;
			// The thickness of the outline of the circle.
			float outline_thickness;
			// The color of the filled region of the circle.
			rgba8 fill_color;
			// The color of the outline of the circle.
			rgba8 outline_color;

			// Provided for tr::gfx::as_vertex_attribute_list.
			static constexpr auto as_vertex_attribute_list{gfx::as_vertex_attribute_list<glm::vec2, float, float, rgba8, rgba8>};
		};
		// Layer information.
		struct layer {
			// The transormation matrix of the layer (or empty for the global default).
			std::optional<glm::mat4> transform;
			// The blending mode of the layer.
			blend_mode blend_mode{alpha_blending};
			// The circles to draw on this layer.
			std::vector<circle> circles;
		};

		// The bindings of the circle renderer vertex format.
		static constexpr std::array vertex_format_bindings{make_vertex_binding<glm::u8vec2>(), make_vertex_binding<circle>()};

		// The ID of the renderer.
		renderer_id m_id;
		// Global default transform.
		glm::mat4 m_default_transform{1.0f};
		// Drawing layers.
		std::map<int, layer> m_layers;
		// The pipeline and shaders used by the renderer.
		owning_shader_pipeline m_pipeline;
		// The circle renderer vertex format.
		vertex_format m_vertex_format;
		// The shader circle buffer.
		dyn_vertex_buffer<circle> m_shader_circles;
		// The vertices of the quad used to draw circles.
		static_vertex_buffer<glm::u8vec2> m_quad_vertices;
		// Last used transform.
		glm::mat4 m_last_transform{1.0f};
		// Last used blending mode.
		blend_mode m_last_blend_mode{alpha_blending};
#ifdef TR_ENABLE_ASSERTS
		// Flag that is set to true when a staggered draw is ongoing.
		bool m_locked{false};
#endif
	};

	// Manager class to which the circle renderer delegates handling a staggered drawing process.
	class circle_renderer::staggered_draw_manager {
	  public:
		staggered_draw_manager(staggered_draw_manager&& r) noexcept;
		// Cleans up the drawing data and unlocks the parent renderer.
		~staggered_draw_manager();

		staggered_draw_manager& operator=(staggered_draw_manager&& r) noexcept;

		// Draws a single layer.
		void draw_layer(int layer, const render_target& target);
		// Draws everything.
		void draw(const render_target& target);

	  private:
		// Reference to the parent renderer.
		opt_ref<circle_renderer> m_renderer;
		// The range of circles to draw.
		std::ranges::subrange<std::map<int, layer>::iterator> m_range;

		// Creates a staggered draw manager.
		staggered_draw_manager(circle_renderer& renderer, std::ranges::subrange<std::map<int, layer>::iterator> range);

		// Sets up the graphical context for drawing.
		void setup_context();
		// Sets up the graphical context for a specific draw call.
		void setup_draw_call_state(const glm::mat4& transform, const blend_mode& blend_mode);

		// Cleans up the drawing data and unlocks the parent renderer.
		void clean_up();

		friend class circle_renderer;
	};
} // namespace tr::gfx