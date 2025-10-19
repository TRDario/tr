#pragma once
#include "concepts.hpp"

namespace tr {
	// Tries to invoke an operation on an optional object if it's non-empty.
	template <class T, class... Args, std::invocable<T&, Args...> Fn> void try_invoke(std::optional<T>& opt, Fn&& fn, Args&&... args);
	// Tries to invoke an operation on an optional object if it's non-empty.
	template <class T, class... Args, std::invocable<const T&, Args...> Fn>
	void try_invoke(const std::optional<T>& opt, Fn&& fn, Args&&... args);
	// Tries to invoke an operation on an optional object if it's non-empty and returns a value.
	template <class T, class... Args, class V, std::invocable<T> Fn>
		requires(std::same_as<V, std::invoke_result_t<Fn, T&>>)
	V invoke_or(std::optional<T>& opt, Fn&& fn, V fallback, Args&&... args);
	// Tries to invoke an operation on an optional object if it's non-empty and returns a value.
	template <class T, class... Args, class V, std::invocable<const T> Fn>
		requires(std::same_as<V, std::invoke_result_t<Fn, const T&>>)
	V invoke_or(const std::optional<T>& opt, Fn&& fn, V fallback, Args&&... args);
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class T, class... Args, std::invocable<T&, Args...> Fn> void tr::try_invoke(std::optional<T>& opt, Fn&& fn, Args&&... args)
{
	if (opt.has_value()) {
		std::invoke(std::forward<Fn>(fn), *opt, std::forward<Args>(args)...);
	}
}

template <class T, class... Args, std::invocable<const T&, Args...> Fn>
void tr::try_invoke(const std::optional<T>& opt, Fn&& fn, Args&&... args)
{
	if (opt.has_value()) {
		std::invoke(std::forward<Fn>(fn), *opt, std::forward<Args>(args)...);
	}
}

template <class T, class... Args, class V, std::invocable<T> Fn>
	requires(std::same_as<V, std::invoke_result_t<Fn, T&>>)
V tr::invoke_or(std::optional<T>& opt, Fn&& fn, V fallback, Args&&... args)
{
	return opt.has_value() ? std::invoke(std::forward<Fn>(fn), *opt, std::forward<Args>(args)...) : std::move(fallback);
}

template <class T, class... Args, class V, std::invocable<const T> Fn>
	requires(std::same_as<V, std::invoke_result_t<Fn, const T&>>)
V tr::invoke_or(const std::optional<T>& opt, Fn&& fn, V fallback, Args&&... args)
{
	return opt.has_value() ? std::invoke(std::forward<Fn>(fn), *opt, std::forward<Args>(args)...) : std::move(fallback);
}