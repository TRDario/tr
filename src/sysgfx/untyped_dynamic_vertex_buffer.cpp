/// @file
/// @brief Implements untyped_dynamic_vertex_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_dynamic_vertex_buffer.hpp>
#include <tr/utility/exception.hpp>

//

namespace
{
	/// Dynamic vertex buffer storage flags.
	constexpr unsigned int storage_flags{GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT};
} // namespace

//

tr::untyped_dynamic_vertex_buffer::untyped_dynamic_vertex_buffer(graphics_context& context, std::string_view label) noexcept
	: graphics_buffer{context, label}
	, m_size{0}
	, m_capacity{0}
{
}

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
	TR_ASSERT(!mapped(), "Tried to clear mapped vertex buffer {}.", *this);
	m_size = 0;
}

void tr::untyped_dynamic_vertex_buffer::resize(usize size)
{
	TR_ASSERT(!mapped(), "Tried to resize mapped vertex buffer {}.", *this);
	reserve(size);
	m_size = size;
}

void tr::untyped_dynamic_vertex_buffer::reserve(usize capacity)
{
	TR_ASSERT(!mapped(), "Tried to reserve mapped vertex buffer {}.", *this);

	const internal::opengl& gl{context().gl()};
	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		reallocate();
		gl.allocate_buffer_storage(unwrap(), capacity, nullptr, storage_flags);
		if (gl.get_error() == GL_OUT_OF_MEMORY) {
			throw out_of_memory{"allocation of vertex buffer {}", *this};
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
	TR_ASSERT(!mapped(), "Tried to set mapped vertex buffer {}.", *this);

	resize(data.size());
	set_region(0, data);
}

void tr::untyped_dynamic_vertex_buffer::set_region(usize offset, std::span<const std::byte> data) noexcept
{
	TR_ASSERT(!mapped(), "Tried to set a region of mapped vertex buffer {}.", *this);
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in vertex buffer {} of size {}.", offset,
			  offset + data.size(), *this, m_size);

	context().gl().set_buffer_sub_data(unwrap(), offset, data.size(), data.data());
}

//

bool tr::untyped_dynamic_vertex_buffer::mapped() const noexcept
{
	int is_mapped;
	context().gl().get_buffer_parameter_iv(unwrap(), GL_BUFFER_MAPPED, &is_mapped);
	return is_mapped;
}

tr::mapped_untyped_graphics_buffer_span tr::untyped_dynamic_vertex_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map already-mapped vertex buffer {}.", *this);
	TR_ASSERT(!empty(), "Tried to map empty vertex buffer {}.", *this);

	const internal::opengl& gl{context().gl()};
	std::byte* const map_pointer{static_cast<std::byte*>(gl.map_buffer_range(unwrap(), 0, m_size, storage_flags))};
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"mapping of vertex buffer {}", *this};
	}
	return mapped_untyped_graphics_buffer_span{context(), unwrap(), std::span{map_pointer, m_size}};
}
