/// @file
/// @brief Provides variant utilities.

#pragma once
#include <tr/utility/concepts.hpp>
#include <tr/utility/opt_ref.hpp>

//

namespace tr
{
	/// @name Variant operations
	/// @{

	/// Wraps over `std::get_if`.
	/// @tparam Alternative Alternative to access.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @param v Variant to access an alternative of.
	/// @return Optional reference to the requested alternative.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	[[nodiscard]] constexpr opt_ref<Alternative> get_if(std::variant<Alternatives...>& v) noexcept
	{
		return make_opt_ref(std::get_if<Alternative>(&v));
	}

	/// Wraps over `std::get_if`.
	/// @tparam Alternative Alternative to access.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @param v Variant to access an alternative of.
	/// @return Moved value of the requested alternative.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	[[nodiscard]] constexpr std::optional<Alternative> get_if(std::variant<Alternatives...>&& v) noexcept
	{
		Alternative* const ptr{std::get_if<Alternative>(&v)};
		return ptr != nullptr ? std::make_optional<Alternative>(std::move(*ptr)) : std::nullopt;
	}

	/// Wraps over `std::get_if`.
	/// @tparam Alternative Alternative to access.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @param v Variant to access an alternative of.
	/// @return Optional reference to the requested alternative.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	[[nodiscard]] constexpr opt_ref<const Alternative> get_if(const std::variant<Alternatives...>& v) noexcept
	{
		return make_opt_ref(std::get_if<Alternative>(&v));
	}

	/// Wraps over `std::get_if`.
	/// @tparam Alternative Alternative to access.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @param v Variant to access an alternative of.
	/// @return Moved value of the requested alternative.
	template <typename Alternative, typename... Alternatives>
		requires(one_of<Alternative, Alternatives...>)
	[[nodiscard]] constexpr std::optional<const Alternative> get_if(const std::variant<Alternatives...>&& v) noexcept
	{
		const Alternative* const ptr{std::get_if<Alternative>(&v)};
		return ptr != nullptr ? std::make_optional<const Alternative>(std::move(*ptr)) : std::nullopt;
	}

	//

	/// Invokes a function if the variant holds a specific alternative.
	/// @tparam Alternative Desired alternative.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @tparam Fn Invocable that accepts a reference to the alternative.
	/// @param v Variant to act upon.
	/// @param fn Invocable to invoke.
	template <typename Alternative, typename... Alternatives, std::invocable<Alternative&> Fn>
	constexpr void if_is(std::variant<Alternatives...>& v, Fn&& fn) noexcept(noexcept(fn(std::declval<Alternative&>())))
	{
		tr::opt_ref<Alternative> ref{get_if<Alternative>(v)};
		if (ref.has_ref()) {
			(void)fn(*ref);
		}
	}

	/// Invokes a function if the variant holds a specific alternative.
	/// @tparam Alternative Desired alternative.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @tparam Fn Invocable that accepts the value of the alternative.
	/// @param v Variant to act upon.
	/// @param fn Invocable to invoke.
	template <typename Alternative, typename... Alternatives, std::invocable<Alternative> Fn>
	constexpr void if_is(std::variant<Alternatives...>&& v, Fn&& fn) noexcept(noexcept(fn(std::declval<const Alternative&>())))
	{
		std::optional<Alternative> value{get_if<Alternative>(std::move(v))};
		if (value.has_value()) {
			(void)fn(std::move(*value));
		}
	}

	/// Invokes a function if the variant holds a specific alternative.
	/// @tparam Alternative Desired alternative.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @tparam Fn Invocable that accepts a reference to the alternative.
	/// @param v Variant to act upon.
	/// @param fn Invocable to invoke.
	template <typename Alternative, typename... Alternatives, std::invocable<const Alternative&> Fn>
	constexpr void if_is(const std::variant<Alternatives...>& v, Fn&& fn) noexcept(noexcept(fn(std::declval<Alternative>())))
	{
		tr::opt_ref<const Alternative> ref{get_if<Alternative>(v)};
		if (ref.has_ref()) {
			(void)fn(*ref);
		}
	}

	/// Invokes a function if the variant holds a specific alternative.
	/// @tparam Alternative Desired alternative.
	/// @tparam Alternatives List of alternatives in the variant.
	/// @tparam Fn Invocable that accepts the value of the alternative.
	/// @param v Variant to act upon.
	/// @param fn Invocable to invoke.
	template <typename Alternative, typename... Alternatives, std::invocable<const Alternative> Fn>
	constexpr void if_is(const std::variant<Alternatives...>&& v, Fn&& fn) noexcept(noexcept(fn(std::declval<const Alternative>())))
	{
		std::optional<const Alternative> value{get_if<Alternative>(std::move(v))};
		if (value.has_value()) {
			(void)fn(std::move(*value));
		}
	}

	/// @}
} // namespace tr