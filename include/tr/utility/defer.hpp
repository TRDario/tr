/// @file
/// @brief Provides a defer mechanism.

#pragma once
#include <tr/utility/macro.hpp>

namespace tr
{
	/// @ingroup meta
	/// @brief RAII deferred function call.
	/// @details To avoid manually dealing with declaring a defer variable, use `TR_DEFER()`.
	/// @tparam Fn Deferred function type.
	template <std::invocable Fn>
		requires std::is_nothrow_invocable_v<Fn>
	class defer
	{
	  public:
		/// @brief Constructs a deferred function call.
		/// @param fn Function to defer.
		constexpr defer(Fn&& fn) noexcept(std::is_nothrow_move_constructible_v<Fn>)
			: m_fn{std::move(fn)}
		{
		}

		/// @brief Calls the deferred function.
		constexpr ~defer() noexcept
		{
			m_fn();
		}

	  private:
		/// The deferred function.
		Fn m_fn;
	};

	/// @ingroup meta
	/// @brief Defers a statement to be executed at scope end.
	/// @param ... Statement to defer to scope end.
	/// @note The statement should not be able to throw, as otherwise the application may be terminated.
	/// @hideinitializer
#define TR_DEFER(...)                                                                                                                      \
	[[maybe_unused]] const ::tr::defer TR_JOIN(defer_, __LINE__)                                                                           \
	{                                                                                                                                      \
		[&]() noexcept { __VA_ARGS__; }                                                                                                    \
	}
} // namespace tr