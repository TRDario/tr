///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides window classes and related datatypes.                                                                                        //
//                                                                                                                                       //
// Windows are constructed using a parameter struct which defines the initial size and mode, as well as graphical parameters:            //
//     - tr::window window{"My App", {.size{500, 500}, .multisamples{tr::max_msaa()}}} -> Creates a 500x500 window with maximum MSAA     //
//     - tr::window window{"My App", {.min_size{640, 480}}} -> Creates a maximized window that can be resized down to 640x480            //
//     - tr::window window{"My App", {.fullscreen{true}}} -> Creates a borderless fullscreen window                                      //
//                                                                                                                                       //
// It's important to note that windows start out as hidden and must be set to be shown afterwards. Hiding and showing a window can be    //
// done at any time:                                                                                                                     //
//     - window.show() -> the window is shown                                                                                            //
//     - window.hide() -> the window is hidden again                                                                                     //
//                                                                                                                                       //
// A window's title can be gotten and reset:                                                                                             //
//     - window.title() -> "My App"                                                                                                      //
//     - window.set_title("My Game") -> window title is updated to "My Game"                                                             //
//                                                                                                                                       //
// A window's icon can be set:                                                                                                           //
//     - window.set_icon(bitmap) -> sets the window icon to the contents of the bitmap                                                   //
//                                                                                                                                       //
// A window's size and pixel density (ratio between logical and actual pixels), as well as whether the window is fullscreen are gettable,//
// and the size and fullscreen flag can be set:                                                                                          //
//     - window.fullscreen() -> true                                                                                                     //
//     - window.set_fullscreen(false)                                                                                                    //
//       window.set_size({960, 720})                                                                                                     //
//       window.size() -> {960, 720}                                                                                                     //
//     - window.pixel_density() -> may be greater than 1 if the user has a different UI scaling factor, for example                      //
//                                                                                                                                       //
// Whether a window is maximized, minimized, and if it has focus can be gotten, and the window can be raised to get window focus:        //
//     - window.maximized() -> true if the window is maximized, false otherwise                                                          //
//     - window.minimized() -> true if the window is minimized, false otherwise                                                          //
//     - window.has_focus() -> true if the window has input focus, false otherwise                                                       //
//     - window.raise() -> tries to raise the window                                                                                     //
//                                                                                                                                       //
// A window's V-sync mode can be set. If adaptive V-sync is not available, the system will try to fallback to regular V-sync:            //
//     - window.set_vsync(tr::vsync::adaptive) -> tries to enable adaptive V-sync                                                        //
//                                                                                                                                       //
// The sending of text inputs may be enabled and disabled per window:                                                                    //
//     - window.enable_text_input() -> tr::text_input_event can now be sent                                                              //
//     - window.disable_text_input() -> tr::text_input_event will no longer be sent                                                      //
//                                                                                                                                       //
// The mouse mode on a window may be changed between absolute and relative:                                                              //
//     - window.set_mouse_mode(tr::mouse_mode::absolute) -> absolute mouse controls: normal operation                                    //
//     - window.set_mouse_mode(tr::mouse_mode::relative) -> relative mouse controls: cursor is hidden and only reports deltas            //
//                                                                                                                                       //
// To display a frame after it is drawn, a window's backbuffer must be flipped:                                                          //
//     - window.flip_backbuffer() -> swaps the front- and backbuffer                                                                     //
//                                                                                                                                       //
// tr::window_view is a lightweight, non-owning view to a window.                                                                        //
// All of the same methods may be invoked on it as they would be on a window.                                                            //
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

		friend class graphics_context;
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
	};
} // namespace tr

#include "impl/window.hpp" // IWYU pragma: export