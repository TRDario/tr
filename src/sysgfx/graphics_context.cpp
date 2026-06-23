#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/window.hpp"
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
	: begin_query{gl_function_address("glBeginQuery")}
	, bind_buffer{gl_function_address("glBindBuffer")}
	, bind_buffer_base{gl_function_address("glBindBufferBase")}
	, bind_buffer_range{gl_function_address("glBindBufferRange")}
	, bind_framebuffer{gl_function_address("glBindFramebuffer")}
	, bind_program_pipeline{gl_function_address("glBindProgramPipeline")}
	, bind_textures{gl_function_address("glBindTextures")}
	, bind_vertex_array{gl_function_address("glBindVertexArray")}
	, bind_vertex_buffer{gl_function_address("glBindVertexBuffer")}
	, blend_equation_separate{gl_function_address("glBlendEquationSeparate")}
	, blend_func_separate{gl_function_address("glBlendFuncSeparate")}
	, clear{gl_function_address("glClear")}
	, clear_color{gl_function_address("glClearColor")}
	, clear_depth{gl_function_address("glClearDepth")}
	, clear_stencil{gl_function_address("glClearStencil")}
	, clear_tex_image{gl_function_address("glClearTexImage")}
	, clear_tex_sub_image{gl_function_address("glClearTexSubImage")}
	, copy_image_sub_data{gl_function_address("glCopyImageSubData")}
	, create_buffers{gl_function_address("glCreateBuffers")}
	, create_framebuffers{gl_function_address("glCreateFramebuffers")}
	, create_program_pipelines{gl_function_address("glCreateProgramPipelines")}
	, create_shader_program_v{gl_function_address("glCreateShaderProgramv")}
	, create_textures{gl_function_address("glCreateTextures")}
	, create_vertex_arrays{gl_function_address("glCreateVertexArrays")}
	, debug_message_callback{gl_function_address("glDebugMessageCallback")}
	, debug_message_control{gl_function_address("glDebugMessageControl")}
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
	, enable_vertex_array_attrib{gl_function_address("glEnableVertexArrayAttrib")}
	, end_query{gl_function_address("glEndQuery")}
	, gen_queries{gl_function_address("glGenQueries")}
	, generate_texture_mipmap{gl_function_address("glGenerateTextureMipmap")}
	, get_error{gl_function_address("glGetError")}
	, get_named_buffer_parameter_iv{gl_function_address("glGetNamedBufferParameteriv")}
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
	, map_named_buffer_range{gl_function_address("glMapNamedBufferRange")}
	, named_buffer_storage{gl_function_address("glNamedBufferStorage")}
	, named_buffer_sub_data{gl_function_address("glNamedBufferSubData")}
	, named_framebuffer_texture{gl_function_address("glNamedFramebufferTexture")}
	, object_label{gl_function_address("glObjectLabel")}
	, pixel_store_i{gl_function_address("glPixelStorei")}
	, polygon_mode{gl_function_address("glPolygonMode")}
	, program_uniform_1f{gl_function_address("glProgramUniform1f")}
	, program_uniform_1fv{gl_function_address("glProgramUniform1fv")}
	, program_uniform_2f{gl_function_address("glProgramUniform2f")}
	, program_uniform_2fv{gl_function_address("glProgramUniform2fv")}
	, program_uniform_3f{gl_function_address("glProgramUniform3f")}
	, program_uniform_3fv{gl_function_address("glProgramUniform3fv")}
	, program_uniform_4f{gl_function_address("glProgramUniform4f")}
	, program_uniform_4fv{gl_function_address("glProgramUniform4fv")}
	, program_uniform_1i{gl_function_address("glProgramUniform1i")}
	, program_uniform_1iv{gl_function_address("glProgramUniform1iv")}
	, program_uniform_2i{gl_function_address("glProgramUniform2i")}
	, program_uniform_2iv{gl_function_address("glProgramUniform2iv")}
	, program_uniform_3i{gl_function_address("glProgramUniform3i")}
	, program_uniform_3iv{gl_function_address("glProgramUniform3iv")}
	, program_uniform_4i{gl_function_address("glProgramUniform4i")}
	, program_uniform_4iv{gl_function_address("glProgramUniform4iv")}
	, program_uniform_1ui{gl_function_address("glProgramUniform1ui")}
	, program_uniform_1uiv{gl_function_address("glProgramUniform1uiv")}
	, program_uniform_2ui{gl_function_address("glProgramUniform2ui")}
	, program_uniform_2uiv{gl_function_address("glProgramUniform2uiv")}
	, program_uniform_3ui{gl_function_address("glProgramUniform3ui")}
	, program_uniform_3uiv{gl_function_address("glProgramUniform3uiv")}
	, program_uniform_4ui{gl_function_address("glProgramUniform4ui")}
	, program_uniform_4uiv{gl_function_address("glProgramUniform4uiv")}
	, program_uniform_matrix_2fv{gl_function_address("glProgramUniformMatrix2fv")}
	, program_uniform_matrix_3fv{gl_function_address("glProgramUniformMatrix3fv")}
	, program_uniform_matrix_4fv{gl_function_address("glProgramUniformMatrix4fv")}
	, program_uniform_matrix_2x3fv{gl_function_address("glProgramUniformMatrix2x3fv")}
	, program_uniform_matrix_2x4fv{gl_function_address("glProgramUniformMatrix2x4fv")}
	, program_uniform_matrix_3x2fv{gl_function_address("glProgramUniformMatrix3x2fv")}
	, program_uniform_matrix_3x4fv{gl_function_address("glProgramUniformMatrix3x4fv")}
	, program_uniform_matrix_4x2fv{gl_function_address("glProgramUniformMatrix4x2fv")}
	, program_uniform_matrix_4x3fv{gl_function_address("glProgramUniformMatrix4x3fv")}
	, scissor{gl_function_address("glScissor")}
	, texture_parameter_fv{gl_function_address("glTextureParameterfv")}
	, texture_parameter_i{gl_function_address("glTextureParameteri")}
	, texture_storage_2d{gl_function_address("glTextureStorage2D")}
	, texture_sub_image_2d{gl_function_address("glTextureSubImage2D")}
	, unmap_named_buffer{gl_function_address("glUnmapNamedBuffer")}
	, use_program_stages{gl_function_address("glUseProgramStages")}
	, vertex_array_attrib_binding{gl_function_address("glVertexArrayAttribBinding")}
	, vertex_array_attrib_format{gl_function_address("glVertexArrayAttribFormat")}
	, vertex_array_binding_divisor{gl_function_address("glVertexArrayBindingDivisor")}
	, viewport{gl_function_address("glViewport")}
{
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
#ifdef TR_ENABLE_ASSERTS
	TR_LOG(log, severity::info, "Created an OpenGL context.");
	TR_LOG_CONTINUE(log, "Vendor: {}", reinterpret_cast<const char*>(m_functions.get_string(GL_VENDOR)));
	TR_LOG_CONTINUE(log, "Renderer: {}", reinterpret_cast<const char*>(m_functions.get_string(GL_RENDERER)));
	TR_LOG_CONTINUE(log, "Version: {}", reinterpret_cast<const char*>(m_functions.get_string(GL_VERSION)));
#endif
	m_functions.enable(GL_BLEND);
	m_functions.enable(GL_SCISSOR_TEST);

#ifdef TR_ENABLE_ASSERTS
	setup_debugging();
#endif
}

