///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a rendering target class.                                                                                                    //
//                                                                                                                                       //
// A render target is nothing more than an opaque reference to a texture that can be drawn to, or the backbuffer. It can thus not be     //
// constructed directly, but is gotten from the likes of tr::graphics_context::backbuffer or render_texture::operator render_target.     //
// The only action that can be performed using it, besides passing it to tr::graphics_context::set_render_target, is getting its size in //
// pixels:                                                                                                                               //
//     - context.backbuffer().size() -> the size of the backbuffer                                                                       //
//                                                                                                                                       //
// Render targets can be cropped, scissored, or both:                                                                                    //
//     - context.backbuffer().cropped({{100, 100}, {100, 100}})                                                                          //
//       -> creates a render target spanning from (100, 100) to (200, 200) in the backbuffer                                             //
//     - context.backbuffer().scissored({{100, 100}, {100, 100}})                                                                        //
//       -> creates a render target spanning the backbuffer, but with only the region from (100, 100) to (200, 200) as writable          //
//     - context.backbuffer().subtarget({{}, {500, 500}}, {{100, 100}, {100, 100}})                                                      //
//       -> equivalent to context.backbuffer().cropped({{}, {500, 500}}).scissored({{100, 100}, {100, 100}})                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/rectangle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Abstraction over a rendering target.
	class render_target {
	  public:
		// Gets the size of the rendering target.
		const glm::ivec2& size() const;

		// Creates a new render target with a cropped viewport and full scissor box.
		render_target cropped(const rectangle<int>& viewport) const;
		// Creates a new render target with the same viewport and a different scissor box.
		render_target scissored(const rectangle<int>& scissor_box) const;
		// Equivalent to cropped(viewport).scissored(scissor_box).
		render_target subtarget(const rectangle<int>& viewport, const rectangle<int>& scissor_box) const;

	  private:
		// The OpenGL ID of the render target's FBO.
		unsigned int m_fbo;
		// Size of the render target's FBO.
		glm::ivec2 m_fbo_size;
		// The viewport of the render target.
		rectangle<int> m_viewport;
		// The scissor box of the render target.
		rectangle<int> m_scissor_box;

		// Creates a render target spanning an entire FBO.
		render_target(unsigned int fbo, glm::ivec2 fbo_size);
		// Creates a render target spanning a region of an FBO.
		render_target(unsigned int fbo, glm::ivec2 fbo_size, const rectangle<int>& viewport, const rectangle<int>& scissor_box);

		friend render_target backbuffer_render_target();
		friend class render_texture;
		friend class graphics_context;
	};
} // namespace tr