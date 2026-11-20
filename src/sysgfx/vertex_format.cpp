/// @file
/// @brief Implements vertex_format.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/vertex_format.hpp>
#include <tr/utility/out_handle.hpp>

//

tr::vertex_format::vertex_format(graphics_context& context, std::span<const vertex_binding> bindings, std::string_view label) noexcept
	: m_handle{gl_vertex_array_object_deleter{context}}
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	, m_bindings{bindings}
#endif
{
	const internal::opengl& gl{context.gl()};
	gl.create_vertex_arrays(1, out_handle{m_handle});
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context.registered_vertex_formats().emplace(id());
#endif
	if (!label.empty()) {
		set_label(label);
	}

	unsigned int attr_id{0};
	for (int binding_id = 0; binding_id < static_cast<int>(bindings.size()); ++binding_id) {
		const vertex_binding& binding{bindings.begin()[binding_id]};

		gl.set_vertex_array_binding_divisor(m_handle.get(), binding_id, binding.divisor);
		unsigned int offset{0};
		for (const vertex_attribute& attribute : binding.attrs) {
			TR_ASSERT(attribute.type != vertex_attribute_type::unknown, "Tried to construct vertex format {} with invalid attribute '{}'.",
					  *this, attribute);

			gl.set_vertex_array_attribute_format(m_handle.get(), attr_id, attribute.elements, std::to_underlying(attribute.type),
												 attribute.normalized, offset);
			gl.enable_vertex_array_attribute(m_handle.get(), attr_id);
			gl.set_vertex_array_attribute_binding(m_handle.get(), attr_id++, binding_id);

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

void tr::vertex_format::gl_vertex_array_object_deleter::operator()(unsigned int handle) const noexcept
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	context->registered_vertex_formats().erase(id.get());
#endif
	context->gl().delete_vertex_arrays(1, &handle);
}

//

void tr::vertex_format::set_label(std::string_view label) noexcept
{
	TR_ASSERT(valid(), "Tried to set the label of a vertex format in an invalid state.");

	context().gl().set_object_label(GL_VERTEX_ARRAY, m_handle.get(), static_cast<int>(label.size()), label.data());
}

//

tr::vertex_format::operator bool() const noexcept
{
	return static_cast<bool>(m_handle);
}

bool tr::vertex_format::valid() const noexcept
{
	return static_cast<bool>(m_handle);
}

tr::graphics_context& tr::vertex_format::context() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the context of a vertex format in an invalid state.");

	return m_handle.get_deleter().context;
}

std::string tr::vertex_format::label() const
{
	TR_ASSERT(valid(), "Tried to get the label of a vertex format in an invalid state.");

	const internal::opengl& gl{context().gl()};
	int label_length;
	gl.get_object_label(GL_VERTEX_ARRAY, m_handle.get(), 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label_string(label_length, '\0');
		gl.get_object_label(GL_VERTEX_ARRAY, m_handle.get(), label_length + 1, nullptr, label_string.data());
		return label_string;
	}
	else {
		return "<unnamed>";
	}
}

unsigned int tr::vertex_format::handle() const noexcept
{
	return m_handle.get();
}

tr::internal::graphics_object_id tr::vertex_format::id() const noexcept
{
	return m_handle.get_deleter().id.get();
}

#ifdef TR_ENABLE_CHECKED_GRAPHICS
std::span<const tr::vertex_binding> tr::vertex_format::bindings() const noexcept
{
	return m_bindings;
}
#endif