#pragma once
#include "render_target.hpp"
#include "texture.hpp"
#include "vertex_format.hpp"

struct SDL_Window;
struct SDL_GLContextState;

namespace tr {
	// The name of the application developer.
	inline const char* _app_developer{nullptr};
	// The name of the application.
	inline const char* _app_name{nullptr};

	// Pointer to the SDL window.
	inline SDL_Window* sdl_window{nullptr};
	// Pointer to the SDL OpenGL context.
	inline SDL_GLContextState* ogl_context{nullptr};
	// Whether the OpenGL context is a debug one.
	inline bool _debug_context{false};

	// Tracks which units are allocated.
	inline std::array<bool, 80> _tex_units;
	// Tracks the texture bound to any given texture unit.
	inline std::array<texture_ref, 80> _tex_unit_textures{};
	// 2D vertex format.
	inline std::optional<vertex_format> _vertex2_format;
	// The current render target.
	inline std::optional<render_target> _render_target;

	// Alias for a list of free rects.
	using _free_rect_list = std::forward_list<tr::irect2>;
	// Alias for an iterator to a free list.
	using _free_rect_it = _free_rect_list::iterator;
	// Doubles the smaller component of a vector.
	glm::ivec2 _double_smaller_component(glm::ivec2 size) noexcept;
	// Gets the area of a rectangle.
	int _area(glm::ivec2 size) noexcept;
	// Finds the predecessor to the smallest suitable free rect or end() if none could be found.
	_free_rect_it _find_free_rect_prev(_free_rect_list& free_rects, glm::ivec2 size) noexcept;
	// Shrinks a free rect that has partially or fully been allocated to a bitmap.
	void _shrink_free_rect(_free_rect_list& free_rects, _free_rect_it prev, glm::ivec2 size);
} // namespace tr