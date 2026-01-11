///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a rendering target class.                                                                                                    //
//                                                                                                                                       //
// A render target is nothing more than an opaque reference to a texture that can be drawn to, or the backbuffer. It can thus not be     //
// constructed directly, but is gotten from the likes of tr::gfx::backbuffer_render_target or render_texture::operator render_target.    //
// The only action that can be performed using it, besides passing it to tr::gfx::set_render_target, is getting its size in pixels:      //
//     - tr::gfx::backbuffer_render_target().size() -> the size of the backbuffer, equivalent to tr::gfx::backbuffer_size()              //
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

	  private:
		// The OpenGL ID of the render target's FBO.
		unsigned int m_fbo;
		// The viewport of the render target.
		irect2 m_viewport;

		render_target(unsigned int fbo, const irect2& viewport);

		friend render_target backbuffer_render_target();
		friend render_target backbuffer_region_render_target(const irect2& rect);
		friend void set_render_target(const render_target& target);
		friend class render_texture;
	};
} // namespace tr::gfx