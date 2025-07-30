#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/utility/overloaded_lambda.hpp"

tr::vertex_format::vertex_format(std::initializer_list<vertex_attribute> attrs)
	: vertex_format{std::span{attrs}}
{
}

tr::vertex_format::vertex_format(std::span<const vertex_attribute> attrs)
{
	GLuint temp;
	TR_GL_CALL(glCreateVertexArrays, 1, &temp);
	vao.reset(temp);
	for (int i = 0; i < static_cast<int>(attrs.size()); ++i) {
		visit(overloaded{[=, v = vao.get()](const vertex_attributef& a) {
							 TR_GL_CALL(glVertexArrayAttribFormat, v, i, a.elements, static_cast<GLenum>(a.type), a.normalized, a.offset);
							 TR_GL_CALL(glVertexArrayAttribBinding, v, i, a.binding);
						 },
						 [=, v = vao.get()](const vertex_attributei& a) {
							 TR_GL_CALL(glVertexArrayAttribIFormat, v, i, a.elements, static_cast<GLenum>(a.type), a.offset);
							 TR_GL_CALL(glVertexArrayAttribBinding, v, i, a.binding);
						 }},
			  attrs[i]);
		TR_GL_CALL(glEnableVertexArrayAttrib, vao.get(), i);
	}
}

void tr::vertex_format::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glDeleteVertexArrays, 1, &id);
}

void tr::vertex_format::set_label(std::string_view label)
{
	TR_GL_CALL(glObjectLabel, GL_VERTEX_ARRAY, vao.get(), static_cast<GLsizei>(label.size()), label.data());
}

tr::vertex_format& tr::vertex2_format()
{
	if (!vertex2_format_.has_value()) {
		vertex2_format_.emplace(std::initializer_list<vertex_attribute>{
			vertex_attributef{vertex_attributef::type::FP32, 2, false, 0, 0},
			vertex_attributef{vertex_attributef::type::FP32, 2, false, 1, 0},
			vertex_attributef{vertex_attributef::type::UI8, 4, true, 2, 0},
		});
		if (gfx_context::debug()) {
			vertex2_format_->set_label("(tr) 2D Vertex Format");
		}
	}
	return *vertex2_format_;
}