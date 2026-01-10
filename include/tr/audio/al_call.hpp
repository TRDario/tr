///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a standard macro for calling OpenAL functions.                                                                               //
//                                                                                                                                       //
// Errors in OpenAL functions are set into global state and must be queried with alGetError(), which returns a rather opaque error code. //
// In order to simplify the process of debugging, OpenAL functions called with TR_AL_CALL in builds where TR_ENABLE_ASSERTS is defined   //
// will inject some validation code which prints the type and location of the error before aborting the program.                         //
// In builds where TR_ENABLE_ASSERTS is not defined, the macro just calls the corresponding OpenAL function with no other effects.       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/macro.hpp"
#include <AL/al.h>
#include <AL/alc.h>

#ifdef TR_ENABLE_ASSERTS
namespace tr::audio {
	// Validates OpenAL state after calling an API function.
	void validate_al_call(const char* file, int line, const char* function);
} // namespace tr::audio

#define TR_AL_CALL_IMPL(file, line, function, ...)                                                                                         \
	do {                                                                                                                                   \
		function(__VA_ARGS__);                                                                                                             \
		tr::audio::validate_al_call(file, line, #function);                                                                                \
	} while (0)
#define TR_AL_CALL(function, ...) TR_AL_CALL_IMPL(TR_FILENAME, __LINE__, function, __VA_ARGS__)
#else
#define TR_AL_CALL(function, ...) function(__VA_ARGS__)
#endif