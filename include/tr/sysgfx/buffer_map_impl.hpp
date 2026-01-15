///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr parts of buffer_map.hpp.                                                                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "buffer_map.hpp"

//////////////////////////////////////////////////////////// BUFFER OBJECT MAP ////////////////////////////////////////////////////////////

template <class T>
tr::gfx::buffer_object_map<T>::buffer_object_map(basic_buffer_map&& map)
	: basic_buffer_map{std::move(map)}
{
}

template <class T> tr::gfx::buffer_object_map<T>::operator T&() const
{
	return *(T*)std::span<std::byte>{*this}.data();
}

template <class T> T* tr::gfx::buffer_object_map<T>::operator->() const
{
	return (T*)std::span<std::byte>{*this}.data();
}

template <class T> template <class T1> T& tr::gfx::buffer_object_map<T>::operator=(T1&& r) const
{
	return ((T&)*this) = std::forward<T1>(r);
}

///////////////////////////////////////////////////////////// BUFFER SPAN MAP /////////////////////////////////////////////////////////////

template <class T>
tr::gfx::buffer_span_map<T>::buffer_span_map(basic_buffer_map&& map)
	: basic_buffer_map{std::move(map)}
{
}

template <class T> tr::gfx::buffer_span_map<T>::operator std::span<T>() const
{
	return as_mut_objects<T>(std::span<std::byte>{*this});
}

template <class T> tr::gfx::buffer_span_map<T>::reference tr::gfx::buffer_span_map<T>::operator[](usize index) const
{
	return (operator std::span<T>())[index];
}

template <class T> tr::gfx::buffer_span_map<T>::pointer tr::gfx::buffer_span_map<T>::data() const
{
	return (operator std::span<T>()).data();
}

template <class T> tr::gfx::buffer_span_map<T>::size_type tr::gfx::buffer_span_map<T>::size() const
{
	return (operator std::span<T>()).size();
}

template <class T> tr::gfx::buffer_span_map<T>::iterator tr::gfx::buffer_span_map<T>::begin() const
{
	return (operator std::span<T>()).begin();
}

template <class T> tr::gfx::buffer_span_map<T>::iterator tr::gfx::buffer_span_map<T>::end() const
{
	return (operator std::span<T>()).end();
}