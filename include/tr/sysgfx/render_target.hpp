/// @file
/// @brief Provides a rendering target class.

#pragma once
#include "../utility/rectangle.hpp"

//

namespace tr
{
	/// Opaque reference to a texture or framebuffer that can be drawn to.
	class render_target
	{
	  public:
		/// @name Size
		/// @{

		/// Gets the size of the render target.
		/// @return Size of the render target.
		glm::ivec2 size() const;

		/// @}
		/// @name Subtargets
		/// @{

		/// Creates a new render target with a cropped viewport and full scissor box.
		/// @param viewport Viewport of the render target.
		/// @return New subtarget.
		render_target cropped(rectangle<int> viewport) const;

		/// Creates a new render target with the same viewport and a different scissor box.
		/// @param scissor_box Scissor box of the render target.
		/// @return New subtarget.
		render_target scissored(rectangle<int> scissor_box) const;

		/// Equivalent to `cropped(viewport).scissored(scissor_box)`.
		/// @param viewport Viewport of the render target.
		/// @param scissor_box Scissor box of the render target.
		/// @return New subtarget.
		render_target subtarget(rectangle<int> viewport, rectangle<int> scissor_box) const;

		/// @}

	  private:
		/// OpenGL ID of the render target's FBO.
		unsigned int m_framebuffer;

		/// Size of the render target's FBO.
		glm::ivec2 m_framebuffer_size;

		/// Viewport of the render target.
		rectangle<int> m_viewport;

		/// Scissor box of the render target.
		rectangle<int> m_scissor_box;

		//

		/// Creates a render target spanning an entire FBO.
		/// @param framebuffer OpenGL framebuffer ID.
		/// @param framebuffer_size Size of the framebuffer.
		render_target(unsigned int framebuffer, glm::ivec2 framebuffer_size);

		/// Creates a render target spanning a region of an FBO.
		/// @param framebuffer OpenGL framebuffer ID.
		/// @param framebuffer_size Size of the framebuffer.
		/// @param viewport Viewport of the render target.
		/// @param scissor_box Scissor box of the render target.
		render_target(unsigned int framebuffer, glm::ivec2 framebuffer_size, rectangle<int> viewport, rectangle<int> scissor_box);

		//

		// Uses the private constructor.
		friend class framebuffer;

		// Uses the private constructor and members.
		friend class graphics_context;
	};
} // namespace tr