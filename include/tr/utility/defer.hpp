///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a defer mechanism using RAII.                                                                                                //
//                                                                                                                                       //
// A deferred function can be constructed using tr::defer, or through the TR_DEFER macro if you wish to avoid boilerplate:               //
//     - tr::defer _{[&]{ std::fclose(file); }} -> defers a file close to the end of the scope                                           //
//     - TR_DEFER(std::fclose(file)) -> equivalent to the above                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "macro.hpp"

namespace tr {
	// RAII deferred function call.
	template <std::invocable Fn> class defer {
	  public:
		// Consturcts a deferred function call.
		defer(Fn&& fn);
		// Calls the deferred function.
		~defer();

	  private:
		// The deferred function.
		Fn m_fn;
	};

	// Defers a statement to be executed at scope end.
#define TR_DEFER(...)                                                                                                                      \
	[[maybe_unused]] const ::tr::defer TR_JOIN(defer_, __LINE__)                                                                           \
	{                                                                                                                                      \
		[&] { __VA_ARGS__; }                                                                                                               \
	}
} // namespace tr

#include "impl/defer.hpp" // IWYU pragma: export