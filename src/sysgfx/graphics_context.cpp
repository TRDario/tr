/// @file
/// @brief Implements graphics_context.hpp.

#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/window_view.hpp"
#include <SDL3/SDL.h>

//

tr::graphics_context_init_error::graphics_context_init_error()
	: m_description{SDL_GetError()}
{
}

std::string_view tr::graphics_context_init_error::name() const
{
	return "Graphics context opening error";
}

std::string_view tr::graphics_context_init_error::description() const
{
	return m_description;
}

std::string_view tr::graphics_context_init_error::details() const
{
	return {};
}

//

namespace tr
{
	namespace
	{
		/// Gets a readable string for an OpenGL debug log message type.
		/// @param value OpenGL debug type.
		/// @return String representation of the debug type.
		std::string_view gl_type(unsigned int value)
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

		/// Gets a readable string for an OpenGL debug log severity.
		/// @param value OpenGL debug severity.
		/// @return String representation of the debug severity.
		std::string_view gl_severity(unsigned int value)
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

		/// Converts OpenGL debug severity to tr severity.
		/// @param value OpenGL debug severity.
		/// @return tr severity equivalent.
		tr::severity tr_severity(unsigned int value)
		{
			switch (value) {
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				return severity::info;
			case GL_DEBUG_SEVERITY_LOW:
				return severity::info;
			case GL_DEBUG_SEVERITY_MEDIUM:
				return severity::warning;
			case GL_DEBUG_SEVERITY_HIGH:
				return severity::error;
			default:
				return severity::info;
			}
		}

		/// Gets a readable string for an OpenGL debug log source.
		/// @param value OpenGL debug source.
		/// @return String representation of the debug source.
		std::string_view gl_source(unsigned int value)
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

		/// OpenGL debug log callback.
		/// @param source Message source.
		/// @param type Message type.
		/// @param severity Message severity.
		/// @param length Length of the debug message.
		/// @param message Pointer to the debug message string.
		/// @param user_param Pointer to the graphics context logger.
		void gl_debug_cb(unsigned int source, unsigned int type, unsigned int, unsigned int severity, int length, const char* message,
						 const void* user_param)
		{
			logger& log{*const_cast<logger*>(static_cast<const logger*>(user_param))};

			const std::string_view msg{message, static_cast<usize>(length)};
			if (log.active()) {
				log.log(tr_severity(severity), "[{}] | [{}] | [{}] | {}", gl_severity(severity), gl_type(type), gl_source(source), msg);
			}
		}

		//

		/// Creates an SDL OpenGL context.
		/// @param window Pointer to the SDL window.
		/// @return SDL OpenGL context pointer.
		SDL_GLContext create_context(SDL_Window* window)
		{
			SDL_GLContext context{SDL_GL_CreateContext(window)};
			if (context == nullptr) {
				throw graphics_context_init_error{};
			}
			return context;
		}
	} // namespace
} // namespace tr

//

