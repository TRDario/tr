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
	/// @brief Dynamic vertex buffer storage flags.
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

void tr::untyped_dynamic_vertex_buffer::clear() noexcept
{
	TR_ASSERT(valid(), "Tried to clear a dynamic vertex buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to clear mapped vertex buffer {}.", *this);

	m_size = 0;
}

void tr::untyped_dynamic_vertex_buffer::reserve(usize capacity)
{
	TR_ASSERT(valid(), "Tried to reserve capacity for a dynamic vertex buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to reserve mapped vertex buffer {}.", *this);

	graphics_context& context{this->context()};
	const internal::opengl& gl{context.gl()};
	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		if (m_capacity == 0) {
			gl.allocate_buffer_storage(handle(), capacity, nullptr, storage_flags);
			if (gl.get_error() == GL_OUT_OF_MEMORY) {
				throw out_of_memory{"allocation of vertex buffer {}", *this};
			}
		}
		else {
			graphics_buffer reallocated{context};
			gl.allocate_buffer_storage(reallocated.handle(), capacity, nullptr, storage_flags);
			if (gl.get_error() == GL_OUT_OF_MEMORY) {
				throw out_of_memory{"allocation of vertex buffer {}", *this};
			}
			replace_with(std::move(reallocated));
		}
		m_capacity = capacity;
	}
	else {
		gl.invalidate_buffer_data(handle());
	}
	m_size = 0;
}

void tr::untyped_dynamic_vertex_buffer::resize(usize size)
{
	TR_ASSERT(valid(), "Tried to resize a dynamic vertex buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to resize mapped vertex buffer {}.", *this);

	reserve(size);
	m_size = size;
}

void tr::untyped_dynamic_vertex_buffer::set(std::span<const std::byte> data)
{
	TR_ASSERT(valid(), "Tried to set a dynamic vertex buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to set mapped dynamic vertex buffer {}.", *this);

	resize(data.size());
	set_region(0, data);
}

void tr::untyped_dynamic_vertex_buffer::set_region(usize offset, std::span<const std::byte> data) noexcept
{
	TR_ASSERT(valid(), "Tried to set a region of a dynamic vertex buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to set a region of mapped dynamic vertex buffer {}.", *this);
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in dynamic vertex buffer {} of size {}.", offset,
			  offset + data.size(), *this, m_size);

	context().gl().set_buffer_sub_data(handle(), offset, data.size(), data.data());
}

tr::mapped_untyped_graphics_buffer_span tr::untyped_dynamic_vertex_buffer::map()
{
	TR_ASSERT(valid(), "Tried to map a dynamic vertex buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to map already-mapped dynamic vertex buffer {}.", *this);
	TR_ASSERT(!empty(), "Tried to map empty dynamic vertex buffer {}.", *this);

	const internal::opengl& gl{context().gl()};
	std::byte* const map_pointer{static_cast<std::byte*>(gl.map_buffer_range(handle(), 0, m_size, storage_flags))};
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"mapping of dynamic vertex buffer {}", *this};
	}
	return mapped_untyped_graphics_buffer_span{context(), handle(), std::span{map_pointer, m_size}};
}

//

bool tr::untyped_dynamic_vertex_buffer::empty() const noexcept
{
	TR_ASSERT(valid(), "Tried to check if a dynamic vertex buffer in an invalid state is empty.");

	return m_size == 0;
}

tr::usize tr::untyped_dynamic_vertex_buffer::size() const noexcept
{
	TR_ASSERT(valid(), "Tried to get size of a dynamic vertex buffer in an invalid state.");

	return m_size;
}

tr::usize tr::untyped_dynamic_vertex_buffer::capacity() const noexcept
{
	TR_ASSERT(valid(), "Tried to get capacity of a dynamic vertex buffer in an invalid state.");

	return m_capacity;
}