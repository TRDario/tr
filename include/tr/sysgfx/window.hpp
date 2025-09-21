#pragma once
#include "../utility/common.hpp"

namespace tr {
	class bitmap;
	class bitmap_view;
} // namespace tr

namespace tr::gfx {
	// Graphics context properties.
	struct properties {
		// Enables the use of depth and stencil buffers.
		bool enable_depth_stencil = false;
		// The number of samples used around a pixel for multisampled anti-aliasing.
		u8 multisamples = 0;
	};
} // namespace tr::gfx

namespace tr::sys {
	// Flash operations.
	enum class flash_operation {
		CANCEL,       // Cancel any existing window flashing, if there is any.
		BRIEF,        // Flash the window briefly.
		UNTIL_FOCUSED // Flash the window continuously until the window regains focus.
	};

	// V-sync modes.
	enum class vsync : i8 {
		ADAPTIVE = -1, // Vsync is enabled, but late swaps happen immediately instead of waiting for the next retrace.
		DISABLED,      // Vsync is disabled.
		ENABLED        // Vsync is enabled.
	};

	// Opens a windowed window.
	// May throw: init_error.
	void open_window(const char* title, glm::ivec2 size, bool resizable = false, const gfx::properties& gfx_properties = {});
	// Opens a fullscreen window.
	// May throw: init_error.
	void open_fullscreen_window(const char* title, bool resizable = false, const gfx::properties& gfx_properties = {});
	// Closes the window.
	void close_window();

	// Gets the title of the window.
	const char* window_title();
	// Sets the title of the window.
	void set_window_title(const char* title);
	// Sets the title of the window.
	void set_window_title(const std::string& title);

	// Sets the icon of the window.
	void set_window_icon(const bitmap& bitmap);
	// Sets the icon of the window.
	void set_window_icon(const bitmap_view& view);

	// Gets the size of the window.
	glm::ivec2 window_size();
	// Gets the window's pixel density factor.
	float window_pixel_density();
	// Sets the size of the window.
	void set_window_size(glm::ivec2 size);

	// Gets whether the window is fullscreen or not.
	bool window_fullscreen();
	// Sets whether the window is fullscreen or not.
	void set_window_fullscreen(bool fullscreen);

	// Unhides the window.
	void show_window();
	// Hides the window.
	void hide_window();

	// Gets whether the window is maximized.
	bool window_maximized();
	// Gets whether the window is minimized.
	bool window_minimized();
	// Gets whether the window has input focus.
	bool window_has_focus();
	// Raises the window to have input focus.
	void raise_window();

	// Sets whether the mouse is grabbed and confined to this window.
	void set_window_mouse_grab(bool grab);

	// Flashes the window to get the user's attention.
	void flash_window(flash_operation operation);

	// Sets the window's V-sync mode.
	void set_window_vsync(vsync vsync);
} // namespace tr::sys