/// @file
/// @brief Implements graphics_buffer.hpp.

#include "../../include/tr/sysgfx/graphics_buffer.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

//

tr::graphics_buffer::graphics_buffer(graphics_context& context)
	: m_handle{{context}}
{
	context.gl().create_buffers(1, out_handle(m_handle));
}

void tr::graphics_buffer::deleter::operator()(unsigned int id) const
{
	context.gl().delete_buffers(1, &id);
}

//

tr::graphics_context& tr::graphics_buffer::context() const
{
	return m_handle.get_deleter().context;
}

//

unsigned int tr::graphics_buffer::id() const
{
	return m_handle.get();
}

void tr::graphics_buffer::reallocate()
{
	const graphics_buffer old_buffer{std::exchange(*this, graphics_buffer{context()})};
	context().move_label(GL_BUFFER, old_buffer.id(), id());
}

//

std::string tr::graphics_buffer::label() const
{
	const gl_api& gl{context().gl()};

	int label_length;
	gl.get_object_label(GL_BUFFER, id(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_BUFFER, id(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

void tr::graphics_buffer::set_label(std::string_view label)
{
	context().gl().set_object_label(GL_BUFFER, id(), label.size(), label.data());
}