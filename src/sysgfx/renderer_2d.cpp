#include "../../include/tr/sysgfx/renderer_2d.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/index_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"

namespace tr {
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
	struct _layer_defaults {
		texture_ref texture{NO_TEXTURE};
		std::optional<glm::mat4> transform;
		blend_mode blend_mode{ALPHA_BLENDING};
	};

	// Mesh data.
	struct _mesh {
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
	struct _mesh_draw_info {
		// Starting offset within the vertex buffer.
		std::size_t vertex_offset;
		// Starting offset within the index buffer.
		std::size_t index_offset;
	};

	// 2D renderer implementation.
	struct _renderer_2d_t {
		// Global default transform.
		glm::mat4 default_transform{1.0f};
		// Layer defaults.
		std::unordered_map<int, _layer_defaults> layer_defaults;
		// The list of meshes to draw.
		std::vector<_mesh> meshes;
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
		_renderer_2d_t() noexcept;

		// Finds an appropriate mesh.
		_mesh& find_mesh(int layer, texture_ref texture, const glm::mat4& mat, const blend_mode& blend_mode, std::size_t space_needed);
		// Sets up the graphical context for drawing.
		void setup_context() noexcept;
		// Uploads meshes to the GPU buffers.
		std::vector<_mesh_draw_info> upload_meshes(std::vector<_mesh>::iterator first, std::vector<_mesh>::iterator last);
		// Sets up the graphical context for a specific draw call.
		void setup_draw_call_state(texture_ref texture, const glm::mat4& transform, const blend_mode& blend_mode) noexcept;
		// Draws meshes.
		void draw(std::vector<_mesh>::iterator first, std::vector<_mesh>::iterator last, const render_target& target);
	};
	// 2D renderer state.
	std::optional<_renderer_2d_t> _renderer_2d;
} // namespace tr

tr::_renderer_2d_t::_renderer_2d_t() noexcept
{
	pipeline.vertex_shader().set_uniform(0, glm::mat4{1.0f});
	pipeline.fragment_shader().set_uniform(1, tex_unit);
	if (gfx_context::debug()) {
		pipeline.set_label("(tr) 2D Renderer Pipeline");
		pipeline.vertex_shader().set_label("(tr) 2D Renderer Vertex Shader");
		pipeline.fragment_shader().set_label("(tr) 2D Renderer Fragment Shader");
		vbuffer_positions.set_label("(tr) 2D Renderer Vertex Position Buffer");
		vbuffer_uvs.set_label("(tr) 2D Renderer Vertex UV Buffer");
		vbuffer_tints.set_label("(tr) 2D Renderer Vertex Tint Buffer");
		ibuffer.set_label("(tr) 2D Renderer Index Buffer");
	}
}

tr::_mesh& tr::_renderer_2d_t::find_mesh(int layer, texture_ref texture, const glm::mat4& mat, const blend_mode& blend_mode,
										 std::size_t space_needed)
{
	auto range{std::ranges::equal_range(meshes, layer, std::less{}, &_mesh::layer)};
	std::vector<_mesh>::iterator it;
	if (texture == NO_TEXTURE) {
		auto find_suitable{[&](const _mesh& mesh) {
			return mesh.mat == mat && mesh.blend_mode == blend_mode && mesh.positions.size() + space_needed <= UINT16_MAX;
		}};
		it = std::ranges::find_if(range, find_suitable);
	}
	else {
		auto find_suitable{[&](const _mesh& mesh) {
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

void tr::_renderer_2d_t::setup_context() noexcept
{
	if (gfx_context::current_renderer() != RENDERER_2D) {
		gfx_context::set_renderer(RENDERER_2D);
		gfx_context::set_shader_pipeline(pipeline);
		gfx_context::set_blend_mode(last_blend_mode);
		gfx_context::set_vertex_format(vertex2_format());
	}
}

std::vector<tr::_mesh_draw_info> tr::_renderer_2d_t::upload_meshes(std::vector<_mesh>::iterator first, std::vector<_mesh>::iterator last)
{
	std::vector<_mesh_draw_info> mesh_borders;
	mesh_borders.emplace_back(0, 0);

	const std::size_t total_vertices{
		std::accumulate(first, last, std::size_t{0}, [](std::size_t sum, const _mesh& m) { return sum + m.positions.size(); }),
	};
	const std::size_t total_indices{
		std::accumulate(first, last, std::size_t{0}, [](std::size_t sum, const _mesh& m) { return sum + m.indices.size(); }),
	};
	vbuffer_positions.resize(total_vertices);
	vbuffer_uvs.resize(total_vertices);
	vbuffer_tints.resize(total_vertices);
	ibuffer.resize(total_indices);
	for (const _mesh& mesh : std::ranges::subrange{first, last}) {
		const _mesh_draw_info start{mesh_borders.back()};

		vbuffer_positions.set_region(start.vertex_offset, mesh.positions);
		vbuffer_uvs.set_region(start.vertex_offset, mesh.uvs);
		vbuffer_tints.set_region(start.vertex_offset, mesh.tints);
		ibuffer.set_region(start.index_offset, mesh.indices);
		mesh_borders.emplace_back(start.vertex_offset + mesh.positions.size(), start.index_offset + mesh.indices.size());
	}
	gfx_context::set_index_buffer(ibuffer);
	return mesh_borders;
}

void tr::_renderer_2d_t::setup_draw_call_state(texture_ref texture, const glm::mat4& transform, const blend_mode& blend_mode) noexcept
{
	tex_unit.set_texture(texture);
	if (last_transform != transform) {
		last_transform = transform;
		pipeline.vertex_shader().set_uniform(0, last_transform);
	}
	if (last_blend_mode != blend_mode) {
		last_blend_mode = blend_mode;
		gfx_context::set_blend_mode(last_blend_mode);
	}
}

void tr::_renderer_2d_t::draw(std::vector<_mesh>::iterator first, std::vector<_mesh>::iterator last, const render_target& target)
{
	if (first == last) {
		return;
	};

	setup_context();
	gfx_context::set_render_target(target);

	const std::vector<_mesh_draw_info> draw_info{upload_meshes(first, last)};
	std::vector<_mesh_draw_info>::const_iterator it{draw_info.begin()};
	for (const _mesh& mesh : std::ranges::subrange{first, last}) {
		setup_draw_call_state(mesh.texture, mesh.mat, mesh.blend_mode);
		gfx_context::set_vertex_buffer(vbuffer_positions, 0, it->vertex_offset);
		gfx_context::set_vertex_buffer(vbuffer_uvs, 1, it->vertex_offset);
		gfx_context::set_vertex_buffer(vbuffer_tints, 2, it->vertex_offset);
		gfx_context::draw_indexed(primitive::TRIS, it->index_offset, std::next(it)->index_offset - it->index_offset);
		++it;
	}
	meshes.erase(first, last);
}

//

void tr::renderer_2d::initialize() noexcept
{
	TR_ASSERT(!_renderer_2d.has_value(), "Tried to reinitialize 2D renderer without shutting it down first.");

	_renderer_2d.emplace();
}

void tr::renderer_2d::shut_down() noexcept
{
	_renderer_2d.reset();
}

//

void tr::renderer_2d::set_default_transform(const glm::mat4& mat) noexcept
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to set 2D renderer's default transform before initializing it.");

	_renderer_2d->default_transform = mat;
}

void tr::renderer_2d::set_default_layer_texture(int layer, texture_ref texture) noexcept
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to set a 2D renderer's layer's default texture before initializing the renderer.");

	_renderer_2d->layer_defaults[layer].texture = texture;
}

void tr::renderer_2d::set_default_layer_transform(int layer, const glm::mat4& mat) noexcept
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to set a 2D renderer's layer's default transform before initializing the renderer.");

	_renderer_2d->layer_defaults[layer].transform = mat;
}

void tr::renderer_2d::set_default_layer_blend_mode(int layer, const blend_mode& blend_mode) noexcept
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to set a 2D renderer's layer's default blend mode before initializing the renderer.");

	_renderer_2d->layer_defaults[layer].blend_mode = blend_mode;
}

//

tr::simple_color_mesh_ref tr::renderer_2d::new_color_fan(int layer, std::size_t vertices)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new color fan before initializing the 2D renderer.");

