#include "../../include/tr/sysgfx/circle_renderer.hpp"

namespace tr::gfx {
	// Circle renderer ID.
	constexpr u32 CIRCLE_RENDERER_ID{5};
} // namespace tr::gfx

//

tr::gfx::circle_renderer::staggered_draw_manager::staggered_draw_manager(circle_renderer& renderer,
																		 std::ranges::subrange<std::map<int, layer>::iterator> range)
	: m_renderer{&renderer}, m_range{range}
{
	std::vector<circle> circles;
	for (const auto& [priority, layer] : m_range) {
		circles.insert(circles.end(), layer.circles.begin(), layer.circles.end());
	}
	m_renderer->m_shader_circles.set(circles);
}

tr::gfx::circle_renderer::staggered_draw_manager::staggered_draw_manager(staggered_draw_manager&& r) noexcept
	: m_renderer{r.m_renderer}, m_range{r.m_range}
{
	r.m_renderer = nullptr;
}

tr::gfx::circle_renderer::staggered_draw_manager::~staggered_draw_manager()
{
	clean_up();
}

tr::gfx::circle_renderer::staggered_draw_manager& tr::gfx::circle_renderer::staggered_draw_manager::operator=(
	staggered_draw_manager&& r) noexcept
{
	clean_up();

	m_renderer = r.m_renderer;
	m_range = r.m_range;

	r.m_renderer = nullptr;
	return *this;
}

//

void tr::gfx::circle_renderer::staggered_draw_manager::draw_layer(int layer, const render_target& target)
{
	setup_context();
	set_render_target(target);

	const auto equal_range_result{m_renderer->m_layers.equal_range(layer)};
	const auto range{std::views::values(std::ranges::subrange{equal_range_result.first, equal_range_result.second})};

	ssize offset{std::distance(m_range.begin(), range.begin().base())};
	for (const circle_renderer::layer& layer_info : range) {
		setup_draw_call_state(layer_info.transform.has_value() ? *layer_info.transform : m_renderer->m_default_transform,
							  layer_info.blend_mode);
		set_vertex_buffer(m_renderer->m_shader_circles, 1, offset);
		draw_instances(primitive::TRI_FAN, 0, 4, layer_info.circles.size());
		offset += layer_info.circles.size();
	}
}

void tr::gfx::circle_renderer::staggered_draw_manager::draw(const render_target& target)
{
	setup_context();
	set_render_target(target);

	ssize offset{0};
	for (const auto& [priority, layer] : m_range) {
		setup_draw_call_state(layer.transform.has_value() ? *layer.transform : m_renderer->m_default_transform, layer.blend_mode);
		set_vertex_buffer(m_renderer->m_shader_circles, 1, offset);
		draw_instances(primitive::TRI_FAN, 0, 4, layer.circles.size());
		offset += layer.circles.size();
	}
}

//

void tr::gfx::circle_renderer::staggered_draw_manager::setup_context()
{
	if (active_renderer != CIRCLE_RENDERER_ID) {
		active_renderer = CIRCLE_RENDERER_ID;
		set_face_culling(false);
		set_depth_test(false);
		set_shader_pipeline(m_renderer->m_pipeline);
		set_blend_mode(m_renderer->m_last_blend_mode);
		set_vertex_format(m_renderer->m_vertex_format);
		set_vertex_buffer(m_renderer->m_quad_vertices, 0, 0);
	}
}

void tr::gfx::circle_renderer::staggered_draw_manager::setup_draw_call_state(const glm::mat4& transform, const blend_mode& blend_mode)
{
	if (m_renderer->m_last_transform != transform) {
		m_renderer->m_last_transform = transform;
		m_renderer->m_pipeline.vertex_shader().set_uniform(0, m_renderer->m_last_transform);
	}

	if (m_renderer->m_last_blend_mode != blend_mode) {
		m_renderer->m_last_blend_mode = blend_mode;
		set_blend_mode(m_renderer->m_last_blend_mode);
	}
}

//

void tr::gfx::circle_renderer::staggered_draw_manager::clean_up()
{
	if (m_renderer != nullptr) {
		m_renderer->m_layers.erase(m_range.begin(), m_range.end());
#ifdef TR_ENABLE_ASSERTS
		m_renderer->m_locked = false;
#endif
	}
}