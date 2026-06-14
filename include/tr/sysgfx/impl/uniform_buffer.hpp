///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of uniform_buffer.hpp.                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../uniform_buffer.hpp"

////////////////////////////////////////////////////////////// UNIFORM BUFFER /////////////////////////////////////////////////////////////

template <typename T>
tr::uniform_buffer<T>::uniform_buffer(graphics_context& context)
	: basic_uniform_buffer{context, sizeof(T)}
{
}

template <typename T> void tr::uniform_buffer<T>::set(const T& data)
{
	basic_uniform_buffer::set(as_bytes(data));
}

template <typename T> tr::graphics_buffer_object_map<T> tr::uniform_buffer<T>::map()
{
	return basic_uniform_buffer::map();
}