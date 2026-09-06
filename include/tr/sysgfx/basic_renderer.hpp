/// @file
/// @brief Provides a batched 2D renderer capable enough for most simple rendering.

#pragma once
#include "../utility/reference.hpp"
#include "blending.hpp"
#include "graphics_context.hpp"
#include "index_buffer.hpp"
#include "render_target.hpp"
#include "shader_pipeline.hpp"
#include "texture_view.hpp"

//

namespace tr
{
	/// Simple basic renderer color mesh allocation reference.
	struct simple_color_mesh_ref
	{
		/// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;

		/// Mesh color data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> colors;
	};

	/// Full basic renderer color mesh allocation reference.
	struct color_mesh_ref
	{
		/// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;

		/// Mesh color data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> colors;

		/// Mesh indices.
		std::ranges::subrange<std::vector<u16>::iterator> indices;

		/// The base index.
		u16 base_index;
	};

	/// Simple basic renderer textured mesh allocation reference.
	struct simple_textured_mesh_ref
	{
		/// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;

		/// Mesh UV data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> uvs;

		/// Mesh tint data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> tints;
	};

	/// Full basic renderer textured mesh allocation reference.
	struct textured_mesh_ref
	{
		/// Mesh position data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> positions;

		/// Mesh UV data.
		std::ranges::subrange<std::vector<glm::vec2>::iterator> uvs;

		/// Mesh tint data.
		std::ranges::subrange<std::vector<tr::rgba8>::iterator> tints;

		/// Mesh indices.
		std::ranges::subrange<std::vector<u16>::iterator> indices;

		/// Base mesh index.
		u16 base_index;
	};

	//

	/// Basic renderer for batched 2D drawing.
	class basic_renderer
	{
	  private:
	  public:
		/// Drawer class to which the basic renderer delegates the calling of draw commands.
		class drawer;

		/// @name Constructors
		/// @{

		/// Creates a basic renderer.
		/// @param context Graphics context to create the renderer on.
		explicit basic_renderer(graphics_context& context);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the renderer is on.
		/// @return Reference to the graphics context the renderer is on.
		graphics_context& context() const;

		/// @}
		/// @name Defaults
		/// @{

		/// Sets the default transformation matrix used by primitives on any layer without its own default transform.
		/// @param mat Default transformation matrix.
		void set_default_transform(const glm::mat4& mat);

		/// Sets the default texture used by textured primitives on a layer.
		/// @param layer Layer to set the default for.
		/// @param texture Default layer texture.
		void set_default_layer_texture(int layer, texture_view texture);

		/// Sets the default transformation matrix used by primitives on a layer.
		/// @param layer Layer to set the default for.
		/// @param mat Default layer transformation matrix.
		void set_default_layer_transform(int layer, const glm::mat4& mat);

		/// Sets the default blending mode used by primitives on a layer.
		/// @param layer Layer to set the default for.
		/// @param blend_mode Default layer blending mode.
		void set_default_layer_blend_mode(int layer, const blend_mode& blend_mode);

		/// @}
		/// @name Triangle meshes
		/// @{

		/// Allocates a new color fan.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the fan.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_color_fan(int layer, usize vertices);

		/// Allocates a new color fan.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the fan.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_color_fan(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode);

		/// Allocates a new color polygon outline.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the polygon.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_color_outline(int layer, usize vertices);

		/// Allocates a new color polygon outline.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the polygon.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_color_outline(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode);

		/// Allocates a new color mesh.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the mesh.
		/// @param indices Number of indices in the mesh.
		/// @return Allocated mesh reference.
		color_mesh_ref new_color_mesh(int layer, usize vertices, usize indices);

		/// Allocates a new color mesh.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the mesh.
		/// @param indices Number of indices in the mesh.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		color_mesh_ref new_color_mesh(int layer, usize vertices, usize indices, const glm::mat4& mat, const blend_mode& blend_mode);

		/// Allocates a new textured fan.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the fan.
		/// @return Allocated mesh reference.
		simple_textured_mesh_ref new_textured_fan(int layer, usize vertices);

