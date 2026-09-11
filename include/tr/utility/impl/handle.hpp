/// @file
/// @brief Implements handle.hpp.

#pragma once
#include "../handle.hpp"
#include "../macro.hpp"

//

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
struct boost::hash<tr::handle<Base, Empty, Deleter>>
{
	constexpr auto operator()(const tr::handle<Base, Empty, Deleter>& handle) const noexcept
	{
		return boost::hash<Base>{}(handle.get(tr::maybe_empty));
	}
};

//

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle() noexcept(std::is_nothrow_default_constructible_v<Deleter>)
	requires(default_constructible_handle_deleter<Deleter>)
	: m_base{Empty}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value) noexcept(std::is_nothrow_default_constructible_v<Deleter>)
	requires(default_constructible_handle_deleter<Deleter>)
	: m_base{value}
{
	TR_ASSERT(value != Empty, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, maybe_empty_t) noexcept(std::is_nothrow_default_constructible_v<Deleter>)
	requires(default_constructible_handle_deleter<Deleter>)
	: m_base{value}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Deleter&& deleter) noexcept(std::is_nothrow_move_constructible_v<Deleter>)
	: Deleter{std::forward<Deleter>(deleter)}
	, m_base{Empty}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, Deleter&& deleter) noexcept(std::is_nothrow_move_constructible_v<Deleter>)
	requires(std::move_constructible<Deleter>)
	: Deleter{std::forward<Deleter>(deleter)}
	, m_base{value}
{
	TR_ASSERT(value != Empty, "Cannot construct a handle from a value set aside as the empty value.");
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(Base value, Deleter&& deleter,
												   maybe_empty_t) noexcept(std::is_nothrow_move_constructible_v<Deleter>)
	requires(std::move_constructible<Deleter>)
	: Deleter{std::forward<Deleter>(deleter)}
	, m_base{value}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::handle(handle<Base, Empty, Deleter>&& move) noexcept(
	std::is_nothrow_move_constructible_v<Deleter>)
	requires(std::move_constructible<Deleter>)
	: Deleter{std::move(move)}
	, m_base{std::exchange(move.m_base, Empty)}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::~handle<Base, Empty, Deleter>() noexcept
{
	if (m_base != Empty) {
		Deleter::operator()(m_base);
	}
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>& tr::handle<Base, Empty, Deleter>::operator=(handle&& r) noexcept(
	std::is_nothrow_move_assignable_v<Deleter>)
	requires(std::is_move_assignable_v<Deleter>)
{
	if (has_value()) {
		Deleter::operator()(m_base);
	}
	m_base = std::exchange(r.m_base, Empty);
	get_deleter() = std::move(r.get_deleter());
	return *this;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr bool tr::handle<Base, Empty, Deleter>::has_value() const noexcept
{
	return m_base != Empty;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr tr::handle<Base, Empty, Deleter>::operator bool() const noexcept
{
	return has_value();
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr const Base& tr::handle<Base, Empty, Deleter>::get() const noexcept
{
	TR_ASSERT(m_base != Empty, "Cannot get the value of an empty handle.");

	return m_base;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr const Base& tr::handle<Base, Empty, Deleter>::get(maybe_empty_t) const noexcept
{
	return m_base;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr Deleter& tr::handle<Base, Empty, Deleter>::get_deleter() noexcept
{
	return *this;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr const Deleter& tr::handle<Base, Empty, Deleter>::get_deleter() const noexcept
{
	return *this;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr Base tr::handle<Base, Empty, Deleter>::release() noexcept
{
	return std::exchange(m_base, Empty);
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr void tr::handle<Base, Empty, Deleter>::reset() noexcept
{
	if (has_value()) {
		Deleter::operator()(m_base);
	}
	m_base = Empty;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr void tr::handle<Base, Empty, Deleter>::reset(Base value) noexcept
{
	TR_ASSERT(value != Empty, "Cannot reset a handle to a value set aside as the empty value.");

	if (has_value()) {
		Deleter::operator()(m_base);
	}
	m_base = value;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
constexpr void tr::handle<Base, Empty, Deleter>::reset(Base value, maybe_empty_t) noexcept
{
	if (has_value()) {
		Deleter::operator()(m_base);
	}
	m_base = value;
}

//

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter, bool SkipEmptyHandleCheck>
tr::out_handle_t<Base, Empty, Deleter, SkipEmptyHandleCheck>::out_handle_t(handle<Base, Empty, Deleter>& handle) noexcept
	: m_handle{handle}
{
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter, bool SkipEmptyHandleCheck>
tr::out_handle_t<Base, Empty, Deleter, SkipEmptyHandleCheck>::~out_handle_t<Base, Empty, Deleter, SkipEmptyHandleCheck>() noexcept
{
	if constexpr (SkipEmptyHandleCheck) {
		m_handle.reset(m_temporary, maybe_empty);
	}
	else {
		m_handle.reset(m_temporary);
	}
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter, bool SkipEmptyHandleCheck>
tr::out_handle_t<Base, Empty, Deleter, SkipEmptyHandleCheck>::operator Base*() noexcept
{
	return &m_temporary;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
tr::out_handle_t<Base, Empty, Deleter, false> tr::out_handle(handle<Base, Empty, Deleter>& handle) noexcept
{
	return handle;
}

template <std::regular Base, Base Empty, tr::handle_deleter<Base> Deleter>
tr::out_handle_t<Base, Empty, Deleter, true> tr::out_handle(handle<Base, Empty, Deleter>& handle, maybe_empty_t) noexcept
{
	return handle;
}