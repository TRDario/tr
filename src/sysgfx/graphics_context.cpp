///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements graphics_context.hpp.                                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/window.hpp"
#include "../../include/tr/utility/enum.hpp"
#include <SDL3/SDL.h>

////////////////////////////////////////////////////// GRAPHICS CONTEXT OPENING ERROR /////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////// OPENGL FUNCTIONS ////////////////////////////////////////////////////////////

// Wrapper around an SDL_FunctionPointer that automatically casts it to another function pointer type.
struct loaded_gl_function_proxy {
	SDL_FunctionPointer ptr;

	template <typename Return, typename... Args> using function_pointer = Return (*)(Args...);
	template <typename Return, typename... Args> operator function_pointer<Return, Args...>()
	{
		return reinterpret_cast<function_pointer<Return, Args...>>(ptr);
	}
};

// Wraps SDL_GL_GetProcAddress to return an OpenGL function proxy.
static loaded_gl_function_proxy gl_function_address(const char* name)
{
	return {SDL_GL_GetProcAddress(name)};
}

tr::graphics_context::functions::functions()
	: allocate_2d_texture_storage{gl_function_address("glTextureStorage2D")}
	, allocate_buffer_storage{gl_function_address("glNamedBufferStorage")}
	, begin_query{gl_function_address("glBeginQuery")}
	, bind_buffer{gl_function_address("glBindBuffer")}
	, bind_buffer_base{gl_function_address("glBindBufferBase")}
	, bind_buffer_range{gl_function_address("glBindBufferRange")}
	, bind_framebuffer{gl_function_address("glBindFramebuffer")}
	, bind_program_pipeline{gl_function_address("glBindProgramPipeline")}
	, bind_textures{gl_function_address("glBindTextures")}
	, bind_vertex_array{gl_function_address("glBindVertexArray")}
	, bind_vertex_buffer{gl_function_address("glBindVertexBuffer")}
	, clear{gl_function_address("glClear")}
	, clear_texture_image{gl_function_address("glClearTexImage")}
	, clear_texture_sub_image{gl_function_address("glClearTexSubImage")}
	, copy_image_sub_data{gl_function_address("glCopyImageSubData")}
	, create_buffers{gl_function_address("glCreateBuffers")}
	, create_framebuffers{gl_function_address("glCreateFramebuffers")}
	, create_program_pipelines{gl_function_address("glCreateProgramPipelines")}
	, create_shader_program_v{gl_function_address("glCreateShaderProgramv")}
	, create_textures{gl_function_address("glCreateTextures")}
	, create_vertex_arrays{gl_function_address("glCreateVertexArrays")}
	, delete_buffers{gl_function_address("glDeleteBuffers")}
	, delete_framebuffers{gl_function_address("glDeleteFramebuffers")}
	, delete_program{gl_function_address("glDeleteProgram")}
	, delete_program_pipelines{gl_function_address("glDeleteProgramPipelines")}
	, delete_queries{gl_function_address("glDeleteQueries")}
	, delete_textures{gl_function_address("glDeleteTextures")}
	, delete_vertex_arrays{gl_function_address("glDeleteVertexArrays")}
	, disable{gl_function_address("glDisable")}
	, draw_arrays{gl_function_address("glDrawArrays")}
	, draw_arrays_instanced{gl_function_address("glDrawArraysInstanced")}
	, draw_elements{gl_function_address("glDrawElements")}
	, draw_elements_instanced{gl_function_address("glDrawElementsInstanced")}
	, enable{gl_function_address("glEnable")}
	, enable_vertex_array_attribute{gl_function_address("glEnableVertexArrayAttrib")}
	, end_query{gl_function_address("glEndQuery")}
	, generate_queries{gl_function_address("glGenQueries")}
	, generate_texture_mipmap{gl_function_address("glGenerateTextureMipmap")}
	, get_error{gl_function_address("glGetError")}
	, get_buffer_parameter_iv{gl_function_address("glGetNamedBufferParameteriv")}
	, get_integer_v{gl_function_address("glGetIntegerv")}
	, get_object_label{gl_function_address("glGetObjectLabel")}
	, get_program_info_log{gl_function_address("glGetProgramInfoLog")}
	, get_program_interface_iv{gl_function_address("glGetProgramInterfaceiv")}
	, get_program_iv{gl_function_address("glGetProgramiv")}
	, get_program_resource_iv{gl_function_address("glGetProgramResourceiv")}
	, get_program_resource_name{gl_function_address("glGetProgramResourceName")}
	, get_query_object_i64v{gl_function_address("glGetQueryObjecti64v")}
	, get_string{gl_function_address("glGetString")}
	, get_texture_parameter_fv{gl_function_address("glGetTextureParameterfv")}
	, get_texture_parameter_iv{gl_function_address("glGetTextureParameteriv")}
	, invalidate_buffer_data{gl_function_address("glInvalidateBufferData")}
	, map_buffer_range{gl_function_address("glMapNamedBufferRange")}
	, set_2d_texture_sub_image{gl_function_address("glTextureSubImage2D")}
	, set_buffer_sub_data{gl_function_address("glNamedBufferSubData")}
	, set_clear_color{gl_function_address("glClearColor")}
	, set_clear_depth{gl_function_address("glClearDepth")}
	, set_clear_stencil{gl_function_address("glClearStencil")}
	, set_debug_message_callback{gl_function_address("glDebugMessageCallback")}
	, set_debug_message_control{gl_function_address("glDebugMessageControl")}
	, set_framebuffer_texture{gl_function_address("glNamedFramebufferTexture")}
	, set_object_label{gl_function_address("glObjectLabel")}
	, set_pixel_store_i{gl_function_address("glPixelStorei")}
	, set_polygon_mode{gl_function_address("glPolygonMode")}
	, set_program_uniform_1f{gl_function_address("glProgramUniform1f")}
	, set_program_uniform_1fv{gl_function_address("glProgramUniform1fv")}
	, set_program_uniform_2f{gl_function_address("glProgramUniform2f")}
	, set_program_uniform_2fv{gl_function_address("glProgramUniform2fv")}
	, set_program_uniform_3f{gl_function_address("glProgramUniform3f")}
	, set_program_uniform_3fv{gl_function_address("glProgramUniform3fv")}
	, set_program_uniform_4f{gl_function_address("glProgramUniform4f")}
	, set_program_uniform_4fv{gl_function_address("glProgramUniform4fv")}
	, set_program_uniform_1i{gl_function_address("glProgramUniform1i")}
	, set_program_uniform_1iv{gl_function_address("glProgramUniform1iv")}
	, set_program_uniform_2i{gl_function_address("glProgramUniform2i")}
	, set_program_uniform_2iv{gl_function_address("glProgramUniform2iv")}
	, set_program_uniform_3i{gl_function_address("glProgramUniform3i")}
	, set_program_uniform_3iv{gl_function_address("glProgramUniform3iv")}
	, set_program_uniform_4i{gl_function_address("glProgramUniform4i")}
	, set_program_uniform_4iv{gl_function_address("glProgramUniform4iv")}
	, set_program_uniform_1ui{gl_function_address("glProgramUniform1ui")}
	, set_program_uniform_1uiv{gl_function_address("glProgramUniform1uiv")}
	, set_program_uniform_2ui{gl_function_address("glProgramUniform2ui")}
	, set_program_uniform_2uiv{gl_function_address("glProgramUniform2uiv")}
	, set_program_uniform_3ui{gl_function_address("glProgramUniform3ui")}
	, set_program_uniform_3uiv{gl_function_address("glProgramUniform3uiv")}
	, set_program_uniform_4ui{gl_function_address("glProgramUniform4ui")}
	, set_program_uniform_4uiv{gl_function_address("glProgramUniform4uiv")}
	, set_program_uniform_matrix2fv{gl_function_address("glProgramUniformMatrix2fv")}
	, set_program_uniform_matrix3fv{gl_function_address("glProgramUniformMatrix3fv")}
	, set_program_uniform_matrix4fv{gl_function_address("glProgramUniformMatrix4fv")}
	, set_program_uniform_matrix2x3fv{gl_function_address("glProgramUniformMatrix2x3fv")}
	, set_program_uniform_matrix2x4fv{gl_function_address("glProgramUniformMatrix2x4fv")}
	, set_program_uniform_matrix3x2fv{gl_function_address("glProgramUniformMatrix3x2fv")}
	, set_program_uniform_matrix3x4fv{gl_function_address("glProgramUniformMatrix3x4fv")}
	, set_program_uniform_matrix4x2fv{gl_function_address("glProgramUniformMatrix4x2fv")}
	, set_program_uniform_matrix4x3fv{gl_function_address("glProgramUniformMatrix4x3fv")}
	, set_scissor{gl_function_address("glScissor")}
	, set_separate_blend_equations{gl_function_address("glBlendEquationSeparate")}
	, set_separate_blend_function{gl_function_address("glBlendFuncSeparate")}
	, set_texture_parameter_fv{gl_function_address("glTextureParameterfv")}
	, set_texture_parameter_i{gl_function_address("glTextureParameteri")}
	, set_vertex_array_attribute_binding{gl_function_address("glVertexArrayAttribBinding")}
	, set_vertex_array_attribute_format{gl_function_address("glVertexArrayAttribFormat")}
	, set_vertex_array_binding_divisor{gl_function_address("glVertexArrayBindingDivisor")}
	, set_viewport{gl_function_address("glViewport")}
	, unmap_buffer{gl_function_address("glUnmapNamedBuffer")}
	, use_program_stages{gl_function_address("glUseProgramStages")}
{
}

