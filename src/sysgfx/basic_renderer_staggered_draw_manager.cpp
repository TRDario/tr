///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the staggered draw manager from basic_renderer.hpp.                                                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/basic_renderer.hpp"

////////////////////////////////////////////////////////// STAGGERED DRAW MANAGER /////////////////////////////////////////////////////////

tr::basic_renderer::staggered_draw_manager::staggered_draw_manager(basic_renderer& renderer,
																   std::ranges::subrange<std::vector<mesh>::iterator> range)
	: m_renderer{renderer}, m_range{range}
{
#ifdef TR_ENABLE_ASSERTS
	TR_ASSERT(!m_renderer->m_locked, "Tried to create multiple simultaneous basic renderer staggered draw managers.");
	m_renderer->m_locked = true;
#endif

	const usize vertices{fold_left(range, 0_uz, [](usize s, const mesh& m) { return s + m.positions.size(); })};
	const usize indices{fold_left(range, 0_uz, [](usize s, const mesh& m) { return s + m.indices.size(); })};

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

	m_renderer->context().set_index_buffer(m_renderer->m_ibuffer);
}

tr::basic_renderer::staggered_draw_manager::staggered_draw_manager(staggered_draw_manager&& r) noexcept
	: m_renderer{std::exchange(r.m_renderer, std::nullopt)}, m_range{r.m_range}, m_data{std::move(r.m_data)}
{
}

tr::basic_renderer::staggered_draw_manager::~staggered_draw_manager()
{
	clean_up();
}

tr::basic_renderer::staggered_draw_manager& tr::basic_renderer::staggered_draw_manager::operator=(staggered_draw_manager&& r) noexcept
{
	clean_up();
	m_renderer = std::exchange(r.m_renderer, std::nullopt);
	m_range = r.m_range;
	m_data = std::move(r.m_data);
	return *this;
}

//

void tr::basic_renderer::staggered_draw_manager::draw_layer(int layer, const render_target& target)
{
	TR_ASSERT(m_renderer.has_ref(), "Tried to draw a layer from a moved-from basic renderer staggered draw manager.");

	const auto range{std::ranges::equal_range(m_range, layer, std::less{}, &mesh::layer)};
	if (range.empty()) {
		return;
	}

	graphics_context& context{m_renderer->context()};

	setup_context(context);
	context.set_render_target(target);

	std::vector<mesh_draw_info>::const_iterator data_it{m_data.begin() + (range.begin() - m_range.begin())};
	for (const mesh& mesh : range) {
		const usize mesh_indices{std::next(data_it)->index_offset - data_it->index_offset};

		setup_draw_call_state(context, mesh.texture, mesh.mat, mesh.blend_mode);
		context.set_vertex_buffer(m_renderer->m_vbuffer_positions, 0, data_it->vertex_offset);
		context.set_vertex_buffer(m_renderer->m_vbuffer_uvs, 1, data_it->vertex_offset);
		context.set_vertex_buffer(m_renderer->m_vbuffer_tints, 2, data_it->vertex_offset);
		context.draw_indexed(mesh.type, data_it->index_offset, mesh_indices);

		++data_it;
	}
}

void tr::basic_renderer::staggered_draw_manager::draw(const render_target& target)
{
	TR_ASSERT(m_renderer.has_ref(), "Tried to draw from a moved-from basic renderer staggered draw manager.");

	if (m_range.empty()) {
		return;
	};

	graphics_context& context{m_renderer->context()};

	setup_context(context);
	context.set_render_target(target);

	std::vector<mesh_draw_info>::const_iterator data_it{m_data.begin()};
	for (const mesh& mesh : m_range) {
		const usize mesh_indices{std::next(data_it)->index_offset - data_it->index_offset};

		setup_draw_call_state(context, mesh.texture, mesh.mat, mesh.blend_mode);
		context.set_vertex_buffer(m_renderer->m_vbuffer_positions, 0, data_it->vertex_offset);
		context.set_vertex_buffer(m_renderer->m_vbuffer_uvs, 1, data_it->vertex_offset);
		context.set_vertex_buffer(m_renderer->m_vbuffer_tints, 2, data_it->vertex_offset);
		context.draw_indexed(mesh.type, data_it->index_offset, mesh_indices);

		++data_it;
	}
}

//

void tr::basic_renderer::staggered_draw_manager::setup_context(graphics_context& context)
{
	if (context.should_setup_renderer(m_renderer->m_id)) {
		context.set_face_culling(false);
		context.set_depth_test(false);
		context.set_shader_pipeline(m_renderer->m_pipeline);
		context.set_blend_mode(m_renderer->m_last_blend_mode);
		context.set_vertex_format(context.vertex2_format());
		context.set_index_buffer(m_renderer->m_ibuffer);
	}
}

void tr::basic_renderer::staggered_draw_manager::setup_draw_call_state(graphics_context& context, texture_ref texture_ref,
																	   const glm::mat4& transform, const blend_mode& blend_mode)
{
	m_renderer->m_pipeline.fragment_shader().set_uniform(1, std::move(texture_ref));

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

void tr::basic_renderer::staggered_draw_manager::clean_up()
{
	if (m_renderer.has_ref()) {
		m_renderer->m_meshes.erase(m_range.begin(), m_range.end());
#ifdef TR_ENABLE_ASSERTS
		m_renderer->m_locked = false;
#endif
	}
}