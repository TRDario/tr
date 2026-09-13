/// @file
/// @brief Implements untyped_graphics_buffer_map.hpp.

#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>

//

tr::mapped_untyped_graphics_buffer_span::mapped_untyped_graphics_buffer_span(graphics_context& context, unsigned int buffer,
																			 std::span<std::byte> span) noexcept
	: m_handle{buffer, unmapper{context}}
	, m_span{span}
{
}

tr::mapped_untyped_graphics_buffer_span::operator std::span<std::byte>() const noexcept
{
	return span();
}

std::span<std::byte> tr::mapped_untyped_graphics_buffer_span::span() const noexcept
{
	return m_span;
}

void tr::mapped_untyped_graphics_buffer_span::unmapper::operator()(unsigned int id) const noexcept
{
	context->gl().unmap_buffer(id);
}