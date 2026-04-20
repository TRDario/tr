///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides template utilities.                                                                                                          //
//                                                                                                                                       //
// tr::unspecialized<Ts...> is used to circumvent the fact that static_assert(false) is not a valid expression on older compilers and is //
// primarily meant for disabling template specializations.                                                                               //
//     - template <typename... Ts> struct example { static_assert(unspecialized<Ts...>); }                                               //
//       -> causes a compilation error for example<Ts...>                                                                                //
//                                                                                                                                       //
// tr::tag<T> wraps any type to produce a constexpr-constructible tag:                                                                   //
//     - std::variant<tag<Ts>...>{tag<T>{}}.index()                                                                                      //
//       -> Any of 'Ts' could not be constexpr or default-constructible, wrapping them bypasses that and always allows getting the index //
//                                                                                                                                       //
// tr::string_literal is used to pass string literals as template parameters. It can be converted to a char pointer, a string view, or a //
// format string:                                                                                                                        //
//     - template <string_literal String> class example {}; example<"string"> value{};                                                   //
//                                                                                                                                       //
// tr::type_name<T>() is used to get a human-readable name string for a type:                                                            //
//     - tr::type_name<float>() -> "float"                                                                                               //
//                                                                                                                                       //
// tr::size_type determines the smallest unsigned integer type able to store the given value:                                            //
//     - tr::size_type_t<30> -> u8                                                                                                       //
//     - tr::size_type_t<400> -> u16                                                                                                     //
//     - tr::size_type_t<100000> -> u32                                                                                                  //
//                                                                                                                                       //
// The tr::is_specialization_of family is used to determine whether a class is a specialization of a template. Due to limitations in the //
// C++ template system, a perfect solution for all templates does not exist, so a few common patterns are provided:                      //
//     - tr::is_specialization_of<std::vector<int>, std::vector>                                                                         //
//       -> for templates taking one or more type parameters                                                                             //
//     - tr::is_specialization_of_v<tr::static_string<30>, tr::static_string>                                                            //
//       -> for templates taking one or more value parameters                                                                            //
//     - tr::is_specialization_of_vt<tr::static_string<30>, tr::static_string>                                                           //
//       -> for templates taking a value parameter and zero or more type parameters                                                      //
//     - tr::is_specialization_of_tv<std::array<int, 30>, std::array>                                                                    //
//       -> for templates taking a type and value parameter and zero or more trailing type parameters                                    //
//                                                                                                                                       //
// Various information can be extracted from function types, including the return type, class type (for methods), and arugment types:    //
//     - tr::return_type_t<int(int, float)> -> int                                                                                       //
//     - tr::class_type_t<int(my_type::*)(int, float)> -> my_type                                                                        //
//     - tr::args_tuple_t<int(int, float)> -> std::tuple<int, float>                                                                     //
//     - tr::nth_arg_t<1, int(int, float)> -> float                                                                                      //
//                                                                                                                                       //
// tr::invoke_wrapper wraps invocable objects to provide a uniform interface.                                                            //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "integer.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Circumvents static_assert(false) not being a valid expression on older compilers, meant for unspecialized templates.
	template <typename...> inline constexpr bool unspecialized{false};

	// Tag class.
	template <typename> struct tag {};

	// Template for string literals passed as template arguments.
	template <usize Size> struct string_literal {
		// The backing char array.
		char data[Size];

		// Constructs the literal from a string literal.
		consteval string_literal(const char (&str)[Size]);

		// Converts the literal to a C-string.
		consteval operator const char*() const;
		// Converts the literal to a string view.
		consteval operator std::string_view() const;
		// Converts the literal to a format string.
		template <typename... Args> consteval operator TR_FORMAT_STRING<Args...>() const;

		// Gets the length of the literal.
		consteval static usize size();
	};
	// Concatenates string literals.
	template <typename First, typename Second, typename... Rest>
	consteval auto concatenate_string_literals(First&& first, Second&& second, Rest&&... rest);

	// Human-readable type name.
	template <typename T> consteval std::string_view type_name();

	// Gets the type needed to store an integer constant and stores it in ::type.
	template <usize S, typename = void> struct size_type;
	// Gets the type needed to store an integer constant.
	template <usize S> using size_type_t = size_type<S>::type;

	// Gets whether a type is a specialization of a template with type parameters and stores it in ::value.
	template <typename T, template <typename...> typename Template> struct is_specialization_of;
	// Gets whether a type is a specialization of a template with value parameters and stores it in ::value.
	template <typename T, template <auto...> typename Template> struct is_specialization_of_v;
	// Gets whether a type is a specialization of a template with a leading value parameter and stores it in ::value.
	template <typename T, template <auto, typename...> typename Template> struct is_specialization_of_vt;
	// Gets whether a type is a specialization of a template with a leading type and size parameter and stores it in ::value.
	template <typename T, template <typename, auto, typename...> typename Template> struct is_specialization_of_tv;

	// Allows extraction of various properties of function types.
	template <typename Function> struct function_traits;
	// Gets the return type of a function.
	template <typename Function> using return_type_t = function_traits<Function>::return_type;
	// Gets the class of a method type.
	template <typename Function> using class_type_t = function_traits<Function>::return_type;
	// Gets the arguments of a function as a tuple.
	template <typename Function> using args_tuple_t = function_traits<Function>::args_tuple;
	// Gets the type of the N-th argument to a function.
	template <usize N, typename Function> using nth_arg_type_t = function_traits<Function>::template nth_arg_type<N>;

	// Wraps invocables.
	template <typename Function> struct invoke_wrapper;
} // namespace tr

#include "impl/template.hpp" // IWYU pragma: export