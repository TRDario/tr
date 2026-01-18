///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides functionality related to the window.                                                                                         //
//                                                                                                                                       //
// tr is designed with the notion that only one window may be open at once, thus treats it as global state. All functionality from the   //
// tr::gfx namespace is inherently tied to the lifetime of the window it was created on; it cannot persist if the window is closed, even //
// if reopened later.                                                                                                                    //
//                                                                                                                                       //
// A window can be opened as windowed or fullscreen. When opening as windowed, the size of the window is passed, or tr::sys::maximized   //
// to set the window as maximized. If the window should be resizable, a minimum allowed size is to be passed, otherwise pass             //
// tr::sys::not_resizable to mark the window as non-resizable. Properties of the graphics context are also passed to the window opening  //
// function. A window is closed with tr::sys::close_window:                                                                              //
//     - tr::sys::open_window("My App")                                                                                                  //
//       -> opens a maximized, non-resizable window titled "My App"                                                                      //
//     - tr::sys::open_window("My App", {960, 720}, {640, 480}, {.multisamples = 8})                                                     //
//       -> opens a 960x720 window titled "My App", resizable down to 640x480, with x8 MSAA                                              //
//     - tr::sys::open_fullscreen_window("My App", tr::sys::not_resizable, {.enable_depth_stencil = true})                               //
//       -> opens a fullscreen, non-resizable window titled "My App" with enabled depth and stencil buffers                              //
//     - tr::sys::close_window() -> closes the window                                                                                    //
//                                                                                                                                       //
// It's important to note that the window starts out as hidden and must be set to be shown afterwards. Hiding and showing the window can //
// be done at any time:                                                                                                                  //
//     - tr::sys::open_window("My App")                                                                                                  //
//       ... (setup system, load things)                                                                                                 //
//       tr::sys::show_window() -> the window is now shown                                                                               //
//                                                                                                                                       //
// The window title can be gotten and reset:                                                                                             //
//     - tr::sys::window_title() -> "My App"                                                                                             //
//     - tr::sys::set_window_title("My Game") -> window title is updated to "My Game"                                                    //
//                                                                                                                                       //
// The window icon can be set:                                                                                                           //
//     - tr::sys::set_window_icon(bitmap) -> sets the window icon to the contents of the bitmap                                          //
//                                                                                                                                       //
// The window's size and pixel density (ratio between logical and actual pixels), as well as whether the window is fullscreen are        //
// gettable, and the size and fullscreen flag can be set:                                                                                //
//     - tr::sys::window_fullscreen() -> true                                                                                            //
//     - tr::sys::set_window_fullscreen(false)                                                                                           //
//       tr::sys::set_window_size({960, 720})                                                                                            //
//       tr::sys::window_size() -> {960, 720}                                                                                            //
//     - tr::sys::window_pixel_density() -> may be greater than 1 if the user has a different UI scaling factor, for example             //
//                                                                                                                                       //
// Whether the window is maximized, minimized, and if it has focus can be gotten, and the window can be raised to get window focus:      //
//     - tr::sys::window_maximized() -> true if the window is maximized, false otherwise                                                 //
//     - tr::sys::window_minimized() -> true if the window is minimized, false otherwise                                                 //
//     - tr::sys::window_has_focus() -> true if the window has input focus, false otherwise                                              //
//     - tr::sys::raise_window() -> tries to raise the window                                                                            //
//                                                                                                                                       //
// The window's V-sync mode can be set. If adaptive V-sync is not available, the system will try to fallback to regular V-sync:          //
//     - tr::sys::set_window_vsync(tr::vsync::adaptive) -> tries to enable adaptive V-sync                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/common.hpp" // IWYU pragma: keep

namespace tr {
	class bitmap;
	class bitmap_view;
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

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
	// Marks a window as maximized.
	constexpr glm::ivec2 maximized{};
	// Marks a window as not resizable.
	constexpr glm::ivec2 not_resizable{};

	// V-sync modes.
	enum class vsync : i8 {
		adaptive = -1, // Vsync is enabled, but late swaps happen immediately instead of waiting for the next retrace.
		disabled,      // Vsync is disabled.
		enabled        // Vsync is enabled.
	};

	// Opens a windowed window.
	// May throw: init_error.
	void open_window(cstring_view title, glm::ivec2 size = maximized, glm::ivec2 min_size = not_resizable,
					 const gfx::properties& gfx_properties = {});
	// Opens a fullscreen window.
	// May throw: init_error.
	void open_fullscreen_window(cstring_view title, glm::ivec2 min_size = not_resizable, const gfx::properties& gfx_properties = {});
	// Closes the window.
	void close_window();

	// Gets the title of the window.
	cstring_view window_title();
	// Sets the title of the window.
	void set_window_title(cstring_view title);

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

	// Sets the window's V-sync mode.
	void set_window_vsync(vsync vsync);
} // namespace tr::sys