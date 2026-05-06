///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of vertex_format.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/sysgfx/impl.hpp"

////////////////////////////////////////////////////////////// VERTEX FORMAT //////////////////////////////////////////////////////////////

tr::gfx::vertex_format::vertex_format(std::span<const vertex_binding> bindings)
#ifdef TR_ENABLE_GL_CHECKS
	: m_bindings{bindings}
#endif
{
	glCreateVertexArrays(1, out_handle(m_vao));
	GLuint attr_id{0};
	for (int binding_id = 0; binding_id < int(bindings.size()); ++binding_id) {
		const vertex_binding& binding{bindings.begin()[binding_id]};

		glVertexArrayBindingDivisor(m_vao.get(), binding_id, binding.divisor);
		GLuint offset{0};
		for (const vertex_attribute& attribute : binding.attrs) {
			TR_ASSERT(attribute.type != vertex_attribute_type::unknown, "Tried to construct vertex format with invalid attribute '{}'.",
					  attribute);

			glVertexArrayAttribFormat(m_vao.get(), attr_id, attribute.elements, GLenum(attribute.type), attribute.normalized, offset);
			glEnableVertexArrayAttrib(m_vao.get(), attr_id);
			glVertexArrayAttribBinding(m_vao.get(), attr_id++, binding_id);

			switch (attribute.type) {
			case vertex_attribute_type::i8:
			case vertex_attribute_type::u8:
				offset += attribute.elements;
				break;
			case vertex_attribute_type::i16:
			case vertex_attribute_type::u16:
				offset += 2 * attribute.elements;
				break;
			case vertex_attribute_type::i32:
			case vertex_attribute_type::u32:
			case vertex_attribute_type::f32:
				offset += 4 * attribute.elements;
				break;
			default:
				TR_UNREACHABLE;
			}
		}
	}
}

void tr::gfx::vertex_format::deleter::operator()(unsigned int id) const
{
	glDeleteVertexArrays(1, &id);
}

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::vertex_format::set_label(std::string_view label)
{
	glObjectLabel(GL_VERTEX_ARRAY, m_vao.get(), GLsizei(label.size()), label.data());
}

std::string tr::gfx::vertex_format::label() const
{
	GLsizei label_length;
	glGetObjectLabel(GL_VERTEX_ARRAY, m_vao.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		glGetObjectLabel(GL_VERTEX_ARRAY, m_vao.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
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