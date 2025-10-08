#include "../../include/tr/sysgfx/window.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/display.hpp"
#include "../../include/tr/sysgfx/glad.h"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/main.hpp"
#include "tr/sysgfx/gl_call.hpp"
#include <SDL3/SDL.h>

namespace tr::gfx {
	// Sets up SDL OpenGL attributes.
	void set_sdl_ogl_attributes(const properties& gfx_properties);
	// Creates an OpenGL context.
	void create_ogl_context();

#ifdef TR_ENABLE_ASSERTS
	// Gets a readable string for an OpenGL debug log source.
	std::string_view ogl_source(GLenum value);
	// Gets a readable string for an OpenGL debug log message type.
	std::string_view ogl_type(GLenum value);
	// Gets a readable string for an OpenGL debug log severity.
	std::string_view ogl_severity(GLenum value);
	// Converts OpenGL debug severity to tr severity.
	severity tr_severity(GLenum value);
	// OpenGL debug log callback.
	void ogl_debug_cb(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei length, const GLchar* message, const void*);
	// Sets up OpenGL debugging.
	void setup_ogl_debugging();
#endif
} // namespace tr::gfx

void tr::gfx::set_sdl_ogl_attributes(const properties& gfx_properties)
{
#ifdef TR_ENABLE_ASSERTS
	constexpr int CONTEXT_FLAGS{SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | SDL_GL_CONTEXT_DEBUG_FLAG};
#else
	constexpr int CONTEXT_FLAGS{SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG};
#endif

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, CONTEXT_FLAGS);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, true);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, gfx_properties.enable_depth_stencil ? 24 : 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, gfx_properties.enable_depth_stencil ? 8 : 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, bool(gfx_properties.multisamples));
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, gfx_properties.multisamples);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
}

void tr::gfx::create_ogl_context()
{
	if ((gfx::ogl_context = SDL_GL_CreateContext(sys::sdl_window)) == nullptr) {
		throw sys::init_error{"Failed to create OpenGL context."};
	}
	else if (!gladLoadGLLoader(GLADloadproc(SDL_GL_GetProcAddress))) {
		throw sys::init_error{"Failed to load OpenGL 4.5."};
	}
#ifdef TR_ENABLE_ASSERTS
	TR_LOG(log, severity::INFO, "Created an OpenGL context.");
	TR_LOG_CONTINUE(log, "Vendor: {}", (const char*)(TR_RETURNING_GL_CALL(glGetString, GL_VENDOR)));
	TR_LOG_CONTINUE(log, "Renderer: {}", (const char*)(TR_RETURNING_GL_CALL(glGetString, GL_RENDERER)));
	TR_LOG_CONTINUE(log, "Version: {}", (const char*)(TR_RETURNING_GL_CALL(glGetString, GL_VERSION)));
#endif
	TR_GL_CALL(glEnable, GL_BLEND);
	TR_GL_CALL(glEnable, GL_SCISSOR_TEST);
}

#ifdef TR_ENABLE_ASSERTS
std::string_view tr::gfx::ogl_source(GLenum value)
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

std::string_view tr::gfx::ogl_type(GLenum value)
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

std::string_view tr::gfx::ogl_severity(GLenum value)
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

tr::severity tr::gfx::tr_severity(GLenum value)
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

void tr::gfx::ogl_debug_cb(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei length, const GLchar* message, const void*)
{
	const std::string_view msg{(const char*)message, usize(length)};
	TR_LOG(gfx::log, tr_severity(severity), "[{}] | [{}] | [{}] | {}", ogl_severity(severity), ogl_type(type), ogl_source(source), msg);
}

void tr::gfx::setup_ogl_debugging()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(ogl_debug_cb, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
}
#endif

void tr::sys::open_window(const char* title, glm::ivec2 size, std::optional<glm::ivec2> min_size, const gfx::properties& gfx_properties)
{
	TR_ASSERT(sdl_window == nullptr, "Tried to reopen window without closing it first.");

	set_sdl_ogl_attributes(gfx_properties);

	const SDL_WindowFlags sdl_flags{(min_size.has_value() ? SDL_WINDOW_RESIZABLE : 0) | SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL |
									SDL_WINDOW_HIGH_PIXEL_DENSITY};
	if ((sdl_window = SDL_CreateWindow(title, size.x, size.y, sdl_flags)) == nullptr) {
		throw init_error{"Failed to open window."};
	}
	if (min_size.has_value()) {
		SDL_SetWindowMinimumSize(sdl_window, min_size->x, min_size->y);
	}

	gfx::create_ogl_context();
#ifdef TR_ENABLE_ASSERTS
	gfx::setup_ogl_debugging();
#endif
}

