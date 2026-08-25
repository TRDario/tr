/// @file
/// @brief Provides window classes and related datatypes.

#pragma once
#include "../utility/exception.hpp"
#include "../utility/zstring_view.hpp"
#ifdef _WIN32
#include "../utility/timer.hpp"
#endif

namespace tr
{
	class bitmap;
	class bitmap_view;
	enum class mouse_mode : bool;
	enum class vsync : i8;
	class window_view;
} // namespace tr

struct SDL_Window;

//

/// @cond implementation_details

/// Default value of tr::window_parameters::debug_graphics_context.
/// @hideinitializer
#ifdef TR_ENABLE_ASSERTS
#define TR_DEBUG_GRAPHICS_CONTEXT_DEFAULT true
#else
#define TR_DEBUG_GRAPHICS_CONTEXT_DEFAULT false
#endif

/// @endcond

namespace tr
{
	/// Marks a window as maximized.
	constexpr glm::ivec2 maximized{};

	/// Marks a window as not resizable.
	constexpr glm::ivec2 not_resizable{};

	//

	/// Window constructor parameters.
	struct window_parameters
	{
		/// Whether the window should be fullscreen.
		bool fullscreen{false};

		/// Size of the window.
		glm::ivec2 size{maximized};

		/// Minimum size of the window when resizing (or not_resizable).
		glm::ivec2 min_size{not_resizable};

		/// Whether graphics contexts associated with the window should be debug contexts.
		bool debug_graphics_context{TR_DEBUG_GRAPHICS_CONTEXT_DEFAULT};

		/// Enables the use of depth and stencil buffers on graphics contexts associated with the window.
		bool enable_depth_stencil{false};

		/// The number of samples used around a pixel for multisampled anti-aliasing on graphics contexts associated with the window.
		u8 multisamples{0};
	};

	/// Window opening error.
	class window_open_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a window opening error.
		explicit window_open_error();

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Window opening error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;
	};

	//

	/// Window object.
	class window
	{
	  public:
		/// @name Constructors
		/// @{

		/// Opens a window.
		/// @param title Initial window title.
		/// @param parameters Initial window parameters.
		/// @exception window_open_error If opening the window failed.
		explicit window(zstring_view title, window_parameters parameters = {});

		/// @}
		/// @name View
		/// @{

		/// Creates a view to the window.
		/// @return View to the window.
		operator window_view();

		/// Creates a view to the window.
		/// @return View to the window.
		window_view view();

		/// @}
		/// @name Title
		/// @{

		/// Gets the title of the window.
		/// @return Title of the window.
		zstring_view title() const;

		/// Sets the title of the window.
		/// @param title New window title string.
		/// @exception window_error If setting the window title failed.
		void set_title(zstring_view title);

		/// @}
		/// @name Icon
		/// @{

		/// Sets the icon of the window.
		/// @param bitmap Icon bitmap.
		/// @exception window_error If setting the window icon failed.
		void set_icon(const bitmap& bitmap);

		/// Sets the icon of the window.
		/// @param bitmap Icon bitmap view.
		/// @exception window_error If setting the window icon failed.
		void set_icon(const bitmap_view& bitmap);

		/// @}
		/// @name Size
		/// @{

		/// Gets the size of the window.
		/// @exception window_error If getting the size of the window failed.
		/// @return Size of the window in pixels.
		glm::ivec2 size() const;

		/// Gets the window's pixel density factor.
		/// @exception window_error If getting the pixel density of the window failed.
		/// @return Pixel density factor of the window.
		float pixel_density() const;

		/// Sets the size of the window.
		/// @param size New size of the window in pixels.
		/// @exception window_error If setting the size of the window failed.
		void set_size(glm::ivec2 size);

		/// @}
		/// @name Fullscreen
		/// @{

		/// Gets whether the window is fullscreen or not.
		/// @return Whether the window is fullscreen or not.
		bool fullscreen() const;

		/// Sets whether the window is fullscreen or not.
		/// @param fullscreen Whether to enable fullscreen or not.
		/// @exception window_error If setting the fullscreen mode of the window failed.
		void set_fullscreen(bool fullscreen);

		/// @}
		/// @name Visibility
		/// @{

		/// Unhides the window.
		/// @exception window_error If showing the window failed.
		void show();

		/// Hides the window.
		/// @exception window_error If hiding the window failed.
		void hide();

		/// @}
		/// @name Status
		/// @{

		/// Gets whether the window is maximized.
		/// @return `true` if the window is maximized, `false` otherwise.
		bool maximized() const;

		/// Gets whether the window is minimized.
		/// @return `true` if the window is minimized, `false` otherwise.
		bool minimized() const;

		/// Gets whether the window has input focus.
		/// @return `true` if the window has input focus, `false` otherwise.
		bool has_focus() const;

		/// Raises the window to have input focus.
		/// @exception window_error If raising the window failed.
		void raise();

		/// @}
		/// @name V-sync
		/// @{

		/// Sets the window's V-sync mode.
		/// @param vsync V-sync mode to set. `vsync::adaptive` may fall back to `vsync::enabled`.
		// @exception window_error If setting the V-sync mode failed.
		void set_vsync(vsync vsync);

		/// @}
		/// @name Text input
		/// @{

		/// Enables the sending of text input events in the window.
		void enable_text_input();

		/// Disables the sending of text input events in the window.
		void disable_text_input();

		/// @}
		/// @name Mouse mode
		/// @{

		/// Sets the mouse mode in the window.
		/// @param mode Mouse mode to set.
		/// @exception window_error If setting the mouse mode failed.
		void set_mouse_mode(mouse_mode mode);

		/// @}
		/// @name Backbuffer
		/// @{

		/// Swaps the window's front- and backbuffer.
		void flip_backbuffer();

		/// @}

	  private:
		/// SDL window deleter.
		struct deleter
		{
			/// Destroys a window.
			/// @param ptr Pointer to an SDL window.
			static void operator()(SDL_Window* ptr);
		};

		//

		/// Pointer to the SDL window.
		std::unique_ptr<SDL_Window, deleter> m_ptr;

#ifdef _WIN32
		/// Cursor graphic reset timer needed as a workaround for an SDL bug.
		std::optional<timer> m_cursor_reset_timer;
#endif

		//

		/// Creates a view to the window (const-qualified).
		/// @return View to the window.
		window_view view() const;
	};
} // namespace tr