/// @file
/// @brief Provides a structure holding OpenGL API functions.

#pragma once
#include "../utility/common.hpp"

//

namespace tr
{
	/// OpenGL debug callback signature.
	using gl_debug_callback = void (*)(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length,
									   const char* message, const void* userParam);

	/// Structure holding OpenGL function pointers.
	struct gl_api
	{
		/// Pointer to glTextureStorage2D.
		void (*allocate_2d_texture_storage)(unsigned int texture, int levels, unsigned int internalformat, int width, int height);

		/// Pointer to glNamedBufferStorage.
		void (*allocate_buffer_storage)(unsigned int buffer, std::intptr_t size, const void* data, unsigned int flags);

		/// Pointer to glBeginQuery.
		void (*begin_query)(unsigned int target, unsigned int id);

		/// Pointer to glBindBuffer.
		void (*bind_buffer)(unsigned int target, unsigned int buffer);

		/// Pointer to glBindBufferBase.
		void (*bind_buffer_base)(unsigned int target, unsigned int index, unsigned int buffer);

		/// Pointer to glBindBufferRange.
		void (*bind_buffer_range)(unsigned int target, unsigned int index, unsigned int buffer, std::intptr_t offset, std::intptr_t size);

		/// Pointer to glBindFramebuffer.
		void (*bind_framebuffer)(unsigned int target, unsigned int framebuffer);

		/// Pointer to glBindProgramPipeline.
		void (*bind_program_pipeline)(unsigned int pipeline);

		/// Pointer to glBindTextures.
		void (*bind_textures)(unsigned int first, int count, const unsigned int* textures);

		/// Pointer to glBindVertexArray.
		void (*bind_vertex_array)(unsigned int array);

		/// Pointer to glBindVertexBuffer.
		void (*bind_vertex_buffer)(unsigned int bindingindex, unsigned int buffer, std::intptr_t offset, int stride);

		/// Pointer to glClear.
		void (*clear)(unsigned int mask);

		/// Pointer to glClearTexImage.
		void (*clear_texture_image)(unsigned int texture, int level, unsigned int format, unsigned int type, const void* data);

		/// Pointer to glClearTexSubImage.
		void (*clear_texture_sub_image)(unsigned int texture, int level, int xoffset, int yoffset, int zoffset, int width, int height,
										int depth, unsigned int format, unsigned int type, const void* data);

		/// Pointer to glCopyImageSubData.
		void (*copy_image_sub_data)(unsigned int srcName, unsigned int srcTarget, int srcLevel, int srcX, int srcY, int srcZ,
									unsigned int dstName, unsigned int dstTarget, int dstLevel, int dstX, int dstY, int dstZ, int srcWidth,
									int srcHeight, int srcDepth);

		/// Pointer to glCreateBuffers.
		void (*create_buffers)(int n, unsigned int* buffers);

		/// Pointer to glCreateFramebuffers.
		void (*create_framebuffers)(int n, unsigned int* framebuffers);

		/// Pointer to glCreateProgramPipelines.
		void (*create_program_pipelines)(int n, unsigned int* pipelines);

		/// Pointer to glCreateShaderProgramv.
		unsigned int (*create_shader_program_v)(unsigned int type, int count, const char** strings);

		/// Pointer to glCreateTextures.
		void (*create_textures)(unsigned int target, int n, unsigned int* textures);

		/// Pointer to glCreateVertexArrays.
		void (*create_vertex_arrays)(int n, unsigned int* arrays);

		/// Pointer to glDeleteBuffers.
		void (*delete_buffers)(int n, const unsigned int* buffers);

		/// Pointer to glDeleteFramebuffers.
		void (*delete_framebuffers)(int n, const unsigned int* framebuffers);

		/// Pointer to glDeleteProgram.
		void (*delete_program)(unsigned int program);

		/// Pointer to glDeleteProgramPipelines.
		void (*delete_program_pipelines)(int n, const unsigned int* pipelines);