void tr::sys::open_fullscreen_window(const char* title, std::optional<glm::ivec2> min_size, const gfx::properties& gfx_properties)
{
	TR_ASSERT(sdl_window == nullptr, "Tried to reopen window without closing it first.");

	set_sdl_ogl_attributes(gfx_properties);

	const glm::ivec2 size{display_size()};
	const SDL_WindowFlags sdl_flags{(min_size.has_value() ? SDL_WINDOW_RESIZABLE : 0) | SDL_WINDOW_HIDDEN | SDL_WINDOW_FULLSCREEN |
									SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY};
	if ((sdl_window = SDL_CreateWindow(title, size.x, size.y, sdl_flags)) == nullptr) {
		throw init_error{"Failed to open fullscreen window."};
	}
	if (min_size.has_value()) {
		SDL_SetWindowMinimumSize(sdl_window, min_size->x, min_size->y);
	}

	gfx::create_ogl_context();
#ifdef TR_ENABLE_ASSERTS
	gfx::setup_ogl_debugging();
#endif
}

void tr::sys::close_window()
{
	gfx::active_renderer = gfx::NO_RENDERER;
	gfx::current_render_target.reset();
	gfx::vertex2_format_.reset();
	SDL_GL_DestroyContext(gfx::ogl_context);
	SDL_DestroyWindow(sdl_window);
	gfx::ogl_context = nullptr;
	sdl_window = nullptr;
}

const char* tr::sys::window_title()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to get window title before opening it.");

	return SDL_GetWindowTitle(sdl_window);
}

void tr::sys::set_window_title(const char* title)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window title before opening it.");

	if (!SDL_SetWindowTitle(sdl_window, title)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set window title to '{}'.", title);
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	};
}

void tr::sys::set_window_title(const std::string& title)
{
	set_window_title(title.c_str());
}

void tr::sys::set_window_icon(const bitmap& bitmap)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window icon before opening it.");

	if (!SDL_SetWindowIcon(sdl_window, bitmap.m_ptr.get())) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set window icon.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::sys::set_window_icon(const bitmap_view& view)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window icon before opening it.");

	if (!SDL_SetWindowIcon(sdl_window, view.m_ptr.get())) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set window icon.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

glm::ivec2 tr::sys::window_size()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to get window size before opening it.");

	glm::ivec2 size{};
	if (!SDL_GetWindowSizeInPixels(sdl_window, &size.x, &size.y)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to get window size.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	return size;
}

float tr::sys::window_pixel_density()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to get window pixel density before opening it.");

	const float density{SDL_GetWindowPixelDensity(sdl_window)};
	if (density == 0.0f) {
		TR_LOG(log, tr::severity::ERROR, "Failed to get window pixel density.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
		return 1.0f;
	}
	return density;
}

void tr::sys::set_window_size(glm::ivec2 size)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window size before opening it.");
	TR_ASSERT(size.x > 0 && size.y > 0, "Tried to set window size to an invalid value of {}x{}.", size.x, size.y);

	if (!SDL_SetWindowSize(sdl_window, int(size.x / window_pixel_density()), int(size.y / window_pixel_density())) ||
		!SDL_SetWindowPosition(sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set window size.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

bool tr::sys::window_fullscreen()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to get window fullscreen status before opening it.");

	return SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_FULLSCREEN;
}

void tr::sys::set_window_fullscreen(bool fullscreen)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window to fullscreen before opening it.");

	if (!SDL_SetWindowFullscreen(sdl_window, fullscreen)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set window {} fullscreen.", fullscreen ? "to" : "from");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::sys::show_window()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to show window before opening it.");

	if (!SDL_ShowWindow(sdl_window)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to show window.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::sys::hide_window()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to hide window before opening it.");

	if (!SDL_HideWindow(sdl_window)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to hide window.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

bool tr::sys::window_has_focus()
{
	return SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_INPUT_FOCUS;
}

bool tr::sys::window_maximized()
{
	return SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_MAXIMIZED;
}

bool tr::sys::window_minimized()
{
	return SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_MINIMIZED;
}

void tr::sys::raise_window()
{
	TR_ASSERT(sdl_window != nullptr, "Tried to raise window before opening it.");

	if (!SDL_RaiseWindow(sdl_window)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to raise window.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::sys::set_window_mouse_grab(bool grab)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window mouse grab before opening it.");

	if (!SDL_SetWindowMouseGrab(sdl_window, grab)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set mouse grab.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::sys::flash_window(flash_operation operation)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to flash window before opening it.");

	if (!SDL_FlashWindow(sdl_window, SDL_FlashOperation(operation))) {
		TR_LOG(log, tr::severity::ERROR, "Failed to flash window.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::sys::set_window_vsync(vsync vsync)
{
	TR_ASSERT(sdl_window != nullptr, "Tried to set window V-sync before opening it.");

	if (!SDL_GL_SetSwapInterval(int(vsync))) {
		if (vsync == vsync::ADAPTIVE) {
			TR_LOG(log, tr::severity::WARN, "Failed to set V-sync to adaptive, falling back to regular.");
			TR_LOG_CONTINUE(log, "{}", SDL_GetError());
			set_window_vsync(vsync::ENABLED);
		}
		else {
			TR_LOG(log, tr::severity::ERROR, "Failed to set V-sync.");
			TR_LOG_CONTINUE(log, "{}", SDL_GetError());
		}
	}
}