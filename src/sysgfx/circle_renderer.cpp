///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements circle_renderer.hpp.                                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/tr/sysgfx/circle_renderer.hpp"

///////////////////////////////////////////////////////////// CIRCLE RENDERER /////////////////////////////////////////////////////////////

namespace {
#include "../../resources/generated/circle_renderer_frag.hpp"
#include "../../resources/generated/circle_renderer_vert.hpp"
} // namespace

tr::circle_renderer::circle_renderer(graphics_context& context, float render_scale)
	: m_id{context.allocate_renderer_id()}
	, m_pipeline{context, vertex_shader{context, circle_renderer_vert}, fragment_shader{context, circle_renderer_frag}}
	, m_vertex_format{context, vertex_format_bindings}
	, m_shader_circles{context}
	, m_quad_vertices{context, std::array<glm::u8vec2, 4>{{{0, 0}, {0, 1}, {1, 1}, {1, 0}}}}
{
	m_pipeline.set_label("(tr) Circle Renderer Pipeline");
	m_pipeline.vertex_shader().set_label("(tr) Circle Renderer Vertex Shader");
	m_pipeline.fragment_shader().set_label("(tr) Circle Renderer Fragment Shader");
	m_vertex_format.set_label("(tr) Circle Renderer Vertex Format");
	m_shader_circles.set_label("(tr) Circle Renderer Circle Buffer");
	m_quad_vertices.set_label("(tr) Circle Renderer Quad Buffer");

	set_render_scale(render_scale);
}

//

void tr::circle_renderer::set_render_scale(float render_scale)
{
	TR_ASSERT(!m_locked, "Tried to set render scale on a locked circle renderer.");

	m_pipeline.vertex_shader().set_uniform(1, render_scale);
}

void tr::circle_renderer::set_default_transform(const glm::mat4& mat)
{
	TR_ASSERT(!m_locked, "Tried to set default transform on a locked circle renderer.");

	m_default_transform = mat;
}

void tr::circle_renderer::set_layer_transform(int layer, const glm::mat4& mat)
{
	TR_ASSERT(!m_locked, "Tried to set default layer transform on a locked circle renderer.");

	m_layers[layer].transform = mat;
}

void tr::circle_renderer::set_layer_blend_mode(int layer, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to set default layer blending mode on a locked circle renderer.");

	m_layers[layer].blend_mode = blend_mode;
}

//

void tr::circle_renderer::add_circle(int layer, const tr::circle& circle, rgba8 color)
{
	TR_ASSERT(!m_locked, "Tried to add a circle to a locked circle renderer.");

	m_layers[layer].circles.emplace_back(circle.c, circle.r, 0, color, rgba8{color.r, color.b, color.b, 0});
}

void tr::circle_renderer::add_circle_outline(int layer, const tr::circle& circle, float outline_thickness, rgba8 color)
{
	TR_ASSERT(!m_locked, "Tried to add a circle outline to a locked circle renderer.");

	m_layers[layer].circles.emplace_back(circle.c, circle.r, outline_thickness, rgba8{color.r, color.b, color.b, 0}, color);
}

void tr::circle_renderer::add_outlined_circle(int layer, const tr::circle& circle, float outline_thickness, rgba8 fill_color,
											  rgba8 outline_color)
{
	TR_ASSERT(!m_locked, "Tried to add an outlined circle to a locked circle renderer.");

	m_layers[layer].circles.emplace_back(circle.c, circle.r, outline_thickness, fill_color, outline_color);
}

//

tr::circle_renderer::staggered_draw_manager tr::circle_renderer::prepare_staggered_draw_range(int min_layer, int max_layer)
{
	return staggered_draw_manager{*this, {m_layers.lower_bound(min_layer), m_layers.upper_bound(max_layer)}};
}

tr::circle_renderer::staggered_draw_manager tr::circle_renderer::prepare_staggered_draw()
{
	return staggered_draw_manager{*this, m_layers};
}

void tr::circle_renderer::draw_layer(int layer, const render_target& target)
{
	staggered_draw_manager{*this, {m_layers.lower_bound(layer), m_layers.upper_bound(layer)}}.draw(target);
}

void tr::circle_renderer::draw_layer_range(int min_layer, int max_layer, const render_target& target)
{
	prepare_staggered_draw_range(min_layer, max_layer).draw(target);
}

void tr::circle_renderer::draw(const render_target& target)
{
	prepare_staggered_draw().draw(target);
}