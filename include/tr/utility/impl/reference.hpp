/// @file
/// @brief Implements reference.hpp.

#pragma once
#include "../macro.hpp"
#include "../reference.hpp"

//

template <typename T>
constexpr tr::ref<T>::ref(T& ref) noexcept
	: m_base{std::addressof(ref)}
{
}

template <typename T>
constexpr tr::ref<T>::operator T&() const noexcept
{
	return *m_base;
}

template <typename T>
constexpr tr::ref<T>::operator ref<const T>() const noexcept
{
	return tr::ref<const T>{*m_base};
}

template <typename T>
template <typename U>
	requires(std::convertible_to<T&, U&>)
constexpr tr::ref<T>::operator ref<U>() const noexcept
{
	return static_cast<U&>(**this);
}

template <typename T>
constexpr bool tr::operator==(const ref<T>& lhs, const T& rhs) noexcept
{
	return lhs == ref{rhs};
}

template <typename T>
constexpr T* tr::ref<T>::as_ptr() const noexcept
{
	return m_base;
}

template <typename T>
constexpr T* tr::ref<T>::operator->() const noexcept
{
	return m_base;
}

template <typename T>
constexpr T& tr::ref<T>::operator*() const noexcept
{
	return *m_base;
}

//

template <typename T>
constexpr tr::opt_ref<T>::opt_ref(std::nullopt_t) noexcept
	: m_base{nullptr}
{
}

template <typename T>
constexpr tr::opt_ref<T>::opt_ref(T* ptr) noexcept
	: m_base{ptr}
{
}

template <typename T>
constexpr tr::opt_ref<T>::opt_ref(T& ref) noexcept
	: m_base{std::addressof(ref)}
{
}

template <typename T>
constexpr tr::opt_ref<T>::operator opt_ref<const T>() const noexcept
{
	return make_opt_ref<const T>(m_base);
}

template <typename T>
template <typename U>
	requires(std::convertible_to<T&, U&>)
constexpr tr::opt_ref<T>::operator opt_ref<U>() const noexcept
{
	return make_opt_ref(static_cast<U*>(as_ptr()));
}

template <typename T>
constexpr bool tr::operator==(opt_ref<T> lhs, const std::type_identity_t<T>& rhs) noexcept
{
	return lhs.as_ptr() == &rhs;
}

template <typename T>
constexpr bool tr::opt_ref<T>::has_ref() const noexcept
{
	return m_base != nullptr;
}

template <typename T>
constexpr T* tr::opt_ref<T>::as_ptr() const noexcept
{
	return m_base;
}

template <typename T>
constexpr T* tr::opt_ref<T>::operator->() const noexcept
{
	TR_ASSERT(has_ref(), "Tried to dereference empty optional reference.");

	return m_base;
}

template <typename T>
constexpr T& tr::opt_ref<T>::operator*() const noexcept
{
	TR_ASSERT(has_ref(), "Tried to dereference empty optional reference.");

	return *m_base;
}

template <typename T>
constexpr tr::opt_ref<T> tr::make_opt_ref(T* ptr) noexcept
{
	return opt_ref<T>{ptr};
}

//

template <typename To, typename From>
	requires(std::derived_from<To, From>)
constexpr tr::opt_ref<To> tr::dynamic_ref_cast(From& ref) noexcept
{
	return make_opt_ref(dynamic_cast<To*>(std::addressof(ref)));
}

template <typename To, typename From>
	requires(std::derived_from<To, From>)
constexpr tr::opt_ref<To> tr::dynamic_ref_cast(ref<From> ref) noexcept
{
	return make_opt_ref(dynamic_cast<To*>(ref.as_ptr()));
}

template <typename To, typename From>
	requires(std::derived_from<To, From>)
constexpr tr::opt_ref<To> tr::dynamic_ref_cast(opt_ref<From> ref) noexcept
{
	return make_opt_ref(dynamic_cast<To*>(ref.as_ptr()));
}