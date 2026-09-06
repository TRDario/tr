/// @file
/// @brief Implements graphics_context.hpp.

#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
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

void tr::graphics_context::deleter::operator()(SDL_GLContextState* context) const
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	TR_ASSERT(registry.framebuffers.empty(), "Tried to destroy a graphics context while one or more framebuffers were still alive on it.");
	TR_ASSERT(registry.shaders.empty(), "Tried to destroy a graphics context while one or more shaders were still alive on it.");
	TR_ASSERT(registry.shader_pipelines.empty(),
			  "Tried to destroy a graphics context while one or more shader pipelines were still alive on it.");
	TR_ASSERT(registry.vertex_formats.empty(),
			  "Tried to destroy a graphics context while one or more vertex formats were still alive on it.");
	TR_ASSERT(registry.buffers.empty(), "Tried to destroy a graphics context while one or more buffers were still alive on it.");
#endif

	SDL_GL_DestroyContext(context);
}

//

struct tr::graphics_context::info tr::graphics_context::info() const
{
	const gl_api& gl{this->gl()};
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
	return render_target{*this};
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
	gl().set_polygon_mode(GL_FRONT_AND_BACK, arg ? GL_LINE : GL_FILL);
}

void tr::graphics_context::set_face_culling(bool arg)
{
	if (arg) {
		gl().enable(GL_CULL_FACE);
	}
	else {
		gl().disable(GL_CULL_FACE);
	}
}

void tr::graphics_context::set_depth_test(bool arg)
{
	if (arg) {
		gl().enable(GL_DEPTH_TEST);
	}
	else {
		gl().disable(GL_DEPTH_TEST);
	}
}

//

void tr::graphics_context::set_render_target(const render_target& target)
{
	const render_target::framebuffer_info_t& framebuffer_info{target.framebuffer_info()};

#ifdef TR_ENABLE_CHECKED_GRAPHICS
	TR_ASSERT(framebuffer_info.context.as_ptr() == this, "Tried to set render target to a context it is not associated with.");
	TR_ASSERT(registry().framebuffers.contains(framebuffer_info.id),
			  "Tried to set render target on a framebuffer in an invalid state to a context.");
#endif

	const gl_api& gl{this->gl()};
	const rectangle<int> viewport{target.viewport()};
	const rectangle<int> scissor_box{target.scissor_box()};
	const int bottom{framebuffer_info.size.y - viewport.tl.y - viewport.size.y};
	gl.bind_framebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_info.fbo);
	gl.set_viewport(viewport.tl.x, bottom, viewport.size.x, viewport.size.y);
	gl.set_scissor(scissor_box.tl.x, bottom, scissor_box.size.x, scissor_box.size.y);

#ifdef TR_ENABLE_CHECKED_GRAPHICS
	m_set_framebuffer_debug_info.id = framebuffer_info.id;
	m_set_framebuffer_debug_info.label = framebuffer_info.label;
#endif
}

void tr::graphics_context::set_shader_pipeline(const shader_pipeline& pipeline)
{
	TR_ASSERT(pipeline.valid(), "Tried to set a shader pipeline in an invalid state to a context.");
	TR_ASSERT(&pipeline.context() == this, "Tried to set shader pipeline {} to a context it is not associated with.", pipeline);
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	TR_ASSERT(registry().shaders.contains(pipeline.vertex_shader_debug_info().id),
			  "Tried to set shader pipeline {} with invalid set vertex shader '{}'.", pipeline, pipeline.vertex_shader_debug_info().label);
	TR_ASSERT(registry().shaders.contains(pipeline.fragment_shader_debug_info().id),
			  "Tried to set shader pipeline {} with invalid set fragment shader '{}'.", pipeline,
			  pipeline.fragment_shader_debug_info().label);
#endif

	gl().bind_program_pipeline(pipeline.unwrap());

#ifdef TR_ENABLE_CHECKED_GRAPHICS
	m_set_shader_pipeline_debug_info.id = pipeline.id();
	m_set_shader_pipeline_debug_info.label = pipeline.label();
#endif
}

void tr::graphics_context::set_blend_mode(const blend_mode& bm)
{
	const gl_api& gl{this->gl()};
	gl.set_separate_blend_equations(std::to_underlying(bm.rgb_fn), std::to_underlying(bm.alpha_fn));
	gl.set_separate_blend_function(std::to_underlying(bm.rgb_src), std::to_underlying(bm.rgb_dst), std::to_underlying(bm.alpha_src),
								   std::to_underlying(bm.alpha_dst));
}

void tr::graphics_context::set_vertex_format(const vertex_format& format)
{
	TR_ASSERT(format.valid(), "Tried to set vertex format in an invalid state to a graphics context.");
	TR_ASSERT(&format.context() == this, "Tried to set vertex format {} to a context it is not associated with.", format);

#ifdef TR_ENABLE_CHECKED_GRAPHICS
	m_set_vertex_format_debug_info.id = format.id();
	m_set_vertex_format_debug_info.label = format.label();
	m_set_vertex_format_debug_info.bindings = format.bindings();
#endif

	gl().bind_vertex_array(format.unwrap());
}

//

