/// @file
/// @brief Implements enum.hpp.

#pragma once
#include "../enum.hpp"

//

template <tr::enumerator Enum>
constexpr tr::enum_wrapper<Enum>::enum_wrapper(Enum value)
	: m_value{value}
{
}

//

template <tr::enumerator Enum>
constexpr tr::enum_wrapper<Enum>::operator bool() const
{
	return m_value != Enum{0};
}

//

template <tr::enumerator Enum>
constexpr tr::enum_wrapper<Enum>::operator Enum() const
{
	return m_value;
}

template <tr::enumerator Enum>
constexpr Enum tr::enum_wrapper<Enum>::unwrap() const
{
	return m_value;
}