#pragma once
#include "common.hpp"
#include "macro.hpp"

namespace tr {
	// Unchecked variant getter.
	template <class T, class... Args> T& unchecked_get(std::variant<Args...>& v);
	// Unchecked variant getter.
	template <class T, class... Args> T&& unchecked_get(std::variant<Args...>&& v);
	// Unchecked variant getter.
	template <class T, class... Args> const T& unchecked_get(const std::variant<Args...>& v);
	// Unchecked variant getter.
	template <class T, class... Args> const T&& unchecked_get(const std::variant<Args...>&& v);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class T, class... Args> T& tr::unchecked_get(std::variant<Args...>& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());
	return *std::get_if<T>(&v);
}

template <class T, class... Args> T&& tr::unchecked_get(std::variant<Args...>&& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());
	return std::move(*std::get_if<T>(&v));
}

template <class T, class... Args> const T& tr::unchecked_get(const std::variant<Args...>& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());
	return *std::get_if<T>(&v);
}

template <class T, class... Args> const T&& tr::unchecked_get(const std::variant<Args...>&& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());
	return std::move(*std::get_if<T>(&v));
}