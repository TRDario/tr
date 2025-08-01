#include "../../include/tr/sysgfx/render_target.hpp"

const glm::ivec2& tr::gfx::render_target::size() const
{
	return m_viewport.size;
}

tr::gfx::render_target::render_target(unsigned int fbo, const irect2& viewport)
	: m_fbo{fbo}, m_viewport{viewport}
{
}