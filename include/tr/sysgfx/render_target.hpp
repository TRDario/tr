/// @file
/// @brief Provides `tr::render_target`.

#pragma once
#include "../utility/rectangle.hpp"
#include "../utility/reference.hpp"

namespace tr
{
	class framebuffer;
	class graphics_context;
	enum class graphics_object_id : unsigned int;
} // namespace tr

//

namespace tr
{
	/// Opaque reference to a texture or framebuffer that can be drawn to.
	class render_target
	{
	  public:
		/// @cond implementation_details

		/// Information about the render target's framebuffer.
		struct framebuffer_info_t
		{
			/// OpenGL FBO ID of the framebuffer.
			unsigned int fbo;

			/// Size of the framebuffer.
			glm::ivec2 size;

#ifdef TR_ENABLE_GL_CHECKS
			/// Reference to the context the render target is on.
			ref<const graphics_context> context;

			/// Unique graphics object ID of the framebuffer.
			graphics_object_id id;

			/// Label of the framebuffer.
			std::string label;
#endif
		};

		/// @endcond
		/// @name Constructors
		/// @{

		/// Constructs a backbuffer render target.
		/// @note This is equivalent to calling `context.backbuffer()`.
		/// @param context Graphics context whose backbuffer to create the render target for.
		explicit render_target(const graphics_context& context);

		/// Creates a render target on a framebuffer.
		/// @param framebuffer Framebuffer the render target is on.
		/// @param framebuffer_size Size of the framebuffer.
		render_target(const framebuffer& framebuffer, glm::ivec2 framebuffer_size);

		/// Creates a render target on a framebuffer.
		/// @param framebuffer Framebuffer the render target is on.
		/// @param framebuffer_size Size of the framebuffer.
		/// @param viewport Viewport of the framebuffer.
		/// @param scissor_box Scissor box of the framebuffer.
		render_target(const framebuffer& framebuffer, glm::ivec2 framebuffer_size, rectangle<int> viewport, rectangle<int> scissor_box);

		/// @}
		/// @name Properties
		/// @{

		/// Gets the size of the render target.
		/// @return Size of the render target.
		glm::ivec2 size() const;

		/// Gets the viewport of the render target within the target framebuffer.
		/// @return Viewport rectangle of the render target within the target framebuffer.
		rectangle<int> viewport() const;

		/// Gets the scissor box of the render target within the target framebuffer.
		/// @return Scissor box rectangle of the render target within the target framebuffer.
		rectangle<int> scissor_box() const;

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

		/// @}
		/// @cond implementation_details

		/// Gets information about the render target's framebuffer.
		/// @return Information about the render target's framebuffer.
		const framebuffer_info_t& framebuffer_info() const;

		/// @endcond

	  private:
		/// Information about the render target's framebuffer.
		framebuffer_info_t m_framebuffer_info;

		/// Viewport of the render target.
		rectangle<int> m_viewport;

		/// Scissor box of the render target.
		rectangle<int> m_scissor_box;
	};
} // namespace tr