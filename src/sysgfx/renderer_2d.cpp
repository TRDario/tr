#include "../../include/tr/sysgfx/renderer_2d.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"

namespace tr::gfx {
	// Untextured UV sentinel.
	inline constexpr glm::vec2 UNTEXTURED_UV{-100, -100};
	// 2D renderer ID.
	inline constexpr u32 RENDERER_2D_ID{3};
	// Vertex shader source code.
	inline constexpr const char* RENDERER_2D_VERT_SRC{
		"#version 450\n#define L(l) layout(location=l)\nL(0)uniform mat4 m;L(0)in vec2 p;L(1)in vec2 u;L(2)in vec4 c;out gl_PerVertex{vec4 "
		"gl_Position;};L(0)out vec2 U;L(1)out vec4 C;void main(){U=u;C=c;gl_Position=m*vec4(p,0,1);}",
	};
	// Fragment shader source code.
	inline constexpr const char* RENDERER_2D_FRAG_SRC{
		"#version 450\n#define L(l) layout(location=l)\nL(1)uniform sampler2D t;L(0)in vec2 u;L(1)in vec4 c;L(0)out vec4 C;void "
		"main(){C=u.x==-100?c:c*texture(t,u);}",
	};
} // namespace tr::gfx

//

tr::gfx::renderer_2d::renderer_2d()
	: m_pipeline{vertex_shader{RENDERER_2D_VERT_SRC}, fragment_shader{RENDERER_2D_FRAG_SRC}}
{
	m_pipeline.vertex_shader().set_uniform(0, glm::mat4{1.0f});
	if (debug()) {
		m_pipeline.set_label("(tr) 2D Renderer Pipeline");
		m_pipeline.vertex_shader().set_label("(tr) 2D Renderer Vertex Shader");
		m_pipeline.fragment_shader().set_label("(tr) 2D Renderer Fragment Shader");
		m_vbuffer_positions.set_label("(tr) 2D Renderer Vertex Position Buffer");
		m_vbuffer_uvs.set_label("(tr) 2D Renderer Vertex UV Buffer");
		m_vbuffer_tints.set_label("(tr) 2D Renderer Vertex Tint Buffer");
		m_ibuffer.set_label("(tr) 2D Renderer Index Buffer");
	}
}

//

void tr::gfx::renderer_2d::set_default_transform(const glm::mat4& mat)
{
	m_default_transform = mat;
}

void tr::gfx::renderer_2d::set_default_layer_texture(int layer, texture_ref texture)
{
	m_layer_defaults[layer].texture = std::move(texture);
}

void tr::gfx::renderer_2d::set_default_layer_transform(int layer, const glm::mat4& mat)
{
	m_layer_defaults[layer].transform = mat;
}

void tr::gfx::renderer_2d::set_default_layer_blend_mode(int layer, const blend_mode& blend_mode)
{
	m_layer_defaults[layer].blend_mode = blend_mode;
}

