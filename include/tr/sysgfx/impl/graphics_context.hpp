/// @file
/// @brief Implements the templated parts of graphics_context.hpp.

#pragma once
#include "../graphics_context.hpp"

//

template <tr::any_vertex_buffer VertexBuffer>
void tr::graphics_context::set_vertex_buffer_base(const VertexBuffer& buffer, int slot, ssize offset, usize stride)
{
	TR_ASSERT(buffer.valid(), "Tried to set a vertex buffer in an invalid state to a graphics context.");
	TR_ASSERT(&buffer.context() == this, "Tried to set vertex buffer {} to a context it is not associated with.", buffer);

	gl().bind_vertex_buffer(slot, buffer.unwrap(), offset, stride);

#ifdef TR_ENABLE_CHECKED_GRAPHICS
	m_set_vertex_buffer_debug_info.id = buffer.id();
	m_set_vertex_buffer_debug_info.label = buffer.label();
#endif
}

template <tr::any_untyped_vertex_buffer UntypedVertexBuffer>
void tr::graphics_context::set_vertex_buffer(const UntypedVertexBuffer& buffer, int slot, ssize offset, usize stride)
{
	set_vertex_buffer_base(buffer, slot, offset, stride);
}

template <tr::any_typed_vertex_buffer TypedVertexBuffer>
void tr::graphics_context::set_vertex_buffer(const TypedVertexBuffer& buffer, int slot, ssize offset)
{
	using element_type = TypedVertexBuffer::value_type;

	set_vertex_buffer_base(buffer, slot, offset * sizeof(element_type), sizeof(element_type));
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	check_typed_vertex_buffer(buffer.label(), slot, as_vertex_attribute_list<element_type>);
#endif
}

template <tr::any_index_buffer IndexBuffer>
void tr::graphics_context::set_index_buffer(const IndexBuffer& buffer)
{
	TR_ASSERT(buffer.valid(), "Tried to set an index buffer in an invalid state to a graphics context.");

	constexpr unsigned int GL_ELEMENT_ARRAY_BUFFER{0x8893};
	gl().bind_buffer(GL_ELEMENT_ARRAY_BUFFER, buffer.unwrap());

#ifdef TR_ENABLE_CHECKED_GRAPHICS
	m_set_index_buffer_debug_info.id = buffer.id();
	m_set_index_buffer_debug_info.label = buffer.label();
#endif
}