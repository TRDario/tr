#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////// BASIC STATIC VERTEX BUFFER ////////////////////////////////////////////////////////

tr::basic_static_vertex_buffer::basic_static_vertex_buffer(std::span<const std::byte> data)
	: size{ssize(data)}
{
	unsigned int id;
	TR_GL_CALL(glCreateBuffers, 1, &id);
	this->vbo.reset(id);

	TR_GL_CALL(glNamedBufferStorage, this->vbo.get(), size, data.data(), 0);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"vertex buffer allocation"};
	}
}

void tr::basic_static_vertex_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

void tr::basic_static_vertex_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, vbo.get(), static_cast<GLsizei>(label.size()), label.data());
}

/////////////////////////////////////////////////////// BASIC DYNAMIC VERTEX BUFFER ///////////////////////////////////////////////////////

tr::basic_dyn_vertex_buffer::basic_dyn_vertex_buffer()
	: size_{0}, capacity_{0}
{
	unsigned int temp;
	TR_GL_CALL(glCreateBuffers, 1, &temp);
	vbo.reset(temp);
}

void tr::basic_dyn_vertex_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

bool tr::basic_dyn_vertex_buffer::empty() const
{
	return size_ == 0;
}

std::size_t tr::basic_dyn_vertex_buffer::size() const
{
	return size_;
}

std::size_t tr::basic_dyn_vertex_buffer::capacity() const
{
	return capacity_;
}

void tr::basic_dyn_vertex_buffer::clear()
{
	size_ = 0;
}

void tr::basic_dyn_vertex_buffer::resize(std::size_t size)
{
	reserve(size);
	size_ = size;
}

void tr::basic_dyn_vertex_buffer::reserve(std::size_t capacity)
{
	if (capacity > capacity_) {
		capacity = std::bit_ceil(capacity);

		unsigned int temp;
		TR_GL_CALL(glCreateBuffers, 1, &temp);
		TR_GL_CALL(glDeleteBuffers, 1, &vbo.get());
		vbo.reset(temp);
		if (!label.empty()) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, vbo.get(), static_cast<GLsizei>(label.size()), label.data());
		}

		TR_GL_CALL(glNamedBufferStorage, vbo.get(), static_cast<GLsizei>(capacity), nullptr, GL_DYNAMIC_STORAGE_BIT);
		if (glGetError() == GL_OUT_OF_MEMORY) {
			throw out_of_memory{"vertex buffer allocation"};
		}
		capacity_ = capacity;
	}
	else {
		TR_GL_CALL(glInvalidateBufferData, vbo.get());
	}
	size_ = 0;
}

void tr::basic_dyn_vertex_buffer::set(std::span<const std::byte> data)
{
	resize(data.size());
	set_region(0, data);
}

void tr::basic_dyn_vertex_buffer::set_region(std::size_t offset, std::span<const std::byte> data)
{
	TR_ASSERT(offset + ssize(data) <= size_, "Tried to set out-of-bounds region [{}, {}) in a vertex buffer of size {}.", offset,
			  offset + ssize(data), size_);

	TR_GL_CALL(glNamedBufferSubData, vbo.get(), offset, ssize(data), data.data());
}

void tr::basic_dyn_vertex_buffer::set_label(const std::string& label)
{
	set_label(std::string{label});
}

void tr::basic_dyn_vertex_buffer::set_label(std::string&& new_label)
{
	label = std::move(new_label);
	TR_GL_CALL(glObjectLabel, GL_BUFFER, vbo.get(), static_cast<GLsizei>(label.size()), label.data());
}