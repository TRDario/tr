#pragma once
#include "backbuffer.hpp"
#include "blending.hpp"
#include "graphics_context.hpp"
#include "index_buffer.hpp"
#include "render_target.hpp"
#include "shader_pipeline.hpp"
#include "texture.hpp"

namespace tr::gfx {
	// Simple basic renderer color mesh allocation reference.
	struct simple_color_mesh_ref {
		// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;
		// Mesh color data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> colors;
	};
	// Full basic renderer color mesh allocation reference.
	struct color_mesh_ref {
		// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;
		// Mesh color data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> colors;
		// Mesh indices.
		std::ranges::subrange<std::vector<u16>::iterator> indices;
		// The base index.
		u16 base_index;
	};
	// Simple basic renderer textured mesh allocation reference.
	struct simple_textured_mesh_ref {
		// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;
		// Mesh UV data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> uvs;
		// Mesh tint data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> tints;
	};
	// Full basic renderer textured mesh allocation reference.
	struct textured_mesh_ref {
		// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;
		// Mesh UV data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> uvs;
		// Mesh tint data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> tints;
		// Mesh indices.
		std::ranges::subrange<std::vector<u16>::iterator> indices;
		// The base index.
		u16 base_index;
	};

	// Basic renderer for batched rawing in 2D.
	class basic_renderer {
	  private:
	  public:
		class staggered_draw_manager;

		// Creates a basic renderer.
		basic_renderer();

		// Sets the default transformation matrix used by primitives on any layer without its own default transform.
		void set_default_transform(const glm::mat4& mat);
		// Sets the default texture used by textured primitives on a layer.
		void set_default_layer_texture(int layer, texture_ref texture);
		// Sets the default transformation matrix used by primitives on a layer.
		void set_default_layer_transform(int layer, const glm::mat4& mat);
		// Sets the default blending mode used by primitives on a layer.
		void set_default_layer_blend_mode(int layer, const blend_mode& blend_mode);

		// Allocates a new color fan.
		simple_color_mesh_ref new_color_fan(int layer, usize vertices);
		// Allocates a new color fan.
		simple_color_mesh_ref new_color_fan(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode);
		// Allocates a new color polygon outline.
		simple_color_mesh_ref new_color_outline(int layer, usize vertices);
		// Allocates a new color polygon outline.
		simple_color_mesh_ref new_color_outline(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode);
		// Allocates a new color mesh.
		color_mesh_ref new_color_mesh(int layer, usize vertices, usize indices);
		// Allocates a new color mesh.
		color_mesh_ref new_color_mesh(int layer, usize vertices, usize indices, const glm::mat4& mat, const blend_mode& blend_mode);
		// Allocates a new textured fan.
		simple_textured_mesh_ref new_textured_fan(int layer, usize vertices);
		// Allocates a new textured fan.usize
		simple_textured_mesh_ref new_textured_fan(int layer, usize vertices, texture_ref texture);
		// Allocates a new textured fan.usize
		simple_textured_mesh_ref new_textured_fan(int layer, usize vertices, texture_ref texture, const glm::mat4& mat,
												  const blend_mode& blend_mode);
		// Allocates a new textured mesh.
		textured_mesh_ref new_textured_mesh(int layer, usize vertices, usize indices);
		// Allocates a new textured mesh.
		textured_mesh_ref new_textured_mesh(int layer, usize vertices, usize indices, texture_ref texture);
		// Allocates a new textured mesh.
		textured_mesh_ref new_textured_mesh(int layer, usize vertices, usize indices, texture_ref texture, const glm::mat4& mat,
											const blend_mode& blend_mode);

		// Allocates a number of new color lines.
		simple_color_mesh_ref new_lines(int layer, usize lines);
		// Allocates a number of new color lines.
		simple_color_mesh_ref new_lines(int layer, usize lines, const glm::mat4& mat, const blend_mode& blend_mode);
		// Allocates a new color line strip.
		simple_color_mesh_ref new_line_strip(int layer, usize vertices);
		// Allocates a new color line strip.
		simple_color_mesh_ref new_line_strip(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode);
		// Allocates a new color line loop.
		simple_color_mesh_ref new_line_loop(int layer, usize vertices);
		// Allocates a new color line loop.
		simple_color_mesh_ref new_line_loop(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode);
		// Allocates a new color line mesh.
		color_mesh_ref new_line_mesh(int layer, usize vertices, usize indices);
		// Allocates a new color line mesh.
		color_mesh_ref new_line_mesh(int layer, usize vertices, usize indices, const glm::mat4& mat, const blend_mode& blend_mode);

