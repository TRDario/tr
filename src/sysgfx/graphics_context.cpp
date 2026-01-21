///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements graphics_context.hpp.                                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"

////////////////////////////////////////////////////////////////// CHECKS /////////////////////////////////////////////////////////////////

#ifdef TR_ENABLE_GL_CHECKS

void tr::gfx::set_vertex_buffer_checked(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride,
										std::span<const vertex_attribute> attrs)
{
	sys::g_window.gfx_context().check_vertex_buffer(buffer.label(), slot, attrs);
	set_vertex_buffer(buffer, slot, offset, stride);
}

void tr::gfx::set_vertex_buffer_checked(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride,
										std::span<const vertex_attribute> attrs)
{
	sys::g_window.gfx_context().check_vertex_buffer(buffer.label(), slot, attrs);
	set_vertex_buffer(buffer, slot, offset, stride);
}
#endif

///////////////////////////////////////////////////////////// GRAPHICS CONTEXT ////////////////////////////////////////////////////////////

tr::gfx::renderer_id tr::gfx::allocate_renderer_id()
{
	static u32 id{2};
	return tr::gfx::renderer_id{id++};
}

bool tr::gfx::should_setup_context(renderer_id id)
{
	return sys::g_window.gfx_context().should_setup_context(id);
}

void tr::gfx::set_wireframe_mode(bool arg)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set wireframe mode before opening the window.");

	TR_GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, arg ? GL_LINE : GL_FILL);
}

void tr::gfx::set_face_culling(bool arg)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set face culling before opening the window.");

	if (arg) {
		TR_GL_CALL(glEnable, GL_CULL_FACE);
	}
	else {
		TR_GL_CALL(glDisable, GL_CULL_FACE);
	}
}

void tr::gfx::set_depth_test(bool arg)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set depth testing before opening the window.");

	if (arg) {
		TR_GL_CALL(glEnable, GL_DEPTH_TEST);
	}
	else {
		TR_GL_CALL(glDisable, GL_DEPTH_TEST);
	}
}

void tr::gfx::set_render_target(const render_target& target)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set graphics context render target before opening the window.");

	sys::g_window.gfx_context().set_render_target(target);
}

void tr::gfx::set_shader_pipeline(const shader_pipeline& pipeline)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set graphics context shader pipeline before opening the window.");

	TR_GL_CALL(glBindProgramPipeline, pipeline.m_ppo.get());
}

void tr::gfx::set_tessellation_patch_size(int vertices)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set tessellation patch size before opening the window.");

	TR_GL_CALL(glPatchParameteri, GL_PATCH_VERTICES, vertices);
}

void tr::gfx::set_blend_mode(const blend_mode& bm)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set graphics context blending mode before opening the window.");

	TR_GL_CALL(glBlendEquationSeparate, GLenum(bm.rgb_fn), GLenum(bm.alpha_fn));
	TR_GL_CALL(glBlendFuncSeparate, GLenum(bm.rgb_src), GLenum(bm.rgb_dst), GLenum(bm.alpha_src), GLenum(bm.alpha_dst));
}

void tr::gfx::set_vertex_format(const vertex_format& format)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set graphics context vertex format before opening the window.");

	TR_GL_CALL(glBindVertexArray, format.m_vao.get());

#ifdef TR_ENABLE_GL_CHECKS
	sys::g_window.gfx_context().set_vertex_format(format.label(), format.m_bindings);
#endif
}

void tr::gfx::set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set graphics context vertex buffer before opening the window.");

	TR_GL_CALL(glBindVertexBuffer, slot, buffer.m_vbo.get(), offset, GLsizei(stride));
}

void tr::gfx::set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set graphics context vertex buffer before opening the window.");

	TR_GL_CALL(glBindVertexBuffer, slot, buffer.m_vbo.get(), offset, GLsizei(stride));
}

void tr::gfx::set_index_buffer(const static_index_buffer& buffer)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set graphics context index buffer before opening the window.");

	TR_GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, buffer.m_ibo.get());
}

void tr::gfx::set_index_buffer(const dyn_index_buffer& buffer)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to set graphics context index buffer before opening the window.");

	TR_GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, buffer.m_ibo.get());
}

void tr::gfx::draw(primitive type, usize offset, usize vertices)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawArrays, GLenum(type), GLint(offset), GLsizei(vertices));
}

void tr::gfx::draw_instances(primitive type, usize offset, usize vertices, int instances)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawArraysInstanced, GLenum(type), GLint(offset), GLsizei(vertices), instances);
}

void tr::gfx::draw_indexed(primitive type, usize offset, usize indices)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawElements, GLenum(type), GLsizei(indices), GL_UNSIGNED_SHORT, (const void*)(offset * sizeof(u16)));
}

void tr::gfx::draw_indexed_instances(primitive type, usize offset, usize indices, int instances)
{
	TR_ASSERT(sys::g_window.is_open(), "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawElementsInstanced, GLenum(type), GLsizei(indices), GL_UNSIGNED_SHORT, (const void*)(offset * sizeof(u16)), instances);
}