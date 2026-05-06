///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides OpenGL typedefs and a struct holding pointers to OpenGL functions.                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////// OPENGL TYPES //////////////////////////////////////////////////////////////

using GLboolean = bool;
using GLchar = char;
using GLubyte = unsigned char;
using GLint = int;
using GLint64 = std::int64_t;
using GLuint = unsigned int;
using GLuint64 = std::uint64_t;
using GLintptr = std::intptr_t;
using GLfloat = float;
using GLdouble = double;
using GLenum = unsigned int;
using GLbitfield = unsigned int;
using GLsizei = int;
using GLsizeiptr = std::intptr_t;
using DEBUGPROC = void (*)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
						   const void* userParam);

/////////////////////////////////////////////////////////////// GL POINTERS ///////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Holds OpenGL function pointers.
	struct gl_functions {
		void (*glBeginQuery)(GLenum target, GLuint id);
		void (*glBindBuffer)(GLenum target, GLuint buffer);
		void (*glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
		void (*glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
		void (*glBindFramebuffer)(GLenum target, GLuint framebuffer);
		void (*glBindProgramPipeline)(GLuint pipeline);
		void (*glBindTextures)(GLuint first, GLsizei count, const GLuint* textures);
		void (*glBindVertexArray)(GLuint array);
		void (*glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
		void (*glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
		void (*glBlendFuncSeparate)(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
		void (*glClear)(GLbitfield mask);
		void (*glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
		void (*glClearDepth)(GLdouble depth);
		void (*glClearStencil)(GLint s);
		void (*glClearTexImage)(GLuint texture, GLint level, GLenum format, GLenum type, const void* data);
		void (*glClearTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
								   GLsizei depth, GLenum format, GLenum type, const void* data);
		void (*glCopyImageSubData)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName,
								   GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth,
								   GLsizei srcHeight, GLsizei srcDepth);
		void (*glCreateBuffers)(GLsizei n, GLuint* buffers);
		void (*glCreateFramebuffers)(GLsizei n, GLuint* framebuffers);
		void (*glCreateProgramPipelines)(GLsizei n, GLuint* pipelines);
		GLuint (*glCreateShaderProgramv)(GLenum type, GLsizei count, const char** strings);
		void (*glCreateTextures)(GLenum target, GLsizei n, GLuint* textures);
		void (*glCreateVertexArrays)(GLsizei n, GLuint* arrays);
		void (*glDebugMessageCallback)(DEBUGPROC callback, const void* userParam);
		void (*glDebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
		void (*glDeleteBuffers)(GLsizei n, const GLuint* buffers);
		void (*glDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
		void (*glDeleteProgram)(GLuint program);
		void (*glDeleteProgramPipelines)(GLsizei n, const GLuint* pipelines);
		void (*glDeleteQueries)(GLsizei n, const GLuint* queries);
		void (*glDeleteTextures)(GLsizei n, const GLuint* textures);
		void (*glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
		void (*glDisable)(GLenum cap);
		void (*glDrawArrays)(GLenum mode, GLint first, GLsizei count);
		void (*glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
		void (*glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
		void (*glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
		void (*glEnable)(GLenum cap);
		void (*glEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
		void (*glEndQuery)(GLenum target);
		void (*glGenQueries)(GLsizei n, GLuint* ids);
		void (*glGenerateTextureMipmap)(GLuint texture);
		GLenum (*glGetError)();
		void (*glGetNamedBufferParameteriv)(GLuint buffer, GLenum pname, GLint* params);
		void (*glGetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, char* label);
		void (*glGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
		void (*glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint* params);
		void (*glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
		void (*glGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props,
									   GLsizei bufSize, GLsizei* length, GLint* params);
		void (*glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length,
										 char* name);
		void (*glGetQueryObjecti64v)(GLuint id, GLenum pname, GLint64* params);
		const GLubyte* (*glGetString)(GLenum name);
		void (*glGetTextureParameterfv)(GLuint texture, GLenum pname, GLfloat* params);
		void (*glGetTextureParameteriv)(GLuint texture, GLenum pname, GLint* params);
		void (*glInvalidateBufferData)(GLuint buffer);
		void* (*glMapNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
		void (*glNamedBufferStorage)(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags);
		void (*glNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
		void (*glNamedFramebufferTexture)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
		void (*glObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const char* label);
		void (*glPixelStorei)(GLenum pname, GLint param);
		void (*glPolygonMode)(GLenum face, GLenum mode);
		void (*glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
		void (*glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
		void (*glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
		void (*glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
		void (*glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
		void (*glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
		void (*glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void (*glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
		void (*glProgramUniform1i)(GLuint program, GLint location, GLint v0);
		void (*glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
		void (*glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
		void (*glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
		void (*glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
		void (*glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
		void (*glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
		void (*glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
		void (*glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
		void (*glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
		void (*glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
		void (*glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
		void (*glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
		void (*glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
		void (*glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
		void (*glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
		void (*glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		void (*glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
		void (*glTextureParameterfv)(GLuint texture, GLenum pname, const GLfloat* params);
		void (*glTextureParameteri)(GLuint texture, GLenum pname, GLint param);
		void (*glTextureStorage2D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
		void (*glTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format,
									GLenum type, const void* pixels);
		GLboolean (*glUnmapNamedBuffer)(GLuint buffer);
		void (*glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
		void (*glVertexArrayAttribBinding)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
		void (*glVertexArrayAttribFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized,
										  GLuint relativeoffset);
		void (*glVertexArrayBindingDivisor)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
		void (*glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);

		// Loads OpenGL function pointers.
		gl_functions();
	};
} // namespace tr::gfx