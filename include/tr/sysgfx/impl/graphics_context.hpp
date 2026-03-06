///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of graphics_context.hpp.                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../graphics_context.hpp"

//////////////////////////////////////////////////////////// SET VERTEX BUFFER ////////////////////////////////////////////////////////////

#ifdef TR_ENABLE_GL_CHECKS
namespace tr::gfx {
	// Sets an active vertex buffer.
	void set_vertex_buffer_checked(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride,
								   std::span<const vertex_attribute> attributes);
	// Sets an active vertex buffer.
	void set_vertex_buffer_checked(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride,
								   std::span<const vertex_attribute> attributes);
} // namespace tr::gfx
#endif

template <tr::standard_layout T> void tr::gfx::set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, ssize offset)
{
#ifdef TR_ENABLE_GL_CHECKS
	set_vertex_buffer_checked(buffer, slot, offset * sizeof(T), sizeof(T), as_vertex_attribute_list<T>);
#else
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
#endif
}

template <tr::standard_layout T> void tr::gfx::set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, ssize offset)
{
#ifdef TR_ENABLE_GL_CHECKS
	set_vertex_buffer_checked(buffer, slot, offset * sizeof(T), sizeof(T), as_vertex_attribute_list<T>);
#else
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
#endif
}