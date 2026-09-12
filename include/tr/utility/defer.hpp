/// @file
/// @brief Provides a defer mechanism.

#pragma once
#include <tr/utility/macro.hpp>

namespace tr
{
	/// RAII deferred function call.
	/// @tparam Fn Deferred function type.
	template <std::invocable Fn>
		requires(std::is_nothrow_invocable_v<Fn>)
	class defer
	{
	  public:
		/// Constructs a deferred function call.
		/// @param fn Function to defer.
		constexpr defer(Fn&& fn) noexcept(std::is_nothrow_move_constructible_v<Fn>)
			: m_fn{std::move(fn)}
		{
		}

		/// Calls the deferred function.
		constexpr ~defer() noexcept
		{
			m_fn();
		}

	  private:
		/// The deferred function.
		Fn m_fn;
	};

	/// Defers a statement to be executed at scope end.
	/// @param ... Statement to defer to scope end.
	/// @hideinitializer
#define TR_DEFER(...)                                                                                                                      \
	[[maybe_unused]] const ::tr::defer TR_JOIN(defer_, __LINE__)                                                                           \
	{                                                                                                                                      \
		[&]() noexcept { __VA_ARGS__; }                                                                                                    \
	}
} // namespace tr