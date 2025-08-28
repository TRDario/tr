#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////////// STATIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::gfx::static_index_buffer::static_index_buffer(std::span<const u16> data)
	: m_size{std::ssize(data)}
{
	unsigned int ibo;
	TR_GL_CALL(glCreateBuffers, 1, &ibo);
	m_ibo.reset(ibo);

	TR_GL_CALL(glNamedBufferStorage, m_ibo.get(), m_size * sizeof(u16), data.data(), 0);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"index buffer allocation"};
	}
}

void tr::gfx::static_index_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

void tr::gfx::static_index_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), GLsizei(label.size()), label.data());
}

////////////////////////////////////////////////////////// DYNAMIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::gfx::dyn_index_buffer::dyn_index_buffer()
	: m_size{0}, m_capacity{0}
{
	unsigned int ibo;
	TR_GL_CALL(glCreateBuffers, 1, &ibo);
	m_ibo.reset(ibo);
}

void tr::gfx::dyn_index_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

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

		unsigned int ibo;
		TR_GL_CALL(glCreateBuffers, 1, &ibo);
		TR_GL_CALL(glDeleteBuffers, 1, &m_ibo.get());
		m_ibo.reset(ibo);
		if (!m_label.empty()) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), GLsizei(m_label.size()), m_label.data());
		}

		TR_GL_CALL(glNamedBufferStorage, m_ibo.get(), GLsizei(capacity * sizeof(u16)), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (glGetError() == GL_OUT_OF_MEMORY) {
			throw out_of_memory{"index buffer allocation"};
		}
		m_capacity = capacity;
	}
	else {
		TR_GL_CALL(glInvalidateBufferData, m_ibo.get());
	}
	m_size = 0;
}

void tr::gfx::dyn_index_buffer::set_region(usize offset, std::span<const u16> data)
{
	TR_ASSERT(offset + std::ssize(data) <= m_size, "Tried to set out-of-bounds region [{}, {}) in an index buffer of size {}.", offset,
			  offset + std::ssize(data), m_size);

	TR_GL_CALL(glNamedBufferSubData, m_ibo.get(), offset * sizeof(u16), std::ssize(data) * sizeof(u16), data.data());
}

void tr::gfx::dyn_index_buffer::set(std::span<const u16> data)
{
	resize(data.size());
	set_region(0, data);
}

void tr::gfx::dyn_index_buffer::set_label(std::string label)
{
	m_label = std::move(label);
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), GLsizei(m_label.size()), m_label.data());
}