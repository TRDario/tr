/// @file
/// @brief Implements dynamic_index_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/dynamic_index_buffer.hpp>
#include <tr/sysgfx/graphics_context.hpp>

//

bool tr::dynamic_index_buffer::empty() const noexcept
{
	return m_size == 0;
}

tr::usize tr::dynamic_index_buffer::size() const noexcept
{
	return m_size;
}

tr::usize tr::dynamic_index_buffer::capacity() const noexcept
{
	return m_capacity;
}

//

void tr::dynamic_index_buffer::clear() noexcept
{
	m_size = 0;
}

void tr::dynamic_index_buffer::resize(usize size)
{
	reserve(size);
	m_size = size;
}

void tr::dynamic_index_buffer::reserve(usize capacity)
{
	const internal::opengl& gl{context().gl()};

	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		reallocate();
		gl.allocate_buffer_storage(unwrap(), capacity * sizeof(u16), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (gl.get_error() == GL_OUT_OF_MEMORY) {
			throw out_of_memory{"allocation of index buffer '{}'", label()};
		}
		m_capacity = capacity;
	}
	else {
		gl.invalidate_buffer_data(unwrap());
	}
	m_size = 0;
}

void tr::dynamic_index_buffer::set_region(usize offset, std::span<const u16> data) noexcept
{
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in index buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	context().gl().set_buffer_sub_data(unwrap(), offset * sizeof(u16), data.size() * sizeof(u16), data.data());
}

void tr::dynamic_index_buffer::set(std::span<const u16> data)
{
	resize(data.size());
	set_region(0, data);
}