		/// Allocates a new textured fan.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the fan.
		/// @param texture Texture used by the mesh.
		/// @return Allocated mesh reference.
		simple_textured_mesh_ref new_textured_fan(int layer, usize vertices, texture_view texture);

		/// Allocates a new textured fan.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the fan.
		/// @param texture Texture used by the mesh.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		simple_textured_mesh_ref new_textured_fan(int layer, usize vertices, texture_view texture, const glm::mat4& mat,
												  const blend_mode& blend_mode);

		/// Allocates a new textured mesh.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the mesh.
		/// @param indices Number of indices in the mesh.
		/// @return Allocated mesh reference.
		textured_mesh_ref new_textured_mesh(int layer, usize vertices, usize indices);

		/// Allocates a new textured mesh.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the mesh.
		/// @param indices Number of indices in the mesh.
		/// @param texture Texture used by the mesh.
		/// @return Allocated mesh reference.
		textured_mesh_ref new_textured_mesh(int layer, usize vertices, usize indices, texture_view texture);

		/// Allocates a new textured mesh.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the mesh.
		/// @param indices Number of indices in the mesh.
		/// @param texture Texture used by the mesh.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		textured_mesh_ref new_textured_mesh(int layer, usize vertices, usize indices, texture_view texture, const glm::mat4& mat,
											const blend_mode& blend_mode);

		/// @}
		/// @name Line meshes
		/// @{

		/// Allocates a number of new color lines.
		/// @param layer Layer to create the mesh on.
		/// @param lines Number of lines in the mesh.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_lines(int layer, usize lines);

		/// Allocates a number of new color lines.
		/// @param layer Layer to create the mesh on.
		/// @param lines Number of lines in the mesh.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_lines(int layer, usize lines, const glm::mat4& mat, const blend_mode& blend_mode);

		/// Allocates a new color line strip.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the strip.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_line_strip(int layer, usize vertices);

		/// Allocates a new color line strip.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the strip.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_line_strip(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode);

		/// Allocates a new color line loop.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the loop.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_line_loop(int layer, usize vertices);

		/// Allocates a new color line loop.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the loop.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		simple_color_mesh_ref new_line_loop(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode);

		/// Allocates a new color line mesh.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the mesh.
		/// @param indices Number of indices in the mesh.
		/// @return Allocated mesh reference.
		color_mesh_ref new_line_mesh(int layer, usize vertices, usize indices);

		/// Allocates a new color line mesh.
		/// @param layer Layer to create the mesh on.
		/// @param vertices Number of vertices in the mesh.
		/// @param indices Number of indices in the mesh.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @return Allocated mesh reference.
		color_mesh_ref new_line_mesh(int layer, usize vertices, usize indices, const glm::mat4& mat, const blend_mode& blend_mode);

		/// @}
		/// @name Drawing
		/// @{

		/// Creates a drawer for all layers in a range.
		/// @param min_layer Minimum included layer.
		/// @param max_layer Maximum included layer.
		/// @post The renderer is "locked" and can't be interacted with while the drawer exists.
		/// @return Drawer for the layer range [`min_layer`, `max_layer`].
		drawer create_drawer(int min_layer, int max_layer);

		/// Creates a drawer for all layers in the renderer.
		/// @post The renderer is "locked" and can't be interacted with while the drawer exists.
		/// @return Drawer for all of the primitives added to the renderer.
		drawer create_drawer();

		/// Draws all added primitives to a rendering target.
		/// @param target Rendering target.
		void draw(const render_target& target);

		/// @}

	  private:
		/// Default layer information.
		struct layer_defaults
		{
			/// Texture used by textured primitives in a layer.
			texture_view texture;

			/// Transformation matrix used by primitives a layer.
			std::optional<glm::mat4> transform;

			/// Blending mode used by primitives a layer.
			blend_mode blend_mode{alpha_blending};
		};

		/// Mesh data.
		struct mesh
		{
			/// Drawing priority of the mesh.
			int layer;

			/// Mesh type.
			primitive type;

			/// Texture used by the mesh.
			texture_view texture;

			/// Transformation matrix used by the mesh.
			glm::mat4 mat;

			/// Blending mode used by the mesh.
			blend_mode blend_mode;

			/// Positions of the vertices of the mesh.
			std::vector<glm::vec2> positions;

			/// UVs of the vertices of the mesh.
			std::vector<glm::vec2> uvs;

			/// Tints of the vertices of the mesh.
			std::vector<tr::rgba8> tints;

			/// Indices of the mesh.
			std::vector<u16> indices;
		};

		//

		/// ID of the renderer.
		renderer_id m_id;

		/// Global default transform.
		glm::mat4 m_default_transform{1.0f};

		/// Layer defaults.
		boost::unordered_flat_map<int, layer_defaults> m_layer_defaults;

		/// List of meshes to draw.
		std::vector<mesh> m_meshes;

		/// Pipeline and shaders used by the renderer.
		owning_shader_pipeline m_pipeline;

		/// Vertex buffer for the positions of the vertices.
		dyn_vertex_buffer<glm::vec2> m_vbuffer_positions;

		/// Vertex buffer for the UVs of the vertices.
		dyn_vertex_buffer<glm::vec2> m_vbuffer_uvs;

		/// Vertex buffer for the tints of the vertices.
		dyn_vertex_buffer<tr::rgba8> m_vbuffer_tints;

		/// Index buffer used by the renderer.
		dyn_index_buffer m_ibuffer;

		/// Last used transform.
		glm::mat4 m_last_transform{1.0f};

		/// Last used blending mode.
		blend_mode m_last_blend_mode{alpha_blending};

