#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

//////////////////////////////////////////////////////////// SHADER BUFFER MAP ////////////////////////////////////////////////////////////

tr::shader_buffer_map::shader_buffer_map(unsigned int buffer, std::span<std::byte> span) noexcept
	: _buffer{buffer}, _span{span}
{
}

void tr::shader_buffer_map::deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glUnmapNamedBuffer, id);
}

////////////////////////////////////////////////////////////// SHADER BUFFER //////////////////////////////////////////////////////////////

tr::shader_buffer::shader_buffer(std::intptr_t header_size, std::intptr_t capacity, access access) noexcept
	: _access{access}, _header_size{header_size}, _array_size{0}, _array_capacity{capacity}
{
	GLuint id;
	TR_GL_CALL(glCreateBuffers, 1, &id);
	_id.reset(id);

	TR_GL_CALL(glNamedBufferStorage, id, header_size + capacity, nullptr, static_cast<GLenum>(access) | GL_DYNAMIC_STORAGE_BIT);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		TR_TERMINATE("Out of video memory", "Exception occurred while allocating a shader buffer.");
	}
}

void tr::shader_buffer::deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

std::intptr_t tr::shader_buffer::header_size() const noexcept
{
	return _header_size;
}

std::intptr_t tr::shader_buffer::array_size() const noexcept
{
	return _array_size;
}

std::intptr_t tr::shader_buffer::array_capacity() const noexcept
{
	return _array_capacity;
}

void tr::shader_buffer::set_header(std::span<const std::byte> data) noexcept
{
	TR_ASSERT(!mapped(), "Tried to set the header of a mapped shader buffer.");
	TR_ASSERT(header_size() != 0, "Tried to set the header for a shader buffer without one.");
	TR_ASSERT(ssize(data) == header_size(), "Tried to set a shader buffer header of size {} with data of size {}.", header_size(),
			  ssize(data));

	TR_GL_CALL(glNamedBufferSubData, _id.get(), 0, data.size(), data.data());
}

void tr::shader_buffer::set_array(std::span<const std::byte> data) noexcept
{
	TR_ASSERT(!mapped(), "Tried to set the array of a mapped shader buffer.");
	TR_ASSERT(array_capacity() != 0, "Tried to set the array for a shader buffer without one.");
	TR_ASSERT(ssize(data) <= array_capacity(), "Tried to set a shader buffer array of capacity {} with data of size {}.", array_capacity(),
			  ssize(data));

	if (!data.empty()) {
		TR_GL_CALL(glNamedBufferSubData, _id.get(), _header_size, data.size(), data.data());
	}
	_array_size = data.size();
}

void tr::shader_buffer::resize_array(std::intptr_t size) noexcept
{
	TR_ASSERT(!mapped(), "Tried to resize the array of a mapped shader buffer.");
	TR_ASSERT(size <= array_capacity(), "Tried to resize a shader buffer array of capacity {} to size {}.", array_capacity(), size);

	_array_size = size;
}

bool tr::shader_buffer::mapped() const noexcept
{
	int mapped;
	TR_GL_CALL(glGetNamedBufferParameteriv, _id.get(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::shader_buffer_map tr::shader_buffer::map_header() noexcept
{
	TR_ASSERT(!mapped(), "Tried to map the header of an already-mapped buffer.");
	TR_ASSERT(header_size() != 0, "Tried to map the header of a buffer without one.");

	std::byte* ptr{
		static_cast<std::byte*>(TR_RETURNING_GL_CALL(glMapNamedBufferRange, _id.get(), 0, _header_size, static_cast<GLenum>(_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		TR_TERMINATE("Out of memory", "Exception occurred while mapping a shader buffer.");
	}
	return shader_buffer_map{_id.get(), std::span{ptr, static_cast<std::size_t>(_array_size)}};
}

tr::shader_buffer_map tr::shader_buffer::map_array() noexcept
{
	TR_ASSERT(!mapped(), "Tried to map the array of an already-mapped buffer.");
	TR_ASSERT(array_size() != 0, "Tried to map the array of a buffer without one.");

	std::byte* ptr{static_cast<std::byte*>(
		TR_RETURNING_GL_CALL(glMapNamedBufferRange, _id.get(), _header_size, _array_size, static_cast<GLenum>(_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		TR_TERMINATE("Out of memory", "Exception occurred while mapping a shader buffer.");
	}
	return shader_buffer_map{_id.get(), std::span{ptr, static_cast<std::size_t>(_array_size)}};
}

tr::shader_buffer_map tr::shader_buffer::map() noexcept
{
	TR_ASSERT(!mapped(), "Tried to map an already-mapped buffer.");

	std::byte* ptr{static_cast<std::byte*>(
		TR_RETURNING_GL_CALL(glMapNamedBufferRange, _id.get(), 0, _header_size + _array_size, static_cast<GLenum>(_access)))};
	if (glGetError() == GL_OUT_OF_MEMORY) {
		TR_TERMINATE("Out of memory", "Exception occurred while mapping a shader buffer.");
	}
	return shader_buffer_map{_id.get(), std::span{ptr, static_cast<std::size_t>(_header_size + _array_size)}};
}

void tr::shader_buffer::set_label(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, _id.get(), static_cast<GLsizei>(label.size()), label.data());
}