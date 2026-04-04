///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements render_target.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/render_target.hpp"

////////////////////////////////////////////////////////////// RENDER TARGET //////////////////////////////////////////////////////////////

tr::gfx::render_target::render_target(unsigned int fbo, const irect2& viewport)
	: m_fbo{fbo}, m_viewport{viewport}
{
}

const glm::ivec2& tr::gfx::render_target::size() const
{
	return m_viewport.size;
}

tr::gfx::render_target tr::gfx::render_target::subtarget(irect2 viewport) const
{
	TR_ASSERT(m_viewport.contains(viewport.tl + viewport.size),
			  "Tried to create out-of-bounds sub-target from ({}, {}) to ({}, {}) of a render target of size {}x{}.", viewport.tl.x,
			  viewport.tl.y, viewport.tl.x + viewport.size.x, viewport.tl.y + viewport.size.y, m_viewport.size.x, m_viewport.size.y);

	return {m_fbo, {m_viewport.tl + viewport.tl, viewport.size}};
}