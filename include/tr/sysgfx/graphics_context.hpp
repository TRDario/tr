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
	inline constexpr u32 NO_RENDERER{0};
	// Allocates a fresh renderer ID.
	u32 alloc_renderer_id();

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

	// Sets whether face culling should be used.
	void set_face_culling(bool arg);
	// Sets whether depth testing should be used.
	void set_depth_test(bool arg);

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
	void set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, std::intptr_t offset, usize stride);
	// Sets an active vertex buffer.
	template <standard_layout T> void set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, std::intptr_t offset);
	// Sets an active vertex buffer.
	void set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, std::intptr_t offset, usize stride);
	// // Sets an active vertex buffer.
	template <standard_layout T> void set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, std::intptr_t offset);
	// Sets the active index buffer.
	void set_index_buffer(const static_index_buffer& buffer);
	// Sets the active index buffer.
	void set_index_buffer(const dyn_index_buffer& buffer);

	// Gets the ID of the currently set renderer.
	u32 current_renderer();
	// Sets the renderer ID.
	void set_renderer(u32 id);

	// Draws a mesh from a vertex buffer.
	void draw(primitive type, usize offset, usize vertices);
	// Draws an instanced mesh from a vertex buffer.
	void draw_instances(primitive type, usize offset, usize vertices, int instances);
	// Draws an indexed mesh.
	void draw_indexed(primitive type, usize offset, usize indices);
	// Draws an instanced indexed mesh.
	void draw_indexed_instances(primitive type, usize offset, usize indices, int instances);
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