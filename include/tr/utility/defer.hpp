/// @file
/// @brief Provides a defer mechanism.

#pragma once
#include "macro.hpp"

namespace tr
{
	/// RAII deferred function call.
	/// @tparam Fn Deferred function type.
	template <std::invocable Fn>
	class defer
	{
	  public:
		/// Constructs a deferred function call.
		/// @param fn Function to defer.
		constexpr defer(Fn&& fn);

		/// Calls the deferred function.
		constexpr ~defer();

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
		[&] { __VA_ARGS__; }                                                                                                               \
	}
} // namespace tr

#include "impl/defer.hpp" // IWYU pragma: export