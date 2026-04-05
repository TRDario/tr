///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements render_target.hpp.                                                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/render_target.hpp"

////////////////////////////////////////////////////////////// RENDER TARGET //////////////////////////////////////////////////////////////

tr::gfx::render_target::render_target(unsigned int fbo, glm::ivec2 fbo_size)
	: m_fbo{fbo}, m_fbo_size{fbo_size}, m_viewport{{}, fbo_size}, m_scissor_box{{}, fbo_size}
{
}

tr::gfx::render_target::render_target(unsigned int fbo, glm::ivec2 fbo_size, const irect2& viewport, const irect2& scissor_box)
	: m_fbo{fbo}, m_fbo_size{fbo_size}, m_viewport{viewport}, m_scissor_box{scissor_box}
{
}

const glm::ivec2& tr::gfx::render_target::size() const
{
	return m_viewport.size;
}

tr::gfx::render_target tr::gfx::render_target::cropped(const irect2& viewport) const
{
	const irect2 absolute_viewport{m_viewport.tl + viewport.tl, viewport.size};

	TR_ASSERT(m_viewport.contains(absolute_viewport),
			  "Tried to create out-of-bounds sub-target from ({}, {}) to ({}, {}) of a render target of size {}x{}.", viewport.tl.x,
			  viewport.tl.y, viewport.tl.x + viewport.size.x, viewport.tl.y + viewport.size.y, m_viewport.size.x, m_viewport.size.y);

	return {m_fbo, m_fbo_size, absolute_viewport, absolute_viewport};
}

tr::gfx::render_target tr::gfx::render_target::scissored(const irect2& scissor_box) const
{
	const irect2 absolute_scissor_box{m_viewport.tl + scissor_box.tl, scissor_box.size};

	TR_ASSERT(m_viewport.contains(absolute_scissor_box),
			  "Tried to create out-of-bounds scissor box from ({}, {}) to ({}, {}) of a render target of size {}x{}.", scissor_box.tl.x,
			  scissor_box.tl.y, scissor_box.tl.x + scissor_box.size.x, scissor_box.tl.y + scissor_box.size.y, m_viewport.size.x,
			  m_viewport.size.y);

	return {m_fbo, m_fbo_size, m_viewport, absolute_scissor_box};
}

tr::gfx::render_target tr::gfx::render_target::subtarget(const irect2& viewport, const irect2& scissor_box) const
{
	const irect2 absolute_viewport{m_viewport.tl + viewport.tl, viewport.size};
	const irect2 absolute_scissor_box{absolute_viewport.tl + scissor_box.tl, scissor_box.size};

	TR_ASSERT(m_viewport.contains(absolute_viewport),
			  "Tried to create out-of-bounds sub-target from ({}, {}) to ({}, {}) of a render target of size {}x{}.", viewport.tl.x,
			  viewport.tl.y, viewport.tl.x + viewport.size.x, viewport.tl.y + viewport.size.y, m_viewport.size.x, m_viewport.size.y);
	TR_ASSERT(absolute_viewport.contains(absolute_scissor_box),
			  "Tried to create out-of-bounds scissor box from ({}, {}) to ({}, {}) of a render target of size {}x{}.", scissor_box.tl.x,
			  scissor_box.tl.y, scissor_box.tl.x + scissor_box.size.x, scissor_box.tl.y + scissor_box.size.y, viewport.size.x,
			  viewport.size.y);

	return {m_fbo, m_fbo_size, absolute_viewport, absolute_scissor_box};
}