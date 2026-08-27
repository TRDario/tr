/// @file
/// @brief Implements the templated parts of graphics_context.hpp.

#pragma once
#include "../graphics_context.hpp"

//

template <tr::standard_layout T>
void tr::graphics_context::set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, ssize offset)
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	check_vertex_buffer(buffer.label(), slot, as_vertex_attribute_list<T>);
#endif
	set_vertex_buffer(buffer.unwrap(), slot, offset * sizeof(T), sizeof(T));
}

template <tr::standard_layout T>
void tr::graphics_context::set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, ssize offset)
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	check_vertex_buffer(buffer.label(), slot, as_vertex_attribute_list<T>);
#endif
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