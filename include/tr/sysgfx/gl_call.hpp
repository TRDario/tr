///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides standard macros for calling OpenGL functions.                                                                                //
//                                                                                                                                       //
// Errors in OpenGL functions are set into global state and must be queried with glGetError(), which returns a rather opaque error code. //
// In order to simplify the process of debugging, OpenGGL functions called with TR_GL_CALL or TR_RET_GL_CALL in builds where             //
// TR_ENABLE_ASSERTS is defined will inject some validation code which prints the type and location of the error before aborting the     //
// program. In builds where TR_ENABLE_ASSERTS is not defined, the macros just call the corresponding OpenGL function with no other       //
// effects.                                                                                                                              //
//     - TR_GL_CALL(glProgramUniform1i, program, index, value) -> calls glProgramUniform1i(program, index, value)                        //
//     - TR_RET_GL_CALL(glCreateShaderProgramv, type, 1, &source) -> calls glCreateShaderProgramv(type, 1, &source)                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/macro.hpp"
#include "glad.h"

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

#ifdef TR_ENABLE_ASSERTS
namespace tr::gfx {
	// Validates an OpenGL call and kills the application if it fails.
	void validate_gl_call(const char* file, int line, const char* function);
	// Validates an OpenGL call and kills the application if it fails.
	auto validate_returning_gl_call(const char* file, int line, const char* function, auto value)
	{
		validate_gl_call(file, line, function);
		return value;
	}
} // namespace tr::gfx

#define TR_GL_CALL_IMPL(file, line, function, ...)                                                                                         \
	do {                                                                                                                                   \
		function(__VA_ARGS__);                                                                                                             \
		tr::gfx::validate_gl_call(file, line, #function);                                                                                  \
	} while (0)
#define TR_RET_GL_CALL_IMPL(file, line, function, ...) tr::gfx::validate_returning_gl_call(file, line, #function, function(__VA_ARGS__))
#define TR_GL_CALL(function, ...) TR_GL_CALL_IMPL(TR_FILENAME, __LINE__, function, __VA_ARGS__)
#define TR_RET_GL_CALL(function, ...) TR_RET_GL_CALL_IMPL(TR_FILENAME, __LINE__, function, __VA_ARGS__)
#else
#define TR_GL_CALL(function, ...) function(__VA_ARGS__)
#define TR_RET_GL_CALL(function, ...) function(__VA_ARGS__)
#endif