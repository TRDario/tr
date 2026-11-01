/// @file
/// @brief Implements dynamic_index_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/dynamic_index_buffer.hpp>
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/utility/exception.hpp>

//

namespace
{
	/// Dynamic index buffer storage flags.
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
	TR_ASSERT(!mapped(), "Tried to clear mapped index buffer {}.", *this);

	m_size = 0;
}

void tr::dynamic_index_buffer::resize(usize size)
{
	TR_ASSERT(!mapped(), "Tried to resize mapped index buffer {}.", *this);

	reserve(size);
	m_size = size;
}

void tr::dynamic_index_buffer::reserve(usize capacity)
{
	TR_ASSERT(!mapped(), "Tried to reserve mapped index buffer {}.", *this);

	const internal::opengl& gl{context().gl()};

	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		reallocate();
		gl.allocate_buffer_storage(unwrap(), capacity * sizeof(u16), nullptr, storage_flags);
		if (gl.get_error() == GL_OUT_OF_MEMORY) {
			throw out_of_memory{"allocation of index buffer {}", *this};
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
	TR_ASSERT(!mapped(), "Tried to set a region of mapped index buffer {}.", *this);
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in index buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	context().gl().set_buffer_sub_data(unwrap(), offset * sizeof(u16), data.size() * sizeof(u16), data.data());
}

void tr::dynamic_index_buffer::set(std::span<const u16> data)
{
	TR_ASSERT(!mapped(), "Tried to set mapped index buffer {}.", *this);

	resize(data.size());
	set_region(0, data);
}

//

bool tr::dynamic_index_buffer::mapped() const noexcept
{
	int mapped;
	context().gl().get_buffer_parameter_iv(unwrap(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::mapped_graphics_buffer_span<tr::u16> tr::dynamic_index_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map already-mapped index buffer {}.", *this);
	TR_ASSERT(!empty(), "Tried to map empty index buffer {}.", *this);

	const usize size_bytes{m_size * sizeof(u16)};
	const internal::opengl& gl{context().gl()};
	std::byte* const map_pointer{static_cast<std::byte*>(gl.map_buffer_range(unwrap(), 0, size_bytes, storage_flags))};
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"mapping of index buffer {}", *this};
	}
	return mapped_graphics_buffer_span<u16>{mapped_untyped_graphics_buffer_span{context(), unwrap(), std::span{map_pointer, size_bytes}}};
}
