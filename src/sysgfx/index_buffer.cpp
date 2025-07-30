#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/////////////////////////////////////////////////////////// STATIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::static_index_buffer::static_index_buffer(std::span<const std::uint16_t> data)
	: size{ssize(data)}
{
	unsigned int temp;
	TR_GL_CALL(glCreateBuffers, 1, &temp);
	ibo.reset(temp);

	TR_GL_CALL(glNamedBufferStorage, ibo.get(), size * sizeof(std::uint16_t), data.data(), 0);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"index buffer allocation"};
	}
}

void tr::static_index_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

void tr::static_index_buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, ibo.get(), static_cast<GLsizei>(label.size()), label.data());
}

////////////////////////////////////////////////////////// DYNAMIC INDEX BUFFER ///////////////////////////////////////////////////////////

tr::dyn_index_buffer::dyn_index_buffer()
	: size_{0}, capacity_{0}
{
	unsigned int temp;
	TR_GL_CALL(glCreateBuffers, 1, &temp);
	ibo.reset(temp);
}

void tr::dyn_index_buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

bool tr::dyn_index_buffer::empty() const
{
	return size_ == 0;
}

std::size_t tr::dyn_index_buffer::size() const
{
	return size_;
}

std::size_t tr::dyn_index_buffer::capacity() const
{
	return capacity_;
}

void tr::dyn_index_buffer::clear()
{
	size_ = 0;
}

void tr::dyn_index_buffer::resize(std::size_t size)
{
	reserve(size);
	size_ = size;
}

void tr::dyn_index_buffer::reserve(std::size_t capacity)
{
	if (capacity > capacity_) {
		capacity = std::bit_ceil(capacity);

		unsigned int temp;
		TR_GL_CALL(glCreateBuffers, 1, &temp);
		TR_GL_CALL(glDeleteBuffers, 1, &ibo.get());
		ibo.reset(temp);
		if (!label.empty()) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, ibo.get(), static_cast<GLsizei>(label.size()), label.data());
		}

		TR_GL_CALL(glNamedBufferStorage, ibo.get(), static_cast<GLsizei>(capacity * sizeof(std::uint16_t)), nullptr,
				   GL_DYNAMIC_STORAGE_BIT);
		if (glGetError() == GL_OUT_OF_MEMORY) {
			throw out_of_memory{"index buffer allocation"};
		}
		capacity_ = capacity;
	}
	else {
		TR_GL_CALL(glInvalidateBufferData, ibo.get());
	}
	size_ = 0;
}

void tr::dyn_index_buffer::set_region(std::size_t offset, std::span<const std::uint16_t> data)
{
	TR_ASSERT(offset + ssize(data) <= size_, "Tried to set out-of-bounds region [{}, {}) in an index buffer of size {}.", offset,
			  offset + ssize(data), size_);

	TR_GL_CALL(glNamedBufferSubData, ibo.get(), offset * sizeof(std::uint16_t), ssize(data) * sizeof(std::uint16_t), data.data());
}

void tr::dyn_index_buffer::set(std::span<const std::uint16_t> data)
{
	resize(data.size());
	set_region(0, data);
}

void tr::dyn_index_buffer::set_label(const std::string& label)
{
	set_label(std::string{label});
}

void tr::dyn_index_buffer::set_label(std::string&& new_label)
{
	label = std::move(new_label);
	TR_GL_CALL(glObjectLabel, GL_BUFFER, ibo.get(), static_cast<GLsizei>(label.size()), label.data());
}