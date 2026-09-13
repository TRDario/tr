/// @file
/// @brief Implements untyped_dynamic_vertex_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/untyped_dynamic_vertex_buffer.hpp>

//

bool tr::untyped_dynamic_vertex_buffer::empty() const noexcept
{
	return m_size == 0;
}

tr::usize tr::untyped_dynamic_vertex_buffer::size() const noexcept
{
	return m_size;
}

tr::usize tr::untyped_dynamic_vertex_buffer::capacity() const noexcept
{
	return m_capacity;
}

//

void tr::untyped_dynamic_vertex_buffer::clear() noexcept
{
	m_size = 0;
}

void tr::untyped_dynamic_vertex_buffer::resize(usize size)
{
	reserve(size);
	m_size = size;
}

void tr::untyped_dynamic_vertex_buffer::reserve(usize capacity)
{
	const internal::opengl& gl{context().gl()};
	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		reallocate();
		gl.allocate_buffer_storage(unwrap(), capacity, nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (gl.get_error() == GL_OUT_OF_MEMORY) {
			throw out_of_memory{"allocation of vertex buffer '{}'", label()};
		}
		m_capacity = capacity;
	}
	else {
		gl.invalidate_buffer_data(unwrap());
	}
	m_size = 0;
}

void tr::untyped_dynamic_vertex_buffer::set(std::span<const std::byte> data)
{
	resize(data.size());
	set_region(0, data);
}

void tr::untyped_dynamic_vertex_buffer::set_region(usize offset, std::span<const std::byte> data) noexcept
{
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in vertex buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	context().gl().set_buffer_sub_data(unwrap(), offset, data.size(), data.data());
}