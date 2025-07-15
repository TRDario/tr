#pragma once
#include "../utility/geometry.hpp"

namespace tr {
	// Abstraction over a rendering target.
	class render_target {
	  public:
		friend bool operator==(const render_target& l, const render_target& r) noexcept = default;

		// Gets the size of the rendering target.
		const glm::ivec2& size() const noexcept;

	  private:
		// The OpenGL ID of the render target's FBO.
		unsigned int _fbo;
		// The viewport of the render target.
		irect2 _viewport;

		render_target(unsigned int fbo, const irect2& viewport) noexcept;

		friend struct backbuffer;
		friend struct gfx_context;
		friend class render_texture;
	};
} // namespace tr