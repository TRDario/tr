/// @file
/// @brief Provides `TR_CHECKED_GRAPHICS_ASSERT()`.

#pragma once
#include <tr/utility/macro.hpp>

//

/// Custom assertion macro only called in builds with graphics checks.
/// @details
/// If `TR_ENABLE_CHECKED_GRAPHICS` is defined, it checks if condition is true, and if not, logs a formatted error message and aborts the
/// program.
///
/// If `TR_ENABLE_CHECKED_GRAPHICS` is not defined, the macro does nothing.
///
/// Error formatting is done through `std::format`, so `fmt` must be a valid format string and all further arguments must be formattable.
/// @param condition Condition that must evalutate to `true`.
/// @param fmt Error message format string.
/// @param ... Formatting arguments.
/// @hideinitializer
#ifdef TR_ENABLE_CHECKED_GRAPHICS
#define TR_CHECKED_GRAPHICS_ASSERT(condition, fmt, ...) TR_ASSERT(condition, fmt, __VA_ARGS__)
#else
#define TR_CHECKED_GRAPHICS_ASSERT(...) void(0)
#endif