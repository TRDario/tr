///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a rendering target class.                                                                                                    //
//                                                                                                                                       //
// A render target is nothing more than an opaque reference to a texture that can be drawn to, or the backbuffer. It can thus not be     //
// constructed directly, but is gotten from the likes of tr::gfx::backbuffer_render_target or render_texture::operator render_target.    //
// The only action that can be performed using it, besides passing it to tr::gfx::set_render_target, is getting its size in pixels:      //
//     - tr::gfx::backbuffer_render_target().size() -> the size of the backbuffer, equivalent to tr::gfx::backbuffer_size()              //
//                                                                                                                                       //
// Render targets can be cropped, scissored, or both:                                                                                    //
//     - tr::gfx::backbuffer_render_target().cropped({{100, 100}, {100, 100}})                                                           //
//       -> creates a render target spanning from (100, 100) to (200, 200) in the backbuffer                                             //
//     - tr::gfx::backbuffer_render_target().scissored({{100, 100}, {100, 100}})                                                         //
//       -> creates a render target spanning the backbuffer, but with only the region from (100, 100) to (200, 200) as writable          //
//     - tr::gfx::backbuffer_render_target().subtarget({{}, {500, 500}}, {{100, 100}, {100, 100}})                                       //
//       -> equivalent to tr::gfx::backbuffer_render_target().cropped({{}, {500, 500}}).scissored({{100, 100}, {100, 100}})              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/geometry.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Abstraction over a rendering target.
	class render_target {
	  public:
		friend bool operator==(const render_target& l, const render_target& r) = default;

		// Gets the size of the rendering target.
		const glm::ivec2& size() const;

		// Creates a new render target with a cropped viewport and full scissor box.
		render_target cropped(const irect2& viewport) const;
		// Creates a new render target with the same viewport and a different scissor box.
		render_target scissored(const irect2& scissor_box) const;
		// Equivalent to cropped(viewport).scissored(scissor_box).
		render_target subtarget(const irect2& viewport, const irect2& scissor_box) const;

	  private:
		// The OpenGL ID of the render target's FBO.
		unsigned int m_fbo;
		// The viewport of the render target.
		irect2 m_viewport;
		// The scissor box of the render target.
		irect2 m_scissor_box;

		render_target(unsigned int fbo, const irect2& viewport, const irect2& scissor_box);

		friend render_target backbuffer_render_target();
		friend class render_texture;
		friend class context;
	};
} // namespace tr::gfx