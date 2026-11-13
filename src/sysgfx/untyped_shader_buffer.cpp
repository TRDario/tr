/// @file
/// @brief Implements untyped_shader_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_shader_buffer.hpp>
#include <tr/utility/exception.hpp>

//

tr::untyped_shader_buffer::untyped_shader_buffer(graphics_context& context, usize header_size, usize capacity, access map_access,
												 std::string_view label)
	: graphics_buffer{context, label}
	, m_map_access{map_access}
	, m_header_size{header_size}
	, m_array_size{0}
	, m_array_capacity{capacity}
{
	const internal::opengl& gl{context.gl()};
	gl.allocate_buffer_storage(handle(), header_size + capacity, nullptr, std::to_underlying(map_access) | GL_DYNAMIC_STORAGE_BIT);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"allocation of shader buffer {}", *this};
	}
}

//

void tr::untyped_shader_buffer::set_header(std::span<const std::byte> data) noexcept
{
	TR_ASSERT(valid(), "Tried to set the header of a shader buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to set the header of mapped shader buffer {}.", *this);
	TR_ASSERT(header_size() != 0, "Tried to set the header for shader buffer {} without one.", *this);
	TR_ASSERT(data.size() == header_size(), "Tried to set header of shader buffer {} of size {} with data of size {}.", *this,
			  header_size(), data.size());

	context().gl().set_buffer_sub_data(handle(), 0, data.size(), data.data());
}

void tr::untyped_shader_buffer::set_array(std::span<const std::byte> data) noexcept
{
	TR_ASSERT(valid(), "Tried to set the array of a shader buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to set the array of a mapped shader buffer.");
	TR_ASSERT(array_capacity() != 0, "Tried to set the array for a shader buffer without one.");
	TR_ASSERT(data.size() <= array_capacity(), "Tried to set a shader buffer array of capacity {} with data of size {}.", array_capacity(),
			  data.size());

	if (!data.empty()) {
		context().gl().set_buffer_sub_data(handle(), m_header_size, data.size(), data.data());
	}
	m_array_size = data.size();
}

void tr::untyped_shader_buffer::resize_array(usize size) noexcept
{
	TR_ASSERT(valid(), "Tried to resize the array of a shader buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to resize the array of mapped shader buffer {}.", *this);
	TR_ASSERT(size <= array_capacity(), "Tried to resize array of shader buffer {} of capacity {} to size {}.", *this, array_capacity(),
			  size);

	m_array_size = size;
}

tr::mapped_untyped_graphics_buffer_span tr::untyped_shader_buffer::map_range(usize offset, usize size)
{
	TR_ASSERT(valid(), "Tried to map a shader buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to map already-mapped shader buffer {}.", *this);

	const internal::opengl& gl{context().gl()};
	std::byte* const map_pointer{static_cast<std::byte*>(gl.map_buffer_range(handle(), offset, size, std::to_underlying(m_map_access)))};
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"mapping of shader buffer {}", *this};
	}
	return mapped_untyped_graphics_buffer_span{context(), handle(), std::span{map_pointer, size}};
}

tr::mapped_untyped_graphics_buffer_span tr::untyped_shader_buffer::map_header()
{
	TR_ASSERT(valid(), "Tried to map the header of a shader buffer in an invalid state.");
	TR_ASSERT(header_size() != 0, "Tried to map the header of shader buffer {} that doesn't have one.", *this);

	return map_range(0, m_header_size);
}

tr::mapped_untyped_graphics_buffer_span tr::untyped_shader_buffer::map_array()
{
	TR_ASSERT(valid(), "Tried to map the array of a shader buffer in an invalid state.");
	TR_ASSERT(array_size() != 0, "Tried to map the array of shader buffer {} that doesn't have one.", *this);

	return map_range(m_array_size, m_array_size);
}

tr::mapped_untyped_graphics_buffer_span tr::untyped_shader_buffer::map()
{
	TR_ASSERT(valid(), "Tried to map a shader buffer in an invalid state.");

	return map_range(0, m_header_size + m_array_size);
}

//

tr::usize tr::untyped_shader_buffer::header_size() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the header size of a shader buffer in an invalid state.");

	return m_header_size;
}

tr::usize tr::untyped_shader_buffer::array_size() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the array size of a shader buffer in an invalid state.");

	return m_array_size;
}

tr::usize tr::untyped_shader_buffer::array_capacity() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the array capacity of a shader buffer in an invalid state.");

	return m_array_capacity;
}