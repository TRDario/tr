///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides implementation details of the system and graphics module.                                                                    //
//                                                                                                                                       //
// tr::sys::g_window is the global window manager. It encapsulates an SDL window, as well as all tr graphical state, which can be        //
// accessed using the .context() method. On windows, it also controls cursor resets, needed as a workaround for an SDL bug.              //
//                                                                                                                                       //
// tr::gfx::context is a class representing the graphical context, instanced in g_window. It encapsulates an OpenGL context, but deals   //
// in tr-specific state as well, such as the active renderer, current render target, and texture unit allocation.                        //
//                                                                                                                                       //
// TR_LOG_SDL_ERROR(...) logs a formatted error message to tr::log, appending the error from SDL on a newline afterwards:                //
//     - TR_LOG_SDL_ERROR("Example")                                                                                                     //
//       -> [21:14:01] [tr] [E] Example                                                                                                  //
//                          --- SDL error message here                                                                                   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "graphics_context.hpp"
#include "render_target.hpp"
#include "texture.hpp"

struct SDL_Window;
struct SDL_GLContextState;
namespace tr::gfx {
	struct properties;
}

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Graphical context manager class.
	class context {
	  public:
		// Creates a graphical context.
		void create();
		// Resets the graphical context.
		void reset();

		// Gets the base SDL OpenGL context pointer.
		SDL_GLContextState* ptr();
		// Gets the 2D vertex format.
		vertex_format& vertex2_format();

		// Checks whether the passed renderer ID is the active renderer, sets it as active and returns true if not.
		bool should_setup_context(renderer_id id);

		// Checks the render target's FBO ID.
		bool is_fbo_of_render_target(unsigned int fbo);
		// Sets the render target.
		void set_render_target(const render_target& target);
		// Clears the render target.
		void clear_render_target();

		// Allocates a texture unit.
		unsigned int allocate_texture_unit();
		// Sets the texture bound to a texture unit.
		void set_texture_unit(unsigned int unit, texture_ref texture);
		// Frees a texture unit.
		void free_texture_unit(unsigned int id);
		// Rebinds texture units bound to a texture that got reallocated.
		void rebind_texture_units(const texture& texture);

#ifdef TR_ENABLE_GL_CHECKS
		// Sets the current vertex format.
		void set_vertex_format(std::string&& name, std::span<const vertex_binding> bindings);
		// Checks if a vertex buffer's type's attribute match those of the current vertex format.
		void check_vertex_buffer(std::string label, int slot, std::span<const vertex_attribute> attrs);
#endif

	  private:
		struct deleter {
			void operator()(SDL_GLContextState* context) const;
		};

		// Pointer to the SDL OpenGL context.
		std::unique_ptr<SDL_GLContextState, deleter> m_ptr;
		// 2D vertex format.
		std::optional<vertex_format> m_vertex2_format;
		// ID of the current active renderer.
		renderer_id m_active_renderer{renderer_id::no_renderer};
		// The current render target.
		std::optional<render_target> m_render_target;
		// Tracks which texture units are allocated and what the textures bound to them are.
		std::array<std::optional<texture_ref>, 80> m_texture_units{};
#ifdef TR_ENABLE_GL_CHECKS
		// Bindings of the last bound vertex format.
		std::span<const vertex_binding> m_vertex_format_bindings;
		// Label of the last bound vertex format.
		std::string m_vertex_format_label;
#endif
	};
} // namespace tr::gfx

namespace tr::sys {
	// Window manager class.
	inline class window {
	  public:
		// Opens a window and returns true if opening was successful.
		bool open(cstring_view title, glm::ivec2 size, unsigned long flags, glm::ivec2 min_size, const gfx::properties& gfx_properties);
		// Closes the window.
		void close();

		// Gets whether the window is open.
		bool is_open() const;
		// Gets the base SDL window pointer.
		SDL_Window* ptr();
		// Gets the graphical context.
		gfx::context& gfx_context();

#ifdef _WIN32
		// Enables periodic resets of the cursor as a workaround for an SDL bug.
		void enable_cursor_resets();
		// Disables periodic resets of the cursor.
		void disable_cursor_resets();
#endif

	  private:
		struct deleter {
			void operator()(SDL_Window* window) const;
		};

		// Pointer to the SDL window.
		std::unique_ptr<SDL_Window, deleter> m_ptr{nullptr};
		// Graphical context.
		gfx::context m_context{};
#ifdef _WIN32
		// Cursor graphic reset timer needed as a workaround for an SDL bug.
		std::optional<timer> m_cursor_reset_timer;
#endif
	} g_window; // Global window manager
} // namespace tr::sys

// Logs a formatted SDL error to tr::log.
#define TR_LOG_SDL_ERROR(...)                                                                                                              \
	do {                                                                                                                                   \
		TR_LOG(::tr::log, ::tr::severity::error, __VA_ARGS__);                                                                             \
		TR_LOG_CONTINUE(::tr::log, "{}", SDL_GetError());                                                                                  \
	} while (0)