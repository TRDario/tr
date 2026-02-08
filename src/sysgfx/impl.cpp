///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements sysgfx/impl.hpp.                                                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/main.hpp"
#include "../../include/tr/sysgfx/window.hpp"
#include <SDL3/SDL.h>

///////////////////////////////////////////////////////////////// CONTEXT /////////////////////////////////////////////////////////////////

#ifdef TR_ENABLE_ASSERTS

// Gets a readable string for an OpenGL debug log source.
static std::string_view ogl_source(GLenum value)
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

// Gets a readable string for an OpenGL debug log message type.
static std::string_view ogl_type(GLenum value)
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

// Gets a readable string for an OpenGL debug log severity.
static std::string_view ogl_severity(GLenum value)
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

// Converts OpenGL debug severity to tr severity.
static tr::severity tr_severity(GLenum value)
{
	switch (value) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return tr::severity::info;
	case GL_DEBUG_SEVERITY_LOW:
		return tr::severity::info;
	case GL_DEBUG_SEVERITY_MEDIUM:
		return tr::severity::warning;
	case GL_DEBUG_SEVERITY_HIGH:
		return tr::severity::error;
	default:
		return tr::severity::info;
	}
}

// OpenGL debug log callback.
static void ogl_debug_cb(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei length, const GLchar* message, const void*)
{
	const std::string_view msg{(const char*)message, tr::usize(length)};
	TR_LOG(tr::gfx::log, tr_severity(severity), "[{}] | [{}] | [{}] | {}", ogl_severity(severity), ogl_type(type), ogl_source(source), msg);
}

// Sets up OpenGL debugging.
static void setup_ogl_debugging()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(ogl_debug_cb, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
}

#endif

void tr::gfx::context::create()
{
	m_ptr.reset(SDL_GL_CreateContext(sys::g_window.ptr()));
	if (m_ptr == nullptr) {
		throw sys::init_error{"Failed to create OpenGL context."};
	}
	else if (!gladLoadGLLoader(GLADloadproc(SDL_GL_GetProcAddress))) {
		throw sys::init_error{"Failed to load OpenGL 4.5."};
	}
#ifdef TR_ENABLE_ASSERTS
	TR_LOG(log, severity::info, "Created an OpenGL context.");
	TR_LOG_CONTINUE(log, "Vendor: {}", (const char*)(TR_RET_GL_CALL(glGetString, GL_VENDOR)));
	TR_LOG_CONTINUE(log, "Renderer: {}", (const char*)(TR_RET_GL_CALL(glGetString, GL_RENDERER)));
	TR_LOG_CONTINUE(log, "Version: {}", (const char*)(TR_RET_GL_CALL(glGetString, GL_VERSION)));
#endif
	TR_GL_CALL(glEnable, GL_BLEND);
	TR_GL_CALL(glEnable, GL_SCISSOR_TEST);

#ifdef TR_ENABLE_ASSERTS
	setup_ogl_debugging();
#endif
}

void tr::gfx::context::reset()
{
	m_texture_units.fill(std::nullopt);
	clear_render_target();
	m_active_renderer = renderer_id::no_renderer;
	m_vertex2_format.reset();
	m_ptr.reset();
}

void tr::gfx::context::deleter::operator()(SDL_GLContextState* context) const
{
	SDL_GL_DestroyContext(context);
}

SDL_GLContextState* tr::gfx::context::ptr()
{
	return m_ptr.get();
}

tr::gfx::vertex_format& tr::gfx::context::vertex2_format()
{
	static constexpr std::array<vertex_binding, 3> bindings{{
		{not_instanced, as_vertex_attribute_list<glm::vec2>},
		{not_instanced, as_vertex_attribute_list<glm::vec2>},
		{not_instanced, as_vertex_attribute_list<rgba8>},
	}};

	if (!m_vertex2_format.has_value()) {
		m_vertex2_format.emplace(bindings);
		TR_SET_LABEL(*m_vertex2_format, "(tr) 2D Vertex Format");
	}
	return *m_vertex2_format;
}

//

bool tr::gfx::context::should_setup_context(renderer_id id)
{
	const bool result{m_active_renderer != id};
	m_active_renderer = id;
	return result;
}

//

bool tr::gfx::context::is_fbo_of_render_target(unsigned int fbo)
{
	return m_render_target.has_value() && m_render_target->m_fbo == fbo;
}

void tr::gfx::context::set_render_target(const render_target& target)
{
	if (m_render_target != target) {
		if (!is_fbo_of_render_target(target.m_fbo)) {
			TR_GL_CALL(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, target.m_fbo);
		}
		TR_GL_CALL(glViewport, target.m_viewport.tl.x, target.m_viewport.tl.y, target.m_viewport.size.x, target.m_viewport.size.y);
		TR_GL_CALL(glScissor, target.m_viewport.tl.x, target.m_viewport.tl.y, target.m_viewport.size.x, target.m_viewport.size.y);
		m_render_target = target;
	}
}

void tr::gfx::context::clear_render_target()
{
	m_render_target.reset();
}

//

unsigned int tr::gfx::context::allocate_texture_unit()
{
	TR_ASSERT(std::ranges::find(m_texture_units, std::optional<texture_ref>{}) != m_texture_units.end(),
			  "Ran out of texture units for shaders.");

	auto free_unit_it{std::ranges::find(m_texture_units, std::optional<texture_ref>{})};
	*free_unit_it = texture_ref{};
	return std::distance(m_texture_units.begin(), free_unit_it);
}

