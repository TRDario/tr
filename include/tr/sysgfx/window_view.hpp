/// @file
/// @brief Provides a non-owning window view class.

#pragma once
#include "../utility/exception.hpp"
#include "../utility/zstring_view.hpp"

namespace tr
{
	class bitmap;
	class bitmap_view;
} // namespace tr

struct SDL_Window;

//

namespace tr
{
	/// V-sync modes.
	enum class vsync : i8
	{
		/// Vsync is enabled, but late swaps happen immediately instead of waiting for the next retrace.
		adaptive = -1,

		/// Vsync is disabled.
		disabled,

		/// Vsync is enabled.
		enabled
	};

	//

	/// Window error.
	class window_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a window error.
		/// @tparam Args Types of the formatting arguments.
		/// @param description_fmt Description format string.
		/// @param args Description formatting arguments.
		template <typename... Args>
		[[nodiscard]] explicit window_error(std::format_string<Args...> description_fmt, Args&&... args) noexcept;

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Window error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// Gets the description of the error.
		/// @return Description of the error.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// Gets further details about the error.
		/// @return Details of the error.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string_view m_details;

		/// Constructs a window error.
		/// @param description Description of the error.
		[[nodiscard]] window_error(std::string&& description) noexcept;
	};

	//

	/// Non-owning window view.
	class window_view
	{
	  public:
		/// @cond sdl_interop
		/// @name Constructors
		/// @{

		/// Wraps a window view.
		/// @param window Window pointer to wrap.
		[[nodiscard]] explicit window_view(SDL_Window* window) noexcept;

		/// @}
		/// @endcond

		/// @name Title
		/// @{

		/// Gets the title of the window.
		/// @return Title of the window.
		[[nodiscard]] zstring_view title() const noexcept;

		/// Sets the title of the window.
		/// @param title New window title string.
		/// @exception window_error If setting the window title failed.
		void set_title(zstring_view title) const;

		/// @}
		/// @name Icon
		/// @{

		/// Sets the icon of the window.
		/// @param bitmap Icon bitmap.
		/// @exception window_error If setting the window icon failed.
		void set_icon(const bitmap& bitmap) const;

		/// Sets the icon of the window.
		/// @param bitmap Icon bitmap view.
		/// @exception window_error If setting the window icon failed.
		void set_icon(const bitmap_view& bitmap) const;

		/// @}
		/// @name Size
		/// @{

		/// Gets the size of the window.
		/// @exception window_error If getting the size of the window failed.
		/// @return Size of the window in pixels.
		[[nodiscard]] glm::ivec2 size() const;

		/// Gets the window's pixel density factor.
		/// @exception window_error If getting the pixel density of the window failed.
		/// @return Pixel density factor of the window.
		[[nodiscard]] float pixel_density() const;

		/// Sets the size of the window.
		/// @param size New size of the window in pixels.
		/// @exception window_error If setting the size of the window failed.
		void set_size(glm::ivec2 size) const;

		/// @}
		/// @name Fullscreen
		/// @{

		/// Gets whether the window is fullscreen or not.
		/// @return Whether the window is fullscreen or not.
		[[nodiscard]] bool fullscreen() const noexcept;

		/// Sets whether the window is fullscreen or not.
		/// @param fullscreen Whether to enable fullscreen or not.
		/// @exception window_error If setting the fullscreen mode of the window failed.
		void set_fullscreen(bool fullscreen) const;

		/// @}
		/// @name Visibility
		/// @{

		/// Unhides the window.
		/// @exception window_error If showing the window failed.
		void show() const;

		/// Hides the window.
		/// @exception window_error If hiding the window failed.
		void hide() const;

		/// @}
		/// @name Status
		/// @{

		/// Gets whether the window is maximized.
		/// @return `true` if the window is maximized, `false` otherwise.
		[[nodiscard]] bool maximized() const noexcept;

		/// Gets whether the window is minimized.
		/// @return `true` if the window is minimized, `false` otherwise.
		[[nodiscard]] bool minimized() const noexcept;

		/// Gets whether the window has input focus.
		/// @return `true` if the window has input focus, `false` otherwise.
		[[nodiscard]] bool has_focus() const noexcept;

		/// Raises the window to have input focus.
		/// @exception window_error If raising the window failed.
		void raise() const;

		/// @}
		/// @name V-sync
		/// @{

		/// Sets the window's V-sync mode.
		/// @param vsync V-sync mode to set. `vsync::adaptive` may fall back to `vsync::enabled`.
		/// @exception window_error If setting the V-sync mode failed.
		void set_vsync(vsync vsync) const;

		/// @}
		/// @name Text input
		/// @{

		/// Enables the sending of text input events in the window.
		void enable_text_input() const noexcept;

		/// Disables the sending of text input events in the window.
		void disable_text_input() const noexcept;

		/// @}
		/// @name Backbuffer
		/// @{

		/// Swaps the window's front- and backbuffer.
		void flip_backbuffer() const noexcept;

		/// @}

		/// @cond sdl_interop
		/// @name SDL interoperability
		/// @{

		/// Unwraps the SDL window pointer.
		/// @return Pointer to the SDL window.
		[[nodiscard]] SDL_Window* unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Pointer to an SDL window.
		SDL_Window* m_ptr;
	};
} // namespace tr

#include "impl/window_view.hpp" // IWYU pragma: export