void tr::graphics_context::deleter::operator()(SDL_GLContextState* context) const
{
	SDL_GL_DestroyContext(context);
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
		TR_SET_LABEL(*m_vertex2_format, "(tr) 2D Vertex Format");
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

	gl.polygon_mode(GL_FRONT_AND_BACK, arg ? GL_LINE : GL_FILL);
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
		gl.viewport(target.m_viewport.tl.x, bottom, target.m_viewport.size.x, target.m_viewport.size.y);
		changed_render_target = true;
	}
	if (!m_render_target.has_value() || m_render_target->m_fbo_size != target.m_fbo_size ||
		m_render_target->m_scissor_box != target.m_scissor_box) {
		const int bottom{target.m_fbo_size.y - target.m_scissor_box.tl.y - target.m_scissor_box.size.y};
		gl.scissor(target.m_scissor_box.tl.x, bottom, target.m_scissor_box.size.x, target.m_scissor_box.size.y);
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

	gl.blend_equation_separate(to_underlying(bm.rgb_fn), to_underlying(bm.alpha_fn));
	gl.blend_func_separate(to_underlying(bm.rgb_src), to_underlying(bm.rgb_dst), to_underlying(bm.alpha_src), to_underlying(bm.alpha_dst));
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
	gl.clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer(const tr::rgbaf& color, double depth, int stencil)
{
	const functions& gl{make_current_and_return_functions()};

	set_render_target(backbuffer());
	gl.clear_color(color.r, color.g, color.b, color.a);
	gl.clear_depth(depth);
	gl.clear_stencil(stencil);
	gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(const tr::irect2& rect, const tr::rgbaf& color)
{
	const functions& gl{make_current_and_return_functions()};

	set_render_target(backbuffer().cropped(rect));
	gl.clear_color(color.r, color.g, color.b, color.a);
	gl.clear(GL_COLOR_BUFFER_BIT);
}

void tr::graphics_context::clear_backbuffer_region(const tr::irect2& rect, const tr::rgbaf& color, double depth, int stencil)
{
	const functions& gl{make_current_and_return_functions()};

	set_render_target(backbuffer().cropped(rect));
	gl.clear_color(color.r, color.g, color.b, color.a);
	gl.clear_depth(depth);
	gl.clear_stencil(stencil);
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

#ifdef TR_ENABLE_ASSERTS

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

// OpenGL debug log callback.
static void gl_debug_cb(unsigned int source, unsigned int type, unsigned int, unsigned int severity, int length, const char* message,
						const void*)
{
	const std::string_view msg{message, tr::usize(length)};
	TR_LOG(tr::log, tr_severity(severity), "OpenGL: [{}] | [{}] | [{}] | {}", gl_severity(severity), gl_type(type), gl_source(source), msg);
}

// Sets up OpenGL debugging.
void tr::graphics_context::setup_debugging()
{
	m_functions.enable(GL_DEBUG_OUTPUT);
	m_functions.enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	m_functions.debug_message_callback(gl_debug_cb, nullptr);
	m_functions.debug_message_control(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	m_functions.debug_message_control(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
}

#endif

//

const tr::graphics_context::functions& tr::graphics_context::make_current_and_return_functions()
{
	SDL_GL_MakeCurrent(m_window, m_ptr.get());

	return m_functions;
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
	constexpr std::optional<texture_ref> free_texture_unit{};

	TR_ASSERT(contains(m_texture_units, free_texture_unit), "Ran out of texture units for shaders.");

	auto free_unit_it{std::ranges::find(m_texture_units, free_texture_unit)};
	free_unit_it->emplace();
	return std::distance(m_texture_units.begin(), free_unit_it);
}

void tr::graphics_context::set_texture_unit(unsigned int unit, texture_ref texture)
{
	TR_ASSERT(m_texture_units[unit].has_value(), "Tried to set unallocated texture unit {}.", unit);

	const functions& gl{make_current_and_return_functions()};

	if (!texture.empty()) {
		if (m_texture_units[unit] != texture && texture.m_ref->m_handle != 0) {
			gl.bind_textures(unit, 1, &texture.m_ref->m_handle);
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

#ifdef TR_ENABLE_ASSERTS
void tr::graphics_context::move_label(unsigned int type, unsigned int old_id, unsigned int new_id)
{
	const functions& gl{make_current_and_return_functions()};

	int label_length;
	gl.get_object_label(type, old_id, 0, &label_length, nullptr);
	if (label_length > 0) {
		std::string label(label_length, '\0');
		gl.get_object_label(type, old_id, label_length, &label_length, label.data());
		gl.object_label(type, new_id, label.size(), label.data());
		gl.object_label(type, old_id, 0, nullptr);
	}
}
#endif