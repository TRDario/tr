///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides miscellaneous macros.                                                                                                        //
//                                                                                                                                       //
// tr::unreachable is a backport of C++23 std::unreachable and marks a code segment as unreachable:                                      //
//     -  return x > 0 ? 1 : x < 0 ? -1 : tr::unreachable() -> Assume that x cannot possibly be 0 in this context.                       //
//                                                                                                                                       //
// TR_ASSUME(condition) hints to the compiler that the condition is assumed and can be optimized for:                                    //
//     - TR_ASSUME(ptr != nullptr) -> Assume that the point is not null.                                                                 //
//                                                                                                                                       //
// TR_STRINGIFY(x) expands into the string representation of x:                                                                          //
//     - TR_STRINGIFY(a < 9) -> "a < 9"                                                                                                  //
//                                                                                                                                       //
// TR_MACRO_COMMA_GUARD(...) is used to circumvent macros taking expressions with macros as multiple arguments:                          //
//     - MY_MACRO(std::array<int, 4>) -> arguments are 'std::array<int', '4>'                                                            //
//     - MY_MACRO(TR_MACRO_COMMA_GUARD(std::array<int, 4>)) -> argument is 'std::array<int, 4>'                                          //
//                                                                                                                                       //
// TR_FILENAME expands into a string literal of the source filename (may fall back to the full path if not supported by the compiler):   //
//     - TR_FILENAME -> "macro.hpp"                                                                                                      //
//                                                                                                                                       //
// TR_ASSERT(condition, fmt, ...) is a custom assertion macro.                                                                           //
// If TR_ENABLE_ASSERTS is defined, it checks if condition is true, and if not, logs a formatted error message and aborts the program.   //
// If TR_ENABLE_ASSERTS is not defined, the macro does nothing.                                                                          //
// Error formatting is done through std::format, so fmt must be a valid format string and all further arguments must be formattable:     //
//     - int a = -1; TR_ASSERT(a > 0, "Tried to pass negative value {}.", a);                                                            //
//       -> Assertion failed at macro.hpp:19:                                                                                            //
//          Tried to pass negative value -1.                                                                                             //
//                                                                                                                                       //
// TR_PATH_CSTR(path) gets a char pointer representation of a standard path; this pointer should always be assumed to be a temporary.    //
// std::filesystem::path::c_str returns a wide char pointer on Windows, so paths must first be converted to a char string.               //
// We can avoid doing this on Unix and thus save on a string allocation over just writing path.string().c_str() everywhere:              //
//     - std::filesystem::path path = "~/example"; path /= "sub"; std::format("{}", TR_PATH_CSTR(path)) -> "~/example/sub"               //
//                                                                                                                                       //
// TR_UNSPECIALIZED_VARIABLE_TEMPLATE(template_arg, return_type, message) is used when declaring variable templates to disable the       //
// default specialization with an error:                                                                                                 //
//     - template <class T> constexpr int my_const{TR_UNSPECIALIZED_VARIABLE_TEMPLATE(T, int, "Nope!")}                                  //
//     - my_const<int> -> static assertion failed to requirement 'unspecialized<int>': Nope!                                             //
//                                                                                                                                       //
// TR_DEFINE_ENUM_BITMASK_OPERATORS(type) defines bitmask operators (&, |, ^, ~) for enum class types:                                   //
//     - enum class my_enum {                                                                                                            //
//           l = 1,                                                                                                                      //
//           r = 2,                                                                                                                      //
//           lr = 3                                                                                                                      //
//       };                                                                                                                              //
//       TR_DEFINE_ENUM_BITMASK_OPERATORS(my_enum);                                                                                      //
//     - my_enum::lr & my_enum::r -> my_enum::r                                                                                          //
//     - my_enum::l | my_enum::r -> my_enum::lr                                                                                          //
//     - my_enum::lr ^ my_enum::r -> my_enum::l                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "logger.hpp"

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

namespace tr {
#if defined(__GNUC__) || defined(__clang__)
	// Marks an unreachable branch of code.
	[[noreturn]] inline __attribute__((always_inline)) void unreachable()
	{
		__builtin_unreachable();
	}
#elif defined(_MSC_VER)
	// Marks an unreachable branch of code.
	[[noreturn]] __forceinline void unreachable()
	{
		__assume(false);
	}
#else
	// Marks an unreachable branch of code.
	inline void unreachable() {}
#endif
} // namespace tr

