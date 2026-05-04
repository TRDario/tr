///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of shader_buffer.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////////// BASIC SHADER BUFFER ///////////////////////////////////////////////////////////

tr::gfx::basic_shader_buffer::basic_shader_buffer(usize header_size, usize capacity, map_type map_type)
	: m_map_type{map_type}, m_header_size{header_size}, m_array_size{0}, m_array_capacity{capacity}
{
	TR_GL_CALL(glNamedBufferStorage, id(), GLsizeiptr(header_size + capacity), nullptr, GLenum(map_type) | GL_DYNAMIC_STORAGE_BIT);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer allocation"};
	}
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

	TR_GL_CALL(glNamedBufferSubData, id(), 0, data.size(), data.data());
}

void tr::gfx::basic_shader_buffer::set_array(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the array of a mapped shader buffer.");
	TR_ASSERT(array_capacity() != 0, "Tried to set the array for a shader buffer without one.");
	TR_ASSERT(data.size() <= array_capacity(), "Tried to set a shader buffer array of capacity {} with data of size {}.", array_capacity(),
			  data.size());

	if (!data.empty()) {
		TR_GL_CALL(glNamedBufferSubData, id(), GLintptr(m_header_size), GLsizeiptr(data.size()), data.data());
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
	TR_GL_CALL(glGetNamedBufferParameteriv, id(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map_range(usize offset, usize size)
{
	TR_ASSERT(!mapped(), "Tried to map the header of already-mapped shader buffer '{}'.", label());

	std::byte* map_pointer{(std::byte*)(TR_RET_GL_CALL(glMapNamedBufferRange, id(), GLintptr(offset), GLintptr(size), GLenum(m_map_type)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
		throw out_of_memory{"mapping of shader buffer '{}'", label()};
#else
		throw out_of_memory{"shader buffer mapping"};
#endif
	}
	return basic_buffer_map{id(), std::span{map_pointer, size}};
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map_header()
{
	TR_ASSERT(header_size() != 0, "Tried to map the header of shader buffer '{}' that doesn't have one.", label());

	return map_range(0, m_header_size);
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map_array()
{
	TR_ASSERT(array_size() != 0, "Tried to map the array of shader buffer '{}' that doesn't have one.", label());

	return map_range(m_array_size, m_array_size);
}

tr::gfx::basic_buffer_map tr::gfx::basic_shader_buffer::map()
{
	return map_range(0, m_header_size + m_array_size);
}