		/// Pointer to glDeleteQueries.
		void (*delete_queries)(int n, const unsigned int* queries);

		/// Pointer to glDeleteTextures.
		void (*delete_textures)(int n, const unsigned int* textures);

		/// Pointer to glDeleteVertexArrays.
		void (*delete_vertex_arrays)(int n, const unsigned int* arrays);

		/// Pointer to glDisable.
		void (*disable)(unsigned int cap);

		/// Pointer to glDrawArrays.
		void (*draw_arrays)(unsigned int mode, int first, int count);

		/// Pointer to glDrawArraysInstanced.
		void (*draw_arrays_instanced)(unsigned int mode, int first, int count, int instancecount);

		/// Pointer to glDrawElements.
		void (*draw_elements)(unsigned int mode, int count, unsigned int type, const void* indices);

		/// Pointer to glDrawElementsInstanced.
		void (*draw_elements_instanced)(unsigned int mode, int count, unsigned int type, const void* indices, int instancecount);

		/// Pointer to glEnable.
		void (*enable)(unsigned int cap);

		/// Pointer to glEnableVertexArrayAttrib.
		void (*enable_vertex_array_attribute)(unsigned int vaobj, unsigned int index);

		/// Pointer to glEndQuery.
		void (*end_query)(unsigned int target);

		/// Pointer to glGenQueries.
		void (*generate_queries)(int n, unsigned int* ids);

		/// Pointer to glGenerateTextureMipmap.
		void (*generate_texture_mipmap)(unsigned int texture);

		/// Pointer to glGetError.
		unsigned int (*get_error)();

		/// Pointer to glGetNamedBufferParameteriv.
		void (*get_buffer_parameter_iv)(unsigned int buffer, unsigned int pname, int* params);

		/// Pointer to glGetIntegerv.
		void (*get_integer_v)(unsigned int pname, int* data);

		/// Pointer to glGetObjectLabel.
		void (*get_object_label)(unsigned int identifier, unsigned int name, int bufSize, int* length, char* label);

		/// Pointer to glGetProgramInfoLog.
		void (*get_program_info_log)(unsigned int program, int maxLength, int* length, char* infoLog);

		/// Pointer to glGetProgramInterfaceiv.
		void (*get_program_interface_iv)(unsigned int program, unsigned int programInterface, unsigned int pname, int* params);

		/// Pointer to glGetProgramiv.
		void (*get_program_iv)(unsigned int program, unsigned int pname, int* params);

		/// Pointer to glGetProgramResourceiv.
		void (*get_program_resource_iv)(unsigned int program, unsigned int programInterface, unsigned int index, int propCount,
										const unsigned int* props, int bufSize, int* length, int* params);

		/// Pointer to glGetProgramResourceName.
		void (*get_program_resource_name)(unsigned int program, unsigned int programInterface, unsigned int index, int bufSize, int* length,
										  char* name);

		/// Pointer to glGetQueryObjecti64v.
		void (*get_query_object_i64v)(unsigned int id, unsigned int pname, std::int64_t* params);

		/// Pointer to glGetString.
		const unsigned char* (*get_string)(unsigned int name);

		/// Pointer to glGetTextureParameterfv.
		void (*get_texture_parameter_fv)(unsigned int texture, unsigned int pname, float* params);

		/// Pointer to glGetTextureParameteriv.
		void (*get_texture_parameter_iv)(unsigned int texture, unsigned int pname, int* params);

		/// Pointer to glInvalidateBufferData.
		void (*invalidate_buffer_data)(unsigned int buffer);

		/// Pointer to glMapNamedBufferRange.
		void* (*map_buffer_range)(unsigned int buffer, std::intptr_t offset, std::intptr_t length, unsigned int access);

		/// Pointer to glTextureSubImage2D.
		void (*set_2d_texture_sub_image)(unsigned int texture, int level, int xoffset, int yoffset, int width, int height,
										 unsigned int format, unsigned int type, const void* pixels);

