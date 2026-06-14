///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements buffer.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/graphics_buffer.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

////////////////////////////////////////////////////////////////// BUFFER /////////////////////////////////////////////////////////////////

tr::graphics_buffer::graphics_buffer(graphics_context& context)
	: m_handle{{context}}
{
	const graphics_context::functions& gl{m_handle.get_deleter().context.make_current_and_return_functions()};

	gl.create_buffers(1, out_handle(m_handle));
}

void tr::graphics_buffer::deleter::operator()(unsigned int id) const
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.delete_buffers(1, &id);
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
#ifdef TR_ENABLE_ASSERTS
	context().move_label(GL_BUFFER, old_buffer.id(), id());
#endif
}

//

#ifdef TR_ENABLE_ASSERTS
std::string tr::graphics_buffer::label() const
{
	const graphics_context::functions& gl{m_handle.get_deleter().context.make_current_and_return_functions()};

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
	const graphics_context::functions& gl{m_handle.get_deleter().context.make_current_and_return_functions()};

	gl.object_label(GL_BUFFER, id(), label.size(), label.data());
}
#endif