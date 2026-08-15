/// @file
/// @brief Provides a double buffer class used to render post-processing effects.

#pragma once
#include "framebuffer.hpp"
#include "graphics_context.hpp"
#include "shader_pipeline.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"
#include "vertex_format.hpp"

//

namespace tr
{
	/// Double buffer used to render post-processing effects.
	class post_processing_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an incomplete buffer.
		/// @param context Graphics context to create the buffer on.
		post_processing_buffer(graphics_context& context);

		/// Allocates a buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param size Initial size of the buffer.
		/// @param mipmaps Whether to generate mipmaps for the buffer textures.
		/// @param format Pixel format of the buffer textures.
		post_processing_buffer(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
							   pixel_format format = pixel_format::rgba32);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the buffer is on.
		/// @return Reference to the graphics context the buffer is on.
		graphics_context& context() const;

		/// @}
		/// @name State

		/// Gets whether the buffer is complete.
		/// @return `true` if the buffer is complete, `false` otherwise.
		bool complete() const;

		/// Gets the size of the buffer.
		/// @return Size of the buffer.
		glm::ivec2 size() const;

		/// @}
		/// @name Allocation
		/// @{

		/// Reallocates the buffer and discards the previous data.
		/// @param size Size of the buffer.
		/// @param mipmaps Whether to generate mipmaps for the buffer textures.
		/// @param format Pixel format of the buffer textures.
		void allocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		/// @}
		/// @name Source
		/// @{

		/// Gets a render target spanning the buffer source.
		/// @pre The buffer must be complete.
		/// @return Render target spanning the buffer source.
		render_target source() const;

		/// Clears the buffer source.
		/// @pre The buffer must be complete.
		void clear_source();

		/// @}
		/// @name Applying

		/// Applies a post-processing shader and swaps the source/destination buffers.
		/// @param shader Post-processing fragment shader to apply.
		/// @pre The buffer must be complete.
		/// @return View to the output buffer.
		texture_view apply(fragment_shader& fragment_shader);

		/// @}

	  private:
		/// Textures of the buffer.
		std::array<texture, 2> m_textures;

		/// Framebuffer of the buffer.
		framebuffer m_framebuffer;

		/// Post-processing passthrough vertex shader.
		vertex_shader m_vertex_shader;

		/// Post-processing shader pipeline.
		shader_pipeline m_shader_pipeline;

		/// Post-processing vertex format.
		vertex_format m_vertex_format;

		/// Vertex buffer holding a rectangle mesh ((-1, -1) to (1, 1)).
		static_vertex_buffer<glm::vec2> m_vertex_buffer;

		/// ID of the renderer.
		renderer_id m_renderer_id;

		/// Index of the source buffer.
		usize m_source_index;
	};
} // namespace tr