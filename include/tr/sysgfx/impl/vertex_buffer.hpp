///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of vertex_buffer.hpp.                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../../utility/ranges.hpp"
#include "../vertex_buffer.hpp"

/////////////////////////////////////////////////////////// STATIC VERTEX BUFFER //////////////////////////////////////////////////////////

template <tr::standard_layout T>
template <tr::typed_contiguous_const_range<T> R>
tr::gfx::static_vertex_buffer<T>::static_vertex_buffer(R&& range)
	: basic_static_vertex_buffer{range_bytes(range)}
{
}

////////////////////////////////////////////////////////// DYNAMIC VERTEX BUFFER //////////////////////////////////////////////////////////

template <tr::standard_layout T> tr::usize tr::gfx::dyn_vertex_buffer<T>::size() const
{
	return basic_dyn_vertex_buffer::size() / sizeof(T);
}

template <tr::standard_layout T> tr::usize tr::gfx::dyn_vertex_buffer<T>::capacity() const
{
	return basic_dyn_vertex_buffer::capacity() / sizeof(T);
}

template <tr::standard_layout T> void tr::gfx::dyn_vertex_buffer<T>::resize(usize size)
{
	basic_dyn_vertex_buffer::resize(size * sizeof(T));
}

template <tr::standard_layout T> void tr::gfx::dyn_vertex_buffer<T>::reserve(usize size)
{
	basic_dyn_vertex_buffer::reserve(size * sizeof(T));
}

template <tr::standard_layout T> template <tr::typed_contiguous_const_range<T> R> void tr::gfx::dyn_vertex_buffer<T>::set(R&& data)
{
	basic_dyn_vertex_buffer::set(range_bytes(data));
}

template <tr::standard_layout T>
template <tr::typed_contiguous_const_range<T> R>
void tr::gfx::dyn_vertex_buffer<T>::set_region(usize offset, R&& data)
{
	basic_dyn_vertex_buffer::set_region(offset * sizeof(T), range_bytes(data));
}