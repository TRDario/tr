///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the constexpr parts of buffer_map.hpp.                                                                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../buffer_map.hpp"

//////////////////////////////////////////////////////////// BUFFER OBJECT MAP ////////////////////////////////////////////////////////////

template <typename Object>
tr::gfx::buffer_object_map<Object>::buffer_object_map(basic_buffer_map&& map)
	: basic_buffer_map{std::move(map)}
{
}

template <typename Object> tr::gfx::buffer_object_map<Object>::operator Object&() const
{
	return as_mut_object<Object>(std::span<std::byte>{*this});
}

template <typename Object> Object& tr::gfx::buffer_object_map<Object>::operator*() const
{
	return *this;
}

template <typename Object> Object* tr::gfx::buffer_object_map<Object>::operator->() const
{
	return &**this;
}

template <typename Object> template <std::assignable_from<Object> T> Object& tr::gfx::buffer_object_map<Object>::operator=(T&& r) const
{
	return **this = std::forward<T>(r);
}

///////////////////////////////////////////////////////////// BUFFER SPAN MAP /////////////////////////////////////////////////////////////

template <typename Element>
tr::gfx::buffer_span_map<Element>::buffer_span_map(basic_buffer_map&& map)
	: basic_buffer_map{std::move(map)}
{
}

template <typename Element> tr::gfx::buffer_span_map<Element>::operator std::span<Element>() const
{
	return as_mut_objects<Element>(std::span<std::byte>{*this});
}

template <typename Element> tr::gfx::buffer_span_map<Element>::reference tr::gfx::buffer_span_map<Element>::operator[](usize index) const
{
	return (operator std::span<Element>())[index];
}

template <typename Element> tr::gfx::buffer_span_map<Element>::pointer tr::gfx::buffer_span_map<Element>::data() const
{
	return (operator std::span<Element>()).data();
}

template <typename Element> tr::gfx::buffer_span_map<Element>::size_type tr::gfx::buffer_span_map<Element>::size() const
{
	return (operator std::span<Element>()).size();
}

template <typename Element> tr::gfx::buffer_span_map<Element>::iterator tr::gfx::buffer_span_map<Element>::begin() const
{
	return (operator std::span<Element>()).begin();
}

template <typename Element> tr::gfx::buffer_span_map<Element>::iterator tr::gfx::buffer_span_map<Element>::end() const
{
	return (operator std::span<Element>()).end();
}