//////////////////////////////////////////////////////// GRAPHICS CONTEXT DEBUGGING ///////////////////////////////////////////////////////

// Gets a readable string for an OpenGL debug log message type.
static std::string_view gl_type(unsigned int value)
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
static std::string_view gl_severity(unsigned int value)
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
static tr::severity tr_severity(unsigned int value)
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

// Gets a readable string for an OpenGL debug log source.
static std::string_view gl_source(unsigned int value)
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

// OpenGL debug log callback.
static void gl_debug_cb(unsigned int source, unsigned int type, unsigned int, unsigned int severity, int length, const char* message,
						const void* user_param)
{
	tr::logger& logger{*const_cast<tr::logger*>(static_cast<const tr::logger*>(user_param))};

	const std::string_view msg{message, static_cast<tr::usize>(length)};
	if (logger.active()) {
		logger.log(tr_severity(severity), "[{}] | [{}] | [{}] | {}", gl_severity(severity), gl_type(type), gl_source(source), msg);
	}
}

///////////////////////////////////////////////////////////// GRAPHICS CONTEXT ////////////////////////////////////////////////////////////

// Creates an SDL OpenGL context.
static SDL_GLContext create_context(SDL_Window* window)
{
	SDL_GLContext context{SDL_GL_CreateContext(window)};
	if (context == nullptr) {
		throw tr::graphics_context_init_error{};
	}
	return context;
}

