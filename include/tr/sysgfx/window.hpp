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
#include "../utility/exception.hpp"
#include "../utility/zstring_view.hpp"
#ifdef _WIN32
#include "../utility/timer.hpp"
#endif

namespace tr {
	class bitmap;
	class bitmap_view;
	enum class mouse_mode : bool;
	class window;
} // namespace tr

struct SDL_Window;

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
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

	// Window constructor parameters.
	struct window_parameters {
		// Whether the window should be fullscreen.
		bool fullscreen{false};
		// Size of the window.
		glm::ivec2 size{maximized};
		// Minimum size of the window when resizing (or not_resizable).
		glm::ivec2 min_size{not_resizable};
		// Whether graphics contexts associated with the window should be debug contexts.
		bool debug_graphics_context{TR_ENABLE_ASSERTS};
		// Enables the use of depth and stencil buffers on graphics contexts associated with the window.
		bool enable_depth_stencil{false};
		// The number of samples used around a pixel for multisampled anti-aliasing on graphics contexts associated with the window.
		u8 multisamples{0};
	};

	// Window opening error.
	class window_open_error : public exception {
	  public:
		// Constructs a window opening error.
		window_open_error();

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// Description of the error.
		std::string m_description;
	};

	// Window error.
	class window_error : public exception {
	  public:
		// Constructs a window error.
		template <typename... Args> window_error(TR_FORMAT_STRING<Args...> description_fmt, Args&&... args);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// Description of the error.
		std::string m_description;
		// Details of the error.
		std::string_view m_details;

		// Constructs a window error.
		window_error(std::string&& description);
	};

	// Non-owning window view.
	class window_view {
	  public:
		// Creates a window view.
		window_view(SDL_Window* window);
		// Creates a window view.
		window_view(window& window);

		// Gets the title of the window.
		zstring_view title() const;
		// Sets the title of the window.
		// May throw: window_error.
		void set_title(zstring_view title) const;

		// Sets the icon of the window.
		// May throw: window_error.
		void set_icon(const bitmap& bitmap) const;
		// Sets the icon of the window.
		// May throw: window_error.
		void set_icon(const bitmap_view& view) const;

		// Gets the size of the window.
		// May throw: window_error.
		glm::ivec2 size() const;
		// Gets the window's pixel density factor.
		// May throw: window_error.
		float pixel_density() const;
		// Sets the size of the window.
		// May throw: window_error.
		void set_size(glm::ivec2 size) const;

		// Gets whether the window is fullscreen or not.
		bool fullscreen() const;
		// Sets whether the window is fullscreen or not.
		// May throw: window_error.
		void set_fullscreen(bool fullscreen) const;

		// Unhides the window.
		// May throw: window_error.
		void show() const;
		// Hides the window.
		// May throw: window_error.
		void hide() const;

		// Gets whether the window is maximized.
		bool maximized() const;
		// Gets whether the window is minimized.
		bool minimized() const;
		// Gets whether the window has input focus.
		bool has_focus() const;
		// Raises the window to have input focus.
		// May throw: window_error.
		void raise() const;

		// Sets the window's V-sync mode.
		// May throw: window_error.
		void set_vsync(vsync vsync) const;

		// Enables the sending of text input events in the window.
		void enable_text_input() const;
		// Disables the sending of text input events in the window.
		void disable_text_input() const;

		// Swaps the window's front- and backbuffer.
		void flip_backbuffer() const;

	  private:
		// Pointer to the SDL window.
		SDL_Window* m_ptr;
	};

	// Window object.
	class window {
	  public:
		// Opens a window.
		// May throw: window_open_error.
		window(zstring_view title, window_parameters parameters = {});

		// Gets the title of the window.
		zstring_view title() const;
		// Sets the title of the window.
		// May throw: window_error.
		void set_title(zstring_view title);

		// Sets the icon of the window.
		// May throw: window_error.
		void set_icon(const bitmap& bitmap);
		// Sets the icon of the window.
		// May throw: window_error.
		void set_icon(const bitmap_view& view);

		// Gets the size of the window.
		// May throw: window_error.
		glm::ivec2 size() const;
		// Gets the window's pixel density factor.
		// May throw: window_error.
		float pixel_density() const;
		// Sets the size of the window.
		// May throw: window_error.
		void set_size(glm::ivec2 size);

		// Gets whether the window is fullscreen or not.
		bool fullscreen() const;
		// Sets whether the window is fullscreen or not.
		// May throw: window_error.
		void set_fullscreen(bool fullscreen);

		// Unhides the window.
		// May throw: window_error.
		void show();
		// Hides the window.
		// May throw: window_error.
		void hide();

		// Gets whether the window is maximized.
		bool maximized() const;
		// Gets whether the window is minimized.
		bool minimized() const;
		// Gets whether the window has input focus.
		bool has_focus() const;
		// Raises the window to have input focus.
		// May throw: window_error.
		void raise();

		// Sets the window's V-sync mode.
		// May throw: window_error.
		void set_vsync(vsync vsync);

		// Enables the sending of text input events in the window.
		void enable_text_input();
		// Disables the sending of text input events in the window.
		void disable_text_input();

		// Sets the mouse mode in the window.
		// May throw: window_error.
		void set_mouse_mode(mouse_mode mode);

		// Swaps the window's front- and backbuffer.
		void flip_backbuffer();

	  private:
		// SDL window deleter.
		struct deleter {
			void operator()(SDL_Window* ptr) const;
		};

		// Pointer to the SDL window.
		std::unique_ptr<SDL_Window, deleter> m_ptr;
#ifdef _WIN32
		// Cursor graphic reset timer needed as a workaround for an SDL bug.
		std::optional<timer> m_cursor_reset_timer;
#endif

		friend class graphics_context;
	};
} // namespace tr

#include "impl/window.hpp" // IWYU pragma: export