	std::unordered_map<int, _layer_defaults>::iterator it{_renderer_2d->layer_defaults.find(layer)};
	if (it != _renderer_2d->layer_defaults.end()) {
		const _layer_defaults& defaults{it->second};
		return new_color_fan(layer, vertices, defaults.transform.has_value() ? *defaults.transform : _renderer_2d->default_transform,
							 defaults.blend_mode);
	}
	else {
		return new_color_fan(layer, vertices, _renderer_2d->default_transform, ALPHA_BLENDING);
	}
}

tr::simple_color_mesh_ref tr::renderer_2d::new_color_fan(int layer, std::size_t vertices, const glm::mat4& mat,
														 const blend_mode& blend_mode)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new color fan before initializing the 2D renderer.");

	_mesh& mesh{_renderer_2d->find_mesh(layer, NO_TEXTURE, mat, blend_mode, vertices)};
	const std::uint16_t base_index{static_cast<std::uint16_t>(mesh.positions.size())};
	mesh.positions.resize(mesh.positions.size() + vertices);
	mesh.uvs.resize(mesh.uvs.size() + vertices);
	std::fill(mesh.uvs.end() - vertices, mesh.uvs.end(), UNTEXTURED_UV);
	mesh.tints.resize(mesh.tints.size() + vertices);
	fill_poly_idx(back_inserter(mesh.indices), static_cast<std::uint16_t>(vertices), base_index);
	return {{mesh.positions.end() - vertices, mesh.positions.end()}, {mesh.tints.end() - vertices, mesh.tints.end()}};
}