#ifdef __clang__
#define TR_ASSUME(condition) __builtin_assume(condition)
#elif defined(_MSC_VER)
#define TR_ASSUME(condition) __assume(condition)
#elif defined(__GNUC__)
#define TR_ASSUME(condition) ((condition) ? (void(0)) : (__builtin_unreachable()))
#else
#define TR_ASSUME(condition)                                                                                                               \
	do {                                                                                                                                   \
	} while (0)
#endif

#define TR_STRINGIFY_IMPL(x) #x
#define TR_STRINGIFY(x) TR_STRINGIFY_IMPL(x)

#define TR_MACRO_COMMA_GUARD(...) __VA_ARGS__

#if defined(__GNUG__) || defined(__clang__)
#define TR_FILENAME __FILE_NAME__
#else
#define TR_FILENAME __FILE__
#endif

#ifdef TR_ENABLE_ASSERTS
#define TR_ASSERT_IMPL(condition, file, line, fmt, ...)                                                                                    \
	do {                                                                                                                                   \
		if (!(condition)) {                                                                                                                \
			TR_LOG(::tr::log, ::tr::severity::fatal, "Assertion failed at " file ":" TR_STRINGIFY(line) ":");                              \
			TR_LOG_CONTINUE(::tr::log, fmt __VA_OPT__(, ) __VA_ARGS__);                                                                    \
			std::abort();                                                                                                                  \
		}                                                                                                                                  \
	} while (0)
#define TR_ASSERT(condition, fmt, ...) TR_ASSERT_IMPL(condition, TR_FILENAME, __LINE__, fmt, __VA_ARGS__)
#else
// Assertion macro.
#define TR_ASSERT(condition, fmt, ...)                                                                                                     \
	do {                                                                                                                                   \
	} while (0)
#endif

#ifdef _WIN32
#define TR_PATH_CSTR(path) (path).string().c_str()
#else
#define TR_PATH_CSTR(path) (path).c_str()
#endif

#define TR_UNSPECIALIZED_VARIABLE_TEMPLATE(template_arg, return_type, message)                                                             \
	[]() -> return_type { static_assert(unspecialized<template_arg>, message); }()

namespace tr {
	// Wraps an enum class to allow conversion to bool.
	template <enumerator T> struct enum_wrapper {
		T base;

		// Converts the enumerator to bool.
		constexpr explicit operator bool() const
		{
			return (base != T{0});
		}

		// Unwraps the enumerator.
		constexpr operator T() const
		{
			return base;
		}

		// Unwraps the enumerator.
		constexpr T unwrap() const
		{
			return base;
		}
	};
} // namespace tr

#define TR_DEFINE_ENUM_BITMASK_OPERATORS(type)                                                                                             \
	constexpr tr::enum_wrapper<type> operator&(type l, type r)                                                                             \
	{                                                                                                                                      \
		return tr::enum_wrapper{type(std::underlying_type_t<type>(l) & std::underlying_type_t<type>(r))};                                  \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator|(type l, type r)                                                                             \
	{                                                                                                                                      \
		return tr::enum_wrapper{type(std::underlying_type_t<type>(l) | std::underlying_type_t<type>(r))};                                  \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator^(type l, type r)                                                                             \
	{                                                                                                                                      \
		return tr::enum_wrapper{type(std::underlying_type_t<type>(l) ^ std::underlying_type_t<type>(r))};                                  \
	}                                                                                                                                      \
	constexpr tr::enum_wrapper<type> operator~(type l)                                                                                     \
	{                                                                                                                                      \
		return tr::enum_wrapper{type(~std::underlying_type_t<type>(l))};                                                                   \
	}                                                                                                                                      \
	constexpr type& operator&=(type& l, type r)                                                                                            \
	{                                                                                                                                      \
		return l = (l & r);                                                                                                                \
	}                                                                                                                                      \
	constexpr type& operator|=(type& l, type r)                                                                                            \
	{                                                                                                                                      \
		return l = (l | r);                                                                                                                \
	}                                                                                                                                      \
	constexpr type& operator^=(type& l, type r)                                                                                            \
	{                                                                                                                                      \
		return l = (l ^ r);                                                                                                                \
	}