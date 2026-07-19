///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the renderer from basic_renderer.hpp.                                                                                      //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/basic_renderer.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/utility/draw_geometry.hpp"
#include "../../include/tr/utility/hash_map.hpp"

// Untextured UV sentinel.
constexpr glm::vec2 untextured_uv{-100, -100};

////////////////////////////////////////////////////////////// BASIC RENDERER /////////////////////////////////////////////////////////////

// Fragment shader source code.
#include "../../resources/generated/basic_renderer_frag.hpp"
// Vertex shader source code.
#include "../../resources/generated/basic_renderer_vert.hpp"

tr::basic_renderer::basic_renderer(graphics_context& context)
	: m_id{context.allocate_renderer_id()}
	, m_pipeline{context, vertex_shader{context, basic_renderer_vert}, fragment_shader{context, basic_renderer_frag}}
	, m_vbuffer_positions{context}
	, m_vbuffer_uvs{context}
	, m_vbuffer_tints{context}
	, m_ibuffer{context}
{
	m_pipeline.set_label("(tr) Basic Renderer Pipeline");
	m_pipeline.vertex_shader().set_label("(tr) Basic Renderer Vertex Shader");
	m_pipeline.fragment_shader().set_label("(tr) Basic Renderer Fragment Shader");
	m_vbuffer_positions.set_label("(tr) Basic Renderer Vertex Position Buffer");
	m_vbuffer_uvs.set_label("(tr) Basic Renderer Vertex UV Buffer");
	m_vbuffer_tints.set_label("(tr) Basic Renderer Vertex Tint Buffer");
	m_ibuffer.set_label("(tr) Basic Renderer Index Buffer");

	m_pipeline.vertex_shader().set_uniform(0, glm::mat4{1.0f});
}

//

void tr::basic_renderer::set_default_transform(const glm::mat4& mat)
{
	TR_ASSERT(!m_locked, "Tried to set default transform of locked basic renderer.");

	m_default_transform = mat;
}

void tr::basic_renderer::set_default_layer_texture(int layer, texture_ref texture)
{
	TR_ASSERT(!m_locked, "Tried to set default layer texture of locked basic renderer.");

	m_layer_defaults[layer].texture = std::move(texture);
}

void tr::basic_renderer::set_default_layer_transform(int layer, const glm::mat4& mat)
{
	TR_ASSERT(!m_locked, "Tried to set default layer transform of locked basic renderer.");

	m_layer_defaults[layer].transform = mat;
}

void tr::basic_renderer::set_default_layer_blend_mode(int layer, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to set default layer blending mode of locked basic renderer.");

	m_layer_defaults[layer].blend_mode = blend_mode;
}

//

tr::simple_color_mesh_ref tr::basic_renderer::new_color_fan(int layer, usize vertices)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_color_fan(layer, vertices, transform, defaults->blend_mode);
	}
	else {
		return new_color_fan(layer, vertices, m_default_transform, alpha_blending);
	}
}

tr::simple_color_mesh_ref tr::basic_renderer::new_color_fan(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new color fan on a locked basic renderer.");

	mesh& mesh{find_mesh(layer, primitive::tris, std::nullopt, mat, blend_mode, vertices)};
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

	std::ranges::fill(uvs, untextured_uv);
	fill_convex_polygon_indices(index_range.begin(), vertices, base_index);

	return {positions, colors};
}

tr::simple_color_mesh_ref tr::basic_renderer::new_color_outline(int layer, usize vertices)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_color_outline(layer, vertices, transform, defaults->blend_mode);
	}
	else {
		return new_color_outline(layer, vertices, m_default_transform, alpha_blending);
	}
}

tr::simple_color_mesh_ref tr::basic_renderer::new_color_outline(int layer, usize polygon_vertices, const glm::mat4& mat,
																const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new color outline on a locked basic renderer.");

	const usize vertices{polygon_vertices * 2};
	mesh& mesh{find_mesh(layer, primitive::tris, std::nullopt, mat, blend_mode, vertices)};
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

	std::ranges::fill(uvs, untextured_uv);
	fill_convex_polygon_outline_indices(index_range.begin(), polygon_vertices, base_index);

	return {positions, colors};
}

tr::color_mesh_ref tr::basic_renderer::new_color_mesh(int layer, usize vertices, usize indices)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_color_mesh(layer, vertices, indices, transform, defaults->blend_mode);
	}
	else {
		return new_color_mesh(layer, vertices, indices, m_default_transform, alpha_blending);
	}
}

