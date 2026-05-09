///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements optional.hpp.                                                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../optional.hpp"
#include "../reference.hpp"

///////////////////////////////////////////////////////////////// OPTIONAL ////////////////////////////////////////////////////////////////

template <typename T> struct tr::add_optional {
	using type = std::optional<T>;
};
template <typename T> struct tr::add_optional<T&> {
	using type = opt_ref<T>;
};
template <typename T> struct tr::add_optional<T&&> {
	using type = std::optional<T>;
};

//

template <typename In, std::invocable<In&> Fn>
constexpr tr::add_optional_t<std::invoke_result_t<Fn, In&>> tr::transform(std::optional<In>& opt, Fn&& fn)
{
	return opt.has_value() ? add_optional_t<std::invoke_result_t<Fn, In&>>{std::invoke(std::forward<Fn>(fn), *opt)} : std::nullopt;
}

template <typename In, std::invocable<const In&> Fn>
constexpr tr::add_optional_t<std::invoke_result_t<Fn, const In&>> tr::transform(const std::optional<In>& opt, Fn&& fn)
{
	return opt.has_value() ? add_optional_t<std::invoke_result_t<Fn, const In&>>{std::invoke(std::forward<Fn>(fn), *opt)} : std::nullopt;
}

template <typename In, std::invocable<In> Fn>
constexpr tr::add_optional_t<std::invoke_result_t<Fn, In>> tr::transform(std::optional<In>&& opt, Fn&& fn)
{
	return opt.has_value() ? add_optional_t<std::invoke_result_t<Fn, In>>{std::invoke(std::forward<Fn>(fn), std::move(*opt))}
						   : std::nullopt;
}

template <typename In, std::invocable<const In> Fn>
constexpr tr::add_optional_t<std::invoke_result_t<Fn, const In>> tr::transform(const std::optional<In>&& opt, Fn&& fn)
{
	return opt.has_value() ? add_optional_t<std::invoke_result_t<Fn, const In>>{std::invoke(std::forward<Fn>(fn), std::move(*opt))}
						   : std::nullopt;
}

//

template <typename In, std::invocable<In&> Fn>
	requires(tr::specialization_of<std::invoke_result_t<Fn, In&>, std::optional>)
constexpr std::invoke_result_t<Fn, In&> tr::and_then(std::optional<In>& opt, Fn&& fn)
{
	return opt.has_value() ? std::invoke(std::forward<Fn>(fn), *opt) : std::nullopt;
}

template <typename In, std::invocable<const In&> Fn>
	requires(tr::specialization_of<std::invoke_result_t<Fn, const In&>, std::optional>)
constexpr std::invoke_result_t<Fn, const In&> tr::and_then(const std::optional<In>& opt, Fn&& fn)
{
	return opt.has_value() ? std::invoke(std::forward<Fn>(fn), *opt) : std::nullopt;
}

template <typename In, std::invocable<In> Fn>
	requires(tr::specialization_of<std::invoke_result_t<Fn, In>, std::optional>)
constexpr std::invoke_result_t<Fn, In> tr::and_then(std::optional<In>&& opt, Fn&& fn)
{
	return opt.has_value() ? std::invoke(std::forward<Fn>(fn), std::move(*opt)) : std::nullopt;
}

template <typename In, std::invocable<const In> Fn>
	requires(tr::specialization_of<std::invoke_result_t<Fn, const In>, std::optional>)
constexpr std::invoke_result_t<Fn, const In> tr::and_then(const std::optional<In>&& opt, Fn&& fn)
{
	return opt.has_value() ? std::invoke(std::forward<Fn>(fn), std::move(*opt)) : std::nullopt;
}

//

template <typename Element, std::invocable Fn>
	requires(std::same_as<std::invoke_result_t<Fn>, std::optional<Element>>)
constexpr std::optional<Element> tr::or_else(const std::optional<Element>& opt, Fn&& fn)
{
	return opt.has_value() ? opt : std::forward<Fn>(fn)();
}

template <typename Element, std::invocable Fn>
	requires(std::same_as<std::invoke_result_t<Fn>, std::optional<Element>>)
constexpr std::optional<Element> tr::or_else(std::optional<Element>&& opt, Fn&& fn)
{
	return opt.has_value() ? std::move(opt) : std::forward<Fn>(fn)();
}