tr::graphics_context::graphics_context(tr::window& window)
	: m_window{window.m_ptr.get()}
	, m_ptr{create_context(m_window)}
{
	m_glapi.enable(GL_BLEND);
	m_glapi.enable(GL_SCISSOR_TEST);

	int context_flags;
	m_glapi.get_integer_v(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		m_glapi.enable(GL_DEBUG_OUTPUT);
		m_glapi.enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		m_glapi.set_debug_message_callback(gl_debug_cb, &logger);
		m_glapi.set_debug_message_control(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		m_glapi.set_debug_message_control(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

		static int logger_id{0};
		logger.replace_backend_with<console_logger>(TR_FMT::format("gfx{}", logger_id++));
	}
}

void tr::graphics_context::deleter::operator()(SDL_GLContextState* context) const
{
	SDL_GL_DestroyContext(context);
}

//

struct tr::graphics_context::info tr::graphics_context::info() const
{
	const functions& gl{make_current_and_return_functions()};
	return {
		reinterpret_cast<const char*>(gl.get_string(GL_VENDOR)),
		reinterpret_cast<const char*>(gl.get_string(GL_RENDERER)),
		reinterpret_cast<const char*>(gl.get_string(GL_VERSION)),
	};
}

//

tr::window_view tr::graphics_context::window() const
{
	return m_window;
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
	m_next_renderer_id = renderer_id{to_underlying(m_next_renderer_id) + 1};
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
	const functions& gl{make_current_and_return_functions()};

	gl.set_polygon_mode(GL_FRONT_AND_BACK, arg ? GL_LINE : GL_FILL);
}

void tr::graphics_context::set_face_culling(bool arg)
{
	const functions& gl{make_current_and_return_functions()};

	if (arg) {
		gl.enable(GL_CULL_FACE);
	}
	else {
		gl.disable(GL_CULL_FACE);
	}
}

void tr::graphics_context::set_depth_test(bool arg)
{
	const functions& gl{make_current_and_return_functions()};

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
	const functions& gl{make_current_and_return_functions()};

	bool changed_render_target{false};

	if (!m_render_target.has_value() || m_render_target->m_fbo != target.m_fbo) {
		gl.bind_framebuffer(GL_DRAW_FRAMEBUFFER, target.m_fbo);
		changed_render_target = true;
	}
	if (!m_render_target.has_value() || m_render_target->m_fbo_size != target.m_fbo_size ||
		m_render_target->m_viewport != target.m_viewport) {
		const int bottom{target.m_fbo_size.y - target.m_viewport.tl.y - target.m_viewport.size.y};
		gl.set_viewport(target.m_viewport.tl.x, bottom, target.m_viewport.size.x, target.m_viewport.size.y);
		changed_render_target = true;
	}
	if (!m_render_target.has_value() || m_render_target->m_fbo_size != target.m_fbo_size ||
		m_render_target->m_scissor_box != target.m_scissor_box) {
		const int bottom{target.m_fbo_size.y - target.m_scissor_box.tl.y - target.m_scissor_box.size.y};
		gl.set_scissor(target.m_scissor_box.tl.x, bottom, target.m_scissor_box.size.x, target.m_scissor_box.size.y);
		changed_render_target = true;
	}

	if (changed_render_target) {
		m_render_target = target;
	}
}

void tr::graphics_context::set_shader_pipeline(const shader_pipeline& pipeline)
{
	const functions& gl{make_current_and_return_functions()};

	gl.bind_program_pipeline(pipeline.m_ppo.get());
}

void tr::graphics_context::set_blend_mode(const blend_mode& bm)
{
	const functions& gl{make_current_and_return_functions()};

	gl.set_separate_blend_equations(to_underlying(bm.rgb_fn), to_underlying(bm.alpha_fn));
	gl.set_separate_blend_function(to_underlying(bm.rgb_src), to_underlying(bm.rgb_dst), to_underlying(bm.alpha_src),
								   to_underlying(bm.alpha_dst));
}

void tr::graphics_context::set_vertex_format(const vertex_format& format)
{
	const functions& gl{make_current_and_return_functions()};

#ifdef TR_ENABLE_GL_CHECKS
	m_vertex_format_bindings = format.m_bindings;
	m_vertex_format_label = format.label();
#endif

	gl.bind_vertex_array(format.m_vao.get());
}

void tr::graphics_context::set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride)
{
	const functions& gl{make_current_and_return_functions()};

	gl.bind_vertex_buffer(slot, buffer.id(), offset, stride);
}

void tr::graphics_context::set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride)
{
	const functions& gl{make_current_and_return_functions()};

	gl.bind_vertex_buffer(slot, buffer.id(), offset, stride);
}

