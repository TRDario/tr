///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements index_buffer.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////////// STATIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::gfx::static_index_buffer::static_index_buffer(std::span<const u16> data)
	: m_size{std::ssize(data)}
{
	TR_GL_CALL(glNamedBufferStorage, id(), GLsizeiptr(m_size * sizeof(u16)), data.data(), 0);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"index buffer allocation"};
	}
}

////////////////////////////////////////////////////////// DYNAMIC INDEX BUFFER ///////////////////////////////////////////////////////////

bool tr::gfx::dyn_index_buffer::empty() const
{
	return m_size == 0;
}

tr::usize tr::gfx::dyn_index_buffer::size() const
{
	return m_size;
}

tr::usize tr::gfx::dyn_index_buffer::capacity() const
{
	return m_capacity;
}

//

void tr::gfx::dyn_index_buffer::clear()
{
	m_size = 0;
}

void tr::gfx::dyn_index_buffer::resize(usize size)
{
	reserve(size);
	m_size = size;
}

void tr::gfx::dyn_index_buffer::reserve(usize capacity)
{
	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		reallocate();
		TR_GL_CALL(glNamedBufferStorage, id(), GLsizeiptr(capacity * sizeof(u16)), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (glGetError() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
			throw out_of_memory{"allocation of index buffer '{}'", label()};
#else
			throw out_of_memory{"index buffer allocation"};
#endif
		}
		m_capacity = capacity;
	}
	else {
		TR_GL_CALL(glInvalidateBufferData, id());
	}
	m_size = 0;
}

void tr::gfx::dyn_index_buffer::set_region(usize offset, std::span<const u16> data)
{
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in index buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	TR_GL_CALL(glNamedBufferSubData, id(), GLintptr(offset * sizeof(u16)), GLsizeiptr(data.size() * sizeof(u16)), data.data());
}

void tr::gfx::dyn_index_buffer::set(std::span<const u16> data)
{
	resize(data.size());
	set_region(0, data);
}