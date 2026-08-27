/// @file
/// @brief Implements function_traits.hpp.

#pragma once
#include "../function_traits.hpp"

//

namespace tr
{
	/// Specialization of `tr::function_traits` for functors.
	/// @tparam Functor Functor type with an unambiguous `operator()`.
	template <typename Functor>
		requires requires(Functor fn) { &Functor::operator(); }
	struct function_traits<Functor>
	{
		/// Type returned by the functor.
		using return_type = function_traits<decltype(&Functor::operator())>::return_type;

		/// Tuple of types of the arguments to the functor.
		using args_tuple = function_traits<decltype(&Functor::operator())>::args_tuple;

		/// Type of the Nth argument to the functor.
		/// @tparam N Index of the parameter to get the type of.
		template <usize N>
		using nth_arg_type = std::tuple_element_t<N, args_tuple>;
	};

	/// Specialization of `tr::function_traits` for functions.
	/// @tparam Return Type returned by the function.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename... Args>
	struct function_traits<Return(Args...)>
	{
		/// Type returned by the function.
		using return_type = Return;

		/// Tuple of types of the arguments to the function.
		using args_tuple = std::tuple<Args...>;

		/// Type of the Nth argument to the function.
		/// @tparam N Index of the parameter to get the type of.
		template <usize N>
		using nth_arg_type = std::tuple_element_t<N, args_tuple>;
	};

	/// Specialization of `tr::function_traits` for noexcept functions.
	/// @tparam Return Type returned by the function.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename... Args>
	struct function_traits<Return(Args...) noexcept> : function_traits<Return(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for function pointers.
	/// @tparam Return Type returned by the function.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename... Args>
	struct function_traits<Return (*)(Args...)> : function_traits<Return(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for noexcept function pointers.
	/// @tparam Return Type returned by the function.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename... Args>
	struct function_traits<Return (*)(Args...) noexcept> : function_traits<Return(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for function references.
	/// @tparam Return Type returned by the function.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename... Args>
	struct function_traits<Return (&)(Args...)> : function_traits<Return(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for noexcept function references.
	/// @tparam Return Type returned by the function.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename... Args>
	struct function_traits<Return (&)(Args...) noexcept> : function_traits<Return(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for member functions.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...)>
	{
		/// Type returned by the function.
		using return_type = Return;

		/// Class the function belongs to.
		using class_type = Class;

		/// Tuple of types of the arguments to the function.
		using args_tuple = std::tuple<Args...>;

		/// Type of the Nth argument to the function.
		/// @tparam N Index of the parameter to get the type of.
		template <usize N>
		using nth_arg_type = std::tuple_element_t<N, args_tuple>;
	};

	/// Specialization of `tr::function_traits` for noexcept member functions.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) noexcept> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for member functions to `const`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) const> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for noexcept member functions to `const`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) const noexcept> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for member functions to `&`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) &> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for noexcept member functions to `&`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) & noexcept> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for member functions to `const&`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) const&> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for noexcept member functions to `const&`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) const & noexcept> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for member functions to `&&`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) &&> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for noexcept member functions to `&&`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) && noexcept> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for member functions to `const&&`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) const&&> : function_traits<Return (Class::*)(Args...)>
	{
	};

	/// Specialization of `tr::function_traits` for noexcept member functions to `const&&`.
	/// @tparam Return Return type of the function.
	/// @tparam Class Class the function belongs to.
	/// @tparam Args Types of the arguments to the function.
	template <typename Return, typename Class, typename... Args>
	struct function_traits<Return (Class::*)(Args...) const && noexcept> : function_traits<Return (Class::*)(Args...)>
	{
	};
} // namespace tr