#pragma once
#include "common.hpp"
#include "macro.hpp"

namespace tr {
	// Unchecked variant getter.
	template <class T, class... Ts> T& unchecked_get(std::variant<Ts...>& v);
	// Unchecked variant getter.
	template <class T, class... Ts> T&& unchecked_get(std::variant<Ts...>&& v);
	// Unchecked variant getter.
	template <class T, class... Ts> const T& unchecked_get(const std::variant<Ts...>& v);
	// Unchecked variant getter.
	template <class T, class... Ts> const T&& unchecked_get(const std::variant<Ts...>&& v);

	// Casts a variant into another variant that is a superset of it.
	template <class To, class From> To superset_cast(From&& v);

	// Match statement helper class.
	template <class... Fs> struct match : Fs... {
		using Fs::operator()...;
	};
	// Deduction guide.
	template <class... Fs> match(Fs...) -> match<Fs...>;

	// Match statement.
	template <class... Ts, class... Fs> auto operator|(std::variant<Ts...>& v, match<Fs...>&& match);
	// Match statement.
	template <class... Ts, class... Fs> auto operator|(std::variant<Ts...>&& v, match<Fs...>&& match);
	// Match statement.
	template <class... Ts, class... Fs> auto operator|(const std::variant<Ts...>& v, match<Fs...>&& match);
	// Match statement.
	template <class... Ts, class... Fs> auto operator|(const std::variant<Ts...>&& v, match<Fs...>&& match);
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

//

template <class To, class From> To tr::superset_cast(From&& v)
{
	return std::visit([]<class T>(T&& v) { return To{std::forward<T>(v)}; }, std::forward<From>(v));
}

//

template <class... Ts, class... Fs> auto tr::operator|(std::variant<Ts...>& v, match<Fs...>&& match)
{
	return std::visit(std::move(match), v);
}

template <class... Ts, class... Fs> auto tr::operator|(std::variant<Ts...>&& v, match<Fs...>&& match)
{
	return std::visit(std::move(match), std::move(v));
}

template <class... Ts, class... Fs> auto tr::operator|(const std::variant<Ts...>& v, match<Fs...>&& match)
{
	return std::visit(std::move(match), v);
}

template <class... Ts, class... Fs> auto tr::operator|(const std::variant<Ts...>&& v, match<Fs...>&& match)
{
	return std::visit(std::move(match), std::move(v));
}