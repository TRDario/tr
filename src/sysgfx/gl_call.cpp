#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"

void tr::_validate_gl_call(const char* file, int line, const char* function) noexcept
{
	switch (glGetError()) {
	case GL_INVALID_ENUM:
		terminate("OpenGL validation error", std::format("Call to {} at {}:{} returned GL_INVALID_ENUM.", function, file, line));
	case GL_INVALID_VALUE:
		terminate("OpenGL validation error", std::format("Call to {} at {}:{} returned GL_INVALID_VALUE.", function, file, line));
	case GL_INVALID_OPERATION:
		terminate("OpenGL validation error", std::format("Call to {} at {}:{} returned GL_INVALID_OPERATION.", function, file, line));
	case GL_OUT_OF_MEMORY:
		terminate("OpenGL validation error", std::format("Call to {} at {}:{} returned GL_OUT_OF_MEMORY.", function, file, line));
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		terminate("OpenGL validation error",
				  std::format("Call to {} at {}:{} returned GL_INVALID_FRAMEBUFFER_OPERATION.", function, file, line));
	default:
		break;
	}
}