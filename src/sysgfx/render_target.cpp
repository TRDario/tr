///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements render_target.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/render_target.hpp"

////////////////////////////////////////////////////////////// RENDER TARGET //////////////////////////////////////////////////////////////

tr::render_target::render_target(unsigned int fbo, glm::ivec2 fbo_size)
	: m_fbo{fbo}
	, m_fbo_size{fbo_size}
	, m_viewport{{}, fbo_size}
	, m_scissor_box{{}, fbo_size}
{
}

tr::render_target::render_target(unsigned int fbo, glm::ivec2 fbo_size, const rectangle<int>& viewport, const rectangle<int>& scissor_box)
	: m_fbo{fbo}
	, m_fbo_size{fbo_size}
	, m_viewport{viewport}
	, m_scissor_box{scissor_box}
{
}

const glm::ivec2& tr::render_target::size() const
{
	return m_viewport.size;
}

tr::render_target tr::render_target::cropped(const rectangle<int>& viewport) const
{
	const rectangle<int> absolute_viewport{m_viewport.tl + viewport.tl, viewport.size};
	return {m_fbo, m_fbo_size, absolute_viewport, absolute_viewport};
}

tr::render_target tr::render_target::scissored(const rectangle<int>& scissor_box) const
{
	const rectangle<int> absolute_scissor_box{m_viewport.tl + scissor_box.tl, scissor_box.size};
	return {m_fbo, m_fbo_size, m_viewport, absolute_scissor_box};
}

tr::render_target tr::render_target::subtarget(const rectangle<int>& viewport, const rectangle<int>& scissor_box) const
{
	const rectangle<int> absolute_viewport{m_viewport.tl + viewport.tl, viewport.size};
	const rectangle<int> absolute_scissor_box{absolute_viewport.tl + scissor_box.tl, scissor_box.size};
	return {m_fbo, m_fbo_size, absolute_viewport, absolute_scissor_box};
}