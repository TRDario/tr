/// @file
/// @brief Implements gl_api.hpp.

#include "../../include/tr/sysgfx/gl_api.hpp"
#include <SDL3/SDL.h>

//

namespace tr
{
	namespace
	{
		/// Hack needed to overload loaded_gl_function_proxy conversion operator.
		/// @tparam Return Function return type.
		/// @tparam Args Function argument types.
		template <typename Return, typename... Args>
		using function_pointer = Return (*)(Args...);

		/// Wrapper around an SDL_FunctionPointer that automatically casts it to another function pointer type.
		struct loaded_gl_function_proxy
		{
			/// Base pointer type.
			SDL_FunctionPointer ptr;

			//

			/// Converts to any function pointer.
			/// @tparam Return Function return type.
			/// @tparam Args Function argument types.
			template <typename Return, typename... Args>
			operator function_pointer<Return, Args...>()
			{
				return reinterpret_cast<function_pointer<Return, Args...>>(ptr);
			}
		};

		/// Wraps SDL_GL_GetProcAddress to return an OpenGL function proxy.
		/// @param name Function name.
		/// @return OpenGL function proxy.
		loaded_gl_function_proxy gl_function_address(const char* name)
		{
			return {SDL_GL_GetProcAddress(name)};
		}
	} // namespace
} // namespace tr

tr::gl_api::gl_api()
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