#ifdef TR_ENABLE_ASSERTS
		/// Flag that is set to true when a drawer for this renderer exists.
		bool m_locked{false};
#endif

		//

		/// Finds an appropriate mesh.
		/// @param layer Layer the mesh is on.
		/// @param type Primitive type used by the mesh.
		/// @param texture Texture used by the mesh.
		/// @param mat Transformation matrix used by the mesh.
		/// @param blend_mode Blending mode used by the mesh.
		/// @param space_needed Number of vertices that need to fit inside the mesh.
		/// @return Reference to an appropriate mesh.
		mesh& find_mesh(int layer, primitive type, texture_view texture, const glm::mat4& mat, const blend_mode& blend_mode,
						usize space_needed);
	};

	/// Drawer class to which the basic renderer delegates the calling of draw commands.
	class basic_renderer::drawer
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
		/// Mesh drawing information.
		struct mesh_draw_info
		{
			/// Starting offset within the vertex buffer.
			usize vertex_offset;

			/// Starting offset within the index buffer.
			usize index_offset;
		};

		//

		/// Reference to the parent renderer.
		opt_ref<basic_renderer> m_renderer;

		/// The range of meshes to draw.
		std::ranges::subrange<std::vector<mesh>::iterator> m_range;

		/// The drawing data.
		std::vector<mesh_draw_info> m_data;

		//

		/// Creates a drawer.
		/// @param renderer Renderer the drawer is associated with.
		/// @param range Range of drawable meshes.
		drawer(basic_renderer& renderer, std::ranges::subrange<std::vector<mesh>::iterator> range);

		//

		/// Sets up the graphical context for drawing.
		/// @param context Context to set up.
		void setup_context(graphics_context& context);

		/// Sets up the graphical context for a specific draw call.
		/// @param context Context to set up.
		/// @param texture Texture used in the draw call.
		/// @param transform Transformation matrix used in the draw call.
		/// @param blend_mode Blending mode used in the draw call.
		void setup_draw_call_state(graphics_context& context, texture_view texture, const glm::mat4& transform,
								   const blend_mode& blend_mode);

		/// Cleans up the drawing data and unlocks the parent renderer.
		void clean_up();

		//

		// Uses the private constructor.
		friend class basic_renderer;
	};
} // namespace tr