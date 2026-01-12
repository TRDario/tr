///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements reference.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "reference.hpp"

//////////////////////////////////////////////////////////// REFERENCE WRAPPER ////////////////////////////////////////////////////////////

template <class T>
constexpr tr::ref<T>::ref(T& ref)
	: m_base{std::addressof(ref)}
{
}

template <class T> constexpr tr::ref<T>::operator T&() const
{
	return *m_base;
}

template <class T> constexpr tr::ref<T>::operator ref<const T>() const
{
	return tr::ref<const T>{*m_base};
}

template <class T> constexpr bool tr::operator==(const T& l, const ref<T>& r)
{
	return ref{l} == r;
}

template <class T> constexpr bool tr::operator==(const ref<T>& l, const T& r)
{
	return l == ref{r};
}

template <class T> constexpr T* tr::ref<T>::as_ptr() const
{
	return m_base;
}

template <class T> constexpr T* tr::ref<T>::operator->() const
{
	return m_base;
}

template <class T> constexpr T& tr::ref<T>::operator*() const
{
	return *m_base;
}

//////////////////////////////////////////////////////// OPTIONAL REFERENCE WRAPPER ///////////////////////////////////////////////////////

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
	: m_base{std::addressof(ref)}
{
}

template <class T> constexpr tr::opt_ref<T>::operator opt_ref<const T>() const
{
	return make_opt_ref<const T>(m_base);
}

template <class T> constexpr bool tr::opt_ref<T>::has_ref() const
{
	return m_base != nullptr;
}

template <class T> constexpr T* tr::opt_ref<T>::as_ptr() const
{
	return m_base;
}

template <class T> constexpr T* tr::opt_ref<T>::operator->() const
{
	TR_ASSERT(has_ref(), "Tried to dereference empty optional reference.");

	return m_base;
}

template <class T> constexpr T& tr::opt_ref<T>::operator*() const
{
	TR_ASSERT(has_ref(), "Tried to dereference empty optional reference.");

	return *m_base;
}

template <class T> tr::opt_ref<T> tr::make_opt_ref(T* ptr)
{
	return opt_ref<T>{ptr};
}