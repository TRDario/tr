/// @file
/// @brief Provides a framebuffer class.

#pragma once
#include "../utility/handle.hpp"

namespace tr
{
	class graphics_context;
	class render_target;
	class texture_view;
} // namespace tr

//

namespace tr
{
	/// Collection of buffers used as a destination for rendering.
	class framebuffer
	{
	  public:
		/// Framebuffer attachments.
		enum class attachment
		{
			/// First color attachment.
			color0 = 36064,
			/// Second color attachment.
			color1,
			/// Third color attachment.
			color2,
			/// Fourth color attachment.
			color3,
			/// Fifth color attachment.
			color4,
			/// Sixth color attachment.
			color5,
			/// Seventh color attachment.
			color6,
			/// Eighth color attachment.
			color7,
			/// Depth attachment.
			depth = 36096,
			/// Stencil attachment.
			stencil = 36128,
		};

		/// @name Constructors
		/// @{

		/// Creates an empty framebuffer.
		/// @param context Graphics context to create the framebuffer on.
		framebuffer(graphics_context& context);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the framebuffer is on.
		graphics_context& context() const;

		/// @}
		/// @name Render target
		/// @{

		/// Creates a render target on the framebuffer.
		/// @details Since the framebuffer does not keep track of its own size, it must be provided manully.
		/// @param size Size of the render target.
		/// @return Render target on the framebuffer.
		render_target render_target(glm::ivec2 size) const;

		/// @}
		/// @name Attachments
		/// @{

		/// Attaches a texture to the framebuffer.
		/// @param attachment Attachment to set.
		/// @param texture Texture to attach to the framebuffer.
		void attach(attachment attachment, texture_view texture);

		/// Detaches whatever is on the specified attachment.
		/// @param attachment Attachment to clear.
		void detach(attachment attachment);

		/// @}

	  private:
		/// Framebuffer deleter.
		struct deleter
		{
			/// Reference to the graphics context the framebuffer is on.
			graphics_context& context;

			//

			/// Deletes a framebuffer.
			/// @param fbo OpenGL framebuffer ID.
			void operator()(unsigned int fbo) const;
		};

		//

		/// Handle to the OpenGL framebuffer.
		handle<unsigned int, 0, deleter> m_handle;

		//

		/// Creates a framebuffer handle.
		void create_handle();
	};
} // namespace tr