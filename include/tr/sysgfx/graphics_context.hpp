#pragma once
#include "vertex_buffer.hpp"

namespace tr {
	struct blend_mode;
	class dyn_index_buffer;
	class render_target;
	class shader_pipeline;
	class static_index_buffer;
} // namespace tr

namespace tr {
	// The ID used to represent no particular renderer being used.
	inline constexpr std::uint32_t NO_RENDERER{0};
	// Allocates a fresh renderer ID.
	std::uint32_t alloc_renderer_id() noexcept;

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

	// Direct graphics context access.
	struct gfx_context {
		// The graphics context log.
		static inline logger log{"gl"};

		// Gets whether the context is a debug context.
		static bool debug() noexcept;

		// Sets the active render target.
		static void set_render_target(const render_target& target) noexcept;
		// Resets the active render target.
		static void reset_render_target() noexcept;
		// Sets the active shader pipeline.
		static void set_shader_pipeline(const shader_pipeline& pipeline) noexcept;
		// Sets the active blending mode.
		static void set_blend_mode(const blend_mode& blend_mode) noexcept;
		// Sets the active vertex format.
		static void set_vertex_format(const vertex_format& format) noexcept;
		// Sets an active vertex buffer.
		static void set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, std::intptr_t offset,
									  std::size_t stride) noexcept;
		// Sets an active vertex buffer.
		template <standard_layout T>
		static void set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, std::intptr_t offset) noexcept;
		// Sets an active vertex buffer.
		static void set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, std::intptr_t offset, std::size_t stride) noexcept;
		// // Sets an active vertex buffer.
		template <standard_layout T>
		static void set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, std::intptr_t offset) noexcept;
		// Sets the active index buffer.
		static void set_index_buffer(const static_index_buffer& buffer) noexcept;
		// Sets the active index buffer.
		static void set_index_buffer(const dyn_index_buffer& buffer) noexcept;

		// Gets the ID of the currently set renderer.
		static std::uint32_t current_renderer() noexcept;
		// Sets the renderer ID.
		static void set_renderer(std::uint32_t id) noexcept;

		// Draws a mesh from a vertex buffer.
		static void draw(primitive type, std::size_t offset, std::size_t vertices) noexcept;
		// Draws an instanced mesh from a vertex buffer.
		static void draw_instances(primitive type, std::size_t offset, std::size_t vertices, int instances) noexcept;
		// Draws an indexed mesh.
		static void draw_indexed(primitive type, std::size_t offset, std::size_t indices) noexcept;
		// Draws an instanced indexed mesh.
		static void draw_indexed_instances(primitive type, std::size_t offset, std::size_t indices, int instances) noexcept;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::standard_layout T>
void tr::gfx_context::set_vertex_buffer(const static_vertex_buffer<T>& buffer, int slot, std::intptr_t offset) noexcept
{
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
}

template <tr::standard_layout T>
void tr::gfx_context::set_vertex_buffer(const dyn_vertex_buffer<T>& buffer, int slot, std::intptr_t offset) noexcept
{
	set_vertex_buffer(buffer, slot, offset * sizeof(T), sizeof(T));
}