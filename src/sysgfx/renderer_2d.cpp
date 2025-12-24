#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/renderer_2d.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture.hpp"

namespace tr::gfx {
#include "../../resources/generated/BASIC_RENDERER_FRAG.hpp"
#include "../../resources/generated/BASIC_RENDERER_VERT.hpp"

	// Untextured UV sentinel.
	inline constexpr glm::vec2 UNTEXTURED_UV{-100, -100};
} // namespace tr::gfx

//

tr::gfx::renderer_2d::renderer_2d()
	: m_pipeline{vertex_shader{BASIC_RENDERER_VERT}, fragment_shader{BASIC_RENDERER_FRAG}}
{
	TR_SET_LABEL(m_pipeline, "(tr) 2D Renderer Pipeline");
	TR_SET_LABEL(m_pipeline.vertex_shader(), "(tr) 2D Renderer Vertex Shader");
	TR_SET_LABEL(m_pipeline.fragment_shader(), "(tr) 2D Renderer Fragment Shader");
	TR_SET_LABEL(m_vbuffer_positions, "(tr) 2D Renderer Vertex Position Buffer");
	TR_SET_LABEL(m_vbuffer_uvs, "(tr) 2D Renderer Vertex UV Buffer");
	TR_SET_LABEL(m_vbuffer_tints, "(tr) 2D Renderer Vertex Tint Buffer");
	TR_SET_LABEL(m_ibuffer, "(tr) 2D Renderer Index Buffer");

	m_pipeline.vertex_shader().set_uniform(0, glm::mat4{1.0f});
}

//

void tr::gfx::renderer_2d::set_default_transform(const glm::mat4& mat)
{
	TR_ASSERT(!m_locked, "Tried to set default transform of locked 2D renderer.");

	m_default_transform = mat;
}

void tr::gfx::renderer_2d::set_default_layer_texture(int layer, texture_ref texture)
{
	TR_ASSERT(!m_locked, "Tried to set default layer texture of locked 2D renderer.");

	m_layer_defaults[layer].texture = std::move(texture);
}

void tr::gfx::renderer_2d::set_default_layer_transform(int layer, const glm::mat4& mat)
{
	TR_ASSERT(!m_locked, "Tried to set default layer transform of locked 2D renderer.");

	m_layer_defaults[layer].transform = mat;
}

