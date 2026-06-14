///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of graphics_context.hpp.                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../graphics_context.hpp"

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <tr::standard_layout T> void tr::graphics_context::set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, ssize offset)
{
#ifdef TR_ENABLE_GL_CHECKS
	check_vertex_buffer(buffer.label(), slot, as_vertex_attribute_list<T>);
#endif
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
}

template <tr::standard_layout T> void tr::graphics_context::set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, ssize offset)
{
#ifdef TR_ENABLE_GL_CHECKS
	check_vertex_buffer(buffer.label(), slot, as_vertex_attribute_list<T>);
#endif
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
}