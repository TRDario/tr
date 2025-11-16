#include "backbuffer.hpp"
#include "blending.hpp"
#include "graphics_context.hpp"
#include "render_target.hpp"
#include "shader_pipeline.hpp"

namespace tr::gfx {
	// Efficient circle renderer.
	class circle_renderer {
	  public:
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

		// Draws a layer to a rendering target.
		void draw_layer(int layer, const render_target& target = backbuffer_render_target());
		// Draws all layers of priority <= max_layer to a rendering target.
		void draw_up_to_layer(int max_layer, const render_target& target = backbuffer_render_target());
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
		};
		// Layer information.
		struct layer {
			// The transormation matrix of the layer (or empty for the global default).
			std::optional<glm::mat4> transform;
			// The blending mode of the layer.
			blend_mode blend_mode{ALPHA_BLENDING};
			// The circles to draw on this layer.
			std::vector<circle> circles;
		};

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
		blend_mode m_last_blend_mode{ALPHA_BLENDING};

		// Sets up the graphical context for drawing.
		void setup_context();
		// Sets up the graphical context for a specific draw call.
		void setup_draw_call_state(const glm::mat4& transform, const blend_mode& blend_mode);
		// Draws layers.
		void draw_layers(std::map<int, layer>::iterator first, std::map<int, layer>::iterator last, const render_target& target);
	};
} // namespace tr::gfx