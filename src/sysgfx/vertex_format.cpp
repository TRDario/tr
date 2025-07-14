#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"

tr::vertex_format::vertex_format(std::initializer_list<vertex_attribute> attrs) noexcept
	: vertex_format{std::span{attrs}}
{
}

tr::vertex_format::vertex_format(std::span<const vertex_attribute> attrs) noexcept
{
	GLuint vao;
	TR_GL_CALL(glCreateVertexArrays, 1, &vao);
	for (int i = 0; i < static_cast<int>(attrs.size()); ++i) {
		visit(overloaded{[=](const vertex_attributef& attr) {
							 TR_GL_CALL(glVertexArrayAttribFormat, vao, i, attr.elements, static_cast<GLenum>(attr.type), attr.normalized,
										attr.offset);
							 TR_GL_CALL(glVertexArrayAttribBinding, vao, i, attr.binding);
						 },
						 [=](const vertex_attributei& attr) {
							 TR_GL_CALL(glVertexArrayAttribIFormat, vao, i, attr.elements, static_cast<GLenum>(attr.type), attr.offset);
							 TR_GL_CALL(glVertexArrayAttribBinding, vao, i, attr.binding);
						 }},
			  attrs[i]);
		TR_GL_CALL(glEnableVertexArrayAttrib, vao, i);
	}
	_id.reset(vao);
}

void tr::vertex_format::deleter::operator()(unsigned int id) const noexcept
{
	TR_GL_CALL(glDeleteVertexArrays, 1, &id);
}

void tr::vertex_format::set_label(std::string_view label) noexcept
{
	TR_GL_CALL(glObjectLabel, GL_VERTEX_ARRAY, _id.get(), static_cast<GLsizei>(label.size()), label.data());
}

tr::vertex_format& tr::vertex2_format() noexcept
{
	if (!_vertex2_format.has_value()) {
		_vertex2_format.emplace(std::initializer_list<vertex_attribute>{
			vertex_attributef{vertex_attributef::type::FP32, 2, false, 0, 0},
			vertex_attributef{vertex_attributef::type::FP32, 2, false, 1, 0},
			vertex_attributef{vertex_attributef::type::UI8, 4, true, 2, 0},
		});
		if (gfx_context::debug()) {
			_vertex2_format->set_label("(tr) 2D Vertex Format");
		}
	}
	return *_vertex2_format;
}