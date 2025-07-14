#include "../../include/tr/sysgfx/gl_call.hpp"

void tr::_validate_gl_call(const char* file, int line, const char* function) noexcept
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
}