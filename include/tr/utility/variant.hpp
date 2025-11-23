#pragma once
#include "concepts.hpp"
#include "macro.hpp"

namespace tr {
	template <class T, class V> struct type_in_variant : std::false_type {};
	template <class T, class... Ts>
		requires(one_of<std::remove_cvref_t<T>, Ts...>)
	struct type_in_variant<T, std::variant<Ts...>> : std::true_type {};
	// Determines whether a type is in a variant's type list.
	template <class T, class V>
	concept in_variant = type_in_variant<T, std::remove_cvref_t<V>>::value;

	template <class T, class V> struct is_superset_of : std::false_type {};
	template <class... Ts, class... Us>
		requires(one_of<Us, Ts...> && ...)
	struct is_superset_of<std::variant<Ts...>, std::variant<Us...>> : std::true_type {};
	// Determines whether a variant type is a superset of another variant type.
	template <class T, class V>
	concept superset_of = is_superset_of<std::remove_cvref_t<T>, std::remove_cvref_t<V>>::value;
	// Determines whether a variant type is a subset of another variant type.
	template <class T, class V>
	concept subset_of = superset_of<V, T>;

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
	constexpr std::size_t type_index{tag_variant_t<V>(tag<std::remove_cvref_t<T>>{}).index()};

	template <class T, class V> struct subset_castable_to_t {};
	template <class... Ts, class... Us>
		requires(subset_of<std::variant<Ts...>, std::variant<Us...>>)
	struct subset_castable_to_t<std::variant<Ts...>, std::variant<Us...>> {
		constexpr bool operator()(const std::variant<Us...>& v) const;
	};
	// Gets whether a variant is holding one of a number of types.
	template <class... Ts, class... Us> constexpr bool holds_one_of(const std::variant<Us...>& v);
	// Gets whether a variant can be casted using subset_cast to another type.
	template <class T, class V>
		requires(subset_of<T, V>)
	constexpr bool subset_castable_to(const V& v);

	// Unchecked variant getter.
	template <class T, class... Ts> T& get(std::variant<Ts...>& v);
	// Unchecked variant getter.
	template <class T, class... Ts> T&& get(std::variant<Ts...>&& v);
	// Unchecked variant getter.
	template <class T, class... Ts> const T& get(const std::variant<Ts...>& v);
	// Unchecked variant getter.
	template <class T, class... Ts> const T&& get(const std::variant<Ts...>&& v);

	// Casts a variant into another variant that is a superset of it.
	template <class To, class From>
		requires(superset_of<To, From>)
	constexpr To superset_cast(From&& v);
	// Casts a variant into another variant that is a subset of it.
	template <class To, class From>
		requires(subset_of<To, From>)
	constexpr To subset_cast(From&& v);

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

template <class... Ts, class... Us> constexpr bool tr::holds_one_of(const std::variant<Us...>& v)
{
	return (std::holds_alternative<Ts>(v) || ...);
}

template <class... Ts, class... Us>
	requires(tr::subset_of<std::variant<Ts...>, std::variant<Us...>>)
constexpr bool tr::subset_castable_to_t<std::variant<Ts...>, std::variant<Us...>>::operator()(const std::variant<Us...>& v) const
{
	return holds_one_of<Ts...>(v);
}

template <class T, class V>
	requires(tr::subset_of<T, V>)
constexpr bool tr::subset_castable_to(const V& v)
{
	return subset_castable_to_t<T, V>{}(v);
}

//

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

	const T* const ptr{std::get_if<T>(&v)};
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

template <class To, class From>
	requires(tr::superset_of<To, From>)
constexpr To tr::superset_cast(From&& v)
{
	return std::visit([]<class T>(T&& v) { return To{std::forward<T>(v)}; }, std::forward<From>(v));
}

template <class To, class From>
	requires(tr::subset_of<To, From>)
constexpr To tr::subset_cast(From&& v)
{
	TR_ASSERT(subset_castable_to<To>(v), "Tried to subset_cast a variant holding a value of a type not in the subset.");

	return std::visit(tr::match{[]<in_variant<To> T>(T&& v) { return To{std::forward<T>(v)}; }, [](auto&&) -> To { unreachable(); }},
					  std::forward<From>(v));
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