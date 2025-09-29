#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"

#ifdef TR_ENABLE_ASSERTS
namespace tr::gfx {
	constexpr std::array<vertex_binding, 3> VERTEX2_ATTRIBUTES{{
		{NOT_INSTANCED, vertex_attributes<glm::vec2>::list},
		{NOT_INSTANCED, vertex_attributes<glm::vec2>::list},
		{NOT_INSTANCED, vertex_attributes<rgba8>::list},
	}};
}
#endif

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
		for (const vertex_attribute& a : binding.attrs) {
			TR_GL_CALL(glVertexArrayAttribFormat, m_vao.get(), attr_id, a.elements, GLenum(a.type), a.normalized, offset);
			TR_GL_CALL(glEnableVertexArrayAttrib, m_vao.get(), attr_id);
			TR_GL_CALL(glVertexArrayAttribBinding, m_vao.get(), attr_id++, binding_id);
			offset += a.size_bytes();
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
		TR_GL_CALL(glGetObjectLabel, GL_VERTEX_ARRAY, m_vao.get(), length, nullptr, str.data());
		return str;
	}
	else {
		return "<unnamed>";
	}
}
#endif

tr::gfx::vertex_format& tr::gfx::vertex2_format()
{
	if (!vertex2_format_.has_value()) {
		vertex2_format_.emplace(VERTEX2_ATTRIBUTES);
		TR_SET_LABEL(*vertex2_format_, "(tr) 2D Vertex Format");
	}
	return *vertex2_format_;
}