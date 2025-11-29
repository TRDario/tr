#pragma once
#include "render_target.hpp"
#include "texture.hpp"
#include "vertex_format.hpp"

struct SDL_Window;
struct SDL_GLContextState;

namespace tr::sys {
	// Buffer allocated to be freed in case of an out-of-memory error.
	inline std::unique_ptr<char[]> g_emergency_buffer{new char[16384]};

	// The name of the application developer.
	inline cstring_view g_app_developer;
	// The name of the application.
	inline cstring_view g_app_name;

	// Pointer to the SDL window.
	inline SDL_Window* g_sdl_window{nullptr};
} // namespace tr::sys

namespace tr::gfx {
	// Pointer to the SDL OpenGL context.
	inline SDL_GLContextState* g_ogl_context{nullptr};
	// Tracks the texture bound to any given texture unit.
	inline std::array<texture_ref, 80> g_texture_unit_textures{};
	// 2D vertex format.
	inline std::optional<vertex_format> g_vertex2_format;
	// The current render target.
	inline std::optional<render_target> g_current_render_target;
} // namespace tr::gfx