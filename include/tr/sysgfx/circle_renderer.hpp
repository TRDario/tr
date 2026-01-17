#include "backbuffer.hpp"
#include "blending.hpp"
#include "graphics_context.hpp"
#include "render_target.hpp"
#include "shader_pipeline.hpp"

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
		// Sets the default transformation matrix used by circles on a layer.
		void set_default_layer_transform(int layer, const glm::mat4& mat);
		// Sets the default blending mode used by circles on a layer.
		void set_default_layer_blend_mode(int layer, const blend_mode& blend_mode);

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