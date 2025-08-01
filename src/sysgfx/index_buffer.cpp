#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////////// STATIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::gfx::static_index_buffer::static_index_buffer(std::span<const std::uint16_t> data)
	: size{ssize(data)}
{
	unsigned int ibo;
	TR_GL_CALL(glCreateBuffers, 1, &ibo);
	m_ibo.reset(ibo);

	TR_GL_CALL(glNamedBufferStorage, m_ibo.get(), size * sizeof(std::uint16_t), data.data(), 0);
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
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), static_cast<GLsizei>(label.size()), label.data());
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

std::size_t tr::gfx::dyn_index_buffer::size() const
{
	return m_size;
}

std::size_t tr::gfx::dyn_index_buffer::capacity() const
{
	return m_capacity;
}

void tr::gfx::dyn_index_buffer::clear()
{
	m_size = 0;
}

void tr::gfx::dyn_index_buffer::resize(std::size_t size)
{
	reserve(size);
	m_size = size;
}

void tr::gfx::dyn_index_buffer::reserve(std::size_t capacity)
{
	if (capacity > m_capacity) {
		capacity = std::bit_ceil(capacity);

		unsigned int ibo;
		TR_GL_CALL(glCreateBuffers, 1, &ibo);
		TR_GL_CALL(glDeleteBuffers, 1, &m_ibo.get());
		m_ibo.reset(ibo);
		if (!m_label.empty()) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), static_cast<GLsizei>(m_label.size()), m_label.data());
		}

		TR_GL_CALL(glNamedBufferStorage, m_ibo.get(), static_cast<GLsizei>(capacity * sizeof(std::uint16_t)), nullptr,
				   GL_DYNAMIC_STORAGE_BIT);
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

void tr::gfx::dyn_index_buffer::set_region(std::size_t offset, std::span<const std::uint16_t> data)
{
	TR_ASSERT(offset + ssize(data) <= m_size, "Tried to set out-of-bounds region [{}, {}) in an index buffer of size {}.", offset,
			  offset + ssize(data), m_size);

	TR_GL_CALL(glNamedBufferSubData, m_ibo.get(), offset * sizeof(std::uint16_t), ssize(data) * sizeof(std::uint16_t), data.data());
}

void tr::gfx::dyn_index_buffer::set(std::span<const std::uint16_t> data)
{
	resize(data.size());
	set_region(0, data);
}

void tr::gfx::dyn_index_buffer::set_label(const std::string& label)
{
	set_label(std::string{label});
}

void tr::gfx::dyn_index_buffer::set_label(std::string&& label)
{
	m_label = std::move(label);
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ibo.get(), static_cast<GLsizei>(m_label.size()), m_label.data());
}