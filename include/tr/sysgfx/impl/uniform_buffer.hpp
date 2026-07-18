///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of uniform_buffer.hpp.                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../uniform_buffer.hpp"

////////////////////////////////////////////////////////////// UNIFORM BUFFER /////////////////////////////////////////////////////////////

template <typename Object>
tr::uniform_buffer<Object>::uniform_buffer(graphics_context& context)
	: basic_uniform_buffer{context, sizeof(Object)}
{
}

template <typename Object> void tr::uniform_buffer<Object>::set(const Object& data)
{
	basic_uniform_buffer::set(as_bytes(data));
}

template <typename Object> tr::graphics_buffer_object_map<Object> tr::uniform_buffer<Object>::map()
{
	return basic_uniform_buffer::map();
}