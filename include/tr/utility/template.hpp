///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides template utilities.                                                                                                          //
//                                                                                                                                       //
// tr::tag<T> wraps any type to produce a constexpr-constructible tag:                                                                   //
//     - std::variant<tag<Ts>...>{tag<T>{}}.index()                                                                                      //
//       -> Any of 'Ts' could not be constexpr or default-constructible, wrapping them bypasses that and always allows getting the index //
//                                                                                                                                       //
// tr::string_literal is used to pass string literals as template parameters. It can be converted to a char pointer, a string view, or a //
// format string:                                                                                                                        //
//     - template <string_literal String> class example {}; example<"string"> value{};                                                   //
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "integer.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Tag class.
	template <class> struct tag {};

	// Template for string literals passed as template arguments.
	template <usize S> struct string_literal {
		// The backing char array.
		char data[S];

		// Constructs the literal from a string literal.
		consteval string_literal(const char (&str)[S]);

		// Converts the literal to a C-string.
		consteval operator const char*() const;
		// Converts the literal to a string view.
		consteval operator std::string_view() const;
		// Converts the literal to a format string.
		template <class... Args> consteval operator TR_FORMAT_STRING<Args...>() const;
	};

	// Gets the type needed to store an integer constant and stores it in ::type.
	template <usize S, class = void> struct size_type;
	// Gets the type needed to store an integer constant.
	template <usize S> using size_type_t = size_type<S>::type;

	// Gets whether a type is a specialization of a template with type parameters and stores it in ::value.
	template <class T, template <class...> class Z> struct is_specialization_of;
	// Gets whether a type is a specialization of a template with value parameters and stores it in ::value.
	template <class T, template <auto...> class Z> struct is_specialization_of_v;
	// Gets whether a type is a specialization of a template with a leading value parameter and stores it in ::value.
	template <class T, template <auto, class...> class Z> struct is_specialization_of_vt;
	// Gets whether a type is a specialization of a template with a leading type and size parameter and stores it in ::value.
	template <class T, template <class, auto, class...> class Z> struct is_specialization_of_tv;
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <tr::usize S> consteval tr::string_literal<S>::string_literal(const char (&str)[S])
{
	std::ranges::copy(str, data);
}

template <tr::usize S> consteval tr::string_literal<S>::operator const char*() const
{
	return data;
}

template <tr::usize S> consteval tr::string_literal<S>::operator std::string_view() const
{
	return data;
}

template <tr::usize S> template <class... Args> consteval tr::string_literal<S>::operator TR_FORMAT_STRING<Args...>() const
{
	return std::string_view{*this};
}

template <tr::usize S> struct tr::size_type<S, std::enable_if_t<(S > UINT32_MAX)>> {
	using type = u64;
};
template <tr::usize S> struct tr::size_type<S, std::enable_if_t<(S > UINT16_MAX && S <= UINT32_MAX)>> {
	using type = u32;
};
template <tr::usize S> struct tr::size_type<S, std::enable_if_t<(S > UINT8_MAX && S <= UINT16_MAX)>> {
	using type = u16;
};
template <tr::usize S> struct tr::size_type<S, std::enable_if_t<(S <= UINT8_MAX)>> {
	using type = u8;
};

template <class T, template <class...> class Z> struct tr::is_specialization_of : std::false_type {};
template <class... Args, template <class...> class Z> struct tr::is_specialization_of<Z<Args...>, Z> : std::true_type {};

template <class T, template <auto...> class Z> struct tr::is_specialization_of_v : std::false_type {};
template <auto... Values, template <auto...> class Z> struct tr::is_specialization_of_v<Z<Values...>, Z> : std::true_type {};

template <class T, template <auto, class...> class Z> struct tr::is_specialization_of_vt : std::false_type {};
template <auto S, class... Args, template <auto, class...> class Z>
struct tr::is_specialization_of_vt<Z<S, Args...>, Z> : std::true_type {};

template <class T, template <class, auto, class...> class Z> struct tr::is_specialization_of_tv : std::false_type {};
template <class T, auto S, class... Args, template <class, auto, class...> class Z>
struct tr::is_specialization_of_tv<Z<T, S, Args...>, Z> : std::true_type {};