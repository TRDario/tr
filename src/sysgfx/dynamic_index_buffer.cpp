/// @file
/// @brief Implements dynamic_index_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/dynamic_index_buffer.hpp>
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/utility/exception.hpp>

//

namespace
{
	/// @brief Dynamic index buffer storage flags.
	constexpr unsigned int storage_flags{GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT};
} // namespace

//

tr::dynamic_index_buffer::dynamic_index_buffer(graphics_context& context, std::string_view label) noexcept
	: graphics_buffer{context, label}
	, m_size{0}
	, m_capacity{0}
{
}

//

void tr::dynamic_index_buffer::clear() noexcept
{
	TR_ASSERT(valid(), "Tried to clear a dynamic index buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to clear mapped dynamic index buffer {}.", *this);

	m_size = 0;
}

void tr::dynamic_index_buffer::reserve(usize capacity)
{
	TR_ASSERT(valid(), "Tried to reserve capacity for a dynamic index buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to reserve mapped dynamic index buffer {}.", *this);

	graphics_context& context{this->context()};
	const internal::opengl& gl{context.gl()};
	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		if (m_capacity == 0) {
			gl.allocate_buffer_storage(handle(), capacity * sizeof(u16), nullptr, storage_flags);
			if (gl.get_error() == GL_OUT_OF_MEMORY) {
				throw out_of_memory{"allocation of dynamic index buffer {}", *this};
			}
		}
		else {
			graphics_buffer reallocated{context};
			gl.allocate_buffer_storage(reallocated.handle(), capacity * sizeof(u16), nullptr, storage_flags);
			if (gl.get_error() == GL_OUT_OF_MEMORY) {
				throw out_of_memory{"allocation of dynamic index buffer {}", *this};
			}
			replace_with(std::move(reallocated));
		}
		m_capacity = capacity;
	}
	else if (capacity > 0) {
		gl.invalidate_buffer_data(handle());
	}
	m_size = 0;
}

void tr::dynamic_index_buffer::resize(usize size)
{
	TR_ASSERT(valid(), "Tried to resize a dynamic index buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to resize mapped dynamic index buffer {}.", *this);

	reserve(size);
	m_size = size;
}

void tr::dynamic_index_buffer::set_region(usize offset, std::span<const u16> data) noexcept
{
	TR_ASSERT(valid(), "Tried to set a region of a dynamic index buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to set a region of mapped dynamic index buffer {}.", *this);
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in dynamic index buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	context().gl().set_buffer_sub_data(handle(), offset * sizeof(u16), data.size() * sizeof(u16), data.data());
}

void tr::dynamic_index_buffer::set(std::span<const u16> data)
{
	TR_ASSERT(valid(), "Tried to set a dynamic index buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to set mapped dynamic index buffer {}.", *this);

	resize(data.size());
	set_region(0, data);
}

tr::mapped_graphics_buffer_span<tr::u16> tr::dynamic_index_buffer::map()
{
	TR_ASSERT(valid(), "Tried to map a dynamic index buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to map already-mapped index buffer {}.", *this);
	TR_ASSERT(!empty(), "Tried to map empty dynamic index buffer {}.", *this);

	const usize size_bytes{m_size * sizeof(u16)};
	const internal::opengl& gl{context().gl()};
	std::byte* const map_pointer{static_cast<std::byte*>(gl.map_buffer_range(handle(), 0, size_bytes, storage_flags))};
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"mapping of dynamic index buffer {}", *this};
	}
	return mapped_graphics_buffer_span<u16>{mapped_untyped_graphics_buffer_span{context(), handle(), std::span{map_pointer, size_bytes}}};
}

//

bool tr::dynamic_index_buffer::empty() const noexcept
{
	TR_ASSERT(valid(), "Tried to check if a dynamic index buffer in an invalid state is empty.");

	return m_size == 0;
}

tr::usize tr::dynamic_index_buffer::size() const noexcept
{
	TR_ASSERT(valid(), "Tried to get size of a dynamic index buffer in an invalid state.");

	return m_size;
}

tr::usize tr::dynamic_index_buffer::capacity() const noexcept
{
	TR_ASSERT(valid(), "Tried to get capacity of a dynamic index buffer in an invalid state.");

	return m_capacity;
}