///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements handle.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../handle.hpp"
#include "../macro.hpp"

////////////////////////////////////////////////////////////////// HANDLE /////////////////////////////////////////////////////////////////

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter> struct std::hash<tr::handle<Base, Empty, Deleter>> {
	constexpr auto operator()(const tr::handle<Base, Empty, Deleter>& handle) const
	{
		return std::hash<Base>{}(handle.get(tr::no_empty_handle_check));
	}
};

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle()
	requires(default_constructible_handle_deleter<Deleter>)
	: m_base{Empty}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value)
	requires(default_constructible_handle_deleter<Deleter>)
	: m_base{value}
{
	TR_ASSERT(value != Empty, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, Deleter& deleter)
	requires(std::copy_constructible<Deleter>)
	: Deleter{deleter}, m_base{value}
{
	TR_ASSERT(value != Empty, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, const Deleter& deleter)
	requires(std::copy_constructible<Deleter> && !(lvalue_reference<Deleter> && !const_qualified<Deleter>))
	: Deleter{deleter}, m_base{value}
{
	TR_ASSERT(value != Empty, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, Deleter&& deleter)
	requires(std::move_constructible<Deleter> && !lvalue_reference<Deleter>)
	: Deleter{std::move(deleter)}, m_base{value}
{
	TR_ASSERT(value != Empty, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, no_empty_handle_check_t)
	requires(default_constructible_handle_deleter<Deleter>)
	: m_base{value}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, Deleter& deleter, no_empty_handle_check_t)
	requires(std::copy_constructible<Deleter>)
	: Deleter{deleter}, m_base{value}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, const Deleter& deleter, no_empty_handle_check_t)
	requires(std::copy_constructible<Deleter> && !(lvalue_reference<Deleter> && !const_qualified<Deleter>))
	: Deleter{deleter}, m_base{value}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, Deleter&& deleter, no_empty_handle_check_t)
	requires(std::move_constructible<Deleter> && !lvalue_reference<Deleter>)
	: Deleter{std::move(deleter)}, m_base{value}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> D>
template <tr::handle_deleter<Base> DeleterR>
constexpr tr::handle<Base, Empty, D>::handle(handle<Base, Empty, DeleterR>&& move) noexcept
	: m_base{std::exchange(move.m_base, Empty)}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::~handle<Base, Empty, Deleter>()
{
	if (m_base != Empty) {
		Deleter::operator()(m_base);
	}
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>& tr::handle<Base, Empty, Deleter>::operator=(handle&& r) noexcept
{
	std::ignore = handle{std::move(*this)};
	std::swap(m_base, r.m_base);
	return *this;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr bool tr::handle<Base, Empty, Deleter>::has_value() const
{
	return m_base != Empty;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter> constexpr tr::handle<Base, Empty, Deleter>::operator bool() const
{
	return has_value();
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr const Base& tr::handle<Base, Empty, Deleter>::get() const
{
	TR_ASSERT(m_base != Empty, "Cannot get the value of an empty handle.");

	return m_base;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr const Base& tr::handle<Base, Empty, Deleter>::get(no_empty_handle_check_t) const
{
	return m_base;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr Deleter& tr::handle<Base, Empty, Deleter>::get_deleter()
{
	return *this;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr Deleter& tr::handle<Base, Empty, Deleter>::get_deleter() const
{
	return *this;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter> constexpr Base tr::handle<Base, Empty, Deleter>::release()
{
	return std::exchange(m_base, Empty);
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter> constexpr void tr::handle<Base, Empty, Deleter>::reset()
{
	std::ignore = std::exchange(*this, handle{});
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr void tr::handle<Base, Empty, Deleter>::reset(Base value)
{
	std::ignore = std::exchange(*this, handle{value});
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr void tr::handle<Base, Empty, Deleter>::reset(Base value, no_empty_handle_check_t)
{
	std::ignore = std::exchange(*this, handle{value, no_empty_handle_check});
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr void tr::handle<Base, Empty, Deleter>::swap(handle& other)
{
	std::swap(m_base, other.m_base);
}