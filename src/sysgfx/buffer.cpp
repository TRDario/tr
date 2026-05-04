///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements buffer.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/buffer.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

////////////////////////////////////////////////////////////////// BUFFER /////////////////////////////////////////////////////////////////

tr::gfx::buffer::buffer()
{
	GLuint id;
	TR_GL_CALL(glCreateBuffers, 1, &id);
	m_handle.reset(id);
}

void tr::gfx::buffer::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteBuffers, 1, &id);
}

//

unsigned int tr::gfx::buffer::id() const
{
	return m_handle.get();
}

void tr::gfx::buffer::reallocate()
{
	const buffer old_buffer{std::exchange(*this, buffer{})};
	TR_MOVE_LABEL(GL_BUFFER, old_buffer.id(), id());
}

//

#ifdef TR_ENABLE_ASSERTS
std::string tr::gfx::buffer::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_BUFFER, id(), 0, &length, nullptr);
	if (length > 0) {
		std::string label_string(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_BUFFER, id(), length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

void tr::gfx::buffer::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_BUFFER, id(), GLsizei(label.size()), label.data());
}
#endif