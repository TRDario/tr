///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of shader_buffer.hpp.                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../shader_buffer.hpp"

////////////////////////////////////////////////////////////// SHADER BUFFER //////////////////////////////////////////////////////////////

template <typename Header, typename ArrayElement>
tr::shader_buffer<Header, ArrayElement>::shader_buffer(graphics_context& context, usize capacity, map_type map_type)
	: basic_shader_buffer{context, sizeof(Header), sizeof(ArrayElement) * capacity, map_type}
{
}

template <typename Header, typename ArrayElement> tr::usize tr::shader_buffer<Header, ArrayElement>::array_size() const
{
	return basic_shader_buffer::array_size() / sizeof(ArrayElement);
}

template <typename Header, typename ArrayElement> tr::usize tr::shader_buffer<Header, ArrayElement>::array_capacity() const
{
	return basic_shader_buffer::array_capacity() / sizeof(ArrayElement);
}

template <typename Header, typename ArrayElement> void tr::shader_buffer<Header, ArrayElement>::set_header(const Header& header)
{
	basic_shader_buffer::set_header(as_bytes(header));
}

template <typename Header, typename ArrayElement>
template <tr::typed_contiguous_const_range<ArrayElement> R>
void tr::shader_buffer<Header, ArrayElement>::set_array(R&& data)
{
	basic_shader_buffer::set_array(range_bytes(data));
}

template <typename Header, typename ArrayElement> void tr::shader_buffer<Header, ArrayElement>::resize_array(usize size)
{
	basic_shader_buffer::resize_array(size * sizeof(ArrayElement));
}

template <typename Header, typename ArrayElement>
tr::graphics_buffer_object_map<Header> tr::shader_buffer<Header, ArrayElement>::map_header()
{
	return basic_shader_buffer::map_header();
}

template <typename Header, typename ArrayElement>
tr::graphics_buffer_span_map<ArrayElement> tr::shader_buffer<Header, ArrayElement>::map_array()
{
	return basic_shader_buffer::map_array();
}

/////////////////////////////////////////////////////////////// SHADER ARRAY //////////////////////////////////////////////////////////////

template <typename Element>
tr::shader_array<Element>::shader_array(graphics_context& context, usize capacity, map_type map_type)
	: basic_shader_buffer{context, 0, sizeof(Element) * capacity, map_type}
{
}

template <typename Element> tr::usize tr::shader_array<Element>::size() const
{
	return basic_shader_buffer::array_size() / sizeof(Element);
}

template <typename Element> tr::usize tr::shader_array<Element>::capacity() const
{
	return basic_shader_buffer::array_capacity() / sizeof(Element);
}

template <typename Element> template <tr::typed_contiguous_const_range<Element> Range> void tr::shader_array<Element>::set(Range&& data)
{
	basic_shader_buffer::set_array(range_bytes(data));
}

template <typename Element> void tr::shader_array<Element>::resize(usize size)
{
	basic_shader_buffer::resize_array(size * sizeof(Element));
}

template <typename Element> tr::graphics_buffer_span_map<Element> tr::shader_array<Element>::map()
{
	return basic_shader_buffer::map_array();
}