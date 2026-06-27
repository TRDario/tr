///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements variant.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../variant.hpp"

////////////////////////////////////////////////////////////////// GET IF /////////////////////////////////////////////////////////////////

template <typename Alternative, typename... Alternatives> tr::opt_ref<Alternative> tr::get_if(std::variant<Alternatives...>& v)
{
	return make_opt_ref(std::get_if<Alternative>(&v));
}

template <typename Alternative, typename... Alternatives> std::optional<Alternative> tr::get_if(std::variant<Alternatives...>&& v)
{
	Alternative* const ptr{std::get_if<Alternative>(&v)};
	return ptr != nullptr ? std::make_optional<Alternative>(std::move(*ptr)) : std::nullopt;
}

template <typename Alternative, typename... Alternatives> tr::opt_ref<const Alternative> tr::get_if(const std::variant<Alternatives...>& v)
{
	return make_opt_ref(std::get_if<Alternative>(&v));
}

template <typename Alternative, typename... Alternatives> std::optional<Alternative> tr::get_if(const std::variant<Alternatives...>&& v)
{
	const Alternative* const ptr{std::get_if<Alternative>(&v)};
	return ptr != nullptr ? std::make_optional<Alternative>(std::move(*ptr)) : std::nullopt;
}

////////////////////////////////////////////////////////////////// IF IS //////////////////////////////////////////////////////////////////

template <typename Alternative, typename... Alternatives, std::invocable<Alternative&> Fn>
constexpr void tr::if_is(std::variant<Alternatives...>& v, Fn&& fn)
{
	tr::opt_ref<Alternative> ref{get_if<Alternative>(v)};
	if (ref.has_ref()) {
		(void)fn(*ref);
	}
}

template <typename Alternative, typename... Alternatives, std::invocable<Alternative> Fn>
constexpr void tr::if_is(std::variant<Alternatives...>&& v, Fn&& fn)
{
	std::optional<Alternative> value{get_if<Alternative>(std::move(v))};
	if (value.has_value()) {
		(void)fn(std::move(*value));
	}
}

template <typename Alternative, typename... Alternatives, std::invocable<const Alternative&> Fn>
constexpr void tr::if_is(const std::variant<Alternatives...>& v, Fn&& fn)
{
	tr::opt_ref<const Alternative> ref{get_if<Alternative>(v)};
	if (ref.has_ref()) {
		(void)fn(*ref);
	}
}

template <typename Alternative, typename... Alternatives, std::invocable<const Alternative> Fn>
constexpr void tr::if_is(const std::variant<Alternatives...>&& v, Fn&& fn)
{
	std::optional<const Alternative> value{get_if<Alternative>(std::move(v))};
	if (value.has_value()) {
		(void)fn(std::move(*value));
	}
}