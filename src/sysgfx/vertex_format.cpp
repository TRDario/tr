///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of vertex_format.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"

////////////////////////////////////////////////////////////// VERTEX FORMAT //////////////////////////////////////////////////////////////

tr::gfx::vertex_format::vertex_format(std::span<const vertex_binding> bindings)
#ifdef TR_ENABLE_GL_CHECKS
	: m_bindings(bindings)
#endif
{
	GLuint vao;
	TR_GL_CALL(glCreateVertexArrays, 1, &vao);
	m_vao.reset(vao);
	GLuint attr_id{0};
	for (int binding_id = 0; binding_id < int(bindings.size()); ++binding_id) {
		const vertex_binding& binding{bindings.begin()[binding_id]};

		TR_GL_CALL(glVertexArrayBindingDivisor, m_vao.get(), binding_id, binding.divisor);
		GLuint offset{0};
		for (const vertex_attribute& attr : binding.attrs) {
			TR_ASSERT(attr.type != vertex_attribute_type::unknown, "Tried to construct vertex format with invalid attribute '{}'.", attr);

			TR_GL_CALL(glVertexArrayAttribFormat, m_vao.get(), attr_id, attr.elements, GLenum(attr.type), attr.normalized, offset);
			TR_GL_CALL(glEnableVertexArrayAttrib, m_vao.get(), attr_id);
			TR_GL_CALL(glVertexArrayAttribBinding, m_vao.get(), attr_id++, binding_id);

			switch (attr.type) {
			case vertex_attribute_type::i8:
			case vertex_attribute_type::u8:
				offset += attr.elements;
			case vertex_attribute_type::i16:
			case vertex_attribute_type::u16:
				offset += 2 * attr.elements;
			case vertex_attribute_type::i32:
			case vertex_attribute_type::u32:
			case vertex_attribute_type::f32:
				offset += 4 * attr.elements;
			default:
				unreachable();
			}
		}
	}
}

void tr::gfx::vertex_format::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteVertexArrays, 1, &id);
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::vertex_format::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_VERTEX_ARRAY, m_vao.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::vertex_format::label() const
{
	GLsizei length;
	TR_GL_CALL(glGetObjectLabel, GL_VERTEX_ARRAY, m_vao.get(), 0, &length, nullptr);
	if (length > 0) {
		std::string str(length, '\0');
		TR_GL_CALL(glGetObjectLabel, GL_VERTEX_ARRAY, m_vao.get(), length + 1, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif

tr::gfx::vertex_format& tr::gfx::vertex2_format()
{
	return sys::g_window.gfx_context().vertex2_format();
}