/// @file
/// @brief Implements the templated parts of shader.hpp.

#pragma once
#include "../shader.hpp"

//

template <typename Header, typename ArrayElement>
void tr::shader_base::set_storage_buffer(unsigned int index, shader_buffer<Header, ArrayElement>& buffer)
{
	set_storage_buffer(index, buffer.id(), sizeof(Header) + sizeof(ArrayElement) * buffer.array_size());
}

template <typename Element>
void tr::shader_base::set_storage_buffer(unsigned int index, shader_array<Element>& buffer)
{
	set_storage_buffer(index, buffer.id(), sizeof(Element) * buffer.size());
}

template <typename Object>
void tr::shader_base::set_uniform_buffer(unsigned int index, const uniform_buffer<Object>& buffer)
{
	set_uniform_buffer(index, buffer.id());
}