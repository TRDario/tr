#include "../../include/tr/sysgfx/renderer_2d.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"

namespace tr::gfx::renderer_2d {
	// Untextured UV sentinel.
	inline constexpr glm::vec2 UNTEXTURED_UV{-100, -100};
	// Renderer ID.
	inline constexpr std::uint32_t RENDERER_2D{3};
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

	// Default layer information.
	struct layer_defaults {
		texture_ref texture{NO_TEXTURE};
		std::optional<glm::mat4> transform;
		blend_mode blend_mode{ALPHA_BLENDING};
	};

	// Mesh data.
	struct mesh {
		// The drawing priority of the mesh.
		int layer;
		// The texture used by the mesh.
		texture_ref texture;
		// The transformation matrix used by the mesh.
		glm::mat4 mat;
		// The blending mode used by the mesh.
		blend_mode blend_mode;
		// The positions of the vertices of the mesh.
		std::vector<glm::vec2> positions;
		// The UVs of the vertices of the mesh.
		std::vector<glm::vec2> uvs;
		// The tints of the vertices of the mesh.
		std::vector<tr::rgba8> tints;
		// The indices of the mesh.
		std::vector<std::uint16_t> indices;
	};

	// Mesh drawing information.
	struct mesh_draw_info {
		// Starting offset within the vertex buffer.
		std::size_t vertex_offset;
		// Starting offset within the index buffer.
		std::size_t index_offset;
	};

	// 2D renderer implementation.
	struct state_t {
		// Global default transform.
		glm::mat4 default_transform{1.0f};
		// Layer defaults.
		std::unordered_map<int, layer_defaults> layer_defaults;
		// The list of meshes to draw.
		std::vector<mesh> meshes;
		// The pipeline and shaders used by the renderer.
		owning_shader_pipeline pipeline{vertex_shader{RENDERER_2D_VERT_SRC}, fragment_shader{RENDERER_2D_FRAG_SRC}};
		// The texture unit used by the renderer.
		texture_unit tex_unit;
		// Vertex buffer for the positions of the vertices.
		dyn_vertex_buffer<glm::vec2> vbuffer_positions;
		// Vertex buffer for the UVs of the vertices.
		dyn_vertex_buffer<glm::vec2> vbuffer_uvs;
		// Vertex buffer for the tints of the vertices.
		dyn_vertex_buffer<tr::rgba8> vbuffer_tints;
		// The index buffer used by the renderer.
		dyn_index_buffer ibuffer;
		// Last used transform.
		glm::mat4 last_transform{1.0f};
		// Last used blending mode.
		blend_mode last_blend_mode{ALPHA_BLENDING};

		// Initializes the 2D renderer.
		state_t();

		// Finds an appropriate mesh.
		mesh& find_mesh(int layer, texture_ref texture, const glm::mat4& mat, const blend_mode& blend_mode, std::size_t space_needed);
		// Sets up the graphical context for drawing.
		void setup_context();
		// Uploads meshes to the GPU buffers.
		std::vector<mesh_draw_info> upload_meshes(std::vector<mesh>::iterator first, std::vector<mesh>::iterator last);
		// Sets up the graphical context for a specific draw call.
		void setup_draw_call_state(texture_ref texture, const glm::mat4& transform, const blend_mode& blend_mode);
		// Draws meshes.
		void draw(std::vector<mesh>::iterator first, std::vector<mesh>::iterator last, const render_target& target);
	};
	// 2D renderer state.
	std::optional<state_t> state;
} // namespace tr::gfx::renderer_2d