		/// Pointer to glNamedBufferSubData.
		void (*set_buffer_sub_data)(unsigned int buffer, std::intptr_t offset, std::intptr_t size, const void* data);

		/// Pointer to glClearColor.
		void (*set_clear_color)(float red, float green, float blue, float alpha);

		/// Pointer to glClearDepth.
		void (*set_clear_depth)(double depth);

		/// Pointer to glClearStencil.
		void (*set_clear_stencil)(int s);

		/// Pointer to glDebugMessageCallback.
		void (*set_debug_message_callback)(gl_debug_callback callback, const void* userParam);

		/// Pointer to glDebugMessageControl.
		void (*set_debug_message_control)(unsigned int source, unsigned int type, unsigned int severity, int count, const unsigned int* ids,
										  bool enabled);

		/// Pointer to glNamedFramebufferTexture.
		void (*set_framebuffer_texture)(unsigned int framebuffer, unsigned int attachment, unsigned int texture, int level);

		/// Pointer to glObjectLabel.
		void (*set_object_label)(unsigned int identifier, unsigned int name, int length, const char* label);

		/// Pointer to glPixelStorei.
		void (*set_pixel_store_i)(unsigned int pname, int param);

		/// Pointer to glPolygonMode.
		void (*set_polygon_mode)(unsigned int face, unsigned int mode);

		/// Pointer to glProgramUniform1f.
		void (*set_program_uniform_1f)(unsigned int program, int location, float v0);

		/// Pointer to glProgramUniform1fv.
		void (*set_program_uniform_1fv)(unsigned int program, int location, int count, const float* value);

		/// Pointer to glProgramUniform2f.
		void (*set_program_uniform_2f)(unsigned int program, int location, float v0, float v1);

		/// Pointer to glProgramUniform2fv.
		void (*set_program_uniform_2fv)(unsigned int program, int location, int count, const float* value);

		/// Pointer to glProgramUniform3f.
		void (*set_program_uniform_3f)(unsigned int program, int location, float v0, float v1, float v2);

		/// Pointer to glProgramUniform3fv.
		void (*set_program_uniform_3fv)(unsigned int program, int location, int count, const float* value);

		/// Pointer to glProgramUniform4f.
		void (*set_program_uniform_4f)(unsigned int program, int location, float v0, float v1, float v2, float v3);

		/// Pointer to glProgramUniform4fv.
		void (*set_program_uniform_4fv)(unsigned int program, int location, int count, const float* value);

		/// Pointer to glProgramUniform1i.
		void (*set_program_uniform_1i)(unsigned int program, int location, int v0);

		/// Pointer to glProgramUniform1iv.
		void (*set_program_uniform_1iv)(unsigned int program, int location, int count, const int* value);

		/// Pointer to glProgramUniform2i.
		void (*set_program_uniform_2i)(unsigned int program, int location, int v0, int v1);

		/// Pointer to glProgramUniform2iv.
		void (*set_program_uniform_2iv)(unsigned int program, int location, int count, const int* value);

		/// Pointer to glProgramUniform3i.
		void (*set_program_uniform_3i)(unsigned int program, int location, int v0, int v1, int v2);

		/// Pointer to glProgramUniform3iv.
		void (*set_program_uniform_3iv)(unsigned int program, int location, int count, const int* value);

		/// Pointer to glProgramUniform4i.
		void (*set_program_uniform_4i)(unsigned int program, int location, int v0, int v1, int v2, int v3);

		/// Pointer to glProgramUniform4iv.
		void (*set_program_uniform_4iv)(unsigned int program, int location, int count, const int* value);

		/// Pointer to glProgramUniform1ui.
		void (*set_program_uniform_1ui)(unsigned int program, int location, unsigned int v0);

