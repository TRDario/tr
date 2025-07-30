#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/display.hpp"
#include "../../include/tr/sysgfx/glad.h"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/window.hpp"
#include "tr/sysgfx/gl_call.hpp"
#include <SDL3/SDL.h>

namespace tr {
	// Sets up SDL OpenGL attributes.
	void _set_sdl_gl_attributes(const gfx_properties& gfx_properties) noexcept;
	// Creates an OpenGL context.
	void _create_gl_context() noexcept;

	// Gets a readable string for an OpenGL debug log source.
	std::string_view _gl_source(GLenum value) noexcept;
	// Gets a readable string for an OpenGL debug log message type.
	std::string_view _gl_type(GLenum value) noexcept;
	// Gets a readable string for an OpenGL debug log severity.
	std::string_view _gl_severity(GLenum value) noexcept;
	// Converts OpenGL debug severity to tr severity.
	severity _tr_severity(GLenum value) noexcept;
	// OpenGL debug log callback.
	void _gl_debug_cb(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei length, const GLchar* message, const void*);
	// Sets up OpenGL debugging.
	void _setup_gl_debugging() noexcept;
} // namespace tr

void tr::_set_sdl_gl_attributes(const gfx_properties& gfx_properties) noexcept
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | static_cast<int>(gfx_properties.debug_context));
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, gfx_properties.depth_bits);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, gfx_properties.stencil_bits);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, static_cast<bool>(gfx_properties.multisamples));
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, gfx_properties.multisamples);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, gfx_properties.double_buffer);
}

void tr::_create_gl_context() noexcept
{
	if ((ogl_context = SDL_GL_CreateContext(sdl_window)) == nullptr) {
		terminate("Failed to create OpenGL context", SDL_GetError());
	}
	else if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
		terminate("Failed to load OpenGL 4.5", {});
	}
	TR_LOG(log, severity::INFO, "Created an OpenGL context.");
	TR_LOG_CONTINUE(log, "Vendor: {}", reinterpret_cast<const char*>(TR_RETURNING_GL_CALL(glGetString, GL_VENDOR)));
	TR_LOG_CONTINUE(log, "Renderer: {}", reinterpret_cast<const char*>(TR_RETURNING_GL_CALL(glGetString, GL_RENDERER)));
	TR_LOG_CONTINUE(log, "Version: {}", reinterpret_cast<const char*>(TR_RETURNING_GL_CALL(glGetString, GL_VERSION)));
	glEnable(GL_BLEND);
	glEnable(GL_SCISSOR_TEST);
}

std::string_view tr::_gl_source(GLenum value) noexcept
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

std::string_view tr::_gl_type(GLenum value) noexcept
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

std::string_view tr::_gl_severity(GLenum value) noexcept
{
	switch (value) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "Info";
	case GL_DEBUG_SEVERITY_LOW:
		return "Low";
	case GL_DEBUG_SEVERITY_MEDIUM:
		return "Mid";
	case GL_DEBUG_SEVERITY_HIGH:
		return "High";
	default:
		return "Unknown";
	}
}

tr::severity tr::_tr_severity(GLenum value) noexcept
{
	switch (value) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return severity::INFO;
	case GL_DEBUG_SEVERITY_LOW:
		return severity::INFO;
	case GL_DEBUG_SEVERITY_MEDIUM:
		return severity::WARN;
	case GL_DEBUG_SEVERITY_HIGH:
		return severity::ERROR;
	default:
		return severity::INFO;
	}
}

void tr::_gl_debug_cb(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei length, const GLchar* message, const void*)
{
	const std::string_view msg{reinterpret_cast<const char*>(message), static_cast<std::size_t>(length)};
	TR_LOG(gfx_context::log, _tr_severity(severity), "[{}] | [{}] | [{}] | {}", _gl_severity(severity), _gl_type(type), _gl_source(source),
		   msg);
}

void tr::_setup_gl_debugging() noexcept
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(_gl_debug_cb, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
}

