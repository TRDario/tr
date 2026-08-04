/// @file
/// @brief Provides variant utilities.

#pragma once
#include "concepts.hpp"
#include "reference.hpp"

//

namespace tr
{
	/// Wraps over `std::get_if`.
	/// @tparam Alternative Alternative to access.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @param v Variant to access an alternative of.
	/// @return Optional reference to the requested alternative.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	constexpr opt_ref<Alternative> get_if(std::variant<Alternatives...>& v);

	/// Wraps over `std::get_if`.
	/// @tparam Alternative Alternative to access.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @param v Variant to access an alternative of.
	/// @return Moved value of the requested alternative.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	constexpr std::optional<Alternative> get_if(std::variant<Alternatives...>&& v);

	/// Wraps over `std::get_if`.
	/// @tparam Alternative Alternative to access.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @param v Variant to access an alternative of.
	/// @return Optional reference to the requested alternative.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	constexpr opt_ref<const Alternative> get_if(const std::variant<Alternatives...>& v);

	/// Wraps over `std::get_if`.
	/// @tparam Alternative Alternative to access.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @param v Variant to access an alternative of.
	/// @return Moved value of the requested alternative.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	constexpr std::optional<const Alternative> get_if(const std::variant<Alternatives...>&& v);

	//

	/// Invokes a function if the variant holds a specific alternative.
	/// @tparam Alternative Desired alternative.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @tparam Fn Invocable that accepts a reference to the alternative.
	/// @param v Variant to act upon.
	/// @param fn Invocable to invoke.
	template <typename Alternative, typename... Alternatives, std::invocable<Alternative&> Fn>
	constexpr void if_is(std::variant<Alternatives...>& v, Fn&& fn);

	/// Invokes a function if the variant holds a specific alternative.
	/// @tparam Alternative Desired alternative.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @tparam Fn Invocable that accepts the value of the alternative.
	/// @param v Variant to act upon.
	/// @param fn Invocable to invoke.
	template <typename Alternative, typename... Alternatives, std::invocable<Alternative> Fn>
	constexpr void if_is(std::variant<Alternatives...>&& v, Fn&& fn);

	/// Invokes a function if the variant holds a specific alternative.
	/// @tparam Alternative Desired alternative.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @tparam Fn Invocable that accepts a reference to the alternative.
	/// @param v Variant to act upon.
	/// @param fn Invocable to invoke.
	template <typename Alternative, typename... Alternatives, std::invocable<const Alternative&> Fn>
	constexpr void if_is(const std::variant<Alternatives...>& v, Fn&& fn);

	/// Invokes a function if the variant holds a specific alternative.
	/// @tparam Alternative Desired alternative.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @tparam Fn Invocable that accepts the value of the alternative.
	/// @param v Variant to act upon.
	/// @param fn Invocable to invoke.
	template <typename Alternative, typename... Alternatives, std::invocable<const Alternative> Fn>
	constexpr void if_is(const std::variant<Alternatives...>&& v, Fn&& fn);
} // namespace tr

#include "impl/variant.hpp" // IWYU pragma: export