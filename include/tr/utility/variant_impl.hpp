///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements variant.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "macro.hpp"
#include "variant.hpp"

//////////////////////////////////////////////////////// IN VARIANT / HOLDS ONE OF ////////////////////////////////////////////////////////

template <class T, tr::specialization_of<std::variant> V> struct tr::type_in_variant : std::false_type {};

template <class T, class... Ts>
	requires(tr::one_of<T, Ts...>)
struct tr::type_in_variant<T, std::variant<Ts...>> : std::true_type {};

template <class... Ts, tr::one_of<Ts...>... Us> constexpr bool tr::holds_one_of(const std::variant<Us...>& v)
{
	return (std::holds_alternative<Ts>(v) || ...);
}

///////////////////////////////////////////////////////////// SUBSET/SUPERSET /////////////////////////////////////////////////////////////

namespace tr {
	template <class T, class V> struct subset_castable_to_helper;
} // namespace tr

template <tr::specialization_of<std::variant> T, tr::specialization_of<std::variant> V> struct tr::is_superset_of : std::false_type {};

template <class... Ts, class... Us>
	requires(tr::one_of<Us, Ts...> && ...)
struct tr::is_superset_of<std::variant<Ts...>, std::variant<Us...>> : std::true_type {};

template <class... Ts, class... Us> struct tr::subset_castable_to_helper<std::variant<Ts...>, std::variant<Us...>> {
	constexpr bool operator()(const std::variant<Us...>& v) const
	{
		return holds_one_of<Ts...>(v);
	}
};

template <class T, class V>
	requires(tr::subset_of<T, V>)
constexpr bool tr::subset_castable_to(const V& v)
{
	return subset_castable_to_helper<T, V>{}(v);
}

///////////////////////////////////////////////////////////////// INDICES /////////////////////////////////////////////////////////////////

template <class... Ts> struct tr::variant_indices<std::variant<Ts...>> {
	template <class T> static constexpr std::size_t get{std::variant<tag<Ts>...>{tag<T>{}}.index()};
};

/////////////////////////////////////////////////////////////////// GET ///////////////////////////////////////////////////////////////////

template <class T, class... Args> T& tr::get(std::variant<Args...>& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());
	return *std::get_if<T>(&v);
}

template <class T, class... Args> T&& tr::get(std::variant<Args...>&& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());
	return std::move(*std::get_if<T>(&v));
}

template <class T, class... Args> const T& tr::get(const std::variant<Args...>& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());
	return std::move(*std::get_if<T>(&v));
}

template <class T, class... Args> const T&& tr::get(const std::variant<Args...>&& v)
{
	TR_ASSERT(std::holds_alternative<T>(v), "Tried to access wrong type on a variant holding the type at index {}.", v.index());
	return std::move(*std::get_if<T>(&v));
}

////////////////////////////////////////////////////////////////// MATCH //////////////////////////////////////////////////////////////////

namespace tr {
	template <class... Fs> match(Fs...) -> match<Fs...>;
	template <class State, class... Fs> stateful_match(State&&, Fs&&...) -> stateful_match<State, Fs...>;
} // namespace tr

template <tr::cvref_specialization_of<std::variant> Variant, tr::cvref_specialization_of<tr::match> Match>
decltype(auto) tr::operator|(Variant&& v, Match&& m)
{
	return std::visit(std::forward<Match>(m), std::forward<Variant>(v));
}

template <class State, class... Fs> template <class... Args> decltype(auto) tr::stateful_match<State, Fs...>::operator()(Args&&... args)
{
	return match<Fs...>::operator()(std::move(m_state), std::forward<Args>(args)...);
}

template <tr::cvref_specialization_of<std::variant> Variant, tr::cvref_specialization_of<tr::stateful_match> Match>
decltype(auto) tr::operator|(Variant&& v, Match&& m)
{
	return std::visit(std::forward<Match>(m), std::forward<Variant>(v));
}

////////////////////////////////////////////////////////////// DEFAULT RESULT /////////////////////////////////////////////////////////////

template <class T> constexpr auto tr::default_result(T&& value)
{
	return [v = std::move(value)](auto&&...) { return std::move(v); };
}