void tr::gfx::renderer_2d::set_default_layer_blend_mode(int layer, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to set default layer blending mode of locked 2D renderer.");

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
	TR_ASSERT(!m_locked, "Tried to allocate a new color fan on a locked 2D renderer.");

	mesh& mesh{find_mesh(layer, primitive::TRIS, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};
	const usize indices{polygon_indices(vertices)};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, UNTEXTURED_UV);
	fill_convex_polygon_indices(index_range.begin(), u16(vertices), base_index);

	return {positions, colors};
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
	TR_ASSERT(!m_locked, "Tried to allocate a new color outline on a locked 2D renderer.");

	const usize vertices{polygon_vertices * 2};
	mesh& mesh{find_mesh(layer, primitive::TRIS, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};
	const usize indices{polygon_outline_indices(polygon_vertices)};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, UNTEXTURED_UV);
	fill_convex_polygon_outline_indices(index_range.begin(), u16(polygon_vertices), base_index);

	return {positions, colors};
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
	TR_ASSERT(!m_locked, "Tried to allocate a new color mesh on a locked 2D renderer.");

	mesh& mesh{find_mesh(layer, primitive::TRIS, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, UNTEXTURED_UV);

	return {positions, colors, index_range, base_index};
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
	TR_ASSERT(!m_locked, "Tried to allocate a new textured fan on a locked 2D renderer.");
	TR_ASSERT(!texture_ref.empty(), "Cannot pass std::nullopt as texture for textured fan.");

	mesh& mesh{find_mesh(layer, primitive::TRIS, std::move(texture_ref), mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};
	const usize indices{polygon_indices(vertices)};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange tints{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	fill_convex_polygon_indices(index_range.begin(), u16(vertices), base_index);

	return {positions, uvs, tints};
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
	TR_ASSERT(!m_locked, "Tried to allocate a new textured mesh on a locked 2D renderer.");
	TR_ASSERT(!texture_ref.empty(), "Cannot pass std::nullopt as texture for textured mesh.");

	mesh& mesh{find_mesh(layer, primitive::TRIS, std::move(texture_ref), mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange tints{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	return {positions, uvs, tints, index_range, base_index};
}

//

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_lines(int layer, usize lines)
{
	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_lines(layer, lines, transform, defaults.blend_mode);
	}
	else {
		return new_lines(layer, lines, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_lines(int layer, usize lines, const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new lines on a locked 2D renderer.");

	const usize vertices{lines * 2};
	mesh& mesh{find_mesh(layer, primitive::LINES, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + vertices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - vertices, mesh.indices.end()};

	std::ranges::fill(uvs, UNTEXTURED_UV);
	std::iota(index_range.begin(), index_range.end(), base_index);

	return {positions, colors};
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_line_strip(int layer, usize vertices)
{
	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_line_strip(layer, vertices, transform, defaults.blend_mode);
	}
	else {
		return new_line_strip(layer, vertices, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_line_strip(int layer, usize vertices, const glm::mat4& mat,
																	const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new line strip on a locked 2D renderer.");

	mesh& mesh{find_mesh(layer, primitive::LINES, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};
	const usize indices{line_strip_indices(vertices)};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, UNTEXTURED_UV);
	fill_line_strip_indices(index_range.begin(), vertices, base_index);

	return {positions, colors};
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_line_loop(int layer, usize vertices)
{
	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_line_loop(layer, vertices, transform, defaults.blend_mode);
	}
	else {
		return new_line_loop(layer, vertices, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_line_loop(int layer, usize vertices, const glm::mat4& mat,
																   const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new line loop on a locked 2D renderer.");

	mesh& mesh{find_mesh(layer, primitive::LINES, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};
	const usize indices{line_loop_indices(vertices)};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, UNTEXTURED_UV);
	fill_line_loop_indices(index_range.begin(), vertices, base_index);

	return {positions, colors};
}

tr::gfx::color_mesh_ref tr::gfx::renderer_2d::new_line_mesh(int layer, usize vertices, usize indices)
{
	std::unordered_map<int, layer_defaults>::iterator it{m_layer_defaults.find(layer)};
	if (it != m_layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		const glm::mat4& transform{defaults.transform.has_value() ? *defaults.transform : m_default_transform};
		return new_line_mesh(layer, vertices, indices, transform, defaults.blend_mode);
	}
	else {
		return new_line_mesh(layer, vertices, indices, m_default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::color_mesh_ref tr::gfx::renderer_2d::new_line_mesh(int layer, usize vertices, usize indices, const glm::mat4& mat,
															const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new line mesh on a locked 2D renderer.");

	mesh& mesh{find_mesh(layer, primitive::LINES, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{u16(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, UNTEXTURED_UV);

	return {positions, colors, index_range, base_index};
}

//

tr::gfx::renderer_2d::staggered_draw_manager tr::gfx::renderer_2d::prepare_staggered_draw_range(int min_layer, int max_layer)
{
	return staggered_draw_manager{*this,
								  {std::ranges::lower_bound(m_meshes, min_layer, std::less{}, &mesh::layer),
								   std::ranges::upper_bound(m_meshes, max_layer, std::less{}, &mesh::layer)}};
}

tr::gfx::renderer_2d::staggered_draw_manager tr::gfx::renderer_2d::prepare_staggered_draw()
{
	return staggered_draw_manager{*this, m_meshes};
}

void tr::gfx::renderer_2d::draw_layer(int layer, const render_target& target)
{
	staggered_draw_manager{*this, std::ranges::equal_range(m_meshes, layer, std::less{}, &mesh::layer)}.draw(target);
}

void tr::gfx::renderer_2d::draw_layer_range(int min_layer, int max_layer, const render_target& target)
{
	prepare_staggered_draw_range(min_layer, max_layer).draw(target);
}

void tr::gfx::renderer_2d::draw(const render_target& target)
{
	prepare_staggered_draw().draw(target);
}

//

tr::gfx::renderer_2d::mesh& tr::gfx::renderer_2d::find_mesh(int layer, primitive type, texture_ref texture_ref, const glm::mat4& mat,
															const blend_mode& blend_mode, usize space_needed)
{
	auto range{std::ranges::equal_range(m_meshes, layer, std::less{}, &mesh::layer)};
	std::vector<mesh>::iterator it;
	if (texture_ref.empty()) {
		auto find_suitable{[&](const mesh& mesh) {
			return mesh.type == type && mesh.mat == mat && mesh.blend_mode == blend_mode &&
				   mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		it = std::ranges::find_if(range, find_suitable);
	}
	else {
		auto find_suitable{[&](const mesh& mesh) {
			return mesh.type == type && (mesh.texture.empty() || mesh.texture == texture_ref) && mesh.mat == mat &&
				   mesh.blend_mode == blend_mode && mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		it = std::ranges::find_if(range, find_suitable);
		if (it != range.end() && it->texture.empty()) {
			it->texture = std::move(texture_ref);
		}
	}

	if (it == range.end()) {
		it = m_meshes.emplace(range.end(), layer, type, std::move(texture_ref), mat, blend_mode);
	}
	return *it;
}