		// Prepares a staggered draw manager for all layers in a priority range. The renderer is "locked" and can't be interacted with while
		// this manager exists.
		staggered_draw_manager prepare_staggered_draw_range(int min_layer, int max_layer);
		// Prepares a staggered draw manager. The renderer is "locked" and can't be interacted with while this manager exists.
		staggered_draw_manager prepare_staggered_draw();
		// Draws a layer to a rendering target.
		void draw_layer(int layer, const render_target& target = backbuffer_render_target());
		// Draws all layers in a priority range to a rendering target.
		void draw_layer_range(int min_layer, int max_layer, const render_target& target = backbuffer_render_target());
		// Draws all added primitives to a rendering target.
		void draw(const render_target& target = backbuffer_render_target());

	  private:
		// Default layer information.
		struct layer_defaults {
			texture_ref texture;
			std::optional<glm::mat4> transform;
			blend_mode blend_mode{alpha_blending};
		};
		// Mesh data.
		struct mesh {
			// The drawing priority of the mesh.
			int layer;
			// The mesh type.
			primitive type;
			// The texture used by the mesh.
			texture_ref texture;
			// The transformation matrix used by the mesh.
			glm::mat4 mat;
			// The blending mode used by the mesh.
			blend_mode blend_mode;
			// The positions of the vertices of the mesh.
			std::vector<glm::vec2> positions;
			// The UVs of the vertices of the mesh.
			std::vector<glm::vec2> uvs;
			// The tints of the vertices of the mesh.
			std::vector<tr::rgba8> tints;
			// The indices of the mesh.
			std::vector<u16> indices;
		};

		// The ID of the renderer.
		renderer_id m_id;
		// Global default transform.
		glm::mat4 m_default_transform{1.0f};
		// Layer defaults.
		std::unordered_map<int, layer_defaults> m_layer_defaults;
		// The list of meshes to draw.
		std::vector<mesh> m_meshes;
		// The pipeline and shaders used by the renderer.
		owning_shader_pipeline m_pipeline;
		// Vertex buffer for the positions of the vertices.
		dyn_vertex_buffer<glm::vec2> m_vbuffer_positions;
		// Vertex buffer for the UVs of the vertices.
		dyn_vertex_buffer<glm::vec2> m_vbuffer_uvs;
		// Vertex buffer for the tints of the vertices.
		dyn_vertex_buffer<tr::rgba8> m_vbuffer_tints;
		// The index buffer used by the renderer.
		dyn_index_buffer m_ibuffer;
		// Last used transform.
		glm::mat4 m_last_transform{1.0f};
		// Last used blending mode.
		blend_mode m_last_blend_mode{alpha_blending};
#ifdef TR_ENABLE_ASSERTS
		// Flag that is set to true when a staggered draw is ongoing.
		bool m_locked{false};
#endif

		// Finds an appropriate mesh.
		mesh& find_mesh(int layer, primitive type, texture_ref texture, const glm::mat4& mat, const blend_mode& blend_mode,
						usize space_needed);
	};

	// Manager class to which the basic renderer delegates handling a staggered drawing process.
	class basic_renderer::staggered_draw_manager {
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
		// Mesh drawing information.
		struct mesh_draw_info {
			// Starting offset within the vertex buffer.
			usize vertex_offset;
			// Starting offset within the index buffer.
			usize index_offset;
		};

		// Reference to the parent renderer.
		opt_ref<basic_renderer> m_renderer;
		// The range of meshes to draw.
		std::ranges::subrange<std::vector<mesh>::iterator> m_range;
		// The drawing data.
		std::vector<mesh_draw_info> m_data;

		// Creates a staggered draw manager.
		staggered_draw_manager(basic_renderer& renderer, std::ranges::subrange<std::vector<mesh>::iterator> range);

		// Sets up the graphical context for drawing.
		void setup_context();
		// Sets up the graphical context for a specific draw call.
		void setup_draw_call_state(texture_ref texture, const glm::mat4& transform, const blend_mode& blend_mode);

		// Cleans up the drawing data and unlocks the parent renderer.
		void clean_up();

		friend class basic_renderer;
	};
} // namespace tr::gfx