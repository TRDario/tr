///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of vertex_buffer.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////// BASIC STATIC VERTEX BUFFER ////////////////////////////////////////////////////////

tr::gfx::basic_static_vertex_buffer::basic_static_vertex_buffer(std::span<const std::byte> data)
	: m_size{std::ssize(data)}
{
	TR_GL_CALL(glNamedBufferStorage, id(), m_size, data.data(), 0);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"vertex buffer allocation"};
	}
}

/////////////////////////////////////////////////////// BASIC DYNAMIC VERTEX BUFFER ///////////////////////////////////////////////////////

bool tr::gfx::basic_dyn_vertex_buffer::empty() const
{
	return m_size == 0;
}

tr::usize tr::gfx::basic_dyn_vertex_buffer::size() const
{
	return m_size;
}

tr::usize tr::gfx::basic_dyn_vertex_buffer::capacity() const
{
	return m_capacity;
}

void tr::gfx::basic_dyn_vertex_buffer::clear()
{
	m_size = 0;
}

void tr::gfx::basic_dyn_vertex_buffer::resize(usize size)
{
	reserve(size);
	m_size = size;
}

void tr::gfx::basic_dyn_vertex_buffer::reserve(usize capacity)
{
	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		reallocate();
		TR_GL_CALL(glNamedBufferStorage, id(), GLsizeiptr(capacity), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (glGetError() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
			throw out_of_memory{"allocation of vertex buffer '{}'", label()};
#else
			throw out_of_memory{"vertex buffer allocation"};
#endif
		}
		m_capacity = capacity;
	}
	else {
		TR_GL_CALL(glInvalidateBufferData, id());
	}
	m_size = 0;
}

void tr::gfx::basic_dyn_vertex_buffer::set(std::span<const std::byte> data)
{
	resize(data.size());
	set_region(0, data);
}

void tr::gfx::basic_dyn_vertex_buffer::set_region(usize offset, std::span<const std::byte> data)
{
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in vertex buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	TR_GL_CALL(glNamedBufferSubData, id(), GLintptr(offset), GLsizeiptr(data.size()), data.data());
}