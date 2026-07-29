///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an internally used structure holding OpenGL function pointers.                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// OpenGL debug callback signature.
	using gl_debug_callback = void (*)(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length,
									   const char* message, const void* userParam);

	// Structure holding OpenGL function pointers.
	struct gl_api {
		// glTextureStorage2D
		void (*allocate_2d_texture_storage)(unsigned int texture, int levels, unsigned int internalformat, int width, int height);
		// glNamedBufferStorage
		void (*allocate_buffer_storage)(unsigned int buffer, std::intptr_t size, const void* data, unsigned int flags);
		// glBeginQuery
		void (*begin_query)(unsigned int target, unsigned int id);
		// glBindBuffer
		void (*bind_buffer)(unsigned int target, unsigned int buffer);
		// glBindBufferBase
		void (*bind_buffer_base)(unsigned int target, unsigned int index, unsigned int buffer);
		// glBindBufferRange
		void (*bind_buffer_range)(unsigned int target, unsigned int index, unsigned int buffer, std::intptr_t offset, std::intptr_t size);
		// glBindFramebuffer
		void (*bind_framebuffer)(unsigned int target, unsigned int framebuffer);
		// glBindProgramPipeline
		void (*bind_program_pipeline)(unsigned int pipeline);
		// glBindTextures
		void (*bind_textures)(unsigned int first, int count, const unsigned int* textures);
		// glBindVertexArray
		void (*bind_vertex_array)(unsigned int array);
		// glBindVertexBuffer
		void (*bind_vertex_buffer)(unsigned int bindingindex, unsigned int buffer, std::intptr_t offset, int stride);
		// glClear
		void (*clear)(unsigned int mask);
		// glClearTexImage
		void (*clear_texture_image)(unsigned int texture, int level, unsigned int format, unsigned int type, const void* data);
		// glClearTexSubImage
		void (*clear_texture_sub_image)(unsigned int texture, int level, int xoffset, int yoffset, int zoffset, int width, int height,
										int depth, unsigned int format, unsigned int type, const void* data);
		// glCopyImageSubData
		void (*copy_image_sub_data)(unsigned int srcName, unsigned int srcTarget, int srcLevel, int srcX, int srcY, int srcZ,
									unsigned int dstName, unsigned int dstTarget, int dstLevel, int dstX, int dstY, int dstZ, int srcWidth,
									int srcHeight, int srcDepth);
		// glCreateBuffers
		void (*create_buffers)(int n, unsigned int* buffers);
		// glCreateFramebuffers
		void (*create_framebuffers)(int n, unsigned int* framebuffers);
		// glCreateProgramPipelines
		void (*create_program_pipelines)(int n, unsigned int* pipelines);
		// glCreateShaderProgramv
		unsigned int (*create_shader_program_v)(unsigned int type, int count, const char** strings);
		// glCreateTextures
		void (*create_textures)(unsigned int target, int n, unsigned int* textures);
		// glCreateVertexArrays
		void (*create_vertex_arrays)(int n, unsigned int* arrays);
		// glDeleteBuffers
		void (*delete_buffers)(int n, const unsigned int* buffers);
		// glDeleteFramebuffers
		void (*delete_framebuffers)(int n, const unsigned int* framebuffers);
		// glDeleteProgram
		void (*delete_program)(unsigned int program);
		// glDeleteProgramPipelines
		void (*delete_program_pipelines)(int n, const unsigned int* pipelines);
		// glDeleteQueries
		void (*delete_queries)(int n, const unsigned int* queries);
		// glDeleteTextures
		void (*delete_textures)(int n, const unsigned int* textures);
		// glDeleteVertexArrays
		void (*delete_vertex_arrays)(int n, const unsigned int* arrays);
		// glDisable
		void (*disable)(unsigned int cap);
		// glDrawArrays
		void (*draw_arrays)(unsigned int mode, int first, int count);
		// glDrawArraysInstanced
		void (*draw_arrays_instanced)(unsigned int mode, int first, int count, int instancecount);
		// glDrawElements
		void (*draw_elements)(unsigned int mode, int count, unsigned int type, const void* indices);
		// glDrawElementsInstanced
		void (*draw_elements_instanced)(unsigned int mode, int count, unsigned int type, const void* indices, int instancecount);
		// glEnable
		void (*enable)(unsigned int cap);
		// glEnableVertexArrayAttrib
		void (*enable_vertex_array_attribute)(unsigned int vaobj, unsigned int index);
		// glEndQuery
		void (*end_query)(unsigned int target);
		// glGenQueries
		void (*generate_queries)(int n, unsigned int* ids);
		// glGenerateTextureMipmap
		void (*generate_texture_mipmap)(unsigned int texture);
		// glGetError
		unsigned int (*get_error)();
		// glGetNamedBufferParameteriv
		void (*get_buffer_parameter_iv)(unsigned int buffer, unsigned int pname, int* params);
		// glGetIntegerv
		void (*get_integer_v)(unsigned int pname, int* data);
		// glGetObjectLabel
		void (*get_object_label)(unsigned int identifier, unsigned int name, int bufSize, int* length, char* label);
		// glGetProgramInfoLog
		void (*get_program_info_log)(unsigned int program, int maxLength, int* length, char* infoLog);
		// glGetProgramInterfaceiv
		void (*get_program_interface_iv)(unsigned int program, unsigned int programInterface, unsigned int pname, int* params);
		// glGetProgramiv
		void (*get_program_iv)(unsigned int program, unsigned int pname, int* params);
		// glGetProgramResourceiv
		void (*get_program_resource_iv)(unsigned int program, unsigned int programInterface, unsigned int index, int propCount,
										const unsigned int* props, int bufSize, int* length, int* params);
		// glGetProgramResourceName
		void (*get_program_resource_name)(unsigned int program, unsigned int programInterface, unsigned int index, int bufSize, int* length,
										  char* name);
		// glGetQueryObjecti64v
		void (*get_query_object_i64v)(unsigned int id, unsigned int pname, std::int64_t* params);
		// glGetString
		const unsigned char* (*get_string)(unsigned int name);
		// glGetTextureParameterfv
		void (*get_texture_parameter_fv)(unsigned int texture, unsigned int pname, float* params);
		// glGetTextureParameteriv
		void (*get_texture_parameter_iv)(unsigned int texture, unsigned int pname, int* params);
		// glInvalidateBufferData
		void (*invalidate_buffer_data)(unsigned int buffer);
		// glMapNamedBufferRange
		void* (*map_buffer_range)(unsigned int buffer, std::intptr_t offset, std::intptr_t length, unsigned int access);
		// glTextureSubImage2D
		void (*set_2d_texture_sub_image)(unsigned int texture, int level, int xoffset, int yoffset, int width, int height,
										 unsigned int format, unsigned int type, const void* pixels);
		// glNamedBufferSubData
		void (*set_buffer_sub_data)(unsigned int buffer, std::intptr_t offset, std::intptr_t size, const void* data);
		// glClearColor
		void (*set_clear_color)(float red, float green, float blue, float alpha);
		// glClearDepth
		void (*set_clear_depth)(double depth);
		// glClearStencil
		void (*set_clear_stencil)(int s);
		// glDebugMessageCallback
		void (*set_debug_message_callback)(gl_debug_callback callback, const void* userParam);
		// glDebugMessageControl
		void (*set_debug_message_control)(unsigned int source, unsigned int type, unsigned int severity, int count, const unsigned int* ids,
										  bool enabled);
		// glNamedFramebufferTexture
		void (*set_framebuffer_texture)(unsigned int framebuffer, unsigned int attachment, unsigned int texture, int level);
		// glObjectLabel
		void (*set_object_label)(unsigned int identifier, unsigned int name, int length, const char* label);
		// glPixelStorei
		void (*set_pixel_store_i)(unsigned int pname, int param);
		// glPolygonMode
		void (*set_polygon_mode)(unsigned int face, unsigned int mode);
		// glProgramUniform1f
		void (*set_program_uniform_1f)(unsigned int program, int location, float v0);
		// glProgramUniform1fv
		void (*set_program_uniform_1fv)(unsigned int program, int location, int count, const float* value);
		// glProgramUniform2f
		void (*set_program_uniform_2f)(unsigned int program, int location, float v0, float v1);
		// glProgramUniform2fv
		void (*set_program_uniform_2fv)(unsigned int program, int location, int count, const float* value);
		// glProgramUniform3f
		void (*set_program_uniform_3f)(unsigned int program, int location, float v0, float v1, float v2);
		// glProgramUniform3fv
		void (*set_program_uniform_3fv)(unsigned int program, int location, int count, const float* value);
		// glProgramUniform4f
		void (*set_program_uniform_4f)(unsigned int program, int location, float v0, float v1, float v2, float v3);
		// glProgramUniform4fv
		void (*set_program_uniform_4fv)(unsigned int program, int location, int count, const float* value);
		// glProgramUniform1i
		void (*set_program_uniform_1i)(unsigned int program, int location, int v0);
		// glProgramUniform1iv
		void (*set_program_uniform_1iv)(unsigned int program, int location, int count, const int* value);
		// glProgramUniform2i
		void (*set_program_uniform_2i)(unsigned int program, int location, int v0, int v1);
		// glProgramUniform2iv
		void (*set_program_uniform_2iv)(unsigned int program, int location, int count, const int* value);
		// glProgramUniform3i
		void (*set_program_uniform_3i)(unsigned int program, int location, int v0, int v1, int v2);
		// glProgramUniform3iv
		void (*set_program_uniform_3iv)(unsigned int program, int location, int count, const int* value);
		// glProgramUniform4i
		void (*set_program_uniform_4i)(unsigned int program, int location, int v0, int v1, int v2, int v3);
		// glProgramUniform4iv
		void (*set_program_uniform_4iv)(unsigned int program, int location, int count, const int* value);
		// glProgramUniform1ui
		void (*set_program_uniform_1ui)(unsigned int program, int location, unsigned int v0);
		// glProgramUniform1uiv
		void (*set_program_uniform_1uiv)(unsigned int program, int location, int count, const unsigned int* value);
		// glProgramUniform2ui
		void (*set_program_uniform_2ui)(unsigned int program, int location, unsigned int v0, unsigned int v1);
		// glProgramUniform2uiv
		void (*set_program_uniform_2uiv)(unsigned int program, int location, int count, const unsigned int* value);
		// glProgramUniform3ui
		void (*set_program_uniform_3ui)(unsigned int program, int location, unsigned int v0, unsigned int v1, unsigned int v2);
		// glProgramUniform3uiv
		void (*set_program_uniform_3uiv)(unsigned int program, int location, int count, const unsigned int* value);
		// glProgramUniform4ui
		void (*set_program_uniform_4ui)(unsigned int program, int location, unsigned int v0, unsigned int v1, unsigned int v2,
										unsigned int v3);
		// glProgramUniform4uiv
		void (*set_program_uniform_4uiv)(unsigned int program, int location, int count, const unsigned int* value);
		// glProgramUniformMatrix2fv
		void (*set_program_uniform_matrix2fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glProgramUniformMatrix3fv
		void (*set_program_uniform_matrix3fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glProgramUniformMatrix4fv
		void (*set_program_uniform_matrix4fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glProgramUniformMatrix2x3fv
		void (*set_program_uniform_matrix2x3fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glProgramUniformMatrix2x4fv
		void (*set_program_uniform_matrix2x4fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glProgramUniformMatrix3x2fv
		void (*set_program_uniform_matrix3x2fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glProgramUniformMatrix3x4fv
		void (*set_program_uniform_matrix3x4fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glProgramUniformMatrix4x2fv
		void (*set_program_uniform_matrix4x2fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glProgramUniformMatrix4x3fv
		void (*set_program_uniform_matrix4x3fv)(unsigned int program, int location, int count, bool transpose, const float* value);
		// glScissor
		void (*set_scissor)(int x, int y, int width, int height);
		// glBlendEquationSeparate
		void (*set_separate_blend_equations)(unsigned int modeRGB, unsigned int modeAlpha);
		// glBlendFuncSeparate
		void (*set_separate_blend_function)(unsigned int srcRGB, unsigned int dstRGB, unsigned int srcAlpha, unsigned int dstAlpha);
		// glTextureParameterfv
		void (*set_texture_parameter_fv)(unsigned int texture, unsigned int pname, const float* params);
		// glTextureParameteri
		void (*set_texture_parameter_i)(unsigned int texture, unsigned int pname, int param);
		// glVertexArrayAttribBinding
		void (*set_vertex_array_attribute_binding)(unsigned int vaobj, unsigned int attribindex, unsigned int bindingindex);
		// glVertexArrayAttribFormat
		void (*set_vertex_array_attribute_format)(unsigned int vaobj, unsigned int attribindex, int size, unsigned int type,
												  bool normalized, unsigned int relativeoffset);
		// glVertexArrayBindingDivisor
		void (*set_vertex_array_binding_divisor)(unsigned int vaobj, unsigned int bindingindex, unsigned int divisor);
		// glViewport
		void (*set_viewport)(int x, int y, int width, int height);
		// glUnmapNamedBuffer
		bool (*unmap_buffer)(unsigned int buffer);
		// glUseProgramStages
		void (*use_program_stages)(unsigned int pipeline, unsigned int stages, unsigned int program);

	  private:
		// Loads OpenGL function pointers.
		gl_api();

		friend class graphics_context;
	};
} // namespace tr