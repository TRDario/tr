/// @file
/// @brief Provides function trait metafunctionality.

#pragma once
#include "integer.hpp"

//

namespace tr
{
	/// Stores properties of function types in `::return_type`, `::class_type`, `::args_tuple`, and `::nth_arg_type<N>`.
	/// @tparam Function Function or functor type with an unambiguous `operator()` definition.
	template <typename Function>
	struct function_traits;

	/// Return type of a function or functor.
	/// @tparam Function Function or functor type with an unambiguous `operator()` definition.
	template <typename Function>
	using return_type_t = function_traits<Function>::return_type;

	/// Class type of a member function.
	/// @tparam Function Member function pointer.
	template <typename Function>
	using class_type_t = function_traits<Function>::class_type;

	/// Tuple of types of the arguments to a function or functor.
	/// @tparam Function Function or functor type with an unambiguous `operator()` definition.
	template <typename Function>
	using args_tuple_t = function_traits<Function>::args_tuple;

	/// Type of the Nth argument to the function or functor.
	/// @tparam N Index of the parameter to get the type of.
	/// @tparam Function Function or functor type with an unambiguous `operator()` definition.
	template <usize N, typename Function>
	using nth_arg_type_t = function_traits<Function>::template nth_arg_type<N>;
} // namespace tr

#include "impl/function_traits.hpp" // IWYU pragma: export