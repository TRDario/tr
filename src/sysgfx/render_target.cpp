/// @file
/// @brief Implements render_target.hpp.

#include "../../include/tr/sysgfx/render_target.hpp"

//

tr::render_target::render_target(unsigned int framebuffer, glm::ivec2 framebuffer_size)
	: m_framebuffer{framebuffer}
	, m_framebuffer_size{framebuffer_size}
	, m_viewport{{}, framebuffer_size}
	, m_scissor_box{{}, framebuffer_size}
{
}

tr::render_target::render_target(unsigned int framebuffer, glm::ivec2 framebuffer_size, rectangle<int> viewport, rectangle<int> scissor_box)
	: m_framebuffer{framebuffer}
	, m_framebuffer_size{framebuffer_size}
	, m_viewport{viewport}
	, m_scissor_box{scissor_box}
{
}

//

glm::ivec2 tr::render_target::size() const
{
	return m_viewport.size;
}

//

tr::render_target tr::render_target::cropped(rectangle<int> viewport) const
{
	const rectangle<int> absolute_viewport{m_viewport.tl + viewport.tl, viewport.size};
	return {m_framebuffer, m_framebuffer_size, absolute_viewport, absolute_viewport};
}

tr::render_target tr::render_target::scissored(rectangle<int> scissor_box) const
{
	const rectangle<int> absolute_scissor_box{m_viewport.tl + scissor_box.tl, scissor_box.size};
	return {m_framebuffer, m_framebuffer_size, m_viewport, absolute_scissor_box};
}

tr::render_target tr::render_target::subtarget(rectangle<int> viewport, rectangle<int> scissor_box) const
{
	const rectangle<int> absolute_viewport{m_viewport.tl + viewport.tl, viewport.size};
	const rectangle<int> absolute_scissor_box{absolute_viewport.tl + scissor_box.tl, scissor_box.size};
	return {m_framebuffer, m_framebuffer_size, absolute_viewport, absolute_scissor_box};
}