tr::gfx::renderer_2d::state_t::state_t()
{
	pipeline.vertex_shader().set_uniform(0, glm::mat4{1.0f});
	pipeline.fragment_shader().set_uniform(1, tex_unit);
	if (debug()) {
		pipeline.set_label("(tr) 2D Renderer Pipeline");
		pipeline.vertex_shader().set_label("(tr) 2D Renderer Vertex Shader");
		pipeline.fragment_shader().set_label("(tr) 2D Renderer Fragment Shader");
		vbuffer_positions.set_label("(tr) 2D Renderer Vertex Position Buffer");
		vbuffer_uvs.set_label("(tr) 2D Renderer Vertex UV Buffer");
		vbuffer_tints.set_label("(tr) 2D Renderer Vertex Tint Buffer");
		ibuffer.set_label("(tr) 2D Renderer Index Buffer");
	}
}

tr::gfx::renderer_2d::mesh& tr::gfx::renderer_2d::state_t::find_mesh(int layer, texture_ref texture, const glm::mat4& mat,
																	 const blend_mode& blend_mode, std::size_t space_needed)
{
	auto range{std::ranges::equal_range(meshes, layer, std::less{}, &mesh::layer)};
	std::vector<mesh>::iterator it;
	if (texture == NO_TEXTURE) {
		auto find_suitable{[&](const mesh& mesh) {
			return mesh.mat == mat && mesh.blend_mode == blend_mode && mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		it = std::ranges::find_if(range, find_suitable);
	}
	else {
		auto find_suitable{[&](const mesh& mesh) {
			return (mesh.texture == NO_TEXTURE || mesh.texture == texture) && mesh.mat == mat && mesh.blend_mode == blend_mode &&
				   mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		it = std::ranges::find_if(range, find_suitable);
		if (it != range.end() && it->texture == NO_TEXTURE) {
			it->texture = texture;
		}
	}

	if (it == range.end()) {
		it = meshes.emplace(range.end(), layer, texture, mat, blend_mode);
	}
	return *it;
}

void tr::gfx::renderer_2d::state_t::setup_context()
{
	if (current_renderer() != RENDERER_2D) {
		set_renderer(RENDERER_2D);
		set_shader_pipeline(pipeline);
		set_blend_mode(last_blend_mode);
		set_vertex_format(vertex2_format());
	}
}

std::vector<tr::gfx::renderer_2d::mesh_draw_info> tr::gfx::renderer_2d::state_t::upload_meshes(std::vector<mesh>::iterator first,
																							   std::vector<mesh>::iterator last)
{
	std::vector<mesh_draw_info> mesh_borders;
	mesh_borders.emplace_back(0, 0);

	const std::size_t total_vertices{
		std::accumulate(first, last, std::size_t{0}, [](std::size_t sum, const mesh& m) { return sum + m.positions.size(); }),
	};
	const std::size_t total_indices{
		std::accumulate(first, last, std::size_t{0}, [](std::size_t sum, const mesh& m) { return sum + m.indices.size(); }),
	};
	vbuffer_positions.resize(total_vertices);
	vbuffer_uvs.resize(total_vertices);
	vbuffer_tints.resize(total_vertices);
	ibuffer.resize(total_indices);
	for (const mesh& mesh : std::ranges::subrange{first, last}) {
		const mesh_draw_info start{mesh_borders.back()};

		vbuffer_positions.set_region(start.vertex_offset, mesh.positions);
		vbuffer_uvs.set_region(start.vertex_offset, mesh.uvs);
		vbuffer_tints.set_region(start.vertex_offset, mesh.tints);
		ibuffer.set_region(start.index_offset, mesh.indices);
		mesh_borders.emplace_back(start.vertex_offset + mesh.positions.size(), start.index_offset + mesh.indices.size());
	}
	set_index_buffer(ibuffer);
	return mesh_borders;
}

void tr::gfx::renderer_2d::state_t::setup_draw_call_state(texture_ref texture, const glm::mat4& transform, const blend_mode& blend_mode)
{
	tex_unit.set_texture(texture);
	if (last_transform != transform) {
		last_transform = transform;
		pipeline.vertex_shader().set_uniform(0, last_transform);
	}
	if (last_blend_mode != blend_mode) {
		last_blend_mode = blend_mode;
		set_blend_mode(last_blend_mode);
	}
}

void tr::gfx::renderer_2d::state_t::draw(std::vector<mesh>::iterator first, std::vector<mesh>::iterator last, const render_target& target)
{
	if (first == last) {
		return;
	};

	setup_context();
	set_render_target(target);

	const std::vector<mesh_draw_info> draw_info{upload_meshes(first, last)};
	std::vector<mesh_draw_info>::const_iterator it{draw_info.begin()};
	for (const mesh& mesh : std::ranges::subrange{first, last}) {
		setup_draw_call_state(mesh.texture, mesh.mat, mesh.blend_mode);
		set_vertex_buffer(vbuffer_positions, 0, it->vertex_offset);
		set_vertex_buffer(vbuffer_uvs, 1, it->vertex_offset);
		set_vertex_buffer(vbuffer_tints, 2, it->vertex_offset);
		draw_indexed(primitive::TRIS, it->index_offset, std::next(it)->index_offset - it->index_offset);
		++it;
	}
	meshes.erase(first, last);
}

//

void tr::gfx::renderer_2d::initialize()
{
	TR_ASSERT(!state.has_value(), "Tried to reinitialize 2D renderer without shutting it down first.");

	state.emplace();
}

void tr::gfx::renderer_2d::shut_down()
{
	state.reset();
}

//

void tr::gfx::renderer_2d::set_default_transform(const glm::mat4& mat)
{
	TR_ASSERT(state.has_value(), "Tried to set 2D renderer's default transform before initializing it.");

	state->default_transform = mat;
}

void tr::gfx::renderer_2d::set_default_layer_texture(int layer, texture_ref texture)
{
	TR_ASSERT(state.has_value(), "Tried to set a 2D renderer's layer's default texture before initializing the renderer.");

	state->layer_defaults[layer].texture = texture;
}

void tr::gfx::renderer_2d::set_default_layer_transform(int layer, const glm::mat4& mat)
{
	TR_ASSERT(state.has_value(), "Tried to set a 2D renderer's layer's default transform before initializing the renderer.");

	state->layer_defaults[layer].transform = mat;
}

void tr::gfx::renderer_2d::set_default_layer_blend_mode(int layer, const blend_mode& blend_mode)
{
	TR_ASSERT(state.has_value(), "Tried to set a 2D renderer's layer's default blend mode before initializing the renderer.");

	state->layer_defaults[layer].blend_mode = blend_mode;
}

//

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_color_fan(int layer, std::size_t vertices)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new color fan before initializing the 2D renderer.");

	std::unordered_map<int, layer_defaults>::iterator it{state->layer_defaults.find(layer)};
	if (it != state->layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		return new_color_fan(layer, vertices, defaults.transform.has_value() ? *defaults.transform : state->default_transform,
							 defaults.blend_mode);
	}
	else {
		return new_color_fan(layer, vertices, state->default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_color_fan(int layer, std::size_t vertices, const glm::mat4& mat,
																   const blend_mode& blend_mode)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new color fan before initializing the 2D renderer.");

	mesh& mesh{state->find_mesh(layer, NO_TEXTURE, mat, blend_mode, vertices)};
	const std::uint16_t base_index{static_cast<std::uint16_t>(mesh.positions.size())};
	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	std::fill(mesh.uvs.end() - vertices, mesh.uvs.end(), UNTEXTURED_UV);
	mesh.tints.resize(mesh.tints.size() + vertices);
	fill_poly_idx(back_inserter(mesh.indices), static_cast<std::uint16_t>(vertices), base_index);
	return {{mesh.positions.end() - vertices, mesh.positions.end()}, {mesh.tints.end() - vertices, mesh.tints.end()}};
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_color_outline(int layer, std::size_t vertices)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new color outline before initializing the 2D renderer.");

	std::unordered_map<int, layer_defaults>::iterator it{state->layer_defaults.find(layer)};
	if (it != state->layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		return new_color_outline(layer, vertices, defaults.transform.has_value() ? *defaults.transform : state->default_transform,
								 defaults.blend_mode);
	}
	else {
		return new_color_outline(layer, vertices, state->default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_color_mesh_ref tr::gfx::renderer_2d::new_color_outline(int layer, std::size_t vertices, const glm::mat4& mat,
																	   const blend_mode& blend_mode)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new color outline before initializing the 2D renderer.");

	mesh& mesh{state->find_mesh(layer, NO_TEXTURE, mat, blend_mode, vertices * 2)};
	const std::uint16_t base_index{static_cast<std::uint16_t>(mesh.positions.size())};
	mesh.positions.resize(mesh.positions.size() + vertices * 2);
	mesh.uvs.resize(mesh.uvs.size() + vertices * 2);
	std::fill(mesh.uvs.end() - vertices * 2, mesh.uvs.end(), UNTEXTURED_UV);
	mesh.tints.resize(mesh.tints.size() + vertices * 2);
	fill_poly_outline_idx(back_inserter(mesh.indices), static_cast<std::uint16_t>(vertices), base_index);
	return {
		{mesh.positions.end() - vertices * 2, mesh.positions.end()},
		{mesh.tints.end() - vertices * 2, mesh.tints.end()},
	};
}

tr::gfx::color_mesh_ref tr::gfx::renderer_2d::new_color_mesh(int layer, std::size_t vertices, std::size_t indices)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new color mesh before initializing the 2D renderer.");

	std::unordered_map<int, layer_defaults>::iterator it{state->layer_defaults.find(layer)};
	if (it != state->layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		return new_color_mesh(layer, vertices, indices, defaults.transform.has_value() ? *defaults.transform : state->default_transform,
							  defaults.blend_mode);
	}
	else {
		return new_color_mesh(layer, vertices, indices, state->default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::color_mesh_ref tr::gfx::renderer_2d::new_color_mesh(int layer, std::size_t vertices, std::size_t indices, const glm::mat4& mat,
															 const blend_mode& blend_mode)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new color mesh before initializing the 2D renderer.");

	mesh& mesh{state->find_mesh(layer, NO_TEXTURE, mat, blend_mode, vertices)};
	const std::uint16_t base_index{static_cast<std::uint16_t>(mesh.positions.size())};
	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	std::fill(mesh.uvs.end() - vertices, mesh.uvs.end(), UNTEXTURED_UV);
	mesh.tints.resize(mesh.tints.size() + vertices);
	mesh.indices.resize(mesh.indices.size() + indices);
	return {
		{mesh.positions.end() - vertices, mesh.positions.end()},
		{mesh.tints.end() - vertices, mesh.tints.end()},
		{mesh.indices.end() - indices, mesh.indices.end()},
		base_index,
	};
}

tr::gfx::simple_textured_mesh_ref tr::gfx::renderer_2d::new_textured_fan(int layer, std::size_t vertices)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new textured fan before initializing the 2D renderer.");

	std::unordered_map<int, layer_defaults>::iterator it{state->layer_defaults.find(layer)};
	if (it != state->layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		return new_textured_fan(layer, vertices, defaults.texture,
								defaults.transform.has_value() ? *defaults.transform : state->default_transform, defaults.blend_mode);
	}
	else {
		return new_textured_fan(layer, vertices, NO_TEXTURE, state->default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_textured_mesh_ref tr::gfx::renderer_2d::new_textured_fan(int layer, std::size_t vertices, texture_ref texture)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new textured fan before initializing the 2D renderer.");
	TR_ASSERT(texture != NO_TEXTURE, "Cannot pass NO_TEXTURE as texture for textured mesh.");

	std::unordered_map<int, layer_defaults>::iterator it{state->layer_defaults.find(layer)};
	if (it != state->layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		return new_textured_fan(layer, vertices, texture, defaults.transform.has_value() ? *defaults.transform : state->default_transform,
								defaults.blend_mode);
	}
	else {
		return new_textured_fan(layer, vertices, texture, state->default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::simple_textured_mesh_ref tr::gfx::renderer_2d::new_textured_fan(int layer, std::size_t vertices, texture_ref texture,
																		 const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new textured fan before initializing the 2D renderer.");
	TR_ASSERT(texture != NO_TEXTURE, "Cannot pass NO_TEXTURE as texture for textured fan.");

	mesh& mesh{state->find_mesh(layer, texture, mat, blend_mode, vertices)};
	const std::uint16_t base_index{static_cast<std::uint16_t>(mesh.positions.size())};
	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	mesh.tints.resize(mesh.tints.size() + vertices);
	fill_poly_idx(back_inserter(mesh.indices), static_cast<std::uint16_t>(vertices), base_index);
	return {
		{mesh.positions.end() - vertices, mesh.positions.end()},
		{mesh.uvs.end() - vertices, mesh.uvs.end()},
		{mesh.tints.end() - vertices, mesh.tints.end()},
	};
}

tr::gfx::textured_mesh_ref tr::gfx::renderer_2d::new_textured_mesh(int layer, std::size_t vertices, std::size_t indices)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new textured mesh before initializing the 2D renderer.");

	const std::unordered_map<int, layer_defaults>::iterator it{state->layer_defaults.find(layer)};
	if (it != state->layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		return new_textured_mesh(layer, vertices, indices, defaults.texture,
								 defaults.transform.has_value() ? *defaults.transform : state->default_transform, defaults.blend_mode);
	}
	else {
		return new_textured_mesh(layer, vertices, indices, NO_TEXTURE, state->default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::textured_mesh_ref tr::gfx::renderer_2d::new_textured_mesh(int layer, std::size_t vertices, std::size_t indices,
																   texture_ref texture)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new textured mesh before initializing the 2D renderer.");
	TR_ASSERT(texture != NO_TEXTURE, "Cannot pass NO_TEXTURE as texture for textured mesh.");

	std::unordered_map<int, layer_defaults>::iterator it{state->layer_defaults.find(layer)};
	if (it != state->layer_defaults.end()) {
		const layer_defaults& defaults{it->second};
		return new_textured_mesh(layer, vertices, indices, texture,
								 defaults.transform.has_value() ? *defaults.transform : state->default_transform, defaults.blend_mode);
	}
	else {
		return new_textured_mesh(layer, vertices, indices, texture, state->default_transform, ALPHA_BLENDING);
	}
}

tr::gfx::textured_mesh_ref tr::gfx::renderer_2d::new_textured_mesh(int layer, std::size_t vertices, std::size_t indices,
																   texture_ref texture, const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(state.has_value(), "Tried to allocate a new textured mesh before initializing the 2D renderer.");
	TR_ASSERT(texture != NO_TEXTURE, "Cannot pass NO_TEXTURE as texture for textured mesh.");

	mesh& mesh{state->find_mesh(layer, texture, mat, blend_mode, vertices)};
	const std::uint16_t base_index{static_cast<std::uint16_t>(mesh.positions.size())};
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
	TR_ASSERT(state.has_value(), "Tried to draw a layer before initializing the 2D renderer.");

	auto range{std::ranges::equal_range(state->meshes, layer, std::equal_to{}, &mesh::layer)};
	state->draw(range.begin(), range.end(), target);
}

void tr::gfx::renderer_2d::draw_up_to_layer(int max_layer, const render_target& target)
{
	auto last{std::ranges::upper_bound(state->meshes, max_layer, std::less{}, &mesh::layer)};
	state->draw(state->meshes.begin(), last, target);
}

void tr::gfx::renderer_2d::draw(const render_target& target)
{
	draw_up_to_layer(INT_MAX, target);
}