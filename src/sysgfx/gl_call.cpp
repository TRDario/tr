#include "../../include/tr/sysgfx/gl_call.hpp"

void tr::gfx::validate_gl_call(const char* file, int line, const char* function)
{
	const GLenum error{glGetError()};
	if (error != GL_NO_ERROR) {
		TR_LOG(log, tr::severity::FATAL, "OpenGL validation error.");
		switch (error) {
		case GL_INVALID_ENUM:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned GL_INVALID_ENUM.", function, file, line);
			break;
		case GL_INVALID_VALUE:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned GL_INVALID_VALUE.", function, file, line);
			break;
		case GL_INVALID_OPERATION:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned GL_INVALID_OPERATION.", function, file, line);
			break;
		case GL_OUT_OF_MEMORY:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned GL_OUT_OF_MEMORY.", function, file, line);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned GL_INVALID_FRAMEBUFFER_OPERATION.", function, file, line);
			break;
		}
		std::abort();
	}
}