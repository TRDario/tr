#include "../../include/tr/sysgfx/uniform_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

tr::gfx::basic_uniform_buffer::basic_uniform_buffer(ssize size)
	: m_size{size}
{
	GLuint ubo;
	TR_GL_CALL(glCreateBuffers, 1, &ubo);
	m_ubo.reset(ubo);

	TR_GL_CALL(glNamedBufferStorage, ubo, size, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_WRITE_ONLY);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"uniform buffer allocation"};
	}
}

void tr::gfx::basic_uniform_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

tr::ssize tr::gfx::basic_uniform_buffer::size() const
{
	return m_size;
}

void tr::gfx::basic_uniform_buffer::set(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the header of a mapped uniform buffer.");
	TR_ASSERT(std::ssize(data) == size(), "Tried to set a uniform buffer of size {} with data of size {}.", size(), std::ssize(data));

	TR_GL_CALL(glNamedBufferSubData, m_ubo.get(), 0, data.size(), data.data());
}

bool tr::gfx::basic_uniform_buffer::mapped() const
{
	int mapped;
	TR_GL_CALL(glGetNamedBufferParameteriv, m_ubo.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::gfx::basic_buffer_map tr::gfx::basic_uniform_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map an already-mapped buffer.");

	std::byte* ptr{
		(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, m_ubo.get(), 0, m_size, GL_DYNAMIC_STORAGE_BIT | GL_WRITE_ONLY))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"uniform buffer mapping"};
	}
	return basic_buffer_map{m_ubo.get(), std::span{ptr, usize(m_size)}};
}

void tr::gfx::basic_uniform_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ubo.get(), GLsizei(label.size()), label.data());
}