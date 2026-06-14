///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-constexpr parts of graphics_buffer_map.hpp.                                                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/graphics_buffer_map.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

///////////////////////////////////////////////////////////// BASIC BUFFER MAP ////////////////////////////////////////////////////////////

tr::basic_graphics_buffer_map::basic_graphics_buffer_map(graphics_context& context, unsigned int buffer, std::span<std::byte> span)
	: m_bo{buffer, {context}}, m_span{span}
{
}

tr::basic_graphics_buffer_map::operator std::span<std::byte>() const
{
	return m_span;
}

void tr::basic_graphics_buffer_map::deleter::operator()(unsigned int id) const
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.unmap_named_buffer(id);
}