//

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_color_fan(int layer, usize vertices)
{
	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_color_fan(layer, vertices, transform, defaults.blend_mode);
	}
	else {
		return new_color_fan(layer, vertices, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_color_fan(int layer, usize vertices, const glm::mat4& mat,
																   const blend_mode& blend_mode)
{
	mesh& mesh{find_mesh(layer, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);

	std::fill(mesh.uvs.end() - vertices, mesh.uvs.end(), UNTEXTURED_UV);
	fill_poly_idx(back_inserter(mesh.indices), u16(vertices), base_index);

	return {{mesh.positions.end() - vertices, mesh.positions.end()}, {mesh.tints.end() - vertices, mesh.tints.end()}};
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_color_outline(int layer, usize vertices)
{
	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_color_outline(layer, vertices, transform, defaults.blend_mode);
	}
	else {
		return new_color_outline(layer, vertices, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_color_outline(int layer, usize polygon_vertices, const glm::mat4& mat,
																	   const blend_mode& blend_mode)
{
	const usize vertices{polygon_vertices * 2};
	mesh& mesh{find_mesh(layer, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);

	std::fill(mesh.uvs.end() - vertices, mesh.uvs.end(), UNTEXTURED_UV);
	fill_poly_outline_idx(back_inserter(mesh.indices), u16(polygon_vertices), base_index);

	return {{mesh.positions.end() - vertices, mesh.positions.end()}, {mesh.tints.end() - vertices, mesh.tints.end()}};
}

tr::gfx::color_mesh_ref tr::gfx::renderer_2d::new_color_mesh(int layer, usize vertices, usize indices)
{
	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_color_mesh(layer, vertices, indices, transform, defaults.blend_mode);
	}
	else {
		return new_color_mesh(layer, vertices, indices, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::color_mesh_ref tr::gfx::renderer_2d::new_color_mesh(int layer, usize vertices, usize indices, const glm::mat4& mat,
															 const blend_mode& blend_mode)
{
	mesh& mesh{find_mesh(layer, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	std::fill(mesh.uvs.end() - vertices, mesh.uvs.end(), UNTEXTURED_UV);

	return {
		{mesh.positions.end() - vertices, mesh.positions.end()},
		{mesh.tints.end() - vertices, mesh.tints.end()},
		{mesh.indices.end() - indices, mesh.indices.end()},
		base_index,
	};
}

tr::gfx::simple_textured_mesh_ref tr::gfx::renderer_2d::new_textured_fan(int layer, usize vertices)
{
	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_textured_fan(layer, vertices, defaults.texture, transform, defaults.blend_mode);
	}
	else {
		return new_textured_fan(layer, vertices, std::nullopt, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_textured_mesh_ref tr::gfx::renderer_2d::new_textured_fan(int layer, usize vertices, texture_ref texture_ref)
{
	TR_ASSERT(!texture_ref.empty(), "Cannot pass std::nullopt as texture for textured mesh.");

	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_textured_fan(layer, vertices, std::move(texture_ref), transform, defaults.blend_mode);
	}
	else {
		return new_textured_fan(layer, vertices, std::move(texture_ref), m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_textured_mesh_ref tr::gfx::renderer_2d::new_textured_fan(int layer, usize vertices, texture_ref texture_ref,
																		 const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(!texture_ref.empty(), "Cannot pass std::nullopt as texture for textured fan.");

	mesh& mesh{find_mesh(layer, std::move(texture_ref), mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);

	fill_poly_idx(back_inserter(mesh.indices), u16(vertices), base_index);

	return {
		{mesh.positions.end() - vertices, mesh.positions.end()},
		{mesh.uvs.end() - vertices, mesh.uvs.end()},
		{mesh.tints.end() - vertices, mesh.tints.end()},
	};
}

tr::gfx::textured_mesh_ref tr::gfx::renderer_2d::new_textured_mesh(int layer, usize vertices, usize indices)
{
	const std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_textured_mesh(layer, vertices, indices, defaults.texture, transform, defaults.blend_mode);
	}
	else {
		return new_textured_mesh(layer, vertices, indices, std::nullopt, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::textured_mesh_ref tr::gfx::renderer_2d::new_textured_mesh(int layer, usize vertices, usize indices, texture_ref texture_ref)
{
	TR_ASSERT(!texture_ref.empty(), "Cannot pass std::nullopt as texture for textured mesh.");

	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_textured_mesh(layer, vertices, indices, std::move(texture_ref), transform, defaults.blend_mode);
	}
	else {
		return new_textured_mesh(layer, vertices, indices, std::move(texture_ref), m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::textured_mesh_ref tr::gfx::renderer_2d::new_textured_mesh(int layer, usize vertices, usize indices, texture_ref texture_ref,
																   const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(!texture_ref.empty(), "Cannot pass std::nullopt as texture for textured mesh.");

	mesh& mesh{find_mesh(layer, std::move(texture_ref), mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	return {
		{mesh.positions.end() - vertices, mesh.positions.end()},
		{mesh.uvs.end() - vertices, mesh.uvs.end()},
		{mesh.tints.end() - vertices, mesh.tints.end()},
		{mesh.indices.end() - indices, mesh.indices.end()},
		base_index,
	};
}

//

void tr::gfx::renderer_2d::draw_layer(int layer, const render_target& target)
{
	auto range{std::ranges::equal_range(m_meshes, layer, std::less{}, &mesh::layer)};
	draw(range.begin(), range.end(), target);
}

void tr::gfx::renderer_2d::draw_up_to_layer(int max_layer, const render_target& target)
{
	auto last{std::ranges::upper_bound(m_meshes, max_layer, std::less{}, &mesh::layer)};
	draw(m_meshes.begin(), last, target);
}

void tr::gfx::renderer_2d::draw(const render_target& target)
{
	draw_up_to_layer(INT_MAX, target);
}

//

tr::gfx::renderer_2d::mesh& tr::gfx::renderer_2d::find_mesh(int layer, texture_ref texture_ref, const glm::mat4& mat,
															const blend_mode& blend_mode, usize space_needed)
{
	auto range{std::ranges::equal_range(m_meshes, layer, std::less{}, &mesh::layer)};
	std::vector<mesh>::iterator it;
	if (texture_ref.empty()) {
		auto find_suitable{[&](const mesh& mesh) {
			return mesh.mat == mat && mesh.blend_mode == blend_mode && mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		it = std::ranges::find_if(range, find_suitable);
	}
	else {
		auto find_suitable{[&](const mesh& mesh) {
			return (mesh.texture.empty() || mesh.texture == texture_ref) && mesh.mat == mat && mesh.blend_mode == blend_mode &&
				   mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		it = std::ranges::find_if(range, find_suitable);
		if (it != range.end() && it->texture.empty()) {
			it->texture = std::move(texture_ref);
		}
	}

	if (it == range.end()) {
		it = m_meshes.emplace(range.end(), layer, std::move(texture_ref), mat, blend_mode);
	}
	return *it;
}

void tr::gfx::renderer_2d::setup_context()
{
	if (active_renderer != RENDERER_2D_ID) {
		active_renderer = RENDERER_2D_ID;
		set_face_culling(false);
		set_depth_test(false);
		set_shader_pipeline(m_pipeline);
		set_blend_mode(m_last_blend_mode);
		set_vertex_format(vertex2_format());
	}
}

std::vector<tr::gfx::renderer_2d::mesh_draw_info> tr::gfx::renderer_2d::upload_meshes(std::vector<mesh>::iterator first,
																					  std::vector<mesh>::iterator last)
{
	std::vector<mesh_draw_info> mesh_borders;
	mesh_borders.emplace_back(0, 0);

	const usize vertices{std::accumulate(first, last, 0_uz, [](usize s, const mesh& m) { return s + m.positions.size(); })};
	const usize indices{std::accumulate(first, last, 0_uz, [](usize s, const mesh& m) { return s + m.indices.size(); })};

	m_vbuffer_positions.resize(vertices);
	m_vbuffer_uvs.resize(vertices);
	m_vbuffer_tints.resize(vertices);
	m_ibuffer.resize(indices);

	for (const mesh& mesh : std::ranges::subrange{first, last}) {
		const mesh_draw_info start{mesh_borders.back()};

		m_vbuffer_positions.set_region(start.vertex_offset, mesh.positions);
		m_vbuffer_uvs.set_region(start.vertex_offset, mesh.uvs);
		m_vbuffer_tints.set_region(start.vertex_offset, mesh.tints);
		m_ibuffer.set_region(start.index_offset, mesh.indices);
		mesh_borders.emplace_back(start.vertex_offset + mesh.positions.size(), start.index_offset + mesh.indices.size());
	}

	set_index_buffer(m_ibuffer);

	return mesh_borders;
}

void tr::gfx::renderer_2d::setup_draw_call_state(texture_ref texture_ref, const glm::mat4& transform, const blend_mode& blend_mode)
{
	m_pipeline.fragment_shader().set_uniform(1, std::move(texture_ref));

	if (m_last_transform != transform) {
		m_last_transform = transform;
		m_pipeline.vertex_shader().set_uniform(0, m_last_transform);
	}

	if (m_last_blend_mode != blend_mode) {
		m_last_blend_mode = blend_mode;
		set_blend_mode(m_last_blend_mode);
	}
}

void tr::gfx::renderer_2d::draw(std::vector<mesh>::iterator first, std::vector<mesh>::iterator last, const render_target& target)
{
	if (first == last) {
		return;
	};

	setup_context();
	set_render_target(target);

	const std::vector<mesh_draw_info> meshes_info{upload_meshes(first, last)};
	std::vector<mesh_draw_info>::const_iterator mesh_info_it{meshes_info.begin()};
	for (const mesh& mesh : std::ranges::subrange{first, last}) {
		const usize mesh_indices{std::next(mesh_info_it)->index_offset - mesh_info_it->index_offset};

		setup_draw_call_state(mesh.texture, mesh.mat, mesh.blend_mode);
		set_vertex_buffer(m_vbuffer_positions, 0, mesh_info_it->vertex_offset);
		set_vertex_buffer(m_vbuffer_uvs, 1, mesh_info_it->vertex_offset);
		set_vertex_buffer(m_vbuffer_tints, 2, mesh_info_it->vertex_offset);
		draw_indexed(primitive::TRIS, mesh_info_it->index_offset, mesh_indices);

		++mesh_info_it;
	}

	m_meshes.erase(first, last);
}