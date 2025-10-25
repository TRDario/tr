#pragma once
#include "render_target.hpp"
#include "texture.hpp"
#include "vertex_format.hpp"

struct SDL_Window;
struct SDL_GLContextState;

namespace tr::sys {
	// Buffer allocated to be freed in case of an out-of-memory error.
	inline std::unique_ptr<char[]> emergency_buffer{new char[16384]};

	// The name of the application developer.
	inline cstring_view app_developer;
	// The name of the application.
	inline cstring_view app_name;

	// Pointer to the SDL window.
	inline SDL_Window* sdl_window{nullptr};
} // namespace tr::sys

namespace tr::gfx {
	// Pointer to the SDL OpenGL context.
	inline SDL_GLContextState* ogl_context{nullptr};
	// Tracks the texture bound to any given texture unit.
	inline std::array<texture_ref, 80> texture_unit_textures{};
	// 2D vertex format.
	inline std::optional<vertex_format> vertex2_format_;
	// The current render target.
	inline std::optional<render_target> current_render_target;
} // namespace tr::gfx