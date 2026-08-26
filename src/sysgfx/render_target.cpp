/// @file
/// @brief Implements render_target.hpp.

#include "../../include/tr/sysgfx/render_target.hpp"
#include "../../include/tr/sysgfx/framebuffer.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/window_view.hpp"

//

tr::render_target::render_target(const graphics_context& context)
#ifdef TR_ENABLE_GL_CHECKS
	: m_framebuffer_info{0, context.window().size(), context, graphics_object_id::invalid, "<backbuffer>"}
#else
	: m_framebuffer_info{0, context.window().size()}
#endif
	, m_viewport{{}, m_framebuffer_info.size}
	, m_scissor_box{{}, m_framebuffer_info.size}
{
}

tr::render_target::render_target(const framebuffer& framebuffer, glm::ivec2 framebuffer_size)
	: render_target{framebuffer, framebuffer_size, {{}, framebuffer_size}, {{}, framebuffer_size}}
{
}

tr::render_target::render_target(const framebuffer& framebuffer, glm::ivec2 framebuffer_size, rectangle<int> viewport,
								 rectangle<int> scissor_box)
#ifdef TR_ENABLE_GL_CHECKS
	: m_framebuffer_info{framebuffer.unwrap(), framebuffer_size, framebuffer.context(), framebuffer.id(), framebuffer.label()}
#else
	: m_framebuffer_info{framebuffer.unwrap(), framebuffer_size}
#endif
	, m_viewport{viewport}
	, m_scissor_box{scissor_box}
{
}

//

glm::ivec2 tr::render_target::size() const
{
	return m_viewport.size;
}

tr::rectangle<int> tr::render_target::viewport() const
{
	return m_viewport;
}

tr::rectangle<int> tr::render_target::scissor_box() const
{
	return m_scissor_box;
}

//

tr::render_target tr::render_target::cropped(rectangle<int> viewport) const
{
	render_target cropped{*this};
	cropped.m_viewport.tl += viewport.tl;
	cropped.m_viewport.size = viewport.size;
	return cropped;
}

tr::render_target tr::render_target::scissored(rectangle<int> scissor_box) const
{
	render_target scissored{*this};
	scissored.m_scissor_box.tl += scissor_box.tl;
	scissored.m_scissor_box.size = scissor_box.size;
	return scissored;
}

//

const tr::render_target::framebuffer_info_t& tr::render_target::framebuffer_info() const
{
	return m_framebuffer_info;
}