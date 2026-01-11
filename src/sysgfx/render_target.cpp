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