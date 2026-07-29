///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a framebuffer class.                                                                                                         //
//                                                                                                                                       //
// Framebuffers are collections of buffers used to define custom destinations for rendering. One or more textures may be attached to a   //
// framebuffer in a number of attachment points:                                                                                         //
//     - tr::framebuffer fbo{context}; fbo.attach(tr::framebuffer::attachment::color0, tex)                                              //
//       -> attaches 'tex' to the first color attachment in 'fbo'                                                                        //
//     - fbo.detach(tr::framebuffer::attachment::color0)                                                                                 //
//       -> Detaches 'tex' from the first color attachment in 'fbo'                                                                      //
//                                                                                                                                       //
// Render targets on a framebuffer can be gotten, but the size of the render target area must be manually supplied, as framebuffers      //
// don't keep track of the sizes of their attachments:                                                                                   //
//     - fbo.render_target({512, 512}) -> 512x512 render target on 'fbo'                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/handle.hpp"

namespace tr {
	class graphics_context;
	class render_target;
	class texture_view;
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Collection of buffers used as a destination for rendering.
	class framebuffer {
	  public:
		// Framebuffer attachments.
		enum class attachment {
			color0 = 36064,
			color1,
			color2,
			color3,
			color4,
			color5,
			color6,
			color7,
			depth = 36096,
			stencil = 36128,
		};

		// Creates an empty framebuffer.
		framebuffer(graphics_context& context);

		// Gets a reference to the graphics context the framebuffer is on.
		graphics_context& context() const;

		// Creates a render target on the framebuffer.
		// Since the framebuffer does not keep track of its own size, it must be provided manully.
		render_target render_target(glm::ivec2 size) const;

		// Attaches a texture to the framebuffer.
		void attach(attachment attachment, texture_view texture);
		// Detaches whatever is on the specified attachment.
		void detach(attachment attachment);

	  private:
		// Framebuffer deleter.
		struct deleter {
			// Reference to the graphics context the framebuffer is on.
			graphics_context& context;

			// Deletes a framebuffer.
			void operator()(unsigned int texture) const;
		};

		// Handle to the OpenGL framebuffer.
		handle<unsigned int, 0, deleter> m_handle;

		// Creates a framebuffer handle.
		void create_handle();
	};
} // namespace tr