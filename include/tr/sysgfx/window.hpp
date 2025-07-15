#pragma once
#include "../utility/common.hpp"

namespace tr {
	class bitmap;
	class bitmap_view;

	// Window flag bitmasks.
	enum class window_flag : std::uint32_t {
		DEFAULT = 0x0,         // Default flags.
		BORDERLESS = 0x10,     // The window has no decoration (topbar, etc.).
		RESIZABLE = 0x20,      // The window is resizable.
		MINIMIZED = 0x40,      // The window is minimized.
		MAXIMIZED = 0x80,      // The window is maximized.
		GRAB_INPUT = 0x200,    // The window has grabbed input focus.
		ALWAYS_ON_TOP = 0x8000 // The window is always on top.
	};
	DEFINE_BITMASK_OPERATORS(window_flag);

	// Graphics context properties.
	struct gfx_properties {
		// Whether a debug context should be used.
		bool debug_context = false;
		// Whether to use double buffering.
		bool double_buffer = true;
		// The number of bits to use for backbuffer depth.
		std::uint8_t depth_bits = 0;
		// The number of bits to use for backbuffer stencil.
		std::uint8_t stencil_bits = 0;
		// The number of samples used around a pixel for multisampled anti-aliasing.
		std::uint8_t multisamples = 0;
	};

	// Flash operations.
	enum class flash_operation {
		CANCEL,       // Cancel any existing window flashing, if there is any.
		BRIEF,        // Flash the window briefly.
		UNTIL_FOCUSED // Flash the window continuously until the window regains focus.
	};

	// V-sync modes.
	enum class vsync : std::int8_t {
		ADAPTIVE = -1, // Vsync is enabled, but late swaps happen immediately instead of waiting for the next retrace.
		DISABLED,      // Vsync is disabled.
		ENABLED        // Vsync is enabled.
	};

	// The main application window.
	struct window {
		// Opens a windowed window.
		static void open_windowed(const char* title, glm::ivec2 size, window_flag flags = window_flag::DEFAULT,
								  const gfx_properties& gfx_properties = {}) noexcept;
		// Opens a fullscreen window.
		static void open_fullscreen(const char* title, window_flag flags = window_flag::DEFAULT,
									const gfx_properties& gfx_properties = {}) noexcept;
		// Closes the window.
		static void close() noexcept;

		// Gets the title of the window.
		static const char* title() noexcept;
		// Sets the title of the window.
		static void set_title(const char* title) noexcept;
		// Sets the title of the window.
		static void set_title(const std::string& title) noexcept;

		// Sets the icon of the window.
		static void set_icon(const bitmap& bitmap) noexcept;
		// Sets the icon of the window.
		static void set_icon(const bitmap_view& view) noexcept;

		// Gets the size of the window.
		static glm::ivec2 size() noexcept;
		// Gets the window's pixel density factor.
		static float pixel_density() noexcept;
		// Sets the size of the window.
		static void set_size(glm::ivec2 size) noexcept;

		// Gets whether the window is fullscreen or not.
		static bool fullscreen() noexcept;
		// Sets whether the window is fullscreen or not.
		static void set_fullscreen(bool fullscreen) noexcept;

		// Unhides the window.
		static void show() noexcept;
		// Hides the window.
		static void hide() noexcept;

		// Gets whether the window is maximized.
		static bool maximized() noexcept;
		// Gets whether the window is minimized.
		static bool minimized() noexcept;
		// Gets whether the window has input focus.
		static bool has_focus() noexcept;

		// Sets whether the mouse is grabbed and confined to this window.
		static void set_mouse_grab(bool grab) noexcept;

		// Flashes the window to get the user's attention.
		static void flash(flash_operation operation) noexcept;

		// Sets the window's V-sync mode.
		static void set_vsync(vsync vsync) noexcept;
	};
} // namespace tr