void tr::graphics_context::clear_backbuffer(tr::rgbaf color)
{
	set_render_target(backbuffer());

	const gl_api& gl{this->gl()};
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer(tr::rgbaf color, double depth, int stencil)
{
	set_render_target(backbuffer());

	const gl_api& gl{this->gl()};
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.set_clear_depth(depth);
	gl.set_clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(rectangle<int> region, tr::rgbaf color)
{
	set_render_target(backbuffer().cropped(region));

	const gl_api& gl{this->gl()};
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(rectangle<int> region, tr::rgbaf color, double depth, int stencil)
{
	set_render_target(backbuffer().cropped(region));

	const gl_api& gl{this->gl()};
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.set_clear_depth(depth);
	gl.set_clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//

void tr::graphics_context::draw(primitive type, usize offset, usize vertices)
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	assert_valid_drawing_state(check_index_buffer::no);
#endif

	gl().draw_arrays(std::to_underlying(type), offset, vertices);
}

void tr::graphics_context::draw_instances(primitive type, usize offset, usize vertices, int instances)
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	assert_valid_drawing_state(check_index_buffer::no);
#endif

	gl().draw_arrays_instanced(std::to_underlying(type), offset, vertices, instances);
}

void tr::graphics_context::draw_indexed(primitive type, usize offset, usize indices)
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	assert_valid_drawing_state(check_index_buffer::yes);
#endif

	gl().draw_elements(std::to_underlying(type), indices, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(offset * sizeof(u16)));
}

void tr::graphics_context::draw_indexed_instances(primitive type, usize offset, usize indices, int instances)
{
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	assert_valid_drawing_state(check_index_buffer::yes);
#endif

	gl().draw_elements_instanced(std::to_underlying(type), indices, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(offset * sizeof(u16)),
								 instances);
}

//

SDL_GLContextState* tr::graphics_context::unwrap() const
{
	return m_ptr.get();
}

//

const tr::gl_api& tr::graphics_context::gl() const
{
	SDL_GL_MakeCurrent(m_window, m_ptr.get());
	return m_gl_api;
}

#ifdef TR_ENABLE_CHECKED_GRAPHICS
tr::graphics_object_registry& tr::graphics_context::registry()
{
	return m_ptr.get_deleter().registry;
}
#endif

//

void tr::graphics_context::move_label(unsigned int type, unsigned int old_id, unsigned int new_id)
{
	const gl_api& gl{this->gl()};

	int label_length;
	gl.get_object_label(type, old_id, 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label(label_length, '\0');
		gl.get_object_label(type, old_id, label_length, &label_length, label.data());
		gl.set_object_label(type, new_id, label.size(), label.data());
		gl.set_object_label(type, old_id, 0, nullptr);
	}
}

//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
void tr::graphics_context::check_typed_vertex_buffer(std::string label, int slot, std::span<const vertex_attribute> attrs)
{
	TR_ASSERT(usize(slot) < m_set_vertex_format_debug_info.bindings.size(),
			  "Tried to set vertex buffer '{}' to invalid slot {} (max in vertex format '{}': {}).", label, slot,
			  m_set_vertex_format_debug_info.label, m_set_vertex_format_debug_info.bindings.size());

	const std::span<const vertex_attribute> ref{m_set_vertex_format_debug_info.bindings.begin()[slot].attrs};
	TR_ASSERT(attrs.size() == ref.size(),
			  "Tried to set vertex buffer '{}' of a different type from the one in vertex format '{}' (has {} attributes instead of {}).",
			  label, m_set_vertex_format_debug_info.label, attrs.size(), ref.size());
	for (usize i = 0; i < attrs.size(); ++i) {
		const vertex_attribute& lhs{attrs.begin()[i]};
		const vertex_attribute& rhs{ref.begin()[i]};
		TR_ASSERT(lhs.type == rhs.type && lhs.elements == rhs.elements,
				  "Tried to set vertex buffer '{}' of a type different from than the one in vertex format '{}' (expected '{}' in "
				  "attribute {}, got '{}').",
				  label, m_set_vertex_format_debug_info.label, rhs, i, lhs);
	}
}
#endif

//

#ifdef TR_ENABLE_CHECKED_GRAPHICS
void tr::graphics_context::assert_valid_drawing_state(check_index_buffer check_index_buffer)
{
	graphics_object_registry& registry{this->registry()};

	TR_ASSERT(m_set_framebuffer_debug_info.id == graphics_object_id::invalid ||
				  registry.framebuffers.contains(m_set_framebuffer_debug_info.id),
			  "Tried to perform a drawing operation with an invalid set render target on framebuffer '{}'.",
			  m_set_framebuffer_debug_info.label);
	TR_ASSERT(registry.shader_pipelines.contains(m_set_shader_pipeline_debug_info.id),
			  "Tried to perform a drawing operation with an invalid set shader pipeline '{}'.", m_set_shader_pipeline_debug_info.label);
	TR_ASSERT(registry.vertex_formats.contains(m_set_vertex_format_debug_info.id),
			  "Tried to perform a drawing operation with an invalid set vertex format '{}'.", m_set_vertex_format_debug_info.label);
	TR_ASSERT(registry.buffers.contains(m_set_vertex_buffer_debug_info.id),
			  "Tried to perform a drawing operation with an invalid set vertex buffer '{}'.", m_set_vertex_buffer_debug_info.label);
	TR_ASSERT(check_index_buffer == check_index_buffer::no || registry.buffers.contains(m_set_index_buffer_debug_info.id),
			  "Tried to perform a drawing operation with an invalid set index buffer '{}'.", m_set_index_buffer_debug_info.label);
}
#endif