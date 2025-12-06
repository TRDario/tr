#include "../../include/tr/sysgfx/renderer_2d.hpp"

namespace tr::gfx {
	// 2D renderer ID.
	constexpr u32 RENDERER_2D_ID{3};
} // namespace tr::gfx

tr::gfx::renderer_2d::staggered_draw_manager::staggered_draw_manager(renderer_2d& renderer,
																	 std::ranges::subrange<std::vector<mesh>::iterator> range)
	: m_renderer{&renderer}, m_range{range}
{
	TR_ASSERT(!m_renderer->m_locked, "Tried to create multiple simultaneous 2D renderer staggered draw managers.");

#ifdef TR_ENABLE_ASSERTS
	m_renderer->m_locked = true;
#endif

	const usize vertices{std::accumulate(range.begin(), range.end(), 0_uz, [](usize s, const mesh& m) { return s + m.positions.size(); })};
	const usize indices{std::accumulate(range.begin(), range.end(), 0_uz, [](usize s, const mesh& m) { return s + m.indices.size(); })};

	m_renderer->m_vbuffer_positions.resize(vertices);
	m_renderer->m_vbuffer_uvs.resize(vertices);
	m_renderer->m_vbuffer_tints.resize(vertices);
	m_renderer->m_ibuffer.resize(indices);

	m_data.emplace_back(0, 0);
	for (const mesh& mesh : range) {
		const mesh_draw_info start{m_data.back()};

		m_renderer->m_vbuffer_positions.set_region(start.vertex_offset, mesh.positions);
		m_renderer->m_vbuffer_uvs.set_region(start.vertex_offset, mesh.uvs);
		m_renderer->m_vbuffer_tints.set_region(start.vertex_offset, mesh.tints);
		m_renderer->m_ibuffer.set_region(start.index_offset, mesh.indices);
		m_data.emplace_back(start.vertex_offset + mesh.positions.size(), start.index_offset + mesh.indices.size());
	}
}

tr::gfx::renderer_2d::staggered_draw_manager::staggered_draw_manager(staggered_draw_manager&& r) noexcept
	: m_renderer{r.m_renderer}, m_range{r.m_range}, m_data{std::move(r.m_data)}
{
	r.m_renderer = nullptr;
}

tr::gfx::renderer_2d::staggered_draw_manager::~staggered_draw_manager()
{
	clean_up();
}

tr::gfx::renderer_2d::staggered_draw_manager& tr::gfx::renderer_2d::staggered_draw_manager::operator=(staggered_draw_manager&& r) noexcept
{
	clean_up();

	m_renderer = r.m_renderer;
	m_range = r.m_range;
	m_data = std::move(r.m_data);

	r.m_renderer = nullptr;
	return *this;
}

//

void tr::gfx::renderer_2d::staggered_draw_manager::draw_layer(int layer, const render_target& target)
{
	TR_ASSERT(m_renderer != nullptr, "Tried to draw a layer from a moved-from 2D renderer staggered draw manager.");

	const auto range{std::ranges::equal_range(m_range, layer, std::less{}, &mesh::layer)};
	if (range.empty()) {
		return;
	}

	setup_context();
	set_render_target(target);

	std::vector<mesh_draw_info>::const_iterator data_it{m_data.begin() + (range.begin() - m_range.begin())};
	for (const mesh& mesh : range) {
		const usize mesh_indices{std::next(data_it)->index_offset - data_it->index_offset};

		setup_draw_call_state(mesh.texture, mesh.mat, mesh.blend_mode);
		set_vertex_buffer(m_renderer->m_vbuffer_positions, 0, data_it->vertex_offset);
		set_vertex_buffer(m_renderer->m_vbuffer_uvs, 1, data_it->vertex_offset);
		set_vertex_buffer(m_renderer->m_vbuffer_tints, 2, data_it->vertex_offset);
		draw_indexed(mesh.type, data_it->index_offset, mesh_indices);

		++data_it;
	}
}

void tr::gfx::renderer_2d::staggered_draw_manager::draw(const render_target& target)
{
	TR_ASSERT(m_renderer != nullptr, "Tried to draw from a moved-from 2D renderer staggered draw manager.");

	if (m_range.empty()) {
		return;
	};

	setup_context();
	set_render_target(target);

	std::vector<mesh_draw_info>::const_iterator data_it{m_data.begin()};
	for (const mesh& mesh : m_range) {
		const usize mesh_indices{std::next(data_it)->index_offset - data_it->index_offset};

		setup_draw_call_state(mesh.texture, mesh.mat, mesh.blend_mode);
		set_vertex_buffer(m_renderer->m_vbuffer_positions, 0, data_it->vertex_offset);
		set_vertex_buffer(m_renderer->m_vbuffer_uvs, 1, data_it->vertex_offset);
		set_vertex_buffer(m_renderer->m_vbuffer_tints, 2, data_it->vertex_offset);
		draw_indexed(mesh.type, data_it->index_offset, mesh_indices);

		++data_it;
	}
}

//

void tr::gfx::renderer_2d::staggered_draw_manager::setup_context()
{
	if (active_renderer != RENDERER_2D_ID) {
		active_renderer = RENDERER_2D_ID;
		set_face_culling(false);
		set_depth_test(false);
		set_shader_pipeline(m_renderer->m_pipeline);
		set_blend_mode(m_renderer->m_last_blend_mode);
		set_vertex_format(vertex2_format());
		set_index_buffer(m_renderer->m_ibuffer);
	}
}

void tr::gfx::renderer_2d::staggered_draw_manager::setup_draw_call_state(texture_ref texture_ref, const glm::mat4& transform,
																		 const blend_mode& blend_mode)
{
	m_renderer->m_pipeline.fragment_shader().set_uniform(1, std::move(texture_ref));

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

void tr::gfx::renderer_2d::staggered_draw_manager::clean_up()
{
	if (m_renderer != nullptr) {
		m_renderer->m_meshes.erase(m_range.begin(), m_range.end());
#ifdef TR_ENABLE_ASSERTS
		m_renderer->m_locked = false;
#endif
	}
}