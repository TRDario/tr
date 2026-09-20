/// @file
/// @brief Provides utilities relating to applying tuples.

#pragma once
#include <tr/utility/integer.hpp>

//

namespace tr
{
#ifdef TR_DOXYGEN
	/// Type trait checking whether a functor of type `T` can be applied on an object of type `TupleLike`.
	/// @tparam TupleLike Tuple-like argument type.
	template <typename T, typename TupleLike>
	struct is_applicable
	{
		/// Holds whether a functor of type `T` can be applied on an object of type `TupleLike`.
		static constexpr bool value;
	};
#else
	namespace internal
	{
		/// Type trait checking whether a functor of type `T` can be applied on an object of type `TupleLike`.
		/// @tparam TupleLike Tuple-like argument type.
		/// @tparam IndexSequence Instance of `std::index_sequence` holding the indices of the tuple-like type.
		/// @details This is the basal implementation for non-tuple-like second arguments.
		template <typename T, typename TupleLike, typename IndexSequence>
		struct is_applicable : std::false_type
		{
		};

		/// Type trait checking whether a functor of type `T` can be applied on an object of type `TupleLike`.
		/// @tparam TupleLike Tuple-like argument type.
		/// @tparam Indices Indices of the tuple-like type.
		/// @details This is the implementation for tuple-like second arguments.
		template <typename T, typename TupleLike, usize... Indices>
			requires(requires { typename std::tuple_element_t<Indices, TupleLike>; } && ...)
		struct is_applicable<T, TupleLike, std::index_sequence<Indices...>>
			: std::is_invocable<T, std::tuple_element_t<Indices, TupleLike>...>
		{
		};
	} // namespace internal

	/// Type trait checking whether a functor of type `T` can be applied on an object of type `TupleLike`.
	/// @tparam TupleLike Tuple-like argument type.
	/// @details This is the basal implementation for non-tuple-like second arguments.
	template <typename T, typename TupleLike>
	struct is_applicable : std::false_type
	{
	};

	/// Specialization of `is_applicable` for a tuple-like second argument.
	/// @tparam TupleLike Tuple-like argument type.
	/// @details This is the implementation for tuple-like second arguments.
	template <typename T, typename TupleLike>
		requires(requires { std::tuple_size<TupleLike>::value; })
	struct is_applicable<T, TupleLike> : internal::is_applicable<T, TupleLike, std::make_index_sequence<std::tuple_size_v<TupleLike>>>
	{
	};
#endif

	/// Checks whether a functor of type `T` can be applied on an object of type `TupleLike`.
	/// @tparam TupleLike Tuple-like argument type.
	template <typename T, typename TupleLike>
	inline constexpr bool is_applicable_v{is_applicable<T, TupleLike>::value};

	/// Concept denoting a functor that can be applied on tuple-like objects of a specific type.
	/// @tparam TupleLike Tuple-like argument type.
	template <typename T, typename TupleLike>
	concept applicable = is_applicable_v<T, TupleLike>;

	//

#ifdef TR_DOXYGEN
	/// Type trait deducing the result type of applying a callable object with a tuple of arguments.
	/// @tparam Callable Callable object type.
	/// @tparam TupleLike Tuple-like argument type.
	template <typename T, typename TupleLike>
		requires applicable<T, TupleLike>
	struct apply_result<T, TupleLike>
	{
		/// Deduced result type of applying a callable object with a tuple of arguments.
		/// @hideinitializer
		using type = void;
	};
#else
	namespace internal
	{
		/// Type trait deducing the result type of applying a callable object with a tuple of arguments.
		/// @tparam Callable Callable object type.
		/// @tparam TupleLike Tuple-like argument type.
		/// @tparam IndexSequence Instance of `std::index_sequence` holding the indices of the tuple-like type.
		/// @details This is a stub for invalid invocations of the type trait.
		template <typename Callable, typename TupleLike, typename IndexSequence>
		struct apply_result
		{
			static_assert(false, "Arguments do not fulfill the requirement 'tr::applicable<Callable, TupleLike>'.");
		};

		/// Type trait deducing the result type of applying a callable object with a tuple of arguments.
		/// @tparam Callable Callable object type.
		/// @tparam TupleLike Tuple-like argument type.
		/// @tparam Indices Indices of the tuple-like type.
		/// @details Assumes `tr::applicable<Callable, TupleLike>` holds.
		template <typename Callable, typename TupleLike, usize... Indices>
		struct apply_result<Callable, TupleLike, std::index_sequence<Indices...>>
			: std::invoke_result<Callable, std::tuple_element_t<Indices, TupleLike>...>
		{
		};
	} // namespace internal

	/// Type trait deducing the result type of applying a callable object with a tuple of arguments.
	/// @tparam Callable Callable object type.
	/// @tparam TupleLike Tuple-like argument type.
	/// @details This is a stub for invalid invocations of the type trait.
	template <typename Callable, typename TupleLike>
	struct apply_result
	{
		static_assert(false, "Arguments do not fulfill the requirement 'tr::applicable<Callable, TupleLike>'.");
	};

	/// Type trait deducing the result type of applying a callable object with a tuple of arguments.
	/// @tparam Callable Callable object type.
	/// @tparam TupleLike Tuple-like argument type.
	template <typename T, typename TupleLike>
		requires applicable<T, TupleLike>
	struct apply_result<T, TupleLike> : internal::apply_result<T, TupleLike, std::make_index_sequence<std::tuple_size_v<TupleLike>>>
	{
	};
#endif

	/// Deduced result type of applying a callable object with a tuple of arguments.
	/// @tparam T Function being invoked.
	/// @tparam Tuple Tuple holding the argument types.
	template <typename Callable, typename TupleLike>
	using apply_result_t = apply_result<Callable, TupleLike>::type;
} // namespace tr