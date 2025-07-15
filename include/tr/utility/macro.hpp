#pragma once
#include "logger.hpp"

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
// clang-format off
#define TR_ASSERT(condition, fmt, ...) do {} while (0)
// clang-format on
#endif