tr::color_mesh_ref tr::basic_renderer::new_color_mesh(int layer, usize vertices, usize indices, const glm::mat4& mat,
													  const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new color mesh on a locked basic renderer.");

	mesh& mesh{find_mesh(layer, primitive::tris, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{static_cast<u16>(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, untextured_uv);

	return {positions, colors, index_range, base_index};
}

tr::simple_textured_mesh_ref tr::basic_renderer::new_textured_fan(int layer, usize vertices)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_textured_fan(layer, vertices, defaults->texture, transform, defaults->blend_mode);
	}
	else {
		return new_textured_fan(layer, vertices, std::nullopt, m_default_transform, alpha_blending);
	}
}

tr::simple_textured_mesh_ref tr::basic_renderer::new_textured_fan(int layer, usize vertices, texture_ref texture_ref)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_textured_fan(layer, vertices, std::move(texture_ref), transform, defaults->blend_mode);
	}
	else {
		return new_textured_fan(layer, vertices, std::move(texture_ref), m_default_transform, alpha_blending);
	}
}

tr::simple_textured_mesh_ref tr::basic_renderer::new_textured_fan(int layer, usize vertices, texture_ref texture_ref, const glm::mat4& mat,
																  const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new textured fan on a locked basic renderer.");
	TR_ASSERT(!texture_ref.empty(), "Cannot pass std::nullopt as texture for textured fan.");

	mesh& mesh{find_mesh(layer, primitive::tris, std::move(texture_ref), mat, blend_mode, vertices)};
	const u16 base_index{static_cast<u16>(mesh.positions.size())};
	const usize indices{polygon_indices(vertices)};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange tints{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	fill_convex_polygon_indices(index_range.begin(), vertices, base_index);

	return {positions, uvs, tints};
}

tr::textured_mesh_ref tr::basic_renderer::new_textured_mesh(int layer, usize vertices, usize indices)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_textured_mesh(layer, vertices, indices, defaults->texture, transform, defaults->blend_mode);
	}
	else {
		return new_textured_mesh(layer, vertices, indices, std::nullopt, m_default_transform, alpha_blending);
	}
}

tr::textured_mesh_ref tr::basic_renderer::new_textured_mesh(int layer, usize vertices, usize indices, texture_ref texture_ref)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_textured_mesh(layer, vertices, indices, std::move(texture_ref), transform, defaults->blend_mode);
	}
	else {
		return new_textured_mesh(layer, vertices, indices, std::move(texture_ref), m_default_transform, alpha_blending);
	}
}

tr::textured_mesh_ref tr::basic_renderer::new_textured_mesh(int layer, usize vertices, usize indices, texture_ref texture_ref,
															const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new textured mesh on a locked basic renderer.");
	TR_ASSERT(!texture_ref.empty(), "Cannot pass std::nullopt as texture for textured mesh.");

	mesh& mesh{find_mesh(layer, primitive::tris, std::move(texture_ref), mat, blend_mode, vertices)};
	const u16 base_index{static_cast<u16>(mesh.positions.size())};

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

tr::simple_color_mesh_ref tr::basic_renderer::new_lines(int layer, usize lines)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_lines(layer, lines, transform, defaults->blend_mode);
	}
	else {
		return new_lines(layer, lines, m_default_transform, alpha_blending);
	}
}

tr::simple_color_mesh_ref tr::basic_renderer::new_lines(int layer, usize lines, const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new lines on a locked basic renderer.");

	const usize vertices{lines * 2};
	mesh& mesh{find_mesh(layer, primitive::lines, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{static_cast<u16>(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + vertices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - vertices, mesh.indices.end()};

	std::ranges::fill(uvs, untextured_uv);
	std::iota(index_range.begin(), index_range.end(), base_index);

	return {positions, colors};
}

tr::simple_color_mesh_ref tr::basic_renderer::new_line_strip(int layer, usize vertices)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_line_strip(layer, vertices, transform, defaults->blend_mode);
	}
	else {
		return new_line_strip(layer, vertices, m_default_transform, alpha_blending);
	}
}

tr::simple_color_mesh_ref tr::basic_renderer::new_line_strip(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new line strip on a locked basic renderer.");

	mesh& mesh{find_mesh(layer, primitive::lines, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{static_cast<u16>(mesh.positions.size())};
	const usize indices{line_strip_indices(vertices)};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, untextured_uv);
	fill_line_strip_indices(index_range.begin(), vertices, base_index);

	return {positions, colors};
}

tr::simple_color_mesh_ref tr::basic_renderer::new_line_loop(int layer, usize vertices)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_line_loop(layer, vertices, transform, defaults->blend_mode);
	}
	else {
		return new_line_loop(layer, vertices, m_default_transform, alpha_blending);
	}
}

