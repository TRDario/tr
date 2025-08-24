#pragma once
#include "backbuffer.hpp"
#include "blending.hpp"
#include "render_target.hpp"

namespace tr::gfx {
	class texture_ref;

	// Simple 2D renderer color mesh allocation reference.
	struct simple_color_mesh_ref {
		// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;
		// Mesh color data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> colors;
	};
	// Full 2D renderer color mesh allocation reference.
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
	// Simple 2D renderer textured mesh allocation reference.
	struct simple_textured_mesh_ref {
		// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;
		// Mesh UV data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> uvs;
		// Mesh tint data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> tints;
	};
	// Full 2D renderer textured mesh allocation reference.
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

	// 2D renderer.
	namespace renderer_2d {
		// Initializes the 2D renderer.
		void initialize();
		// Shuts down the 2D renderer.
		void shut_down();

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

		// Draws a layer to a rendering target.
		void draw_layer(int layer, const render_target& target = backbuffer_render_target());
		// Draws all layers of priority <= max_layer to a rendering target.
		void draw_up_to_layer(int max_layer, const render_target& target = backbuffer_render_target());
		// Draws all added primitives to a rendering target.
		void draw(const render_target& target = backbuffer_render_target());
	}; // namespace renderer_2d
} // namespace tr::gfx