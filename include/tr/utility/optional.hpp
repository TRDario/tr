#pragma once
#include "concepts.hpp"

namespace tr {
	// Tries to invoke an operation on an optional object if it's non-empty.
	template <class T, std::invocable<T> Fn> void try_invoke(std::optional<T>& opt, Fn&& fn);
	// Tries to invoke an operation on an optional object if it's non-empty.
	template <class T, std::invocable<const T> Fn> void try_invoke(const std::optional<T>& opt, Fn&& fn);
	// Tries to invoke an operation on an optional object if it's non-empty and returns a value.
	template <class T, class V, std::invocable<T> Fn>
		requires(std::same_as<V, std::invoke_result_t<Fn, T&>>)
	V try_invoke_or(std::optional<T>& opt, Fn&& fn, V fallback);
	// Tries to invoke an operation on an optional object if it's non-empty and returns a value.
	template <class T, class V, std::invocable<const T> Fn>
		requires(std::same_as<V, std::invoke_result_t<Fn, const T&>>)
	V try_invoke_or(const std::optional<T>& opt, Fn&& fn, V fallback);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class T, std::invocable<T> Fn> void tr::try_invoke(std::optional<T>& opt, Fn&& fn)
{
	if (opt.has_value()) {
		std::invoke(std::forward<Fn>(fn), *opt);
	}
}

template <class T, std::invocable<const T> Fn> void tr::try_invoke(const std::optional<T>& opt, Fn&& fn)
{
	if (opt.has_value()) {
		std::invoke(std::forward<Fn>(fn), *opt);
	}
}

template <class T, class V, std::invocable<T> Fn>
	requires(std::same_as<V, std::invoke_result_t<Fn, T&>>)
V tr::try_invoke_or(std::optional<T>& opt, Fn&& fn, V fallback)
{
	return opt.has_value() ? std::invoke(std::forward<Fn>(fn), *opt) : std::move(fallback);
}

template <class T, class V, std::invocable<const T> Fn>
	requires(std::same_as<V, std::invoke_result_t<Fn, const T&>>)
V tr::try_invoke_or(const std::optional<T>& opt, Fn&& fn, V fallback)
{
	return opt.has_value() ? std::invoke(std::forward<Fn>(fn), *opt) : std::move(fallback);
}