void tr::window::open_windowed(const char* title, glm::ivec2 size, window_flag flags, const gfx_properties& gfx_properties) noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to open window before initializing the application.");
	TR_ASSERT(sdl_window == nullptr, "Tried to reopen window without closing it first.");

	_set_sdl_gl_attributes(gfx_properties);
	const SDL_WindowFlags sdl_flags{static_cast<SDL_WindowFlags>(flags) | SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL |
									SDL_WINDOW_HIGH_PIXEL_DENSITY};
	if ((sdl_window = SDL_CreateWindow(title, size.x, size.y, sdl_flags)) == nullptr) {
		terminate(std::format("Failed to open {}x{} window", size.x, size.y), SDL_GetError());
	}
	_create_gl_context();
	if (gfx_properties.debug_context) {
		_setup_gl_debugging();
	}
	_debug_context = gfx_properties.debug_context;
}

void tr::window::open_fullscreen(const char* title, window_flag flags, const gfx_properties& gfx_properties) noexcept
{
	TR_ASSERT(SDL_WasInit(0), "Tried to open window before initializing the application.");
	TR_ASSERT(sdl_window == nullptr, "Tried to reopen window without closing it first.");

	_set_sdl_gl_attributes(gfx_properties);
	const glm::ivec2 size{display_size()};
	const SDL_WindowFlags sdl_flags{static_cast<SDL_WindowFlags>(flags) | SDL_WINDOW_HIDDEN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL |
									SDL_WINDOW_HIGH_PIXEL_DENSITY};
	if ((sdl_window = SDL_CreateWindow(title, size.x, size.y, sdl_flags)) == nullptr) {
		terminate("Failed to open fullscreen window", SDL_GetError());
	}
	_create_gl_context();
	if (gfx_properties.debug_context) {
		_setup_gl_debugging();
	}
	_debug_context = gfx_properties.debug_context;
}

void tr::window::close() noexcept
{
	tr::gfx_context::set_renderer(NO_RENDERER);
	_render_target.reset();
	_tex_units.fill(false);
	_vertex2_format.reset();
	SDL_GL_DestroyContext(ogl_context);
	SDL_DestroyWindow(sdl_window);
	ogl_context = nullptr;
	sdl_window = nullptr;
}

const char* tr::window::title() noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to get window title before opening it.");

	return SDL_GetWindowTitle(sdl_window);
}

void tr::window::set_title(const char* title) noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window title before opening it.");

	SDL_SetWindowTitle(sdl_window, title);
}

void tr::window::set_title(const std::string& title) noexcept
{
	set_title(title.c_str());
}

void tr::window::set_icon(const bitmap& bitmap) noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window icon before opening it.");

	SDL_SetWindowIcon(sdl_window, bitmap._impl.get());
}

void tr::window::set_icon(const bitmap_view& view) noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window icon before opening it.");

	SDL_SetWindowIcon(sdl_window, view._impl.get());
}

glm::ivec2 tr::window::size() noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to get window size before opening it.");

	glm::ivec2 size;
	SDL_GetWindowSizeInPixels(sdl_window, &size.x, &size.y);
	return size;
}

float tr::window::pixel_density() noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to get window pixel density before opening it.");

	return SDL_GetWindowPixelDensity(sdl_window);
}

void tr::window::set_size(glm::ivec2 size) noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window size before opening it.");
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to set window size to an invalid value of {}x{}.", size.x, size.y);

	SDL_SetWindowSize(sdl_window, static_cast<int>(size.x / pixel_density()), static_cast<int>(size.y / pixel_density()));
	SDL_SetWindowPosition(sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

bool tr::window::fullscreen() noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to get window fullscreen status before opening it.");

	return SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_FULLSCREEN;
}

void tr::window::set_fullscreen(bool fullscreen) noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window to fullscreen before opening it.");

	SDL_SetWindowFullscreen(sdl_window, fullscreen);
}

void tr::window::show() noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to show window before opening it.");

	SDL_ShowWindow(sdl_window);
}

void tr::window::hide() noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to hide window before opening it.");

	SDL_HideWindow(sdl_window);
}

bool tr::window::has_focus() noexcept
{
	return SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_INPUT_FOCUS;
}

void tr::window::set_mouse_grab(bool grab) noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window mouse grab before opening it.");

	SDL_SetWindowMouseGrab(sdl_window, grab);
}

void tr::window::flash(flash_operation operation) noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to flash window before opening it.");

	SDL_FlashWindow(sdl_window, static_cast<SDL_FlashOperation>(operation));
}

void tr::window::set_vsync(vsync vsync) noexcept
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window V-sync before opening it.");

	if (!SDL_GL_SetSwapInterval(static_cast<int>(vsync)) && vsync == vsync::ADAPTIVE) {
		set_vsync(vsync::ENABLED);
	}
}