/// @file
/// @brief Implements enum.hpp.

#pragma once
#include "../enum.hpp"

//

template <tr::enumerator Enum>
constexpr tr::enum_wrapper<Enum>::enum_wrapper(Enum value) noexcept
	: m_value{value}
{
}

//

template <tr::enumerator Enum>
constexpr tr::enum_wrapper<Enum>::operator bool() const noexcept
{
	return m_value != Enum{0};
}

//

template <tr::enumerator Enum>
constexpr tr::enum_wrapper<Enum>::operator Enum() const noexcept
{
	return m_value;
}

template <tr::enumerator Enum>
constexpr Enum tr::enum_wrapper<Enum>::unwrap() const noexcept
{
	return m_value;
}