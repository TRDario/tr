#pragma once
#include "vertex_buffer.hpp"

namespace tr::gfx {
	struct blend_mode;
	class dyn_index_buffer;
	class render_target;
	class shader_pipeline;
	class static_index_buffer;
	class vertex_format;

	// The ID used to represent no particular renderer being used.
	inline constexpr std::uint32_t NO_RENDERER{0};
	// Allocates a fresh renderer ID.
	std::uint32_t alloc_renderer_id();

	// Rendering primitives.
	enum class primitive {
		POINTS,     // The vertices are drawn as individual points.
		LINES,      // The vertices are drawn in pairs as lines.
		LINE_LOOP,  // The vertices are drawn as a continuous line loop.
		LINE_STRIP, // The vertices are drawn as a continuous line strip.
		TRIS,       // The vertices are drawn in groups of three as triangles.
		TRI_STRIP,  // The vertices are drawn as a continuous triangle strip.
		TRI_FAN     // The vertices are drawn as a continuous triangle fan.
	};

	// The graphics context log.
	inline logger log{"gl"};

	// Gets whether the context is a debug context.
	bool debug();

	// Sets the active render target.
	void set_render_target(const render_target& target);
	// Resets the active render target.
	void reset_render_target();
	// Sets the active shader pipeline.
	void set_shader_pipeline(const shader_pipeline& pipeline);
	// Sets the active blending mode.
	void set_blend_mode(const blend_mode& blend_mode);
	// Sets the active vertex format.
	void set_vertex_format(const vertex_format& format);
	// Sets an active vertex buffer.
	void set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, std::intptr_t offset, std::size_t stride);
	// Sets an active vertex buffer.
	template <standard_layout T> void set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, std::intptr_t offset);
	// Sets an active vertex buffer.
	void set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, std::intptr_t offset, std::size_t stride);
	// // Sets an active vertex buffer.
	template <standard_layout T> void set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, std::intptr_t offset);
	// Sets the active index buffer.
	void set_index_buffer(const static_index_buffer& buffer);
	// Sets the active index buffer.
	void set_index_buffer(const dyn_index_buffer& buffer);

	// Gets the ID of the currently set renderer.
	std::uint32_t current_renderer();
	// Sets the renderer ID.
	void set_renderer(std::uint32_t id);

	// Draws a mesh from a vertex buffer.
	void draw(primitive type, std::size_t offset, std::size_t vertices);
	// Draws an instanced mesh from a vertex buffer.
	void draw_instances(primitive type, std::size_t offset, std::size_t vertices, int instances);
	// Draws an indexed mesh.
	void draw_indexed(primitive type, std::size_t offset, std::size_t indices);
	// Draws an instanced indexed mesh.
	void draw_indexed_instances(primitive type, std::size_t offset, std::size_t indices, int instances);
} // namespace tr::gfx

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::standard_layout T> void tr::gfx::set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, std::intptr_t offset)
{
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
}

template <tr::standard_layout T> void tr::gfx::set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, std::intptr_t offset)
{
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
}