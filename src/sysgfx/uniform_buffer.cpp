#include "../../include/tr/sysgfx/uniform_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

tr::gfx::basic_uniform_buffer::basic_uniform_buffer(usize size)
	: m_size{size}
{
	GLuint ubo;
	TR_GL_CALL(glCreateBuffers, 1, &ubo);
	m_ubo.reset(ubo);

	TR_GL_CALL(glNamedBufferStorage, ubo, GLsizei(size), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"uniform buffer allocation"};
	}
}

void tr::gfx::basic_uniform_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

tr::usize tr::gfx::basic_uniform_buffer::size() const
{
	return m_size;
}

void tr::gfx::basic_uniform_buffer::set(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the header of mapped uniform buffer '{}'.", label());
	TR_ASSERT(data.size() == size(), "Tried to set uniform buffer '{}' of size {} with data of size {}.", label(), size(), data.size());

	TR_GL_CALL(glNamedBufferSubData, m_ubo.get(), 0, GLintptr(data.size()), data.data());
}

bool tr::gfx::basic_uniform_buffer::mapped() const
{
	GLint mapped;
	TR_GL_CALL(glGetNamedBufferParameteriv, m_ubo.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::gfx::basic_buffer_map tr::gfx::basic_uniform_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map already-mapped uniform buffer '{}'.", label());

	const GLenum flags{GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT};
	std::byte* ptr{(std::byte*)(TR_RETURNING_GL_CALL(glMapNamedBufferRange, m_ubo.get(), 0, m_size, flags))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
		throw out_of_memory{std::format("mapping of uniform buffer '{}'", label())};
#else
		throw out_of_memory{"uniform buffer mapping"};
#endif
	}
	return basic_buffer_map{m_ubo.get(), std::span{ptr, m_size}};
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::basic_uniform_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, m_ubo.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::basic_uniform_buffer::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_ubo.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, m_ubo.get(), length + 1, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif