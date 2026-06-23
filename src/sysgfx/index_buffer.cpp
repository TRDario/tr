///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements index_buffer.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

/////////////////////////////////////////////////////////// STATIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::static_index_buffer::static_index_buffer(graphics_context& context, std::span<const u16> data)
	: graphics_buffer{context}
	, m_size{std::ssize(data)}
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.named_buffer_storage(id(), m_size * sizeof(u16), data.data(), 0);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"index buffer allocation"};
	}
}

////////////////////////////////////////////////////////// DYNAMIC INDEX BUFFER ///////////////////////////////////////////////////////////

bool tr::dyn_index_buffer::empty() const
{
	return m_size == 0;
}

tr::usize tr::dyn_index_buffer::size() const
{
	return m_size;
}

tr::usize tr::dyn_index_buffer::capacity() const
{
	return m_capacity;
}

//

void tr::dyn_index_buffer::clear()
{
	m_size = 0;
}

void tr::dyn_index_buffer::resize(usize size)
{
	reserve(size);
	m_size = size;
}

void tr::dyn_index_buffer::reserve(usize capacity)
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		reallocate();
		gl.named_buffer_storage(id(), capacity * sizeof(u16), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (gl.get_error() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
			throw out_of_memory{"allocation of index buffer '{}'", label()};
#else
			throw out_of_memory{"index buffer allocation"};
#endif
		}
		m_capacity = capacity;
	}
	else {
		gl.invalidate_buffer_data(id());
	}
	m_size = 0;
}

void tr::dyn_index_buffer::set_region(usize offset, std::span<const u16> data)
{
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in index buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.named_buffer_sub_data(id(), offset * sizeof(u16), data.size() * sizeof(u16), data.data());
}

void tr::dyn_index_buffer::set(std::span<const u16> data)
{
	resize(data.size());
	set_region(0, data);
}