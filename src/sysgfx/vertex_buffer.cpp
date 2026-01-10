#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////// BASIC STATIC VERTEX BUFFER ////////////////////////////////////////////////////////

tr::gfx::basic_static_vertex_buffer::basic_static_vertex_buffer(std::span<const std::byte> data)
	: m_size{std::ssize(data)}
{
	GLuint vbo;
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

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::basic_static_vertex_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::basic_static_vertex_buffer::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_vbo.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_vbo.get(), length + 1, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif

/////////////////////////////////////////////////////// BASIC DYNAMIC VERTEX BUFFER ///////////////////////////////////////////////////////

tr::gfx::basic_dyn_vertex_buffer::basic_dyn_vertex_buffer()
	: m_size{0}, m_capacity{0}
{
	GLuint vbo;
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

#ifdef TR_ENABLE_ASSERTS
		char label_buffer[64];
		GLsizei label_length;
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_vbo.get(), std::size(label_buffer), &label_length, label_buffer);
#endif

		GLuint vbo;
		TR_GL_CALL(glCreateBuffers, 1, &vbo);
		m_vbo.reset(vbo);

#ifdef TR_ENABLE_ASSERTS
		if (label_length > 0) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), label_length, label_buffer);
		}
#endif

		TR_GL_CALL(glNamedBufferStorage, m_vbo.get(), GLsizeiptr(capacity), nullptr, GL_DYNAMIC_STORAGE_BIT);
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
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in vertex buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	TR_GL_CALL(glNamedBufferSubData, m_vbo.get(), GLintptr(offset), GLsizeiptr(data.size()), data.data());
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::basic_dyn_vertex_buffer::set_label(std::string_view label)
{
	TR_ASSERT(label.size() <= 64, "Tried to set an overlong vertex buffer label of {} characters (max is 64).", label.size());

	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_vbo.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::basic_dyn_vertex_buffer::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_vbo.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_vbo.get(), length + 1, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif