#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"

tr::gfx::basic_shader_buffer::basic_shader_buffer(usize header_size, usize capacity, access access)
	: m_access{access}, m_header_size{header_size}, m_array_size{0}, m_array_capacity{capacity}
{
	GLuint sbo;
	TR_GL_CALL(glCreateBuffers, 1, &sbo);
	m_sbo.reset(sbo);

	TR_GL_CALL(glNamedBufferStorage, sbo, GLsizeiptr(header_size + capacity), nullptr, GLenum(access) | GL_DYNAMIC_STORAGE_BIT);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer allocation"};
	}
}

void tr::gfx::basic_shader_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

tr::usize tr::gfx::basic_shader_buffer::header_size() const
{
	return m_header_size;
}

tr::usize tr::gfx::basic_shader_buffer::array_size() const
{
	return m_array_size;
}

tr::usize tr::gfx::basic_shader_buffer::array_capacity() const
{
	return m_array_capacity;
}

void tr::gfx::basic_shader_buffer::set_header(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the header of mapped shader buffer '{}'.", label());
	TR_ASSERT(header_size() != 0, "Tried to set the header for shader buffer '{}' without one.", label());
	TR_ASSERT(data.size() == header_size(), "Tried to set header of shader buffer '{}' of size {} with data of size {}.", label(),
			  header_size(), data.size());

	TR_GL_CALL(glNamedBufferSubData, m_sbo.get(), 0, data.size(), data.data());
}

void tr::gfx::basic_shader_buffer::set_array(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the array of a mapped shader buffer.");
	TR_ASSERT(array_capacity() != 0, "Tried to set the array for a shader buffer without one.");
	TR_ASSERT(data.size() <= array_capacity(), "Tried to set a shader buffer array of capacity {} with data of size {}.", array_capacity(),
			  data.size());

	if (!data.empty()) {
		TR_GL_CALL(glNamedBufferSubData, m_sbo.get(), GLintptr(m_header_size), GLsizeiptr(data.size()), data.data());
	}
	m_array_size = data.size();
}

void tr::gfx::basic_shader_buffer::resize_array(usize size)
{
	TR_ASSERT(!mapped(), "Tried to resize the array of mapped shader buffer '{}'.", label());
	TR_ASSERT(size <= array_capacity(), "Tried to resize array of shader buffer '{}' of capacity {} to size {}.", label(), array_capacity(),
			  size);

	m_array_size = size;
}

bool tr::gfx::basic_shader_buffer::mapped() const
{
	GLint mapped;
	TR_GL_CALL(glGetNamedBufferParameteriv, m_sbo.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map_header()
{
	TR_ASSERT(!mapped(), "Tried to map the header of already-mapped shader buffer '{}'.", label());
	TR_ASSERT(header_size() != 0, "Tried to map the header of shader buffer '{}' that doesn't have one.", label());

	std::byte* ptr{(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, m_sbo.get(), 0, GLsizeiptr(m_header_size), GLenum(m_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
		throw out_of_memory{std::format("mapping of shader buffer '{}'", label())};
#else
		throw out_of_memory{"shader buffer mapping"};
#endif
	}
	return basic_buffer_map{m_sbo.get(), std::span{ptr, m_array_size}};
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map_array()
{
	TR_ASSERT(!mapped(), "Tried to map the array of already-mapped shader buffer '{}'.", label());
	TR_ASSERT(array_size() != 0, "Tried to map the array of shader buffer '{}' that doesn't have one.", label());

	std::byte* ptr{(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, m_sbo.get(), GLintptr(m_header_size), GLintptr(m_array_size),
													 GLenum(m_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
		throw out_of_memory{std::format("mapping of shader buffer '{}'", label())};
#else
		throw out_of_memory{"shader buffer mapping"};
#endif
	}
	return basic_buffer_map{m_sbo.get(), std::span{ptr, m_array_size}};
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map already-mapped buffer '{}'.", label());

	const GLsizeiptr size{GLsizeiptr(m_header_size + m_array_size)};
	std::byte* ptr{(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, m_sbo.get(), 0, size, GLenum(m_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer mapping"};
	}
	return basic_buffer_map{m_sbo.get(), std::span{ptr, usize(size)}};
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::basic_shader_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_sbo.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::basic_shader_buffer::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_sbo.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_sbo.get(), length, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif