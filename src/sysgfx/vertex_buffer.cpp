///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of vertex_buffer.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/utility/exception.hpp"

/////////////////////////////////////////////////////// BASIC STATIC VERTEX BUFFER ////////////////////////////////////////////////////////

tr::basic_static_vertex_buffer::basic_static_vertex_buffer(graphics_context& context, std::span<const std::byte> data)
	: graphics_buffer{context}
	, m_size{std::ssize(data)}
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.named_buffer_storage(id(), m_size, data.data(), 0);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"vertex buffer allocation"};
	}
}

/////////////////////////////////////////////////////// BASIC DYNAMIC VERTEX BUFFER ///////////////////////////////////////////////////////

bool tr::basic_dyn_vertex_buffer::empty() const
{
	return m_size == 0;
}

tr::usize tr::basic_dyn_vertex_buffer::size() const
{
	return m_size;
}

tr::usize tr::basic_dyn_vertex_buffer::capacity() const
{
	return m_capacity;
}

void tr::basic_dyn_vertex_buffer::clear()
{
	m_size = 0;
}

void tr::basic_dyn_vertex_buffer::resize(usize size)
{
	reserve(size);
	m_size = size;
}

void tr::basic_dyn_vertex_buffer::reserve(usize capacity)
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		reallocate();
		gl.named_buffer_storage(id(), capacity, nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (gl.get_error() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
			throw out_of_memory{"allocation of vertex buffer '{}'", label()};
#else
			throw out_of_memory{"vertex buffer allocation"};
#endif
		}
		m_capacity = capacity;
	}
	else {
		gl.invalidate_buffer_data(id());
	}
	m_size = 0;
}

void tr::basic_dyn_vertex_buffer::set(std::span<const std::byte> data)
{
	resize(data.size());
	set_region(0, data);
}

void tr::basic_dyn_vertex_buffer::set_region(usize offset, std::span<const std::byte> data)
{
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in vertex buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.named_buffer_sub_data(id(), offset, data.size(), data.data());
}