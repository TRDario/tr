///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements gl_functions.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/gl_functions.hpp"
#include <SDL3/SDL.h>

/////////////////////////////////////////////////////////////// GL FUNCTIONS ///////////////////////////////////////////////////////////////

// Wrapper around an SDL_FunctionPointer that automatically casts it to another function pointer type.
struct loaded_gl_function_proxy {
	SDL_FunctionPointer ptr;

	template <typename Return, typename... Args> using function_pointer = Return (*)(Args...);
	template <typename Return, typename... Args> operator function_pointer<Return, Args...>()
	{
		return function_pointer<Return, Args...>(ptr);
	}
};

// Wraps SDL_GL_GetProcAddress to return an OpenGL function proxy.
static loaded_gl_function_proxy gl_function_address(const char* name)
{
	return {SDL_GL_GetProcAddress(name)};
}

tr::gfx::gl_functions::gl_functions()
	: glBeginQuery{gl_function_address("glBeginQuery")}
	, glBindBuffer{gl_function_address("glBindBuffer")}
	, glBindBufferBase{gl_function_address("glBindBufferBase")}
	, glBindBufferRange{gl_function_address("glBindBufferRange")}
	, glBindFramebuffer{gl_function_address("glBindFramebuffer")}
	, glBindProgramPipeline{gl_function_address("glBindProgramPipeline")}
	, glBindTextures{gl_function_address("glBindTextures")}
	, glBindVertexArray{gl_function_address("glBindVertexArray")}
	, glBindVertexBuffer{gl_function_address("glBindVertexBuffer")}
	, glBlendEquationSeparate{gl_function_address("glBlendEquationSeparate")}
	, glBlendFuncSeparate{gl_function_address("glBlendFuncSeparate")}
	, glClear{gl_function_address("glClear")}
	, glClearColor{gl_function_address("glClearColor")}
	, glClearDepth{gl_function_address("glClearDepth")}
	, glClearStencil{gl_function_address("glClearStencil")}
	, glClearTexImage{gl_function_address("glClearTexImage")}
	, glClearTexSubImage{gl_function_address("glClearTexSubImage")}
	, glCopyImageSubData{gl_function_address("glCopyImageSubData")}
	, glCreateBuffers{gl_function_address("glCreateBuffers")}
	, glCreateFramebuffers{gl_function_address("glCreateFramebuffers")}
	, glCreateProgramPipelines{gl_function_address("glCreateProgramPipelines")}
	, glCreateShaderProgramv{gl_function_address("glCreateShaderProgramv")}
	, glCreateTextures{gl_function_address("glCreateTextures")}
	, glCreateVertexArrays{gl_function_address("glCreateVertexArrays")}
	, glDebugMessageCallback{gl_function_address("glDebugMessageCallback")}
	, glDebugMessageControl{gl_function_address("glDebugMessageControl")}
	, glDeleteBuffers{gl_function_address("glDeleteBuffers")}
	, glDeleteFramebuffers{gl_function_address("glDeleteFramebuffers")}
	, glDeleteProgram{gl_function_address("glDeleteProgram")}
	, glDeleteProgramPipelines{gl_function_address("glDeleteProgramPipelines")}
	, glDeleteQueries{gl_function_address("glDeleteQueries")}
	, glDeleteTextures{gl_function_address("glDeleteTextures")}
	, glDeleteVertexArrays{gl_function_address("glDeleteVertexArrays")}
	, glDisable{gl_function_address("glDisable")}
	, glDrawArrays{gl_function_address("glDrawArrays")}
	, glDrawArraysInstanced{gl_function_address("glDrawArraysInstanced")}
	, glDrawElements{gl_function_address("glDrawElements")}
	, glDrawElementsInstanced{gl_function_address("glDrawElementsInstanced")}
	, glEnable{gl_function_address("glEnable")}
	, glEnableVertexArrayAttrib{gl_function_address("glEnableVertexArrayAttrib")}
	, glEndQuery{gl_function_address("glEndQuery")}
	, glGenQueries{gl_function_address("glGenQueries")}
	, glGenerateTextureMipmap{gl_function_address("glGenerateTextureMipmap")}
	, glGetError{gl_function_address("glGetError")}
	, glGetNamedBufferParameteriv{gl_function_address("glGetNamedBufferParameteriv")}
	, glGetObjectLabel{gl_function_address("glGetObjectLabel")}
	, glGetProgramInfoLog{gl_function_address("glGetProgramInfoLog")}
	, glGetProgramInterfaceiv{gl_function_address("glGetProgramInterfaceiv")}
	, glGetProgramiv{gl_function_address("glGetProgramiv")}
	, glGetProgramResourceiv{gl_function_address("glGetProgramResourceiv")}
	, glGetProgramResourceName{gl_function_address("glGetProgramResourceName")}
	, glGetQueryObjecti64v{gl_function_address("glGetQueryObjecti64v")}
	, glGetString{gl_function_address("glGetString")}
	, glGetTextureParameterfv{gl_function_address("glGetTextureParameterfv")}
	, glGetTextureParameteriv{gl_function_address("glGetTextureParameteriv")}
	, glInvalidateBufferData{gl_function_address("glInvalidateBufferData")}
	, glMapNamedBufferRange{gl_function_address("glMapNamedBufferRange")}
	, glNamedBufferStorage{gl_function_address("glNamedBufferStorage")}
	, glNamedBufferSubData{gl_function_address("glNamedBufferSubData")}
	, glNamedFramebufferTexture{gl_function_address("glNamedFramebufferTexture")}
	, glObjectLabel{gl_function_address("glObjectLabel")}
	, glPixelStorei{gl_function_address("glPixelStorei")}
	, glPolygonMode{gl_function_address("glPolygonMode")}
	, glProgramUniform1f{gl_function_address("glProgramUniform1f")}
	, glProgramUniform1fv{gl_function_address("glProgramUniform1fv")}
	, glProgramUniform2f{gl_function_address("glProgramUniform2f")}
	, glProgramUniform2fv{gl_function_address("glProgramUniform2fv")}
	, glProgramUniform3f{gl_function_address("glProgramUniform3f")}
	, glProgramUniform3fv{gl_function_address("glProgramUniform3fv")}
	, glProgramUniform4f{gl_function_address("glProgramUniform4f")}
	, glProgramUniform4fv{gl_function_address("glProgramUniform4fv")}
	, glProgramUniform1i{gl_function_address("glProgramUniform1i")}
	, glProgramUniform1iv{gl_function_address("glProgramUniform1iv")}
	, glProgramUniform2i{gl_function_address("glProgramUniform2i")}
	, glProgramUniform2iv{gl_function_address("glProgramUniform2iv")}
	, glProgramUniform3i{gl_function_address("glProgramUniform3i")}
	, glProgramUniform3iv{gl_function_address("glProgramUniform3iv")}
	, glProgramUniform4i{gl_function_address("glProgramUniform4i")}
	, glProgramUniform4iv{gl_function_address("glProgramUniform4iv")}
	, glProgramUniform1ui{gl_function_address("glProgramUniform1ui")}
	, glProgramUniform1uiv{gl_function_address("glProgramUniform1uiv")}
	, glProgramUniform2ui{gl_function_address("glProgramUniform2ui")}
	, glProgramUniform2uiv{gl_function_address("glProgramUniform2uiv")}
	, glProgramUniform3ui{gl_function_address("glProgramUniform3ui")}
	, glProgramUniform3uiv{gl_function_address("glProgramUniform3uiv")}
	, glProgramUniform4ui{gl_function_address("glProgramUniform4ui")}
	, glProgramUniform4uiv{gl_function_address("glProgramUniform4uiv")}
	, glProgramUniformMatrix2fv{gl_function_address("glProgramUniformMatrix2fv")}
	, glProgramUniformMatrix3fv{gl_function_address("glProgramUniformMatrix3fv")}
	, glProgramUniformMatrix4fv{gl_function_address("glProgramUniformMatrix4fv")}
	, glProgramUniformMatrix2x3fv{gl_function_address("glProgramUniformMatrix2x3fv")}
	, glProgramUniformMatrix2x4fv{gl_function_address("glProgramUniformMatrix2x4fv")}
	, glProgramUniformMatrix3x2fv{gl_function_address("glProgramUniformMatrix3x2fv")}
	, glProgramUniformMatrix3x4fv{gl_function_address("glProgramUniformMatrix3x4fv")}
	, glProgramUniformMatrix4x2fv{gl_function_address("glProgramUniformMatrix4x2fv")}
	, glProgramUniformMatrix4x3fv{gl_function_address("glProgramUniformMatrix4x3fv")}
	, glScissor{gl_function_address("glScissor")}
	, glTextureParameterfv{gl_function_address("glTextureParameterfv")}
	, glTextureParameteri{gl_function_address("glTextureParameteri")}
	, glTextureStorage2D{gl_function_address("glTextureStorage2D")}
	, glTextureSubImage2D{gl_function_address("glTextureSubImage2D")}
	, glUnmapNamedBuffer{gl_function_address("glUnmapNamedBuffer")}
	, glUseProgramStages{gl_function_address("glUseProgramStages")}
	, glVertexArrayAttribBinding{gl_function_address("glVertexArrayAttribBinding")}
	, glVertexArrayAttribFormat{gl_function_address("glVertexArrayAttribFormat")}
	, glVertexArrayBindingDivisor{gl_function_address("glVertexArrayBindingDivisor")}
	, glViewport{gl_function_address("glViewport")}
{
}