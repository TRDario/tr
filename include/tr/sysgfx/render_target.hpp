#pragma once
#include "../utility/geometry.hpp"

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