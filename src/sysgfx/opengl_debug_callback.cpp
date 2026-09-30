/// @file
/// @brief Implements internal/opengl_debug_callback.hpp.

#include "internal/opengl_debug_callback.hpp"
#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/logger.hpp>

//

std::string_view tr::internal::opengl_debug_type_string(unsigned int value) noexcept
{
	switch (value) {
	case GL_DEBUG_TYPE_ERROR:
		return "Error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "Deprecated Behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "Undefined Behavior";
	case GL_DEBUG_TYPE_PORTABILITY:
		return "Portability Concern";
	case GL_DEBUG_TYPE_PERFORMANCE:
		return "Performance Concern";
	case GL_DEBUG_TYPE_MARKER:
		return "Marker";
	case GL_DEBUG_TYPE_PUSH_GROUP:
		return "Group Push";
	case GL_DEBUG_TYPE_POP_GROUP:
		return "Group Pop";
	case GL_DEBUG_TYPE_OTHER:
		return "Other";
	default:
		return "Unknown";
	}
}

tr::log_level tr::internal::opengl_debug_severity_log_level(unsigned int value) noexcept
{
	switch (value) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return log_level::trace;
	case GL_DEBUG_SEVERITY_LOW:
		return log_level::info;
	case GL_DEBUG_SEVERITY_MEDIUM:
		return log_level::warning;
	case GL_DEBUG_SEVERITY_HIGH:
		return log_level::error;
	default:
		return log_level::info;
	}
}

std::string_view tr::internal::opengl_debug_source_string(unsigned int value) noexcept
{
	switch (value) {
	case GL_DEBUG_SOURCE_API:
		return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "Window System";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "Shader Compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "Third Party";
	case GL_DEBUG_SOURCE_APPLICATION:
		return "Application";
	case GL_DEBUG_SOURCE_OTHER:
		return "Other";
	default:
		return "Unknown";
	}
}

void tr::internal::opengl_debug_callback(unsigned int source, unsigned int type, unsigned int, unsigned int severity, int length,
										 const char* message, const void*) noexcept
{
	try {
		logger::instance().log(opengl_debug_severity_log_level(severity), "gfx", "OpenGL: [{}] | [{}] | {}", opengl_debug_type_string(type),
							   opengl_debug_source_string(source), std::string_view{message, static_cast<usize>(length)});
	}
	catch (...) {
	}
}