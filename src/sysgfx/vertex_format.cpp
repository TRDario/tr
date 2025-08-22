#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/utility/overloaded_lambda.hpp"

tr::gfx::vertex_format::vertex_format(std::initializer_list<vertex_binding> bindings)
{
	GLuint temp;
	TR_GL_CALL(glCreateVertexArrays, 1, &temp);
	m_vao.reset(temp);
	int attr_id{0};
	for (int binding_id = 0; binding_id < static_cast<int>(bindings.size()); ++binding_id) {
		const vertex_binding& binding{bindings.begin()[binding_id]};

		TR_GL_CALL(glVertexArrayBindingDivisor, m_vao.get(), binding_id, binding.divisor);
		for (const vertex_attribute& a : binding.attrs) {
			TR_GL_CALL(glVertexArrayAttribFormat, m_vao.get(), attr_id, a.elements, static_cast<GLenum>(a.type), a.normalized, a.offset);
			TR_GL_CALL(glEnableVertexArrayAttrib, m_vao.get(), attr_id);
			TR_GL_CALL(glVertexArrayAttribBinding, m_vao.get(), attr_id++, binding_id);
		}
	}
}

void tr::gfx::vertex_format::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteVertexArrays, 1, &id);
}

void tr::gfx::vertex_format::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_VERTEX_ARRAY, m_vao.get(), static_cast<GLsizei>(label.size()), label.data());
}

tr::gfx::vertex_format& tr::gfx::vertex2_format()
{
	if (!vertex2_format_.has_value()) {
		vertex2_format_ = {
			{NOT_INSTANCED, {{vertex_attribute::type::FP32, 2, false, 0}}},
			{NOT_INSTANCED, {{vertex_attribute::type::FP32, 2, false, 0}}},
			{NOT_INSTANCED, {{vertex_attribute::type::UI8, 4, true, 0}}},
		};
		if (debug()) {
			vertex2_format_->set_label("(tr) 2D Vertex Format");
		}
	}
	return *vertex2_format_;
}