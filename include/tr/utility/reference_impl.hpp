#pragma once
#include "reference.hpp"

template <class T>
constexpr tr::opt_ref<T>::opt_ref(std::nullopt_t)
	: m_base{nullptr}
{
}

template <class T>
constexpr tr::opt_ref<T>::opt_ref(T* ptr)
	: m_base{ptr}
{
}

template <class T>
constexpr tr::opt_ref<T>::opt_ref(T& ref)
	: m_base{&ref}
{
}

template <class T> constexpr tr::opt_ref<T>::operator opt_ref<const T>() const
{
	return make_opt_ref<const T>(m_base);
}

template <class T> constexpr tr::opt_ref<T>::operator bool() const
{
	return has_value();
}

template <class T> constexpr bool tr::opt_ref<T>::has_value() const
{
	return m_base != nullptr;
}

template <class T> constexpr T* tr::opt_ref<T>::as_ptr() const
{
	return m_base;
}

template <class T> constexpr T* tr::opt_ref<T>::operator->() const
{
	TR_ASSERT(has_value(), "Tried to dereference empty optional reference.");

	return m_base;
}

template <class T> constexpr T& tr::opt_ref<T>::operator*() const
{
	TR_ASSERT(has_value(), "Tried to dereference empty optional reference.");

	return *m_base;
}

template <class T> tr::opt_ref<T> tr::make_opt_ref(T* ptr)
{
	return opt_ref<T>{ptr};
}