		/// Pointer to glProgramUniform1uiv.
		void (*set_program_uniform_1uiv)(unsigned int program, int location, int count, const unsigned int* value);

		/// Pointer to glProgramUniform2ui.
		void (*set_program_uniform_2ui)(unsigned int program, int location, unsigned int v0, unsigned int v1);

		/// Pointer to glProgramUniform2uiv.
		void (*set_program_uniform_2uiv)(unsigned int program, int location, int count, const unsigned int* value);

		/// Pointer to glProgramUniform3ui.
		void (*set_program_uniform_3ui)(unsigned int program, int location, unsigned int v0, unsigned int v1, unsigned int v2);

		/// Pointer to glProgramUniform3uiv.
		void (*set_program_uniform_3uiv)(unsigned int program, int location, int count, const unsigned int* value);

		/// Pointer to glProgramUniform4ui.
		void (*set_program_uniform_4ui)(unsigned int program, int location, unsigned int v0, unsigned int v1, unsigned int v2,
										unsigned int v3);

		/// Pointer to glProgramUniform4uiv.
		void (*set_program_uniform_4uiv)(unsigned int program, int location, int count, const unsigned int* value);

		/// Pointer to glProgramUniformMatrix2fv.
		void (*set_program_uniform_matrix2fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glProgramUniformMatrix3fv.
		void (*set_program_uniform_matrix3fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glProgramUniformMatrix4fv.
		void (*set_program_uniform_matrix4fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glProgramUniformMatrix2x3fv.
		void (*set_program_uniform_matrix2x3fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glProgramUniformMatrix2x4fv.
		void (*set_program_uniform_matrix2x4fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glProgramUniformMatrix3x2fv.
		void (*set_program_uniform_matrix3x2fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glProgramUniformMatrix3x4fv.
		void (*set_program_uniform_matrix3x4fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glProgramUniformMatrix4x2fv.
		void (*set_program_uniform_matrix4x2fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glProgramUniformMatrix4x3fv.
		void (*set_program_uniform_matrix4x3fv)(unsigned int program, int location, int count, bool transpose, const float* value);

		/// Pointer to glScissor.
		void (*set_scissor)(int x, int y, int width, int height);

		/// Pointer to glBlendEquationSeparate.
		void (*set_separate_blend_equations)(unsigned int modeRGB, unsigned int modeAlpha);

		/// Pointer to glBlendFuncSeparate.
		void (*set_separate_blend_function)(unsigned int srcRGB, unsigned int dstRGB, unsigned int srcAlpha, unsigned int dstAlpha);

		/// Pointer to glTextureParameterfv.
		void (*set_texture_parameter_fv)(unsigned int texture, unsigned int pname, const float* params);

		/// Pointer to glTextureParameteri.
		void (*set_texture_parameter_i)(unsigned int texture, unsigned int pname, int param);

		/// Pointer to glVertexArrayAttribBinding.
		void (*set_vertex_array_attribute_binding)(unsigned int vaobj, unsigned int attribindex, unsigned int bindingindex);

		/// Pointer to glVertexArrayAttribFormat.
		void (*set_vertex_array_attribute_format)(unsigned int vaobj, unsigned int attribindex, int size, unsigned int type,
												  bool normalized, unsigned int relativeoffset);

		/// Pointer to glVertexArrayBindingDivisor.
		void (*set_vertex_array_binding_divisor)(unsigned int vaobj, unsigned int bindingindex, unsigned int divisor);

		/// Pointer to glViewport.
		void (*set_viewport)(int x, int y, int width, int height);

		/// Pointer to glUnmapNamedBuffer.
		bool (*unmap_buffer)(unsigned int buffer);

		/// Pointer to glUseProgramStages.
		void (*use_program_stages)(unsigned int pipeline, unsigned int stages, unsigned int program);

	  private:
		/// Loads OpenGL function pointers.
		gl_api();

		//

		// Uses the private constructor.
		friend class graphics_context;
	};
} // namespace tr