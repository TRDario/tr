#pragma once
#include "concepts.hpp"
#include "macro.hpp"

namespace tr {
	template <class T, class V> struct type_in_variant : std::false_type {};
	template <class T, class... Ts>
		requires(one_of<T, Ts...>)
	struct type_in_variant<T, std::variant<Ts...>> : std::true_type {};
	// Determines whether a type is in a variant's type list.
	template <class T, class V>
	concept in_variant = type_in_variant<T, V>::value;

	template <class T>
		requires(specialization_of<T, std::variant>)
	struct tag_variant {};
	template <class... Ts> struct tag_variant<std::variant<Ts...>> {
		using type = std::variant<tag<Ts>...>;
	};
	template <class T> using tag_variant_t = tag_variant<T>::type;

	// Gets the index of a type in a variant.
	template <class T, class V>
		requires(in_variant<T, V>)
	constexpr std::size_t type_index{tag_variant_t<V>(tag<T>{}).index()};

	// Unchecked variant getter.
	template <class T, class... Ts> T& get(std::variant<Ts...>& v);
	// Unchecked variant getter.
	template <class T, class... Ts> T&& get(std::variant<Ts...>&& v);
	// Unchecked variant getter.
	template <class T, class... Ts> const T& get(const std::variant<Ts...>& v);
	// Unchecked variant getter.
	template <class T, class... Ts> const T&& get(const std::variant<Ts...>&& v);

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

template <class T, class... Args> T& tr::get(std::variant<Args...>& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());

	T* const ptr{std::get_if<T>(&v)};
	TR_ASSUME(ptr != nullptr);
	return *ptr;
}

template <class T, class... Args> T&& tr::get(std::variant<Args...>&& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());

	T* const ptr{std::get_if<T>(&v)};
	TR_ASSUME(ptr != nullptr);
	return std::move(*ptr);
}

template <class T, class... Args> const T& tr::get(const std::variant<Args...>& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());

	T* const ptr{std::get_if<T>(&v)};
	TR_ASSUME(ptr != nullptr);
	return std::move(*ptr);
}

template <class T, class... Args> const T&& tr::get(const std::variant<Args...>&& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());

	const T* const ptr{std::get_if<T>(&v)};
	TR_ASSUME(ptr != nullptr);
	return std::move(*ptr);
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