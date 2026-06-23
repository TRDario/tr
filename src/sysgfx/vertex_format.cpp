///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of vertex_format.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

////////////////////////////////////////////////////////////// VERTEX FORMAT //////////////////////////////////////////////////////////////

tr::vertex_format::vertex_format(graphics_context& context, std::span<const vertex_binding> bindings)
#ifdef TR_ENABLE_GL_CHECKS
	: m_vao{{context}}
	, m_bindings{bindings}
#endif
{
	const graphics_context::functions& gl{m_vao.get_deleter().context.make_current_and_return_functions()};

	gl.create_vertex_arrays(1, out_handle(m_vao));
	unsigned int attr_id{0};
	for (int binding_id = 0; binding_id < int(bindings.size()); ++binding_id) {
		const vertex_binding& binding{bindings.begin()[binding_id]};

		gl.vertex_array_binding_divisor(m_vao.get(), binding_id, binding.divisor);
		unsigned int offset{0};
		for (const vertex_attribute& attribute : binding.attrs) {
			TR_ASSERT(attribute.type != vertex_attribute_type::unknown, "Tried to construct vertex format with invalid attribute '{}'.",
					  attribute);

			gl.vertex_array_attrib_format(m_vao.get(), attr_id, attribute.elements, (unsigned int)(attribute.type), attribute.normalized,
										  offset);
			gl.enable_vertex_array_attrib(m_vao.get(), attr_id);
			gl.vertex_array_attrib_binding(m_vao.get(), attr_id++, binding_id);

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

void tr::vertex_format::deleter::operator()(unsigned int id) const
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.delete_vertex_arrays(1, &id);
}

//

tr::graphics_context& tr::vertex_format::context() const
{
	return m_vao.get_deleter().context;
}

//

#ifdef TR_ENABLE_ASSERTS
void tr::vertex_format::set_label(std::string_view label)
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.object_label(GL_VERTEX_ARRAY, m_vao.get(), label.size(), label.data());
}

std::string tr::vertex_format::label() const
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	int label_length;
	gl.get_object_label(GL_VERTEX_ARRAY, m_vao.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_VERTEX_ARRAY, m_vao.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}
#endif