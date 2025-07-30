#include "../../include/tr/sysgfx/render_target.hpp"

const glm::ivec2& tr::render_target::size() const
{
	return viewport.size;
}

tr::render_target::render_target(unsigned int fbo, const irect2& viewport)
	: fbo{fbo}, viewport{viewport}
{
}