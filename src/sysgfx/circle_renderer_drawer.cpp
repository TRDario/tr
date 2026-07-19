///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the drawer from circle_renderer.hpp.                                                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/circle_renderer.hpp"

////////////////////////////////////////////////////////////////// DRAWER /////////////////////////////////////////////////////////////////

tr::circle_renderer::drawer::drawer(circle_renderer& renderer, std::ranges::subrange<std::map<int, layer>::iterator> range)
	: m_renderer{renderer}
	, m_range{range}
{
	TR_ASSERT(!m_renderer->m_locked, "Tried to create multiple simultaneous circle renderer drawers.");

#ifdef TR_ENABLE_ASSERTS
	m_renderer->m_locked = true;
#endif

	std::vector<circle> circles;
	for (const auto& [priority, layer] : m_range) {
		circles.insert(circles.end(), layer.circles.begin(), layer.circles.end());
	}
	m_renderer->m_shader_circles.set(circles);
}

tr::circle_renderer::drawer::drawer(drawer&& r) noexcept
	: m_renderer{std::exchange(r.m_renderer, std::nullopt)}
	, m_range{r.m_range}
{
}

tr::circle_renderer::drawer::~drawer()
{
	clean_up();
}

tr::circle_renderer::drawer& tr::circle_renderer::drawer::operator=(drawer&& r) noexcept
{
	clean_up();
	m_renderer = std::exchange(r.m_renderer, std::nullopt);
	m_range = r.m_range;
	return *this;
}

//

int tr::circle_renderer::drawer::min_layer() const
{
	return !m_range.empty() ? m_range.front().first : INT_MAX;
}

int tr::circle_renderer::drawer::max_layer() const
{
	return !m_range.empty() ? m_range.back().first : INT_MIN;
}

//

void tr::circle_renderer::drawer::draw_layer(int layer, const render_target& target)
{
	TR_ASSERT(m_renderer.has_ref(), "Tried to draw a layer from a moved-from circle renderer drawer.");

	const auto layer_it{m_renderer->m_layers.find(layer)};
	if (layer_it == m_renderer->m_layers.end()) {
		return;
	}

	graphics_context& context{m_renderer->context()};
	const circle_renderer::layer& info{layer_it->second};
	const ssize offset{std::accumulate(m_range.begin(), layer_it, 0_z, [](ssize s, auto& p) { return s + p.second.circles.size(); })};

	setup_context(context);
	context.set_render_target(target);
	setup_draw_call_state(context, info.transform.has_value() ? *info.transform : m_renderer->m_default_transform, info.blend_mode);
	context.set_vertex_buffer(m_renderer->m_shader_circles, 1, offset);
	context.draw_instances(primitive::tri_fan, 0, 4, info.circles.size());
}

void tr::circle_renderer::drawer::draw(const render_target& target)
{
	TR_ASSERT(m_renderer.has_ref(), "Tried to draw from a moved-from circle renderer drawer.");

	if (m_range.empty()) {
		return;
	}

	graphics_context& context{m_renderer->context()};

	setup_context(context);
	context.set_render_target(target);

	ssize offset{0};
	for (const auto& [priority, layer] : m_range) {
		setup_draw_call_state(context, layer.transform.has_value() ? *layer.transform : m_renderer->m_default_transform, layer.blend_mode);
		context.set_vertex_buffer(m_renderer->m_shader_circles, 1, offset);
		context.draw_instances(primitive::tri_fan, 0, 4, layer.circles.size());
		offset += layer.circles.size();
	}
}

//

void tr::circle_renderer::drawer::setup_context(graphics_context& context)
{
	if (context.should_setup_renderer(m_renderer->m_id)) {
		context.set_face_culling(false);
		context.set_depth_test(false);
		context.set_shader_pipeline(m_renderer->m_pipeline);
		context.set_blend_mode(m_renderer->m_last_blend_mode);
		context.set_vertex_format(m_renderer->m_vertex_format);
		context.set_vertex_buffer(m_renderer->m_quad_vertices, 0, 0);
	}
}

void tr::circle_renderer::drawer::setup_draw_call_state(graphics_context& context, const glm::mat4& transform, const blend_mode& blend_mode)
{
	if (m_renderer->m_last_transform != transform) {
		m_renderer->m_last_transform = transform;
		m_renderer->m_pipeline.vertex_shader().set_uniform(0, m_renderer->m_last_transform);
	}

	if (m_renderer->m_last_blend_mode != blend_mode) {
		m_renderer->m_last_blend_mode = blend_mode;
		context.set_blend_mode(m_renderer->m_last_blend_mode);
	}
}

//

void tr::circle_renderer::drawer::clean_up()
{
	if (m_renderer.has_ref()) {
		m_renderer->m_layers.erase(m_range.begin(), m_range.end());
#ifdef TR_ENABLE_ASSERTS
		m_renderer->m_locked = false;
#endif
	}
}