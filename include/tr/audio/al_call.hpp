#pragma once
#include "../utility/macro.hpp"
#include <AL/al.h>
#include <AL/alc.h>

namespace tr {
	void _validate_al_call(const char* file, int line, const char* function);
} // namespace tr

#ifdef TR_ENABLE_ASSERTS
#define TR_AL_CALL_IMPL(file, line, function, ...)                                                                                         \
	do {                                                                                                                                   \
		function(__VA_ARGS__);                                                                                                             \
		tr::_validate_al_call(file, line, #function);                                                                                      \
	} while (0)
#define TR_AL_CALL(function, ...) TR_AL_CALL_IMPL(TR_FILENAME, __LINE__, function, __VA_ARGS__)
#else
#define TR_AL_CALL(function, ...) function(__VA_ARGS__)
#endif