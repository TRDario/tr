#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////////// STATIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::gfx::static_index_buffer::static_index_buffer(std::span<const u16> data)
	: m_size{std::ssize(data)}
{
	GLuint ibo;
	TR_GL_CALL(glCreateBuffers, 1, &ibo);
	m_ibo.reset(ibo);

	TR_GL_CALL(glNamedBufferStorage, m_ibo.get(), GLsizeiptr(m_size * sizeof(u16)), data.data(), 0);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"index buffer allocation"};
	}
}

void tr::gfx::static_index_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::static_index_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::static_index_buffer::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_ibo.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_ibo.get(), length, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif

////////////////////////////////////////////////////////// DYNAMIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::gfx::dyn_index_buffer::dyn_index_buffer()
	: m_size{0}, m_capacity{0}
{
	GLuint ibo;
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

#ifdef TR_ENABLE_ASSERTS
		char label_buffer[64];
		GLsizei label_length;
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_ibo.get(), std::size(label_buffer), &label_length, label_buffer);
#endif

		unsigned int ibo;
		TR_GL_CALL(glCreateBuffers, 1, &ibo);
		m_ibo.reset(ibo);

#ifdef TR_ENABLE_ASSERTS
		if (label_length > 0) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), label_length, label_buffer);
		}
#endif

		TR_GL_CALL(glNamedBufferStorage, m_ibo.get(), GLsizeiptr(capacity * sizeof(u16)), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (glGetError() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
			throw out_of_memory{std::format("allocation of index buffer '{}'", label())};
#else
			throw out_of_memory{"index buffer allocation"};
#endif
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
	TR_ASSERT(offset + data.size() <= m_size, "Tried to set out-of-bounds region [{}, {}) in index buffer '{}' of size {}.", offset,
			  offset + data.size(), label(), m_size);

	TR_GL_CALL(glNamedBufferSubData, m_ibo.get(), GLintptr(offset * sizeof(u16)), GLsizeiptr(data.size() * sizeof(u16)), data.data());
}

void tr::gfx::dyn_index_buffer::set(std::span<const u16> data)
{
	resize(data.size());
	set_region(0, data);
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::dyn_index_buffer::set_label(std::string_view label)
{
	TR_ASSERT(label.size() <= 64, "Tried to set an overlong index buffer label of {} characters (max is 64).", label.size());

	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::dyn_index_buffer::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_ibo.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_ibo.get(), length, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif