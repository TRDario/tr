/// @file
/// @brief Implements reference.hpp.

#pragma once
#include "../macro.hpp"
#include "../reference.hpp"

//

template <typename T>
constexpr tr::ref<T>::ref(T& ref)
	: m_base{std::addressof(ref)}
{
}

template <typename T>
constexpr tr::ref<T>::operator T&() const
{
	return *m_base;
}

template <typename T>
constexpr tr::ref<T>::operator ref<const T>() const
{
	return tr::ref<const T>{*m_base};
}

template <typename T>
template <typename U>
	requires(std::convertible_to<T&, U&>)
constexpr tr::ref<T>::operator ref<U>() const
{
	return static_cast<U&>(**this);
}

template <typename T>
constexpr bool tr::operator==(const ref<T>& lhs, const T& rhs)
{
	return lhs == ref{rhs};
}

template <typename T>
constexpr T* tr::ref<T>::as_ptr() const
{
	return m_base;
}

template <typename T>
constexpr T* tr::ref<T>::operator->() const
{
	return m_base;
}

template <typename T>
constexpr T& tr::ref<T>::operator*() const
{
	return *m_base;
}

//

template <typename T>
constexpr tr::opt_ref<T>::opt_ref(std::nullopt_t)
	: m_base{nullptr}
{
}

template <typename T>
constexpr tr::opt_ref<T>::opt_ref(T* ptr)
	: m_base{ptr}
{
}

template <typename T>
constexpr tr::opt_ref<T>::opt_ref(T& ref)
	: m_base{std::addressof(ref)}
{
}

template <typename T>
constexpr tr::opt_ref<T>::operator opt_ref<const T>() const
{
	return opt_ref_from_ptr<const T>(m_base);
}

template <typename T>
template <typename U>
	requires(std::convertible_to<T&, U&>)
constexpr tr::opt_ref<T>::operator opt_ref<U>() const
{
	return opt_ref_from_ptr(static_cast<U*>(as_ptr()));
}

template <typename T>
constexpr bool tr::operator==(opt_ref<T> lhs, const std::type_identity_t<T>& rhs)
{
	return lhs.as_ptr() == &rhs;
}

template <typename T>
constexpr bool tr::opt_ref<T>::has_ref() const
{
	return m_base != nullptr;
}

template <typename T>
constexpr T* tr::opt_ref<T>::as_ptr() const
{
	return m_base;
}

template <typename T>
constexpr T* tr::opt_ref<T>::operator->() const
{
	TR_ASSERT(has_ref(), "Tried to dereference empty optional reference.");

	return m_base;
}

template <typename T>
constexpr T& tr::opt_ref<T>::operator*() const
{
	TR_ASSERT(has_ref(), "Tried to dereference empty optional reference.");

	return *m_base;
}

template <typename T>
constexpr tr::opt_ref<T> tr::make_opt_ref(T* ptr)
{
	return opt_ref<T>{ptr};
}

//

template <typename To, typename From>
	requires(std::derived_from<To, From>)
constexpr tr::opt_ref<To> tr::dynamic_ref_cast(From& ref)
{
	return make_opt_ref(dynamic_cast<To*>(std::addressof(ref)));
}

template <typename To, typename From>
	requires(std::derived_from<To, From>)
constexpr tr::opt_ref<To> tr::dynamic_ref_cast(ref<From> ref)
{
	return make_opt_ref(dynamic_cast<To*>(ref.as_ptr()));
}

template <typename To, typename From>
	requires(std::derived_from<To, From>)
constexpr tr::opt_ref<To> tr::dynamic_ref_cast(opt_ref<From> ref)
{
	return make_opt_ref(dynamic_cast<To*>(ref.as_ptr()));
}