///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr parts of graphics_buffer_map.hpp.                                                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../graphics_buffer_map.hpp"

//////////////////////////////////////////////////////////// BUFFER OBJECT MAP ////////////////////////////////////////////////////////////

template <typename Object>
tr::graphics_buffer_object_map<Object>::graphics_buffer_object_map(basic_graphics_buffer_map&& map)
	: basic_graphics_buffer_map{std::move(map)}
{
}

template <typename Object> tr::graphics_buffer_object_map<Object>::operator Object&() const
{
	return as_mut_object<Object>(std::span<std::byte>{*this});
}

template <typename Object> Object& tr::graphics_buffer_object_map<Object>::operator*() const
{
	return *this;
}

template <typename Object> Object* tr::graphics_buffer_object_map<Object>::operator->() const
{
	return &**this;
}

template <typename Object> template <std::assignable_from<Object> T> Object& tr::graphics_buffer_object_map<Object>::operator=(T&& r) const
{
	return **this = std::forward<T>(r);
}

///////////////////////////////////////////////////////////// BUFFER SPAN MAP /////////////////////////////////////////////////////////////

template <typename Element>
tr::graphics_buffer_span_map<Element>::graphics_buffer_span_map(basic_graphics_buffer_map&& map)
	: basic_graphics_buffer_map{std::move(map)}
{
}

template <typename Element> tr::graphics_buffer_span_map<Element>::operator std::span<Element>() const
{
	return as_mut_objects<Element>(std::span<std::byte>{*this});
}

template <typename Element>
tr::graphics_buffer_span_map<Element>::reference tr::graphics_buffer_span_map<Element>::operator[](usize index) const
{
	return (operator std::span<Element>())[index];
}

template <typename Element> tr::graphics_buffer_span_map<Element>::pointer tr::graphics_buffer_span_map<Element>::data() const
{
	return (operator std::span<Element>()).data();
}

template <typename Element> tr::graphics_buffer_span_map<Element>::size_type tr::graphics_buffer_span_map<Element>::size() const
{
	return (operator std::span<Element>()).size();
}

template <typename Element> tr::graphics_buffer_span_map<Element>::iterator tr::graphics_buffer_span_map<Element>::begin() const
{
	return (operator std::span<Element>()).begin();
}

template <typename Element> tr::graphics_buffer_span_map<Element>::iterator tr::graphics_buffer_span_map<Element>::end() const
{
	return (operator std::span<Element>()).end();
}