///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements variant.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../macro.hpp"
#include "../variant.hpp"

//////////////////////////////////////////////////////// IN VARIANT / HOLDS ONE OF ////////////////////////////////////////////////////////

template <typename T, tr::specialization_of<std::variant> Variant> struct tr::type_in_variant : std::false_type {};

template <typename T, typename... Alternatives>
	requires(tr::one_of<T, Alternatives...>)
struct tr::type_in_variant<T, std::variant<Alternatives...>> : std::true_type {};

template <typename... Choices, tr::one_of<Choices...>... Alternatives>
constexpr bool tr::holds_one_of(const std::variant<Alternatives...>& v)
{
	return (std::holds_alternative<Choices>(v) || ...);
}

template <typename... Alternatives> constexpr std::string_view tr::type_name_of_held_alternative(const std::variant<Alternatives...>& v)
{
	constexpr std::array alternative_names{type_name<Alternatives>()...};
	return alternative_names[v.index()];
}

///////////////////////////////////////////////////////////// SUBSET/SUPERSET /////////////////////////////////////////////////////////////

namespace tr {
	template <typename OutVariant, typename InVariant> struct subset_castable_to_helper;
} // namespace tr

template <tr::specialization_of<std::variant> VariantL, tr::specialization_of<std::variant> VariantR>
struct tr::is_superset_of : std::false_type {};

template <typename... AlternativesL, typename... AlternativesR>
	requires(tr::one_of<AlternativesR, AlternativesL...> && ...)
struct tr::is_superset_of<std::variant<AlternativesL...>, std::variant<AlternativesR...>> : std::true_type {};

template <typename... OutAlternatives, typename... InAlternatives>
struct tr::subset_castable_to_helper<std::variant<OutAlternatives...>, std::variant<InAlternatives...>> {
	constexpr bool operator()(const std::variant<InAlternatives...>& v) const
	{
		return holds_one_of<OutAlternatives...>(v);
	}
};

template <typename OutVariant, typename InVariant>
	requires(tr::subset_of<OutVariant, InVariant>)
constexpr bool tr::subset_castable_to(const InVariant& v)
{
	return subset_castable_to_helper<OutVariant, InVariant>{}(v);
}

///////////////////////////////////////////////////////////////// INDICES /////////////////////////////////////////////////////////////////

template <typename... Alternatives> struct tr::variant_indices<std::variant<Alternatives...>> {
	template <typename Alternative> static constexpr std::size_t get{std::variant<tag<Alternatives>...>{tag<Alternative>{}}.index()};
};

/////////////////////////////////////////////////////////////////// GET ///////////////////////////////////////////////////////////////////

template <typename Alternative, typename... Alternatives> Alternative& tr::get(std::variant<Alternatives...>& v)
{
	TR_ASSERT(std::holds_alternative<Alternative>(v), "Tried to access '{}' on a variant holding '{}'.", type_name<Alternative>(),
			  v.index(), type_name_of_held_alternative(v));
	return *std::get_if<Alternative>(&v);
}

template <typename Alternative, typename... Alternatives> Alternative&& tr::get(std::variant<Alternatives...>&& v)
{
	TR_ASSERT(std::holds_alternative<Alternative>(v), "Tried to access '{}' on a variant holding '{}'.", type_name<Alternative>(),
			  v.index(), type_name_of_held_alternative(v));
	return std::move(*std::get_if<Alternative>(&v));
}

template <typename Alternative, typename... Alternatives> const Alternative& tr::get(const std::variant<Alternatives...>& v)
{
	TR_ASSERT(std::holds_alternative<Alternative>(v), "Tried to access '{}' on a variant holding '{}'.", type_name<Alternative>(),
			  v.index(), type_name_of_held_alternative(v));
	return std::move(*std::get_if<Alternative>(&v));
}

template <typename Alternative, typename... Alternatives> const Alternative&& tr::get(const std::variant<Alternatives...>&& v)
{
	TR_ASSERT(std::holds_alternative<Alternative>(v), "Tried to access '{}' on a variant holding '{}'.", type_name<Alternative>(),
			  v.index(), type_name_of_held_alternative(v));
	return std::move(*std::get_if<Alternative>(&v));
}

////////////////////////////////////////////////////////////////// MATCH //////////////////////////////////////////////////////////////////

namespace tr {
	template <typename... Functions> match(Functions...) -> match<Functions...>;
	template <typename State, typename... Functions> stateful_match(State&&, Functions...) -> stateful_match<State, Functions...>;
} // namespace tr

template <tr::cvref_specialization_of<std::variant> Variant, tr::cvref_specialization_of<tr::match> Match>
decltype(auto) tr::operator|(Variant&& v, Match&& m)
{
	return std::visit(std::forward<Match>(m), std::forward<Variant>(v));
}

template <typename State, typename... Functions>
tr::stateful_match<State, Functions...>::stateful_match(State&& state, Functions... fns)
	: tr::match<Functions...>{std::move(fns)...}, m_state{std::forward<State>(state)}
{
}

template <typename State, typename... Functions>
template <typename... Args>
decltype(auto) tr::stateful_match<State, Functions...>::operator()(Args&&... args)
{
	return match<Functions...>::operator()(std::forward<State>(m_state), std::forward<Args>(args)...);
}

template <tr::cvref_specialization_of<std::variant> Variant, tr::cvref_specialization_of<tr::stateful_match> Match>
decltype(auto) tr::operator|(Variant&& v, Match&& m)
{
	return std::visit(std::forward<Match>(m), std::forward<Variant>(v));
}

////////////////////////////////////////////////////////////// DEFAULT RESULT /////////////////////////////////////////////////////////////

template <typename Value> constexpr auto tr::default_result(Value&& value)
{
	return [v = std::move(value)](auto&&...) { return std::move(v); };
}