tr::graphics_context::graphics_context(window_view window)
	: m_window{window.unwrap()}
	, m_ptr{create_context(m_window)}
{
	m_gl_api.enable(GL_BLEND);
	m_gl_api.enable(GL_SCISSOR_TEST);

	int context_flags;
	m_gl_api.get_integer_v(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		m_gl_api.enable(GL_DEBUG_OUTPUT);
		m_gl_api.enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		m_gl_api.set_debug_message_callback(gl_debug_cb, &logger);
		m_gl_api.set_debug_message_control(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		m_gl_api.set_debug_message_control(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

		static int logger_id{0};
		logger.replace_backend_with<console_logger>(std::format("gfx-{}", logger_id++));
	}
}

void tr::graphics_context::deleter::operator()(SDL_GLContextState* context)
{
	SDL_GL_DestroyContext(context);
}

//

struct tr::graphics_context::info tr::graphics_context::info() const
{
	const gl_api& gl{make_current_and_return_gl_api()};
	return {
		reinterpret_cast<const char*>(gl.get_string(GL_VENDOR)),
		reinterpret_cast<const char*>(gl.get_string(GL_RENDERER)),
		reinterpret_cast<const char*>(gl.get_string(GL_VERSION)),
	};
}

//

tr::window_view tr::graphics_context::window() const
{
	return window_view{m_window};
}

tr::render_target tr::graphics_context::backbuffer() const
{
	return render_target{0, window().size()};
}

const tr::vertex_format& tr::graphics_context::vertex2_format()
{
	static constexpr std::array<vertex_binding, 3> bindings{{
		{not_instanced, as_vertex_attribute_list<glm::vec2>},
		{not_instanced, as_vertex_attribute_list<glm::vec2>},
		{not_instanced, as_vertex_attribute_list<rgba8>},
	}};

	if (!m_vertex2_format.has_value()) {
		m_vertex2_format.emplace(*this, bindings);
		m_vertex2_format->set_label("(tr) 2D Vertex Format");
	}
	return *m_vertex2_format;
}

//

tr::renderer_id tr::graphics_context::allocate_renderer_id()
{
	const renderer_id id{m_next_renderer_id};
	m_next_renderer_id = renderer_id{std::to_underlying(m_next_renderer_id) + 1};
	return id;
}

bool tr::graphics_context::should_setup_renderer(renderer_id id)
{
	const bool result{m_active_renderer != id};
	m_active_renderer = id;
	return result;
}

//

void tr::graphics_context::set_wireframe_mode(bool arg)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.set_polygon_mode(GL_FRONT_AND_BACK, arg ? GL_LINE : GL_FILL);
}

void tr::graphics_context::set_face_culling(bool arg)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	if (arg) {
		gl.enable(GL_CULL_FACE);
	}
	else {
		gl.disable(GL_CULL_FACE);
	}
}

void tr::graphics_context::set_depth_test(bool arg)
{
	const gl_api& gl{make_current_and_return_gl_api()};

	if (arg) {
		gl.enable(GL_DEPTH_TEST);
	}
	else {
		gl.disable(GL_DEPTH_TEST);
	}
}

//

void tr::graphics_context::set_render_target(const render_target& target)
{
	const gl_api& gl{make_current_and_return_gl_api()};

	bool changed_render_target{false};

	if (!m_render_target.has_value() || m_render_target->m_framebuffer != target.m_framebuffer) {
		gl.bind_framebuffer(GL_DRAW_FRAMEBUFFER, target.m_framebuffer);
		changed_render_target = true;
	}
	if (!m_render_target.has_value() || m_render_target->m_framebuffer_size != target.m_framebuffer_size ||
		m_render_target->m_viewport != target.m_viewport) {
		const int bottom{target.m_framebuffer_size.y - target.m_viewport.tl.y - target.m_viewport.size.y};
		gl.set_viewport(target.m_viewport.tl.x, bottom, target.m_viewport.size.x, target.m_viewport.size.y);
		changed_render_target = true;
	}
	if (!m_render_target.has_value() || m_render_target->m_framebuffer_size != target.m_framebuffer_size ||
		m_render_target->m_scissor_box != target.m_scissor_box) {
		const int bottom{target.m_framebuffer_size.y - target.m_scissor_box.tl.y - target.m_scissor_box.size.y};
		gl.set_scissor(target.m_scissor_box.tl.x, bottom, target.m_scissor_box.size.x, target.m_scissor_box.size.y);
		changed_render_target = true;
	}

	if (changed_render_target) {
		m_render_target = target;
	}
}

void tr::graphics_context::set_shader_pipeline(const shader_pipeline& pipeline)
{
#ifdef TR_ENABLE_GL_CHECKS
	pipeline.assert_settable(*this);
#endif

	const gl_api& gl{make_current_and_return_gl_api()};
	gl.bind_program_pipeline(pipeline.gid());
}

