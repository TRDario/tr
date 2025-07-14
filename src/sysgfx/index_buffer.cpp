#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

/********************************************** static_index_buffer **********************************************/

tr::static_index_buffer::static_index_buffer(std::span<const std::uint16_t> data) noexcept
	: _size{ssize(data)}
{
	unsigned int id;
	TR_GL_CALL(glCreateBuffers, 1, &id);
	_id.reset(id);

	TR_GL_CALL(glNamedBufferStorage, _id.get(), _size * sizeof(std::uint16_t), data.data(), 0);
	if (glGetError() == GL_OUT_OF_MEMORY) {
		TR_TERMINATE("Out of video memory", "Exception occurred while allocating an index buffer.");
	}
}

void tr::static_index_buffer::deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

void tr::static_index_buffer::set_label(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, _id.get(), static_cast<GLsizei>(label.size()), label.data());
}

/************************************************* dyn_index_buffer **************************************************/

tr::dyn_index_buffer::dyn_index_buffer() noexcept
	: _size{0}, _capacity{0}
{
	unsigned int id;
	TR_GL_CALL(glCreateBuffers, 1, &id);
	_id.reset(id);
}

void tr::dyn_index_buffer::deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

bool tr::dyn_index_buffer::empty() const noexcept
{
	return _size == 0;
}

std::size_t tr::dyn_index_buffer::size() const noexcept
{
	return _size;
}

std::size_t tr::dyn_index_buffer::capacity() const noexcept
{
	return _capacity;
}

void tr::dyn_index_buffer::clear() noexcept
{
	_size = 0;
}

void tr::dyn_index_buffer::resize(std::size_t size) noexcept
{
	reserve(size);
	_size = size;
}

void tr::dyn_index_buffer::reserve(std::size_t capacity) noexcept
{
	if (capacity > _capacity) {
		capacity = std::bit_ceil(capacity);

		unsigned int id;
		TR_GL_CALL(glCreateBuffers, 1, &id);
		TR_GL_CALL(glDeleteBuffers, 1, &_id.get());
		_id.reset(id);
		if (!_label.empty()) {
			TR_GL_CALL(glObjectLabel, GL_BUFFER, _id.get(), static_cast<GLsizei>(_label.size()), _label.data());
		}

		TR_GL_CALL(glNamedBufferStorage, _id.get(), static_cast<GLsizei>(capacity * sizeof(std::uint16_t)), nullptr,
				   GL_DYNAMIC_STORAGE_BIT);
		if (glGetError() == GL_OUT_OF_MEMORY) {
			TR_TERMINATE("Out of video memory", "Exception occurred while allocating an index buffer.");
		}
		_capacity = capacity;
	}
	else {
		TR_GL_CALL(glInvalidateBufferData, _id.get());
	}
	_size = 0;
}

void tr::dyn_index_buffer::set_region(std::size_t offset, std::span<const std::uint16_t> data) noexcept
{
	TR_ASSERT(offset + ssize(data) <= _size, "Tried to set out-of-bounds region [{}, {}) in an index buffer of size {}.", offset,
			  offset + ssize(data), _size);

	TR_GL_CALL(glNamedBufferSubData, _id.get(), offset * sizeof(std::uint16_t), ssize(data) * sizeof(std::uint16_t), data.data());
}

void tr::dyn_index_buffer::set(std::span<const std::uint16_t> data) noexcept
{
	resize(data.size());
	set_region(0, data);
}

void tr::dyn_index_buffer::set_label(const std::string& label) noexcept
{
	try {
		set_label(std::string{label});
	}
	catch (std::bad_alloc&) {
		TR_TERMINATE("Out of memory", "Exception occurred while setting an index buffer label.");
	}
}

void tr::dyn_index_buffer::set_label(std::string&& label) noexcept
{
	_label = std::move(label);
	TR_GL_CALL(glObjectLabel, GL_BUFFER, _id.get(), static_cast<GLsizei>(_label.size()), _label.data());
}