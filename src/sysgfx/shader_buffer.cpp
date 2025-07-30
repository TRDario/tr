#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

//////////////////////////////////////////////////////////// SHADER BUFFER MAP ////////////////////////////////////////////////////////////

tr::shader_buffer_map::shader_buffer_map(unsigned int buffer, std::span<std::byte> span)
	: buffer{buffer}, span{span}
{
}

void tr::shader_buffer_map::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glUnmapNamedBuffer, id);
}

////////////////////////////////////////////////////////////// SHADER BUFFER //////////////////////////////////////////////////////////////

tr::shader_buffer::shader_buffer(std::intptr_t header_size, std::intptr_t capacity, access access)
	: access_{access}, hsize{header_size}, asize{0}, acapacity{capacity}
{
	GLuint id;
	TR_GL_CALL(glCreateBuffers, 1, &id);
	sbo.reset(id);

	TR_GL_CALL(glNamedBufferStorage, id, header_size + capacity, nullptr, static_cast<GLenum>(access) | GL_DYNAMIC_STORAGE_BIT);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer allocation"};
	}
}

void tr::shader_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

std::intptr_t tr::shader_buffer::header_size() const
{
	return hsize;
}

std::intptr_t tr::shader_buffer::array_size() const
{
	return asize;
}

std::intptr_t tr::shader_buffer::array_capacity() const
{
	return acapacity;
}

void tr::shader_buffer::set_header(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the header of a mapped shader buffer.");
	TR_ASSERT(header_size() != 0, "Tried to set the header for a shader buffer without one.");
	TR_ASSERT(ssize(data) == header_size(), "Tried to set a shader buffer header of size {} with data of size {}.", header_size(),
			  ssize(data));

	TR_GL_CALL(glNamedBufferSubData, sbo.get(), 0, data.size(), data.data());
}

void tr::shader_buffer::set_array(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the array of a mapped shader buffer.");
	TR_ASSERT(array_capacity() != 0, "Tried to set the array for a shader buffer without one.");
	TR_ASSERT(ssize(data) <= array_capacity(), "Tried to set a shader buffer array of capacity {} with data of size {}.", array_capacity(),
			  ssize(data));

	if (!data.empty()) {
		TR_GL_CALL(glNamedBufferSubData, sbo.get(), hsize, data.size(), data.data());
	}
	asize = data.size();
}

void tr::shader_buffer::resize_array(std::intptr_t size)
{
	TR_ASSERT(!mapped(), "Tried to resize the array of a mapped shader buffer.");
	TR_ASSERT(size <= array_capacity(), "Tried to resize a shader buffer array of capacity {} to size {}.", array_capacity(), size);

	asize = size;
}

bool tr::shader_buffer::mapped() const
{
	int mapped;
	TR_GL_CALL(glGetNamedBufferParameteriv, sbo.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::shader_buffer_map tr::shader_buffer::map_header()
{
	TR_ASSERT(!mapped(), "Tried to map the header of an already-mapped buffer.");
	TR_ASSERT(header_size() != 0, "Tried to map the header of a buffer without one.");

	std::byte* ptr{static_cast<std::byte*>(TR_RETURNING_GL_CALL(glMapNamedBufferRange, sbo.get(), 0, hsize, static_cast<GLenum>(access_)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer mapping"};
	}
	return shader_buffer_map{sbo.get(), std::span{ptr, static_cast<std::size_t>(asize)}};
}

tr::shader_buffer_map tr::shader_buffer::map_array()
{
	TR_ASSERT(!mapped(), "Tried to map the array of an already-mapped buffer.");
	TR_ASSERT(array_size() != 0, "Tried to map the array of a buffer without one.");

	std::byte* ptr{
		static_cast<std::byte*>(TR_RETURNING_GL_CALL(glMapNamedBufferRange, sbo.get(), hsize, asize, static_cast<GLenum>(access_)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer mapping"};
	}
	return shader_buffer_map{sbo.get(), std::span{ptr, static_cast<std::size_t>(asize)}};
}

tr::shader_buffer_map tr::shader_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map an already-mapped buffer.");

	std::byte* ptr{
		static_cast<std::byte*>(TR_RETURNING_GL_CALL(glMapNamedBufferRange, sbo.get(), 0, hsize + asize, static_cast<GLenum>(access_)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer mapping"};
	}
	return shader_buffer_map{sbo.get(), std::span{ptr, static_cast<std::size_t>(hsize + asize)}};
}

void tr::shader_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, sbo.get(), static_cast<GLsizei>(label.size()), label.data());
}