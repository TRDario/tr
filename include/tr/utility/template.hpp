/// @file
/// @brief Provides miscellaneous template utilities.

#pragma once
#include "integer.hpp"

//

namespace tr
{
	/// Literal object holding a character array intended to allow passing string literals as non-type template parameters.
	/// @tparam Size Number of characters in the literal + 1 for the NUL-terminator.
	template <usize Size>
	struct string_literal
	{
		/// Backing character array of the string.
		char data[Size];

		//

		/// Constructs a template string literal from a real string literal.
		/// @param[in] str Source string literal to copy.
		consteval string_literal(const char (&str)[Size]);

		//

		/// Gets a C-string pointing to the string literal.
		/// @return C-string pointing to the string literal.
		consteval operator const char*() const;

		/// Gets a string view to the string literal.
		/// @return String view to the string literal.
		consteval operator std::string_view() const;

		/// Gets a format string view to the string literal.
		/// @tparam Args List of arguments to the formatting function.
		/// @return A format string view to the string literal.
		template <typename... Args>
		consteval operator std::format_string<Args...>() const;

		//

		/// Gets the length of the literal.
		/// @return Length of the string literal.
		consteval static usize size();
	};

	/// Concatenates two or more string literals into a new string literal.
	/// @tparam First Raw string literal or a `tr::string_literal` specialization.
	/// @tparam Second Raw string literal or a `tr::string_literal` specialization.
	/// @tparam Rest List of string literals and `tr::string_literal` specializations.
	/// @param first First string to concatenate.
	/// @param second Second string to concatenate.
	/// @param rest Additional strings to concatenate.
	/// @return A new string literal stored in a `tr::string_literal` object.
	template <typename First, typename Second, typename... Rest>
	consteval auto concatenate_string_literals(First&& first, Second&& second, Rest&&... rest);

	//

	/// Gets a human-readable name string for a type.
	/// @note May not return the exact same string on all compilers.
	/// @tparam T Type to get the name of.
	/// @return String view to the name of the type.
	template <typename T>
	consteval std::string_view type_name();

	//

	/// Stores the type that can store an integer in the range [0, `S`] in `::type`.
	/// @tparam S Maximum value that needs to be stored.
	template <usize S>
	struct size_type;

	/// Defines an integer type that can store an integer in the range [0, `S`].
	/// @tparam S Maximum value that needs to be stored.
	template <usize S>
	using size_type_t = size_type<S>::type;

	//

	/// Stores a type that wraps any invocable into a function object with an `operator()` in `::type`.
	/// @tparam Function Built-in function or a function object.
	template <typename Function>
	struct wrapped_invocable;

	/// Defines a type that wraps any invocable into a function object with an `operator()`.
	/// @tparam Function Built-in function or a function object.
	template <typename Function>
	using wrapped_invocable_t = wrapped_invocable<Function>::type;

	/// Wraps functions to use a uniform calling convention (`operator()`).
	/// @tparam Function Built-in function or a function object.
	/// @param fn Function to wrap.
	/// @return Wrapped function object.
	template <typename Function>
	constexpr decltype(auto) wrap_invocable(Function&& fn);
} // namespace tr

#include "impl/template.hpp" // IWYU pragma: export