tr::simple_color_mesh_ref tr::basic_renderer::new_line_loop(int layer, usize vertices, const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new line loop on a locked basic renderer.");

	mesh& mesh{find_mesh(layer, primitive::lines, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{static_cast<u16>(mesh.positions.size())};
	const usize indices{line_loop_indices(vertices)};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, untextured_uv);
	fill_line_loop_indices(index_range.begin(), vertices, base_index);

	return {positions, colors};
}

tr::color_mesh_ref tr::basic_renderer::new_line_mesh(int layer, usize vertices, usize indices)
{
	const opt_ref<const layer_defaults> defaults{try_get(m_layer_defaults, layer)};
	if (defaults.has_ref()) {
		const glm::mat4& transform{defaults->transform.has_value() ? *defaults->transform : m_default_transform};
		return new_line_mesh(layer, vertices, indices, transform, defaults->blend_mode);
	}
	else {
		return new_line_mesh(layer, vertices, indices, m_default_transform, alpha_blending);
	}
}

tr::color_mesh_ref tr::basic_renderer::new_line_mesh(int layer, usize vertices, usize indices, const glm::mat4& mat,
													 const blend_mode& blend_mode)
{
	TR_ASSERT(!m_locked, "Tried to allocate a new line mesh on a locked basic renderer.");

	mesh& mesh{find_mesh(layer, primitive::lines, std::nullopt, mat, blend_mode, vertices)};
	const u16 base_index{static_cast<u16>(mesh.positions.size())};

	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);

	const std::ranges::subrange positions{mesh.positions.end() - vertices, mesh.positions.end()};
	const std::ranges::subrange uvs{mesh.uvs.end() - vertices, mesh.uvs.end()};
	const std::ranges::subrange colors{mesh.tints.end() - vertices, mesh.tints.end()};
	const std::ranges::subrange index_range{mesh.indices.end() - indices, mesh.indices.end()};

	std::ranges::fill(uvs, untextured_uv);

	return {positions, colors, index_range, base_index};
}

//

tr::basic_renderer::drawer tr::basic_renderer::create_drawer(int min_layer, int max_layer)
{
	return drawer{*this,
				  {std::ranges::lower_bound(m_meshes, min_layer, std::less{}, &mesh::layer),
				   std::ranges::upper_bound(m_meshes, max_layer, std::less{}, &mesh::layer)}};
}

tr::basic_renderer::drawer tr::basic_renderer::create_drawer()
{
	return drawer{*this, m_meshes};
}

void tr::basic_renderer::draw(const render_target& target)
{
	create_drawer().draw(target);
}

//

tr::basic_renderer::mesh& tr::basic_renderer::find_mesh(int layer, primitive type, texture_ref texture_ref, const glm::mat4& mat,
														const blend_mode& blend_mode, usize space_needed)
{
	auto range{std::ranges::equal_range(m_meshes, layer, std::less{}, &mesh::layer)};
	std::vector<mesh>::iterator mesh_it;
	if (texture_ref.empty()) {
		auto find_suitable{[&](const mesh& mesh) {
			return mesh.type == type && mesh.mat == mat && mesh.blend_mode == blend_mode &&
				   mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		mesh_it = std::ranges::find_if(range, find_suitable);
	}
	else {
		auto find_suitable{[&](const mesh& mesh) {
			return mesh.type == type && (mesh.texture.empty() || mesh.texture == texture_ref) && mesh.mat == mat &&
				   mesh.blend_mode == blend_mode && mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		mesh_it = std::ranges::find_if(range, find_suitable);
		if (mesh_it != range.end() && mesh_it->texture.empty()) {
			mesh_it->texture = std::move(texture_ref);
		}
	}

	if (mesh_it == range.end()) {
		mesh_it = m_meshes.emplace(range.end(), layer, type, std::move(texture_ref), mat, blend_mode);
	}
	return *mesh_it;
}