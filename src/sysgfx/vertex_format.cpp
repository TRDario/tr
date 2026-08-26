/// @file
/// @brief Implements the non-templated parts of vertex_format.hpp.

#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

//

tr::vertex_format::vertex_format(graphics_context& context, std::span<const vertex_binding> bindings)
	: m_handle{deleter{context}}
#ifdef TR_ENABLE_GL_CHECKS
	, m_bindings{bindings}
#endif
{
	const gl_api& gl{context.gl()};
	gl.create_vertex_arrays(1, out_handle(m_handle));
#ifdef TR_ENABLE_GL_CHECKS
	context.registry().register_vertex_format(id(), unwrap());
#endif

	unsigned int attr_id{0};
	for (int binding_id = 0; binding_id < static_cast<int>(bindings.size()); ++binding_id) {
		const vertex_binding& binding{bindings.begin()[binding_id]};

		gl.set_vertex_array_binding_divisor(unwrap(), binding_id, binding.divisor);
		unsigned int offset{0};
		for (const vertex_attribute& attribute : binding.attrs) {
			TR_ASSERT(attribute.type != vertex_attribute_type::unknown, "Tried to construct vertex format with invalid attribute '{}'.",
					  attribute);

			gl.set_vertex_array_attribute_format(unwrap(), attr_id, attribute.elements, std::to_underlying(attribute.type),
												 attribute.normalized, offset);
			gl.enable_vertex_array_attribute(unwrap(), attr_id);
			gl.set_vertex_array_attribute_binding(unwrap(), attr_id++, binding_id);

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

void tr::vertex_format::deleter::operator()(unsigned int vao) const
{
	context->gl().delete_vertex_arrays(1, &vao);
#ifdef TR_ENABLE_GL_CHECKS
	context->registry().unregister_vertex_format(id);
#endif
}

//

tr::graphics_context& tr::vertex_format::context() const
{
	TR_ASSERT(valid(), "Tried to get context of a vertex format in an invalid state.");

	return m_handle.get_deleter().context;
}

//

bool tr::vertex_format::valid() const
{
	return m_handle.has_value();
}

//

void tr::vertex_format::set_label(std::string_view label)
{
	TR_ASSERT(valid(), "Tried to set the label of a vertex format in an invalid state.");

	context().gl().set_object_label(GL_VERTEX_ARRAY, unwrap(), label.size(), label.data());
}

std::string tr::vertex_format::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a vertex format in an invalid state.");

	const gl_api& gl{context().gl()};
	int label_length;
	gl.get_object_label(GL_VERTEX_ARRAY, unwrap(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_VERTEX_ARRAY, unwrap(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

//

unsigned int tr::vertex_format::unwrap() const
{
	return m_handle.get();
}

#ifdef TR_ENABLE_GL_CHECKS
tr::graphics_object_id tr::vertex_format::id() const
{
	return m_handle.get_deleter().id;
}

std::span<const tr::vertex_binding> tr::vertex_format::bindings() const
{
	return m_bindings;
}
#endif