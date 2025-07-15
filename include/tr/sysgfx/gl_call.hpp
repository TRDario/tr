#pragma once
#include "../utility/logger.hpp"
#include "glad.h"

namespace tr {
	// Validates an OpenGL call and kills the application if it fails.
	void _validate_gl_call(const char* file, int line, const char* function) noexcept;
	// Validates an OpenGL call and kills the application if it fails.
	inline auto _validate_returning_gl_call(const char* file, int line, const char* function, auto value) noexcept;
} // namespace tr

#ifdef TR_ENABLE_ASSERTS
#define TR_GL_CALL_IMPL(file, line, function, ...)                                                                                         \
	do {                                                                                                                                   \
		function(__VA_ARGS__);                                                                                                             \
		tr::_validate_gl_call(file, line, #function);                                                                                      \
	} while (0)
#define TR_RETURNING_GL_CALL_IMPL(file, line, function, ...) tr::_validate_returning_gl_call(file, line, #function, function(__VA_ARGS__))
#define TR_GL_CALL(function, ...) TR_GL_CALL_IMPL(TR_FILENAME, __LINE__, function, __VA_ARGS__)
#define TR_RETURNING_GL_CALL(function, ...) TR_RETURNING_GL_CALL_IMPL(TR_FILENAME, __LINE__, function, __VA_ARGS__)
#else
#define TR_GL_CALL(function, ...) function(__VA_ARGS__)
#define TR_RETURNING_GL_CALL(function, ...) function(__VA_ARGS__)
#endif

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

inline auto tr::_validate_returning_gl_call(const char* file, int line, const char* function, auto value) noexcept
{
	switch (glGetError()) {
	case GL_INVALID_ENUM:
		TR_LOG(log, severity::FATAL, "Call to OpenGL function {} at {}:{} returned GL_INVALID_ENUM.", function, file, line);
		std::abort();
	case GL_INVALID_VALUE:
		TR_LOG(log, severity::FATAL, "Call to OpenGL function {} at {}:{} returned GL_INVALID_VALUE.", function, file, line);
		std::abort();
	case GL_INVALID_OPERATION:
		TR_LOG(log, severity::FATAL, "Call to OpenGL function {} at {}:{} returned GL_INVALID_OPERATION.", function, file, line);
		std::abort();
	case GL_OUT_OF_MEMORY:
		TR_LOG(log, severity::FATAL, "Call to OpenGL function {} at {}:{} returned GL_OUT_OF_MEMORY.", function, file, line);
		std::abort();
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		TR_LOG(log, severity::FATAL, "Call to OpenGL function {} at {}:{} returned GL_INVALID_FRAMEBUFFER_OPERATION.", function, file,
			   line);
		std::abort();
	default:
		break;
	}
	return value;
}