#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

tr::gfx::basic_shader_buffer::basic_shader_buffer(ssize header_size, ssize capacity, access access)
	: m_access{access}, m_header_size{header_size}, m_array_size{0}, m_array_capacity{capacity}
{
	GLuint sbo;
	TR_GL_CALL(glCreateBuffers, 1, &sbo);
	m_sbo.reset(sbo);

	TR_GL_CALL(glNamedBufferStorage, sbo, header_size + capacity, nullptr, GLenum(access) | GL_DYNAMIC_STORAGE_BIT);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer allocation"};
	}
}

void tr::gfx::basic_shader_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

tr::ssize tr::gfx::basic_shader_buffer::header_size() const
{
	return m_header_size;
}

tr::ssize tr::gfx::basic_shader_buffer::array_size() const
{
	return m_array_size;
}

tr::ssize tr::gfx::basic_shader_buffer::array_capacity() const
{
	return m_array_capacity;
}

void tr::gfx::basic_shader_buffer::set_header(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the header of a mapped shader buffer.");
	TR_ASSERT(header_size() != 0, "Tried to set the header for a shader buffer without one.");
	TR_ASSERT(std::ssize(data) == header_size(), "Tried to set a shader buffer header of size {} with data of size {}.", header_size(),
			  std::ssize(data));

	TR_GL_CALL(glNamedBufferSubData, m_sbo.get(), 0, data.size(), data.data());
}

void tr::gfx::basic_shader_buffer::set_array(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the array of a mapped shader buffer.");
	TR_ASSERT(array_capacity() != 0, "Tried to set the array for a shader buffer without one.");
	TR_ASSERT(std::ssize(data) <= array_capacity(), "Tried to set a shader buffer array of capacity {} with data of size {}.",
			  array_capacity(), std::ssize(data));

	if (!data.empty()) {
		TR_GL_CALL(glNamedBufferSubData, m_sbo.get(), m_header_size, data.size(), data.data());
	}
	m_array_size = data.size();
}

void tr::gfx::basic_shader_buffer::resize_array(ssize size)
{
	TR_ASSERT(!mapped(), "Tried to resize the array of a mapped shader buffer.");
	TR_ASSERT(size <= array_capacity(), "Tried to resize a shader buffer array of capacity {} to size {}.", array_capacity(), size);

	m_array_size = size;
}

bool tr::gfx::basic_shader_buffer::mapped() const
{
	int mapped;
	TR_GL_CALL(glGetNamedBufferParameteriv, m_sbo.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map_header()
{
	TR_ASSERT(!mapped(), "Tried to map the header of an already-mapped buffer.");
	TR_ASSERT(header_size() != 0, "Tried to map the header of a buffer without one.");

	std::byte* ptr{(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, m_sbo.get(), 0, m_header_size, GLenum(m_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer mapping"};
	}
	return basic_buffer_map{m_sbo.get(), std::span{ptr, usize(m_array_size)}};
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map_array()
{
	TR_ASSERT(!mapped(), "Tried to map the array of an already-mapped buffer.");
	TR_ASSERT(array_size() != 0, "Tried to map the array of a buffer without one.");

	std::byte* ptr{(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, m_sbo.get(), m_header_size, m_array_size, GLenum(m_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer mapping"};
	}
	return basic_buffer_map{m_sbo.get(), std::span{ptr, usize(m_array_size)}};
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map an already-mapped buffer.");

	std::byte* ptr{
		(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, m_sbo.get(), 0, m_header_size + m_array_size, GLenum(m_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer mapping"};
	}
	return basic_buffer_map{m_sbo.get(), std::span{ptr, usize(m_header_size + m_array_size)}};
}

void tr::gfx::basic_shader_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_sbo.get(), GLsizei(label.size()), label.data());
}