tr::simple_color_mesh_ref tr::renderer_2d::new_color_outline(int layer, std::size_t vertices)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new color outline before initializing the 2D renderer.");

	std::unordered_map<int, _layer_defaults>::iterator it{_renderer_2d->layer_defaults.find(layer)};
	if (it != _renderer_2d->layer_defaults.end()) {
		const _layer_defaults& defaults{it->second};
		return new_color_outline(layer, vertices, defaults.transform.has_value() ? *defaults.transform : _renderer_2d->default_transform,
								 defaults.blend_mode);
	}
	else {
		return new_color_outline(layer, vertices, _renderer_2d->default_transform, ALPHA_BLENDING);
	}
}

tr::simple_color_mesh_ref tr::renderer_2d::new_color_outline(int layer, std::size_t vertices, const glm::mat4& mat,
															 const blend_mode& blend_mode)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new color outline before initializing the 2D renderer.");

	_mesh& mesh{_renderer_2d->find_mesh(layer, NO_TEXTURE, mat, blend_mode, vertices * 2)};
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

tr::color_mesh_ref tr::renderer_2d::new_color_mesh(int layer, std::size_t vertices, std::size_t indices)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new color mesh before initializing the 2D renderer.");

	std::unordered_map<int, _layer_defaults>::iterator it{_renderer_2d->layer_defaults.find(layer)};
	if (it != _renderer_2d->layer_defaults.end()) {
		const _layer_defaults& defaults{it->second};
		return new_color_mesh(layer, vertices, indices,
							  defaults.transform.has_value() ? *defaults.transform : _renderer_2d->default_transform, defaults.blend_mode);
	}
	else {
		return new_color_mesh(layer, vertices, indices, _renderer_2d->default_transform, ALPHA_BLENDING);
	}
}

tr::color_mesh_ref tr::renderer_2d::new_color_mesh(int layer, std::size_t vertices, std::size_t indices, const glm::mat4& mat,
												   const blend_mode& blend_mode)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new color mesh before initializing the 2D renderer.");

	_mesh& mesh{_renderer_2d->find_mesh(layer, NO_TEXTURE, mat, blend_mode, vertices)};
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

tr::simple_textured_mesh_ref tr::renderer_2d::new_textured_fan(int layer, std::size_t vertices)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new textured fan before initializing the 2D renderer.");

	std::unordered_map<int, _layer_defaults>::iterator it{_renderer_2d->layer_defaults.find(layer)};
	if (it != _renderer_2d->layer_defaults.end()) {
		const _layer_defaults& defaults{it->second};
		return new_textured_fan(layer, vertices, defaults.texture,
								defaults.transform.has_value() ? *defaults.transform : _renderer_2d->default_transform,
								defaults.blend_mode);
	}
	else {
		return new_textured_fan(layer, vertices, NO_TEXTURE, _renderer_2d->default_transform, ALPHA_BLENDING);
	}
}

