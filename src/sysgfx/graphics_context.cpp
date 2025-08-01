#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"

namespace tr::gfx {
	// The ID of the current renderer.
	inline std::uint32_t current_renderer_{0};
} // namespace tr::gfx

std::uint32_t tr::gfx::alloc_renderer_id()
{
	static std::uint32_t id{1000};
	return id++;
}

bool tr::gfx::debug()
{
	return debug_context;
}

void tr::gfx::set_render_target(const render_target& target)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context render target before opening the window.");

	if (target != current_render_target) {
		if ((!current_render_target.has_value() && target.m_fbo != 0) ||
			(current_render_target.has_value() && target.m_fbo != current_render_target->m_fbo)) {
			TR_GL_CALL(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, target.m_fbo);
		}
		TR_GL_CALL(glViewport, target.m_viewport.tl.x, target.m_viewport.tl.y, target.m_viewport.size.x, target.m_viewport.size.y);
		TR_GL_CALL(glScissor, target.m_viewport.tl.x, target.m_viewport.tl.y, target.m_viewport.size.x, target.m_viewport.size.y);
		current_render_target = target;
	}
}

void tr::gfx::set_shader_pipeline(const shader_pipeline& pipeline)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context shader pipeline before opening the window.");

	TR_GL_CALL(glBindProgramPipeline, pipeline.m_ppo.get());
}

void tr::gfx::set_blend_mode(const blend_mode& blend_mode)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context blending mode before opening the window.");

	TR_GL_CALL(glBlendEquationSeparate, static_cast<GLenum>(blend_mode.rgb_fn), static_cast<GLenum>(blend_mode.alpha_fn));
	TR_GL_CALL(glBlendFuncSeparate, static_cast<GLenum>(blend_mode.rgb_src), static_cast<GLenum>(blend_mode.rgb_dst),
			   static_cast<GLenum>(blend_mode.alpha_src), static_cast<GLenum>(blend_mode.alpha_dst));
}

void tr::gfx::set_vertex_format(const vertex_format& format)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context vertex format before opening the window.");

	TR_GL_CALL(glBindVertexArray, format.m_vao.get());
}

void tr::gfx::set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, std::intptr_t offset, std::size_t stride)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context vertex buffer before opening the window.");

	TR_GL_CALL(glBindVertexBuffer, slot, buffer.m_vbo.get(), offset, static_cast<GLsizei>(stride));
}

void tr::gfx::set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, std::intptr_t offset, std::size_t stride)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context vertex buffer before opening the window.");

	TR_GL_CALL(glBindVertexBuffer, slot, buffer.m_vbo.get(), offset, static_cast<GLsizei>(stride));
}

void tr::gfx::set_index_buffer(const static_index_buffer& buffer)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context index buffer before opening the window.");

	TR_GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, buffer.m_ibo.get());
}

void tr::gfx::set_index_buffer(const dyn_index_buffer& buffer)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context index buffer before opening the window.");

	TR_GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, buffer.m_ibo.get());
}

std::uint32_t tr::gfx::current_renderer()
{
	return current_renderer_;
}

void tr::gfx::set_renderer(std::uint32_t id)
{
	current_renderer_ = id;
}

void tr::gfx::draw(primitive type, std::size_t offset, std::size_t vertices)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawArrays, static_cast<GLenum>(type), static_cast<GLint>(offset), static_cast<GLsizei>(vertices));
}

void tr::gfx::draw_instances(primitive type, std::size_t offset, std::size_t vertices, int instances)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawArraysInstanced, static_cast<GLenum>(type), static_cast<GLint>(offset), static_cast<GLsizei>(vertices), instances);
}

void tr::gfx::draw_indexed(primitive type, std::size_t offset, std::size_t indices)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawElements, static_cast<GLenum>(type), static_cast<GLsizei>(indices), GL_UNSIGNED_SHORT,
			   reinterpret_cast<const void*>(offset * sizeof(std::uint16_t)));
}

void tr::gfx::draw_indexed_instances(primitive type, std::size_t offset, std::size_t indices, int instances)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawElementsInstanced, static_cast<GLenum>(type), static_cast<GLsizei>(indices), GL_UNSIGNED_SHORT,
			   reinterpret_cast<const void*>(offset * sizeof(std::uint16_t)), instances);
}