///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-constexpr parts of buffer_map.hpp.                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/buffer_map.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"

///////////////////////////////////////////////////////////// BASIC BUFFER MAP ////////////////////////////////////////////////////////////

tr::gfx::basic_buffer_map::basic_buffer_map(unsigned int buffer, std::span<std::byte> span)
	: m_bo{buffer}, m_span{span}
{
}

tr::gfx::basic_buffer_map::operator std::span<std::byte>() const
{
	return m_span;
}

void tr::gfx::basic_buffer_map::deleter::operator()(unsigned int id) const
{
	TR_GL_CALL(glUnmapNamedBuffer, id);
}