void tr::gfx::context::set_texture_unit(unsigned int unit, texture_ref texture)
{
	TR_ASSERT(m_texture_units[unit].has_value(), "Tried to set unallocated texture unit {}.", unit);

	if (!texture.empty()) {
		if (m_texture_units[unit] != texture && texture.m_ref->m_handle != 0) {
			TR_GL_CALL(glBindTextures, unit, 1, &texture.m_ref->m_handle);
		}
	}
	m_texture_units[unit] = std::move(texture);
}

void tr::gfx::context::free_texture_unit(unsigned int unit)
{
	TR_ASSERT(m_texture_units[unit].has_value(), "Tried to free unallocated texture unit {}.", unit);

	m_texture_units[unit] = std::nullopt;
}

void tr::gfx::context::rebind_texture_units(const texture& texture)
{
	for (int i = 0; i < 80; ++i) {
		if (m_texture_units[i] == texture) {
			TR_GL_CALL(glBindTextures, i, 1, &texture.m_handle);
		}
	}
}

//

#ifdef TR_ENABLE_GL_CHECKS

void tr::gfx::context::set_vertex_format(std::string&& name, std::span<const vertex_binding> bindings)
{
	m_vertex_format_bindings = bindings;
	m_vertex_format_label = std::move(name);
}

void tr::gfx::context::check_vertex_buffer(std::string label, int slot, std::span<const vertex_attribute> attrs)
{
	TR_ASSERT(usize(slot) < m_vertex_format_bindings.size(),
			  "Tried to bind vertex buffer '{}' to invalid slot {} (max in vertex format '{}': {}).", label, slot, m_vertex_format_label,
			  m_vertex_format_bindings.size());

	const std::span<const vertex_attribute> ref{m_vertex_format_bindings.begin()[slot].attrs};
	TR_ASSERT(attrs.size() == ref.size(),
			  "Tried to bind vertex buffer '{}' of a different type from the one in vertex format '{}' (has {} attributes instead of {}).",
			  label, m_vertex_format_label, attrs.size(), ref.size());
	for (usize i = 0; i < attrs.size(); ++i) {
		const vertex_attribute& l{attrs.begin()[i]};
		const vertex_attribute& r{ref.begin()[i]};
		TR_ASSERT(l.type == r.type && l.elements == r.elements,
				  "Tried to bind vertex buffer '{}' of a type different from than the one in vertex format '{}' (expected '{}' in "
				  "attribute {}, got '{}').",
				  label, m_vertex_format_label, r, i, l);
	}
}

#endif

/////////////////////////////////////////////////////////////// OBJECT LABEL //////////////////////////////////////////////////////////////

#ifdef TR_ENABLE_ASSERTS
void tr::gfx::move_label(unsigned int type, unsigned int old_id, unsigned int new_id)
{
	tr::static_string<64> label(64);
	GLsizei label_length;
	TR_GL_CALL(glGetObjectLabel, type, old_id, label.size(), &label_length, label.data());
	label.resize(label_length);

	if (!label.empty()) {
		TR_GL_CALL(glObjectLabel, type, new_id, label.size(), label.data());
		TR_GL_CALL(glObjectLabel, type, old_id, 0, nullptr);
	}
}
#endif

////////////////////////////////////////////////////////////////// WINDOW /////////////////////////////////////////////////////////////////

// Sets up SDL OpenGL attributes.
static void set_sdl_ogl_attributes(const tr::gfx::properties& gfx_properties)
{
#ifdef TR_ENABLE_ASSERTS
	constexpr int context_flags{SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | SDL_GL_CONTEXT_DEBUG_FLAG};
#else
	constexpr int context_flags{SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG};
#endif

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
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

//

bool tr::sys::window::open(cstring_view title, glm::ivec2 size, unsigned long flags, glm::ivec2 min_size,
						   const gfx::properties& gfx_properties)
{
	set_sdl_ogl_attributes(gfx_properties);
	m_ptr.reset(SDL_CreateWindow(title, size.x, size.y, flags));
	if (m_ptr == nullptr) {
		return false;
	}
	if (min_size != not_resizable) {
		SDL_SetWindowMinimumSize(m_ptr.get(), min_size.x, min_size.y);
	}
	m_context.create();
	return true;
}

void tr::sys::window::close()
{
	m_context.reset();
#ifdef _WIN32
	m_cursor_reset_timer.reset();
#endif
	m_ptr.reset();
}

void tr::sys::window::deleter::operator()(SDL_Window* window) const
{
	SDL_DestroyWindow(window);
}

//

bool tr::sys::window::is_open() const
{
	return m_ptr != nullptr;
}

SDL_Window* tr::sys::window::ptr()
{
	return m_ptr.get();
}

//

#ifdef _WIN32

void tr::sys::window::enable_cursor_resets()
{
	if (!m_cursor_reset_timer.has_value()) {
		m_cursor_reset_timer.emplace(std::chrono::seconds{1}, [] {
			float x, y;
			SDL_GetMouseState(&x, &y);
			SDL_SetWindowRelativeMouseMode(m_ptr.get(), false);
			const bool cursor_visible{SDL_CursorVisible()};
			if (cursor_visible) {
				SDL_HideCursor();
			}
			SDL_WarpMouseInWindow(m_ptr.get(), x, y);
			if (cursor_visible) {
				SDL_ShowCursor();
			}
			SDL_SetWindowRelativeMouseMode(m_ptr.get(), true);
		});
	}
}

void tr::sys::window::disable_cursor_resets()
{
	m_cursor_reset_timer.reset();
}

#endif