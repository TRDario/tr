///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of vertex_buffer.hpp.                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../../utility/ranges.hpp"
#include "../vertex_buffer.hpp"

/////////////////////////////////////////////////////////// STATIC VERTEX BUFFER //////////////////////////////////////////////////////////

template <tr::standard_layout Element>
template <tr::typed_contiguous_const_range<Element> Range>
tr::static_vertex_buffer<Element>::static_vertex_buffer(graphics_context& context, Range&& range)
	: basic_static_vertex_buffer{context, range_bytes(range)}
{
}

////////////////////////////////////////////////////////// DYNAMIC VERTEX BUFFER //////////////////////////////////////////////////////////

template <tr::standard_layout Element> tr::usize tr::dyn_vertex_buffer<Element>::size() const
{
	return basic_dyn_vertex_buffer::size() / sizeof(Element);
}

template <tr::standard_layout Element> tr::usize tr::dyn_vertex_buffer<Element>::capacity() const
{
	return basic_dyn_vertex_buffer::capacity() / sizeof(Element);
}

template <tr::standard_layout Element> void tr::dyn_vertex_buffer<Element>::resize(usize size)
{
	basic_dyn_vertex_buffer::resize(size * sizeof(Element));
}

template <tr::standard_layout Element> void tr::dyn_vertex_buffer<Element>::reserve(usize size)
{
	basic_dyn_vertex_buffer::reserve(size * sizeof(Element));
}

template <tr::standard_layout Element>
template <tr::typed_contiguous_const_range<Element> Range>
void tr::dyn_vertex_buffer<Element>::set(Range&& data)
{
	basic_dyn_vertex_buffer::set(range_bytes(data));
}

template <tr::standard_layout Element>
template <tr::typed_contiguous_const_range<Element> Range>
void tr::dyn_vertex_buffer<Element>::set_region(usize offset, Range&& data)
{
	basic_dyn_vertex_buffer::set_region(offset * sizeof(Element), range_bytes(data));
}