#include "../../include/tr/sysgfx/render_target.hpp"

const glm::ivec2& tr::render_target::size() const noexcept
{
	return _viewport.size;
}

tr::render_target::render_target(unsigned int fbo, const irect2& viewport) noexcept
	: _fbo{fbo}, _viewport{viewport}
{
}