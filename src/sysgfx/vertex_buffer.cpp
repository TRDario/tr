#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////// BASIC STATIC VERTEX BUFFER ////////////////////////////////////////////////////////

tr::gfx::basic_static_vertex_buffer::basic_static_vertex_buffer(std::span<const std::byte> data)
	: m_size{std::ssize(data)}
{
	unsigned int vbo;
	TR_GL_CALL(glCreateBuffers, 1, &vbo);
	m_vbo.reset(vbo);

	TR_GL_CALL(glNamedBufferStorage, m_vbo.get(), m_size, data.data(), 0);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"vertex buffer allocation"};
	}
}

void tr::gfx::basic_static_vertex_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

void tr::gfx::basic_static_vertex_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), GLsizei(label.size()), label.data());
}

/////////////////////////////////////////////////////// BASIC DYNAMIC VERTEX BUFFER ///////////////////////////////////////////////////////

tr::gfx::basic_dyn_vertex_buffer::basic_dyn_vertex_buffer()
	: m_size{0}, m_capacity{0}
{
	unsigned int vbo;
	TR_GL_CALL(glCreateBuffers, 1, &vbo);
	m_vbo.reset(vbo);
}

void tr::gfx::basic_dyn_vertex_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

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

		char label_buffer[128];
		GLsizei label_length;
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_vbo.get(), 128, &label_length, label_buffer);

		unsigned int vbo;
		TR_GL_CALL(glCreateBuffers, 1, &vbo);
		m_vbo.reset(vbo);
		if (label_length > 0) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), label_length, label_buffer);
		}

		TR_GL_CALL(glNamedBufferStorage, m_vbo.get(), GLsizei(capacity), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (glGetError() == GL_OUT_OF_MEMORY) {
			throw out_of_memory{"vertex buffer allocation"};
		}
		m_capacity = capacity;
	}
	else {
		TR_GL_CALL(glInvalidateBufferData, m_vbo.get());
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
	TR_ASSERT(offset + std::ssize(data) <= m_size, "Tried to set out-of-bounds region [{}, {}) in a vertex buffer of size {}.", offset,
			  offset + std::ssize(data), m_size);

	TR_GL_CALL(glNamedBufferSubData, m_vbo.get(), offset, std::ssize(data), data.data());
}

void tr::gfx::basic_dyn_vertex_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), GLsizei(label.size()), label.data());
}