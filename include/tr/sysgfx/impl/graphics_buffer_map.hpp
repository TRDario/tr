/// @file
/// @brief Implements the templated parts of graphics_buffer_map.hpp.

#pragma once
#include "../graphics_buffer_map.hpp"

//

template <typename Object>
tr::graphics_buffer_object_map<Object>::graphics_buffer_object_map(basic_graphics_buffer_map&& map) noexcept
	: basic_graphics_buffer_map{std::move(map)}
{
}

template <typename Object>
tr::graphics_buffer_object_map<Object>::operator Object&() const noexcept
{
	return get();
}

template <typename Object>
Object& tr::graphics_buffer_object_map<Object>::get() const noexcept
{
	return as_mut_object<Object>(std::span<std::byte>{*this});
}

template <typename Object>
Object& tr::graphics_buffer_object_map<Object>::operator*() const noexcept
{
	return get();
}

template <typename Object>
Object* tr::graphics_buffer_object_map<Object>::operator->() const noexcept
{
	return &get();
}

template <typename Object>
template <std::assignable_from<Object> T>
Object& tr::graphics_buffer_object_map<Object>::operator=(T&& r) const noexcept(std::is_nothrow_assignable_v<Object, T>)
{
	return get() = std::forward<T>(r);
}

//

template <typename Element>
tr::graphics_buffer_span_map<Element>::graphics_buffer_span_map(basic_graphics_buffer_map&& map) noexcept
	: basic_graphics_buffer_map{std::move(map)}
{
}

template <typename Element>
tr::graphics_buffer_span_map<Element>::operator std::span<Element>() const noexcept
{
	return as_mut_objects<Element>(std::span<std::byte>{*this});
}

template <typename Element>
tr::graphics_buffer_span_map<Element>::reference tr::graphics_buffer_span_map<Element>::operator[](usize index) const noexcept
{
	return span()[index];
}

template <typename Element>
tr::graphics_buffer_span_map<Element>::pointer tr::graphics_buffer_span_map<Element>::data() const noexcept
{
	return span().data();
}

template <typename Element>
tr::graphics_buffer_span_map<Element>::size_type tr::graphics_buffer_span_map<Element>::size() const noexcept
{
	return span().size();
}

template <typename Element>
tr::graphics_buffer_span_map<Element>::iterator tr::graphics_buffer_span_map<Element>::begin() const noexcept
{
	return span().begin();
}

template <typename Element>
tr::graphics_buffer_span_map<Element>::iterator tr::graphics_buffer_span_map<Element>::end() const noexcept
{
	return span().end();
}