void tr::graphics_context::set_index_buffer(const static_index_buffer& buffer)
{
	const functions& gl{make_current_and_return_functions()};

	gl.bind_buffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id());
}

void tr::graphics_context::set_index_buffer(const dyn_index_buffer& buffer)
{
	const functions& gl{make_current_and_return_functions()};

	gl.bind_buffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id());
}

//

void tr::graphics_context::clear_backbuffer(const tr::rgbaf& color)
{
	const functions& gl{make_current_and_return_functions()};

	set_render_target(backbuffer());
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer(const tr::rgbaf& color, double depth, int stencil)
{
	const functions& gl{make_current_and_return_functions()};

	set_render_target(backbuffer());
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.set_clear_depth(depth);
	gl.set_clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(const rectangle<int>& region, const tr::rgbaf& color)
{
	const functions& gl{make_current_and_return_functions()};

	set_render_target(backbuffer().cropped(region));
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(const rectangle<int>& region, const tr::rgbaf& color, double depth, int stencil)
{
	const functions& gl{make_current_and_return_functions()};

	set_render_target(backbuffer().cropped(region));
	gl.set_clear_color(color.r, color.g, color.b, color.a);
	gl.set_clear_depth(depth);
	gl.set_clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//

void tr::graphics_context::draw(primitive type, usize offset, usize vertices)
{
	const functions& gl{make_current_and_return_functions()};

	gl.draw_arrays(to_underlying(type), offset, vertices);
}

void tr::graphics_context::draw_instances(primitive type, usize offset, usize vertices, int instances)
{
	const functions& gl{make_current_and_return_functions()};

	gl.draw_arrays_instanced(to_underlying(type), offset, vertices, instances);
}

void tr::graphics_context::draw_indexed(primitive type, usize offset, usize indices)
{
	const functions& gl{make_current_and_return_functions()};

	gl.draw_elements(to_underlying(type), indices, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(offset * sizeof(u16)));
}

void tr::graphics_context::draw_indexed_instances(primitive type, usize offset, usize indices, int instances)
{
	const functions& gl{make_current_and_return_functions()};

	gl.draw_elements_instanced(to_underlying(type), indices, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(offset * sizeof(u16)),
							   instances);
}

//

const tr::graphics_context::functions& tr::graphics_context::make_current_and_return_functions() const
{
	SDL_GL_MakeCurrent(m_window, m_ptr.get());

	return m_glapi;
}

//

bool tr::graphics_context::is_fbo_of_render_target(unsigned int fbo)
{
	return m_render_target.has_value() && m_render_target->m_fbo == fbo;
}

void tr::graphics_context::clear_render_target()
{
	m_render_target.reset();
}

//

unsigned int tr::graphics_context::allocate_texture_unit()
{
	const auto free_unit_it{std::ranges::find_if(m_texture_units, [](const std::optional<texture_ref>& v) { return !v.has_value(); })};

	TR_ASSERT(free_unit_it != m_texture_units.end(), "Ran out of texture units for shaders.");

	free_unit_it->emplace();
	return std::distance(m_texture_units.begin(), free_unit_it);
}

void tr::graphics_context::set_texture_unit(unsigned int unit, texture_ref texture)
{
	TR_ASSERT(m_texture_units[unit].has_value(), "Tried to set unallocated texture unit {}.", unit);

	const functions& gl{make_current_and_return_functions()};

	if (!texture.empty()) {
		if (m_texture_units[unit] != texture && texture->m_handle != 0) {
			gl.bind_textures(unit, 1, &texture->m_handle);
		}
	}
	m_texture_units[unit] = std::move(texture);
}

void tr::graphics_context::free_texture_unit(unsigned int unit)
{
	TR_ASSERT(m_texture_units[unit].has_value(), "Tried to free unallocated texture unit {}.", unit);

	m_texture_units[unit] = std::nullopt;
}

void tr::graphics_context::rebind_texture_units(const texture& texture)
{
	const functions& gl{make_current_and_return_functions()};

	for (int i = 0; i < 80; ++i) {
		if (m_texture_units[i] == texture) {
			gl.bind_textures(i, 1, &texture.m_handle);
		}
	}
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
	const functions& gl{make_current_and_return_functions()};

	int label_length;
	gl.get_object_label(type, old_id, 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label(label_length, '\0');
		gl.get_object_label(type, old_id, label_length, &label_length, label.data());
		gl.set_object_label(type, new_id, label.size(), label.data());
		gl.set_object_label(type, old_id, 0, nullptr);
	}
}