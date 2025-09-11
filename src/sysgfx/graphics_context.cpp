#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/gl_call.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/impl.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"

namespace tr::gfx {
	// The ID of the current renderer.
	inline u32 current_renderer_{0};

#ifdef TR_ENABLE_GL_CHECKS
	// Bindings of the last bound vertex buffer.
	inline std::initializer_list<vertex_binding> last_bound_vertex_buffer_bindings;

	// Fails an assertion if a type mismatch is detected.
	void check_vertex_buffer(int slot, std::initializer_list<vertex_attribute> attributes);
#endif
} // namespace tr::gfx

tr::u32 tr::gfx::alloc_renderer_id()
{
	static u32 id{1000};
	return id++;
}

bool tr::gfx::debug()
{
	return debug_context;
}

void tr::gfx::set_face_culling(bool arg)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set face culling before opening the window.");

	if (arg) {
		TR_GL_CALL(glEnable, GL_CULL_FACE);
	}
	else {
		TR_GL_CALL(glDisable, GL_CULL_FACE);
	}
}

void tr::gfx::set_depth_test(bool arg)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set depth testing before opening the window.");

	if (arg) {
		TR_GL_CALL(glEnable, GL_DEPTH_TEST);
	}
	else {
		TR_GL_CALL(glDisable, GL_DEPTH_TEST);
	}
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

	TR_GL_CALL(glBlendEquationSeparate, GLenum(blend_mode.rgb_fn), GLenum(blend_mode.alpha_fn));
	TR_GL_CALL(glBlendFuncSeparate, GLenum(blend_mode.rgb_src), GLenum(blend_mode.rgb_dst), GLenum(blend_mode.alpha_src),
			   GLenum(blend_mode.alpha_dst));
}

void tr::gfx::set_vertex_format(const vertex_format& format)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context vertex format before opening the window.");

	TR_GL_CALL(glBindVertexArray, format.m_vao.get());

#ifdef TR_ENABLE_GL_CHECKS
	last_bound_vertex_buffer_bindings = format.m_bindings;
#endif
}

#ifdef TR_ENABLE_GL_CHECKS
void tr::gfx::check_vertex_buffer(int slot, std::initializer_list<vertex_attribute> attrs)
{
	TR_ASSERT(usize(slot) < last_bound_vertex_buffer_bindings.size(), "Tried to bind vertex buffer to invalid slot {} (max: {}).", slot,
			  last_bound_vertex_buffer_bindings.size());

	const std::initializer_list<vertex_attribute> ref(last_bound_vertex_buffer_bindings.begin()[slot].attrs);
	TR_ASSERT(attrs.size() == ref.size(),
			  "Tried to bind vertex buffer of a different type from the one in the vertex format (has {} attributes instead of {}).",
			  attrs.size(), ref.size());
	for (usize i = 0; i < attrs.size(); ++i) {
		const vertex_attribute& l{attrs.begin()[i]};
		const vertex_attribute& r{ref.begin()[i]};
		TR_ASSERT(l.type == r.type && l.elements == r.elements,
				  "Tried to bind vertex buffer of a type different from than the one in the vertex format (expected '{}' in attribute {}, "
				  "got '{}').",
				  r, i, l);
	}
}
#endif

void tr::gfx::set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context vertex buffer before opening the window.");

	TR_GL_CALL(glBindVertexBuffer, slot, buffer.m_vbo.get(), offset, GLsizei(stride));
}

void tr::gfx::set_vertex_buffer_checked(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride,
										std::initializer_list<vertex_attribute> attrs)
{
	check_vertex_buffer(slot, attrs);
	set_vertex_buffer(buffer, slot, offset, stride);
}

void tr::gfx::set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to set graphics context vertex buffer before opening the window.");

	TR_GL_CALL(glBindVertexBuffer, slot, buffer.m_vbo.get(), offset, GLsizei(stride));
}

void tr::gfx::set_vertex_buffer_checked(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride,
										std::initializer_list<vertex_attribute> attrs)
{
	check_vertex_buffer(slot, attrs);
	set_vertex_buffer(buffer, slot, offset, stride);
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

tr::u32 tr::gfx::current_renderer()
{
	return current_renderer_;
}

void tr::gfx::set_renderer(u32 id)
{
	current_renderer_ = id;
}

void tr::gfx::draw(primitive type, usize offset, usize vertices)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawArrays, GLenum(type), GLint(offset), GLsizei(vertices));
}

void tr::gfx::draw_instances(primitive type, usize offset, usize vertices, int instances)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawArraysInstanced, GLenum(type), GLint(offset), GLsizei(vertices), instances);
}

void tr::gfx::draw_indexed(primitive type, usize offset, usize indices)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawElements, GLenum(type), GLsizei(indices), GL_UNSIGNED_SHORT, (const void*)(offset * sizeof(u16)));
}

void tr::gfx::draw_indexed_instances(primitive type, usize offset, usize indices, int instances)
{
	TR_ASSERT(ogl_context != nullptr, "Tried to perform a drawing operation before opening the window.");

	TR_GL_CALL(glDrawElementsInstanced, GLenum(type), GLsizei(indices), GL_UNSIGNED_SHORT, (const void*)(offset * sizeof(u16)), instances);
}