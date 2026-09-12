/// @file
/// @brief Implements ping_pong_target.hpp.

#include "../../include/tr/sysgfx/ping_pong_target.hpp"
#include "../../include/tr/sysgfx/render_target.hpp"
#include "../../include/tr/sysgfx/texture_view.hpp"

//

tr::ping_pong_target::ping_pong_target(graphics_context& context) noexcept
	: m_targets{texture_target{context}, texture_target{context}}
	, m_source_index{0}
{
}

tr::ping_pong_target::ping_pong_target(graphics_context& context, glm::ivec2 size, mipmaps mipmaps, pixel_format format)
	: m_targets{texture_target{context, size, mipmaps, format}, texture_target{context, size, mipmaps, format}}
	, m_source_index{0}
{
}

//

tr::graphics_context& tr::ping_pong_target::context() const noexcept
{
	TR_ASSERT(valid(), "Tried to get context of a ping-pong target in an invalid state.");

	return m_targets[0].context();
}

//

bool tr::ping_pong_target::valid() const noexcept
{
	return m_targets[0].valid();
}

bool tr::ping_pong_target::complete() const noexcept
{
	TR_ASSERT(valid(), "Tried to check completeness of a ping-pong target in an invalid state.");

	return m_targets[0].complete();
}

glm::ivec2 tr::ping_pong_target::size() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the size of a ping-pong target in an invalid state.");

	return m_targets[0].size();
}

//

void tr::ping_pong_target::allocate(glm::ivec2 size, mipmaps mipmaps, pixel_format format)
{
	TR_ASSERT(valid(), "Tried to allocate to a ping-pong buffer in an invalid state.");

	for (texture_target& target : m_targets) {
		target.allocate(size, mipmaps, format);
	}
}

void tr::ping_pong_target::clear_destination(rgbaf color) noexcept
{
	TR_ASSERT(valid(), "Tried to clear the destination of a ping-pong target in an invalid state.");
	TR_ASSERT(complete(), "Tried to clear the destination of an incomplete ping-pong target.");

	m_targets[1 - m_source_index].clear(color);
}

//

void tr::ping_pong_target::set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept
{
	TR_ASSERT(valid(), "Tried to set filtering of a ping-pong target in an invalid state.");
	TR_ASSERT(complete(), "Tried to set filtering of an incomplete ping-pong target.");

	for (texture_target& target : m_targets) {
		target.set_filtering(min_filter, mag_filter);
	}
}

void tr::ping_pong_target::set_wrap(wrap wrap) noexcept
{
	TR_ASSERT(valid(), "Tried to set wrapping of a ping-pong target in an invalid state.");
	TR_ASSERT(complete(), "Tried to set wrapping of an incomplete ping-pong target.");

	for (texture_target& target : m_targets) {
		target.set_wrap(wrap);
	}
}

void tr::ping_pong_target::set_border_color(rgbaf color) noexcept
{
	TR_ASSERT(valid(), "Tried to set border color of a ping-pong target in an invalid state.");
	TR_ASSERT(complete(), "Tried to set border color of an incomplete ping-pong target.");

	for (texture_target& target : m_targets) {
		target.set_border_color(color);
	}
}

//

tr::texture_view tr::ping_pong_target::source() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the source of a ping-pong target in an invalid state.");
	TR_ASSERT(complete(), "Tried to get the source of an incomplete ping-pong target.");

	return m_targets[m_source_index];
}

tr::render_target tr::ping_pong_target::destination() noexcept
{
	TR_ASSERT(valid(), "Tried to get the destination of a ping-pong target in an invalid state.");
	TR_ASSERT(complete(), "Tried to get the destination of an incomplete ping-pong target.");

	return m_targets[1 - m_source_index];
}

void tr::ping_pong_target::swap() noexcept
{
	TR_ASSERT(valid(), "Tried to swap targets of a ping-pong target in an invalid state.");
	TR_ASSERT(complete(), "Tried to swap targets of an incomplete ping-pong target.");

	m_source_index = (m_source_index + 1) % 2;
}

//

std::string tr::ping_pong_target::label() const
{
	TR_ASSERT(valid(), "Tried to get label of a ping-pong target target in an invalid state.");

	std::string label{m_targets[0].label()};
	if (label.ends_with("- Target 0")) {
		label.resize(label.size() - 10);
	}
	return label;
}

void tr::ping_pong_target::set_label(std::string_view label)
{
	TR_ASSERT(valid(), "Tried to set label of a ping-pong target target in an invalid state.");

	m_targets[0].set_label(std::format("{} - Target 0", label));
	m_targets[1].set_label(std::format("{} - Target 1", label));
}