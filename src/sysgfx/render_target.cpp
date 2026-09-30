/// @file
/// @brief Implements render_target.hpp.

#include <tr/sysgfx/framebuffer.hpp>
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/internal/graphics_object_id.hpp>
#include <tr/sysgfx/render_target.hpp>
#include <tr/sysgfx/window_view.hpp>

//

tr::render_target::render_target([[maybe_unused]] const graphics_context& context, internal::graphics_object_id framebuffer_id,
								 unsigned int framebuffer_fbo, int framebuffer_height, rectangle<u16> viewport,
								 rectangle<u16> scissor_box) noexcept
	:
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	m_context{context}
	,
#endif
	m_framebuffer_id{framebuffer_id}
	, m_framebuffer_fbo{framebuffer_fbo}
	, m_framebuffer_height{framebuffer_height}
	, m_viewport{viewport}
	, m_scissor_box{scissor_box}
{
}

tr::render_target::render_target(const graphics_context& context) noexcept
	: render_target{context,
					internal::graphics_object_id::invalid,
					0,
					context.window().size().y,
					{{}, context.window().size()},
					{{}, context.window().size()}}
{
}

tr::render_target::render_target(const framebuffer& framebuffer, glm::ivec2 framebuffer_size) noexcept
	: render_target{framebuffer, framebuffer_size.y, {{}, framebuffer_size}, {{}, framebuffer_size}}
{
}

tr::render_target::render_target(const framebuffer& framebuffer, int framebuffer_height, rectangle<u16> viewport,
								 rectangle<u16> scissor_box) noexcept
	: render_target{framebuffer.context(), framebuffer.id(), framebuffer.unwrap(), framebuffer_height, viewport, scissor_box}
{
}

//

glm::ivec2 tr::render_target::size() const noexcept
{
	return m_viewport.size;
}

tr::rectangle<tr::u16> tr::render_target::viewport() const noexcept
{
	return m_viewport;
}

tr::rectangle<tr::u16> tr::render_target::scissor_box() const noexcept
{
	return m_scissor_box;
}

//

tr::render_target tr::render_target::cropped(rectangle<u16> viewport) const noexcept
{
	render_target cropped{*this};
	cropped.m_viewport.tl += viewport.tl;
	cropped.m_viewport.size = viewport.size;
	return cropped;
}

tr::render_target tr::render_target::scissored(rectangle<u16> scissor_box) const noexcept
{
	render_target scissored{*this};
	scissored.m_scissor_box.tl += scissor_box.tl;
	scissored.m_scissor_box.size = scissor_box.size;
	return scissored;
}

//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
const tr::graphics_context& tr::render_target::context() const noexcept
{
	return m_context;
}
#endif

tr::internal::graphics_object_id tr::render_target::framebuffer_id() const noexcept
{
	return m_framebuffer_id;
}

unsigned int tr::render_target::framebuffer_fbo() const noexcept
{
	return m_framebuffer_fbo;
}

int tr::render_target::framebuffer_height() const noexcept
{
	return m_framebuffer_height;
}