tr::simple_textured_mesh_ref tr::renderer_2d::new_textured_fan(int layer, std::size_t vertices, texture_ref texture)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new textured fan before initializing the 2D renderer.");
	TR_ASSERT(texture != NO_TEXTURE, "Cannot pass NO_TEXTURE as texture for textured mesh.");

	std::unordered_map<int, _layer_defaults>::iterator it{_renderer_2d->layer_defaults.find(layer)};
	if (it != _renderer_2d->layer_defaults.end()) {
		const _layer_defaults& defaults{it->second};
		return new_textured_fan(layer, vertices, texture,
								defaults.transform.has_value() ? *defaults.transform : _renderer_2d->default_transform,
								defaults.blend_mode);
	}
	else {
		return new_textured_fan(layer, vertices, texture, _renderer_2d->default_transform, ALPHA_BLENDING);
	}
}

tr::simple_textured_mesh_ref tr::renderer_2d::new_textured_fan(int layer, std::size_t vertices, texture_ref texture, const glm::mat4& mat,
															   const blend_mode& blend_mode)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new textured fan before initializing the 2D renderer.");
	TR_ASSERT(texture != NO_TEXTURE, "Cannot pass NO_TEXTURE as texture for textured fan.");

	_mesh& mesh{_renderer_2d->find_mesh(layer, texture, mat, blend_mode, vertices)};
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

tr::textured_mesh_ref tr::renderer_2d::new_textured_mesh(int layer, std::size_t vertices, std::size_t indices)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new textured mesh before initializing the 2D renderer.");

	const std::unordered_map<int, _layer_defaults>::iterator it{_renderer_2d->layer_defaults.find(layer)};
	if (it != _renderer_2d->layer_defaults.end()) {
		const _layer_defaults& defaults{it->second};
		return new_textured_mesh(layer, vertices, indices, defaults.texture,
								 defaults.transform.has_value() ? *defaults.transform : _renderer_2d->default_transform,
								 defaults.blend_mode);
	}
	else {
		return new_textured_mesh(layer, vertices, indices, NO_TEXTURE, _renderer_2d->default_transform, ALPHA_BLENDING);
	}
}

tr::textured_mesh_ref tr::renderer_2d::new_textured_mesh(int layer, std::size_t vertices, std::size_t indices, texture_ref texture)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new textured mesh before initializing the 2D renderer.");
	TR_ASSERT(texture != NO_TEXTURE, "Cannot pass NO_TEXTURE as texture for textured mesh.");

	std::unordered_map<int, _layer_defaults>::iterator it{_renderer_2d->layer_defaults.find(layer)};
	if (it != _renderer_2d->layer_defaults.end()) {
		const _layer_defaults& defaults{it->second};
		return new_textured_mesh(layer, vertices, indices, texture,
								 defaults.transform.has_value() ? *defaults.transform : _renderer_2d->default_transform,
								 defaults.blend_mode);
	}
	else {
		return new_textured_mesh(layer, vertices, indices, texture, _renderer_2d->default_transform, ALPHA_BLENDING);
	}
}

tr::textured_mesh_ref tr::renderer_2d::new_textured_mesh(int layer, std::size_t vertices, std::size_t indices, texture_ref texture,
														 const glm::mat4& mat, const blend_mode& blend_mode)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to allocate a new textured mesh before initializing the 2D renderer.");
	TR_ASSERT(texture != NO_TEXTURE, "Cannot pass NO_TEXTURE as texture for textured mesh.");

	_mesh& mesh{_renderer_2d->find_mesh(layer, texture, mat, blend_mode, vertices)};
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

void tr::renderer_2d::draw_layer(int layer, const render_target& target)
{
	TR_ASSERT(_renderer_2d.has_value(), "Tried to draw a layer before initializing the 2D renderer.");

	auto range{std::ranges::equal_range(_renderer_2d->meshes, layer, std::equal_to{}, &_mesh::layer)};
	_renderer_2d->draw(range.begin(), range.end(), target);
}

void tr::renderer_2d::draw_up_to_layer(int max_layer, const render_target& target)
{
	auto last{std::ranges::upper_bound(_renderer_2d->meshes, max_layer, std::less{}, &_mesh::layer)};
	_renderer_2d->draw(_renderer_2d->meshes.begin(), last, target);
}

void tr::renderer_2d::draw(const render_target& target)
{
	draw_up_to_layer(INT_MAX, target);
}