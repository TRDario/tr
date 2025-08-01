#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////// BASIC STATIC VERTEX BUFFER ////////////////////////////////////////////////////////

tr::gfx::basic_static_vertex_buffer::basic_static_vertex_buffer(std::span<const std::byte> data)
	: m_size{ssize(data)}
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
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), static_cast<GLsizei>(label.size()), label.data());
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

std::size_t tr::gfx::basic_dyn_vertex_buffer::size() const
{
	return m_size;
}

std::size_t tr::gfx::basic_dyn_vertex_buffer::capacity() const
{
	return m_capacity;
}

void tr::gfx::basic_dyn_vertex_buffer::clear()
{
	m_size = 0;
}

void tr::gfx::basic_dyn_vertex_buffer::resize(std::size_t size)
{
	reserve(size);
	m_size = size;
}

void tr::gfx::basic_dyn_vertex_buffer::reserve(std::size_t capacity)
{
	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		unsigned int vbo;
		TR_GL_CALL(glCreateBuffers, 1, &vbo);
		TR_GL_CALL(glDeleteBuffers, 1, &m_vbo.get());
		m_vbo.reset(vbo);
		if (!m_label.empty()) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), static_cast<GLsizei>(m_label.size()), m_label.data());
		}

		TR_GL_CALL(glNamedBufferStorage, m_vbo.get(), static_cast<GLsizei>(capacity), nullptr, GL_DYNAMIC_STORAGE_BIT);
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

void tr::gfx::basic_dyn_vertex_buffer::set_region(std::size_t offset, std::span<const std::byte> data)
{
	TR_ASSERT(offset + ssize(data) <= m_size, "Tried to set out-of-bounds region [{}, {}) in a vertex buffer of size {}.", offset,
			  offset + ssize(data), m_size);

	TR_GL_CALL(glNamedBufferSubData, m_vbo.get(), offset, ssize(data), data.data());
}

void tr::gfx::basic_dyn_vertex_buffer::set_label(const std::string& label)
{
	set_label(std::string{label});
}

void tr::gfx::basic_dyn_vertex_buffer::set_label(std::string&& label)
{
	m_label = std::move(label);
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), static_cast<GLsizei>(m_label.size()), m_label.data());
}