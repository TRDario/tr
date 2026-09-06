/// @file
/// @brief Provides `tr::post_processing_buffer`.

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
	/// Texture buffer used to render post-processing effects.
	/// @details
	/// `post_processing_buffer` is modeled as a pair of texture buffers: the source buffer and the destination buffer. The source buffer is
	/// exposed as a rendering target that may be cleared and written to. When using `apply()`, the source buffer is sampled by the
	/// user-provided fragment shader and the result is written to the destination buffer, the view to which is returned by the function and
	/// may be used to draw to the screen. Afterwards, the buffers are swapped so that the post-processed buffer may be used as a
	/// post-processing source once more.
	///
	/// `post_processing_buffer` has a notion of 'completeness' much like textures. An instance of the class may be constructed without an
	/// initial size or parameters to defer allocating resources until a call to `allocate()`. Such an instance is 'incomplete' and is not
	/// suitable for most operations other than querying and allocating. Once `allocate()` is called, or if the class is immediately
	/// constructed with a size and parameters, it is considered 'complete' and may be used normally.
	///
	/// Every instance of `post_processing_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// `post_processing_buffer` is movable, but not copyable.
	class post_processing_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an incomplete post-processing buffer.
		/// @param context Graphics context to create the post-processing buffer on.
		post_processing_buffer(graphics_context& context);

		/// Constructs a complete post-processing buffer.
		/// @param context Graphics context to create the post-processing buffer on.
		/// @param size Initial size of the buffer textures.
		/// @param mipmaps Whether to generate mipmaps for the buffer textures.
		/// @param format Pixel format of the buffer textures.
		post_processing_buffer(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
							   pixel_format format = pixel_format::rgba32);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the post-processing buffer is on.
		/// @return Reference to the graphics context the post-processing buffer is on.
		graphics_context& context() const;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the buffer is complete.
		/// @return `true` if the buffer is complete, `false` otherwise.
		bool complete() const;

		/// Gets the size of the buffer.
		/// @return Size of the buffer.
		glm::ivec2 size() const;

		/// @}
		/// @name Allocation
		/// @{

		/// (Re-)allocates the buffer and discards any previous data.
		/// @details If the buffer was previously constucted incomplete, this function completes it.
		/// @param size Size of the buffer textures.
		/// @param mipmaps Whether to generate mipmaps for the buffer textures.
		/// @param format Pixel format of the buffer textures.
		void allocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		/// @}
		/// @name Render target
		/// @{

		/// Gets a render target spanning the source buffer.
		/// @return Render target spanning the source buffer.
		operator render_target() const;

		/// Gets a render target spanning the source buffer.
		/// @return Render target spanning the source buffer.
		render_target target() const;

		/// @}
		/// @name Texture
		/// @{

		/// Gets a view to the source buffer's texture.
		/// @details This texture view stays valid until a call to `allocate()`, `apply()`, or the destruction of the buffer.
		/// @pre The buffer must be complete to call this function.
		/// @return View to the source buffer's texture.
		operator texture_view() const;

		/// Gets a view to the source buffer's texture.
		/// @details This texture view stays valid until a call to `allocate()`, `apply()`, or the destruction of the buffer.
		/// @pre The buffer must be complete to call this function.
		/// @return View to the source buffer's texture.
		texture_view texture() const;

		/// @}
		/// @name Applying
		/// @{

		/// Clears the source buffer.
		/// @pre The buffer must be complete to call this function.
		void clear();

		/// Applies a post-processing shader and swaps the source/destination buffers.
		/// @details This view stays valid until a call to `allocate()`, `apply()`, or the destruction of the buffer.
		/// @param fragment_shader Post-processing fragment shader to apply.
		/// Post-processing fragment shaders are expected to contain a `sampler2D` uniform at location 0 and a `vec2` texture size uniform
		/// at location 1, take in a `vec2` uv input used to sample the source buffer, and output a `vec4` color.
		/// @pre The buffer must be complete to call this function.
		void apply(fragment_shader& fragment_shader);

		/// Draws the current source buffer to a render target.
		/// @param target Rendering target.
		/// @pre The buffer must be complete to call this function.
		void draw(const render_target& target);

		/// @}

	  private:
		/// Textures of the buffer.
		std::array<tr::texture, 2> m_textures;

		/// Framebuffer of the buffer.
		framebuffer m_framebuffer;

		/// Post-processing passthrough vertex shader.
		vertex_shader m_vertex_shader;

		/// Post-processing shader pipeline.
		shader_pipeline m_shader_pipeline;

		/// Vertex buffer holding a rectangle mesh ((-1, -1) to (1, 1)).
		static_vertex_buffer<glm::vec2> m_vertex_buffer;

		/// ID of the renderer.
		renderer_id m_renderer_id;

		/// Index of the source buffer.
		usize m_source_index;
	};
} // namespace tr