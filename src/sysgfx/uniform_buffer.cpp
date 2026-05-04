///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of uniform_buffer.hpp.                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/uniform_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

////////////////////////////////////////////////////////// BASIC UNIFORM BUFFER ///////////////////////////////////////////////////////////

tr::gfx::basic_uniform_buffer::basic_uniform_buffer(usize size)
	: m_size{size}
{
	TR_GL_CALL(glNamedBufferStorage, id(), GLsizei(size), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"uniform buffer allocation"};
	}
}

tr::usize tr::gfx::basic_uniform_buffer::size() const
{
	return m_size;
}

void tr::gfx::basic_uniform_buffer::set(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the header of mapped uniform buffer '{}'.", label());
	TR_ASSERT(data.size() == size(), "Tried to set uniform buffer '{}' of size {} with data of size {}.", label(), size(), data.size());

	TR_GL_CALL(glNamedBufferSubData, id(), 0, GLintptr(data.size()), data.data());
}

bool tr::gfx::basic_uniform_buffer::mapped() const
{
	GLint mapped;
	TR_GL_CALL(glGetNamedBufferParameteriv, id(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::gfx::basic_buffer_map tr::gfx::basic_uniform_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map already-mapped uniform buffer '{}'.", label());

	const GLenum flags{GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT};
	std::byte* map_pointer{(std::byte*)(TR_RET_GL_CALL(glMapNamedBufferRange, id(), 0, m_size, flags))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
		throw out_of_memory{"mapping of uniform buffer '{}'", label()};
#else
		throw out_of_memory{"uniform buffer mapping"};
#endif
	}
	return basic_buffer_map{id(), std::span{map_pointer, m_size}};
}