void tr::graphics_context::set_blend_mode(const blend_mode& bm)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.set_separate_blend_equations(std::to_underlying(bm.rgb_fn), std::to_underlying(bm.alpha_fn));
	gl.set_separate_blend_function(std::to_underlying(bm.rgb_src), std::to_underlying(bm.rgb_dst), std::to_underlying(bm.alpha_src),
								   std::to_underlying(bm.alpha_dst));
}

void tr::graphics_context::set_vertex_format(const vertex_format& format)
{
	const gl_api& gl{make_current_and_return_gl_api()};

#ifdef TR_ENABLE_GL_CHECKS
	m_vertex_format_bindings = format.bindings();
	m_vertex_format_label = format.label();
#endif

	gl.bind_vertex_array(format.id());
}

void tr::graphics_context::set_vertex_buffer(unsigned int buffer_id, int slot, ssize offset, usize stride)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.bind_vertex_buffer(slot, buffer_id, offset, stride);
}

void tr::graphics_context::set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride)
{
	set_vertex_buffer(buffer.id(), slot, offset, stride);
}

void tr::graphics_context::set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride)
{
	set_vertex_buffer(buffer.id(), slot, offset, stride);
}

void tr::graphics_context::set_index_buffer(const static_index_buffer& buffer)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.bind_buffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id());
}

void tr::graphics_context::set_index_buffer(const dyn_index_buffer& buffer)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.bind_buffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id());
}

//

void tr::graphics_context::clear_backbuffer(tr::rgbaf color)
{
	const gl_api& gl{make_current_and_return_gl_api()};

	set_render_target(backbuffer());
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer(tr::rgbaf color, double depth, int stencil)
{
	const gl_api& gl{make_current_and_return_gl_api()};

	set_render_target(backbuffer());
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.set_clear_depth(depth);
	gl.set_clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(rectangle<int> region, tr::rgbaf color)
{
	const gl_api& gl{make_current_and_return_gl_api()};

	set_render_target(backbuffer().cropped(region));
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(rectangle<int> region, tr::rgbaf color, double depth, int stencil)
{
	const gl_api& gl{make_current_and_return_gl_api()};

	set_render_target(backbuffer().cropped(region));
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.set_clear_depth(depth);
	gl.set_clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//

void tr::graphics_context::draw(primitive type, usize offset, usize vertices)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.draw_arrays(std::to_underlying(type), offset, vertices);
}

void tr::graphics_context::draw_instances(primitive type, usize offset, usize vertices, int instances)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.draw_arrays_instanced(std::to_underlying(type), offset, vertices, instances);
}

void tr::graphics_context::draw_indexed(primitive type, usize offset, usize indices)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.draw_elements(std::to_underlying(type), indices, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(offset * sizeof(u16)));
}

void tr::graphics_context::draw_indexed_instances(primitive type, usize offset, usize indices, int instances)
{
	const gl_api& gl{make_current_and_return_gl_api()};
	gl.draw_elements_instanced(std::to_underlying(type), indices, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(offset * sizeof(u16)),
							   instances);
}

//

const tr::gl_api& tr::graphics_context::make_current_and_return_gl_api() const
{
	SDL_GL_MakeCurrent(m_window, m_ptr.get());
	return m_gl_api;
}

//

bool tr::graphics_context::is_fbo_of_render_target(unsigned int fbo)
{
	return m_render_target.has_value() && m_render_target->m_framebuffer == fbo;
}

void tr::graphics_context::clear_render_target()
{
	m_render_target.reset();
}

//

#ifdef TR_ENABLE_GL_CHECKS
void tr::graphics_context::check_vertex_buffer(std::string label, int slot, std::span<const vertex_attribute> attrs)
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

//

void tr::graphics_context::move_label(unsigned int type, unsigned int old_id, unsigned int new_id)
{
	const gl_api& gl{make_current_and_return_gl_api()};

	int label_length;
	gl.get_object_label(type, old_id, 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label(label_length, '\0');
		gl.get_object_label(type, old_id, label_length, &label_length, label.data());
		gl.set_object_label(type, new_id, label.size(), label.data());
		gl.set_object_label(type, old_id, 0, nullptr);
	}
}