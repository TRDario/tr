#pragma once
#include "logger.hpp"

#ifdef __clang__
#define TR_ASSUME(x) __builtin_assume(x)
#elif defined(_MSC_VER)
#define TR_ASSUME(x) __assume(x)
#elif defined(__GNUC__)
#define TR_ASSUME(x) ((x) ? (void(0)) : (__builtin_unreachable()))
#else
#define TR_ASSUME(x)                                                                                                                       \
	do {                                                                                                                                   \
	} while (0)
#endif

#define TR_STRINGIFY_IMPL(x) #x
#define TR_STRINGIFY(x) TR_STRINGIFY_IMPL(x)

#if defined(__GNUG__) || defined(__clang__)
#define TR_FILENAME __FILE_NAME__
#else
#define TR_FILENAME __FILE__
#endif

#ifdef TR_ENABLE_ASSERTS
#define TR_ASSERT_IMPL(condition, file, line, fmt, ...)                                                                                    \
	do {                                                                                                                                   \
		if (!(condition)) {                                                                                                                \
			TR_LOG(::tr::log, ::tr::severity::FATAL, "Assertion failed at " file ":" TR_STRINGIFY(line) ":");                              \
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