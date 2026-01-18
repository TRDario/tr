///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements handle.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "handle.hpp"
#include "macro.hpp"

////////////////////////////////////////////////////////////////// HANDLE /////////////////////////////////////////////////////////////////

template <std::regular T, T E, tr::handle_deleter<T> D> struct std::hash<tr::handle<T, E, D>> {
	constexpr auto operator()(const tr::handle<T, E, D>& handle) const
	{
		return std::hash<T>{}(handle.get(tr::no_empty_handle_check));
	}
};

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle()
	requires(default_constructible_handle_deleter<D>)
	: m_base{E}
{
}

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value)
	requires(default_constructible_handle_deleter<D>)
	: m_base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D& deleter)
	requires(std::copy_constructible<D>)
	: D{deleter}, m_base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, const D& deleter)
	requires(std::copy_constructible<D> && !(lvalue_reference<D> && !const_qualified<D>))
	: D{deleter}, m_base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D&& deleter)
	requires(std::move_constructible<D> && !lvalue_reference<D>)
	: D{std::move(deleter)}, m_base{value}
{
	TR_ASSERT(value != E, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, no_empty_handle_check_t)
	requires(default_constructible_handle_deleter<D>)
	: m_base{value}
{
}

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D& deleter, no_empty_handle_check_t)
	requires(std::copy_constructible<D>)
	: D{deleter}, m_base{value}
{
}

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, const D& deleter, no_empty_handle_check_t)
	requires(std::copy_constructible<D> && !(lvalue_reference<D> && !const_qualified<D>))
	: D{deleter}, m_base{value}
{
}

template <std::regular T, T E, tr::handle_deleter<T> D>
constexpr tr::handle<T, E, D>::handle(T value, D&& deleter, no_empty_handle_check_t)
	requires(std::move_constructible<D> && !lvalue_reference<D>)
	: D{std::move(deleter)}, m_base{value}
{
}

template <std::regular T, T E, tr::handle_deleter<T> D>
template <tr::handle_deleter<T> D1>
constexpr tr::handle<T, E, D>::handle(handle<T, E, D1>&& move) noexcept
	: m_base{std::exchange(move.m_base, E)}
{
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>::~handle<T, E, D>()
{
	if (m_base != E) {
		D::operator()(m_base);
	}
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>& tr::handle<T, E, D>::operator=(handle&& r) noexcept
{
	std::ignore = handle{std::move(*this)};
	std::swap(m_base, r.m_base);
	return *this;
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr bool tr::handle<T, E, D>::has_value() const
{
	return m_base != E;
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr tr::handle<T, E, D>::operator bool() const
{
	return has_value();
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr const T& tr::handle<T, E, D>::get() const
{
	TR_ASSERT(m_base != E, "Cannot get the value of an empty handle.");

	return m_base;
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr const T& tr::handle<T, E, D>::get(no_empty_handle_check_t) const
{
	return m_base;
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr D& tr::handle<T, E, D>::get_deleter()
{
	return *this;
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr D& tr::handle<T, E, D>::get_deleter() const
{
	return *this;
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr T tr::handle<T, E, D>::release()
{
	return std::exchange(m_base, E);
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::reset()
{
	std::ignore = std::exchange(*this, handle{});
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::reset(T value)
{
	std::ignore = std::exchange(*this, handle{value});
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::reset(T value, no_empty_handle_check_t)
{
	std::ignore = std::exchange(*this, handle{value, no_empty_handle_check});
}

template <std::regular T, T E, tr::handle_deleter<T> D> constexpr void tr::handle<T, E, D>::swap(handle& other)
{
	std::swap(m_base, other.m_base);
}