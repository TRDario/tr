/// @file
/// @brief Implements defer.hpp.

#pragma once
#include "../defer.hpp"

//

template <std::invocable Fn>
	requires(std::is_nothrow_invocable_v<Fn>)
constexpr tr::defer<Fn>::defer(Fn&& fn) noexcept(std::is_nothrow_move_constructible_v<Fn>)
	: m_fn{std::move(fn)}
{
}

template <std::invocable Fn>
	requires(std::is_nothrow_invocable_v<Fn>)
constexpr tr::defer<Fn>::~defer<Fn>() noexcept
{
	m_fn();
}