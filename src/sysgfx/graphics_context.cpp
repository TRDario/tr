#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"

namespace tr {
	// The ID of the current renderer.
	inline std::uint32_t _cur_renderer{0};
} // namespace tr

std::uint32_t tr::alloc_renderer_id() noexcept
{
	static std::uint32_t id{1000};
	return id++;
}

bool tr::gfx_context::debug() noexcept
{
	return _debug_context;
}

void tr::gfx_context::set_render_target(const render_target& target) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to set graphics context render target before opening the window.");

	if (target != _render_target) {
		if ((!_render_target.has_value() && target._fbo != 0) || (_render_target.has_value() && target._fbo != _render_target->_fbo)) {
			TR_GL_CALL(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, target._fbo);
		}
		TR_GL_CALL(glViewport, target._viewport.tl.x, target._viewport.tl.y, target._viewport.size.x, target._viewport.size.y);
		TR_GL_CALL(glScissor, target._viewport.tl.x, target._viewport.tl.y, target._viewport.size.x, target._viewport.size.y);
		_render_target = target;
	}
}

void tr::gfx_context::set_shader_pipeline(const shader_pipeline& pipeline) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to set graphics context shader pipeline before opening the window.");

	TR_GL_CALL(glBindProgramPipeline, pipeline._id.get());
}

void tr::gfx_context::set_blend_mode(const blend_mode& blend_mode) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to set graphics context blending mode before opening the window.");

	TR_GL_CALL(glBlendEquationSeparate, static_cast<GLenum>(blend_mode.rgb_fn), static_cast<GLenum>(blend_mode.alpha_fn));
	TR_GL_CALL(glBlendFuncSeparate, static_cast<GLenum>(blend_mode.rgb_src), static_cast<GLenum>(blend_mode.rgb_dst),
			   static_cast<GLenum>(blend_mode.alpha_src), static_cast<GLenum>(blend_mode.alpha_dst));
}

void tr::gfx_context::set_vertex_format(const vertex_format& format) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to set graphics context vertex format before opening the window.");

	TR_GL_CALL(glBindVertexArray, format._id.get());
}

void tr::gfx_context::set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, std::intptr_t offset,
										std::size_t stride) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to set graphics context vertex buffer before opening the window.");

	TR_GL_CALL(glBindVertexBuffer, slot, buffer._id.get(), offset, static_cast<GLsizei>(stride));
}

void tr::gfx_context::set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, std::intptr_t offset, std::size_t stride) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to set graphics context vertex buffer before opening the window.");

	TR_GL_CALL(glBindVertexBuffer, slot, buffer._id.get(), offset, static_cast<GLsizei>(stride));
}

void tr::gfx_context::set_index_buffer(const static_index_buffer& buffer) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to set graphics context index buffer before opening the window.");

	TR_GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, buffer._id.get());
}

void tr::gfx_context::set_index_buffer(const dyn_index_buffer& buffer) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to set graphics context index buffer before opening the window.");

	TR_GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, buffer._id.get());
}

std::uint32_t tr::gfx_context::current_renderer() noexcept
{
	return _cur_renderer;
}

void tr::gfx_context::set_renderer(std::uint32_t id) noexcept
{
	_cur_renderer = id;
}

void tr::gfx_context::draw(primitive type, std::size_t offset, std::size_t vertices) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawArrays, static_cast<GLenum>(type), static_cast<GLint>(offset), static_cast<GLsizei>(vertices));
}

void tr::gfx_context::draw_instances(primitive type, std::size_t offset, std::size_t vertices, int instances) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawArraysInstanced, static_cast<GLenum>(type), static_cast<GLint>(offset), static_cast<GLsizei>(vertices), instances);
}

void tr::gfx_context::draw_indexed(primitive type, std::size_t offset, std::size_t indices) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawElements, static_cast<GLenum>(type), static_cast<GLsizei>(indices), GL_UNSIGNED_SHORT,
			   reinterpret_cast<const void*>(offset * sizeof(std::uint16_t)));
}

void tr::gfx_context::draw_indexed_instances(primitive type, std::size_t offset, std::size_t indices, int instances) noexcept
{
	TR_ASSERT(_glctx != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawElementsInstanced, static_cast<GLenum>(type), static_cast<GLsizei>(indices), GL_UNSIGNED_SHORT,
			   reinterpret_cast<const void*>(offset * sizeof(std::uint16_t)), instances);
}