#pragma once
#include "render_target.hpp"
#include "texture.hpp"
#include "vertex_format.hpp"

struct SDL_Window;
struct SDL_GLContextState;

namespace tr::system {
	// Buffer allocated to be freed in case of an out-of-memory error.
	inline std::unique_ptr<char[]> emergency_buffer{new char[16384]};

	// The name of the application developer.
	inline const char* app_developer{nullptr};
	// The name of the application.
	inline const char* app_name{nullptr};

	// Pointer to the SDL window.
	inline SDL_Window* sdl_window{nullptr};
} // namespace tr::system

namespace tr::gfx {
	// Pointer to the SDL OpenGL context.
	inline SDL_GLContextState* ogl_context{nullptr};
	// Whether the OpenGL context is a debug one.
	inline bool debug_context{false};
	// List of known textures.
	inline std::unordered_set<unsigned int> textures;
	// Tracks which units are allocated.
	inline std::array<bool, 80> texture_units;
	// Tracks the texture bound to any given texture unit.
	inline std::array<texture_ref, 80> texture_unit_textures{};
	// 2D vertex format.
	inline std::optional<vertex_format> vertex2_format_;
	// The current render target.
	inline std::optional<render_target> current_render_target;
} // namespace tr::gfx