/// @file
/// @brief Provides macro utilities.

#pragma once
#ifdef TR_ENABLE_ASSERTS
#include "logger.hpp" // IWYU pragma: keep
#endif

//

/// @cond implementation_details
#ifdef TR_ENABLE_ASSERTS
#define TR_IMPL_UNREACHABLE(file, line)                                                                                                    \
	do {                                                                                                                                   \
		::tr::error_logger.log(::tr::severity::fatal, "Unreachable code section reached at " file ":" TR_STRINGIFY(line) ".");             \
		std::abort();                                                                                                                      \
	} while (0)
#endif
/// @endcond

/// Marks a code segment as unreachable.
/// @details If `TR_ENABLE_ASSERTS` is defined, it logs an error message and aborts the program. Otherwise, `std::unreachable()` is invoked.
/// @hideinitializer
#ifdef TR_ENABLE_ASSERTS
#define TR_UNREACHABLE TR_IMPL_UNREACHABLE(TR_FILENAME, __LINE__)
#else
#define TR_UNREACHABLE std::unreachable()
#endif

//

/// @cond implementation_details
#define TR_IMPL_STRINGIFY(x) #x
/// @endcond

/// Expands `x` into its string representation.
/// @param x Expression that will be stringified.
/// @hideinitializer
#define TR_STRINGIFY(x) TR_IMPL_STRINGIFY(x)

//

/// @cond implementation_details
#define TR_IMPL_JOIN(x, y) x##y
/// @endcond

/// Joins `x` and `y`.
/// @param x, y Expressions to join.
/// @hideinitializer
#define TR_JOIN(x, y) TR_IMPL_JOIN(x, y)

//

/// Circumvents macros taking expressions with macros as multiple arguments.
/// @param ... An expression with commas.
/// @hideinitializer
#define TR_MACRO_COMMA_GUARD(...) __VA_ARGS__

/// Expands into a string literal of the source filename.
/// @note May fall back to the full path if not supported by the compiler.
/// @hideinitializer
#if defined(__GNUG__) || defined(__clang__)
#define TR_FILENAME __FILE_NAME__
#else
#define TR_FILENAME __FILE__
#endif

/// @cond implementation_details
#ifdef TR_ENABLE_ASSERTS
#define TR_IMPL_ASSERT(condition, file, line, fmt, ...)                                                                                    \
	do {                                                                                                                                   \
		if (!(condition)) {                                                                                                                \
			if (::tr::error_logger.active()) {                                                                                             \
				::tr::error_logger.log(::tr::severity::fatal, "Assertion failed at " file ":" TR_STRINGIFY(line) ":");                     \
				::tr::error_logger.log_continue(fmt __VA_OPT__(, ) __VA_ARGS__);                                                           \
			}                                                                                                                              \
			std::abort();                                                                                                                  \
		}                                                                                                                                  \
	} while (0)
#endif
/// @endcond

/// Custom assertion macro.
/// @details
/// If `TR_ENABLE_ASSERTS` is defined, it checks if condition is true, and if not, logs a formatted error message and aborts the program.
///
/// If `TR_ENABLE_ASSERTS` is not defined, the macro does nothing.
///
/// Error formatting is done through `std::format`, so `fmt` must be a valid format string and all further arguments must be formattable.
/// @param condition Condition that must evalutate to `true`.
/// @param fmt Error message format string.
/// @param ... Formatting arguments.
/// @hideinitializer
#ifdef TR_ENABLE_ASSERTS
#define TR_ASSERT(condition, fmt, ...) TR_IMPL_ASSERT(condition, TR_FILENAME, __LINE__, fmt, __VA_ARGS__)
#else
#define TR_ASSERT(condition, fmt, ...) void(0)
#endif

/// Gets a character pointer representation of a standard path.
/// @note The resulting pointer should always be assumed to be a temporary.
/// @hideinitializer
#ifdef _WIN32
#define TR_PATH_CSTR(path) (path).string().c_str()
#else
#define TR_PATH_CSTR(path) (path).c_str()
#endif

/// Disables the default specialization of a templated variable with an error.
/// @hideinitializer
#define TR_UNSPECIALIZED_VARIABLE_TEMPLATE(template_arg, return_type, message) [] -> return_type { static_assert(false, message); }()