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

template <typename OutVariant, typename InVariant>
	requires(tr::superset_of<OutVariant, std::remove_cvref_t<InVariant>>)
constexpr OutVariant tr::superset_cast(InVariant&& v)
{
	return std::visit([](auto&& v) { return OutVariant{std::forward<decltype(v)>(v)}; }, v);
}

template <typename OutVariant, typename InVariant>
	requires(tr::subset_of<OutVariant, std::remove_cvref_t<InVariant>>)
constexpr OutVariant tr::subset_cast(InVariant&& v)
{
	return v | tr::match{
				   []<in_variant<OutVariant> T>(T&& v) { return OutVariant{std::forward<T>(v)}; },
				   [](auto&&) -> OutVariant { TR_UNREACHABLE; },
			   };
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

//

template <typename Alternative, typename... Alternatives> tr::opt_ref<Alternative> tr::get_if(std::variant<Alternatives...>& v)
{
	return make_opt_ref(std::get_if<Alternative>(&v));
}

template <typename Alternative, typename... Alternatives> std::optional<Alternative> tr::get_if(std::variant<Alternatives...>&& v)
{
	const tr::opt_ref<Alternative> ref{get_if<Alternative>(v)};
	return ref.has_ref() ? std::make_optional<Alternative>(std::move(*ref)) : std::nullopt;
}

template <typename Alternative, typename... Alternatives> tr::opt_ref<const Alternative> tr::get_if(const std::variant<Alternatives...>& v)
{
	return make_opt_ref(std::get_if<Alternative>(&v));
}

template <typename Alternative, typename... Alternatives> std::optional<Alternative> tr::get_if(const std::variant<Alternatives...>&& v)
{
	const tr::opt_ref<const Alternative> ref{get_if<Alternative>(v)};
	return ref.has_ref() ? std::make_optional<Alternative>(std::move(*ref)) : std::nullopt;
}

////////////////////////////////////////////////////////////////// MATCH //////////////////////////////////////////////////////////////////

namespace tr {
	template <typename... Functions> match(Functions&&...) -> match<Functions...>;
	template <typename State, typename... Functions> stateful_match(State&&, Functions&&...) -> stateful_match<State, Functions...>;
} // namespace tr

template <typename... Functions>
constexpr tr::match<Functions...>::match(Functions&&... fns)
	: tr::wrapped_invocable_t<Functions>{wrap_invocable(std::forward<Functions>(fns))}...
{
}

template <tr::cvref_specialization_of<std::variant> Variant, tr::cvref_specialization_of<tr::match> Match>
constexpr decltype(auto) tr::operator|(Variant&& v, Match&& m)
{
	return std::visit(std::forward<Match>(m), std::forward<Variant>(v));
}

template <typename State, typename... Functions>
constexpr tr::stateful_match<State, Functions...>::stateful_match(State&& state, Functions&&... fns)
	: tr::match<Functions...>{std::forward<Functions>(fns)...}, m_state{std::forward<State>(state)}
{
}

template <typename State, typename... Functions>
template <typename... Args>
constexpr decltype(auto) tr::stateful_match<State, Functions...>::operator()(Args&&... args)
{
	return match<Functions...>::operator()(std::forward<State>(m_state), std::forward<Args>(args)...);
}

template <tr::cvref_specialization_of<std::variant> Variant, tr::cvref_specialization_of<tr::stateful_match> Match>
constexpr decltype(auto) tr::operator|(Variant&& v, Match&& m)
{
	return std::visit(std::forward<Match>(m), std::forward<Variant>(v));
}

////////////////////////////////////////////////////////////// DEFAULT RESULT /////////////////////////////////////////////////////////////

template <typename Value> constexpr auto tr::default_result(Value&& value)
{
	return [v = std::move(value)](auto&&...) mutable -> Value { return std::move(v); };
}