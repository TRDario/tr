#pragma once
#include "../utility/geometry.hpp"

namespace tr {
	class render_target;
	namespace backbuffer {
		// Gets a render target spanning the entire backbuffer.
		tr::render_target render_target();
		// Gets a render target spanning a region of the backbuffer.
		tr::render_target region_render_target(const irect2& rect);
	}; // namespace backbuffer
	namespace gfx_context {
		void set_render_target(const render_target& target);
	};

	// Abstraction over a rendering target.
	class render_target {
	  public:
		friend bool operator==(const render_target& l, const render_target& r) = default;

		// Gets the size of the rendering target.
		const glm::ivec2& size() const;

	  private:
		// The OpenGL ID of the render target's FBO.
		unsigned int fbo;
		// The viewport of the render target.
		irect2 viewport;

		render_target(unsigned int fbo, const irect2& viewport);

		friend render_target backbuffer::render_target();
		friend render_target backbuffer::region_render_target(const irect2& rect);
		friend void gfx_context::set_render_target(const render_target& target);
		friend class render_texture;
	};
} // namespace tr