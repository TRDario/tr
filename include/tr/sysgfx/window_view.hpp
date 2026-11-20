/// @file
/// @brief Provides a non-owning window view class.

#pragma once
#include <tr/utility/exception.hpp>
#include <tr/utility/zstring_view.hpp>

namespace tr
{
	class bitmap;
	class bitmap_view;
} // namespace tr

struct SDL_Window;

//

namespace tr
{
	/// @brief Vertical synchronisation configuration.
	enum class vsync : i8
	{
		/// @brief Vertical synchronisation is enabled, but late swaps happen immediately instead of waiting for the next retrace.
		/// @hideinitializer
		adaptive = -1,

		/// @brief Vertical synchronisation is disabled.
		disabled,

		/// @brief Vertical synchronisation is enabled.
		enabled
	};

	/// @brief Window display mode.
	enum class display_mode : bool
	{
		/// @brief Windowed display mode.
		windowed,

		/// @brief Borderless fullscreen display mode.
		fullscreen
	};

	//

	/// @brief Non-owning view to a window object.
	/// @details
	/// `tr::window_view` is a lightweight, non-owning view to a window object. It is designed to be cheap to pass around and copy.
	///
	/// The window view should not outlive the window it is pointing at, as it will otherwise become dangling.
	///
	/// @warning It is important to stress the distinction between windows and instances of `tr::window` for the purposes of this class, see
	/// the documentation of `tr::window` for more information.
	class window_view
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @cond sdl_interop
		/// @brief Constructs a window view.
		/// @param ptr Underlying SDL window pointer.
		/// @pre `ptr` cannot be `nullptr`.
		[[nodiscard]] explicit window_view(SDL_Window* ptr) noexcept;
		/// @endcond

		/// @brief Constructs a window view by copying from another window view.
		/// @details Window views are trivially copyable.
		/// @param rhs Window view to copy from.
		[[nodiscard]] window_view(const window_view& rhs) noexcept = default;

		/// @brief Replaces the window view with a view to another window.
		/// @details Window views are trivially copyable.
		/// @param rhs Window view to copy from.
		/// @return Reference to `*this`.
		[[nodiscard]] window_view& operator=(const window_view& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the title of the window.
		/// @param title New window title string.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If setting the window title failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set_title(zstring_view title) const;

		/// @brief Sets the icon of the window.
		/// @param bitmap Icon bitmap.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If setting the window icon failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set_icon(const bitmap& bitmap) const;

		/// @brief Sets the icon of the window.
		/// @param bitmap Icon bitmap view.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If setting the window icon failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set_icon(const bitmap_view& bitmap) const;

		/// @brief Sets the size of the window.
		/// @details
		/// If the window is fullscreen, this function has no effect.
		///
		/// This request may be asynchronous and the new window size may not have necessarily been applied by the return of this function.
		///
		/// When the window size changes, a `tr::backbuffer_resize_event` event will be emitted with the new dimensions of the window's
		/// backbuffer. Note that the new dimensions may not match the exact size requested, as the windowing system could restrict the
		/// window size or outright deny the request.
		///
		/// After the resize, an attempt will be made to center the window on the screen, but this is not guaranteed to work, and fails
		/// silently.
		/// @param size New size of the window in pixels.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If setting the size of the window failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set_size(glm::ivec2 size) const;

		/// @brief Sets the window's display mode.
		/// @details
		/// This request may be asynchronous and the new display mode may not have necessarily been applied by the return of this function.
		///
		/// Note that, as this is just a request, this can be denied by the windowing system.
		/// @param display_mode New display mode of the window.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If setting the display mode of the window failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set_display_mode(tr::display_mode display_mode) const;

		/// @brief Unhides the window.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If showing the window failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void show() const;

		/// @brief Hides the window.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If hiding the window failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void hide() const;

		/// @brief Raises the window to have input focus.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If raising the window failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void raise() const;

		/// @brief Sets the window's vertical synchronisation configuration.
		/// @details `vsync::adaptive` may fall back to `vsync::enabled`.
		/// @param vsync Vertical synchronisation configuration to set.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If setting the vertical synchronisation configuration failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set_vsync(vsync vsync) const;

		/// @brief Enables the sending of text input events in the window.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		void enable_text_input() const noexcept;

		/// @brief Disables the sending of text input events in the window.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		void disable_text_input() const noexcept;

		/// @brief Swaps the window's front- and backbuffer.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		void flip_backbuffer() const noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the title of the window.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @post The title string view is guaranteed to stay valid until the title is changed or the window is destroyed.
		/// @return Title of the window.
		[[nodiscard]] zstring_view title() const noexcept;

		/// @brief Returns the size of the window in pixels.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If getting the size of the window failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Size of the window in pixels.
		[[nodiscard]] glm::ivec2 size() const;

		/// @brief Returns the window's pixel density factor.
		/// @details This is a ratio of pixel size to window size. For example, if the window is 1920x1080 and it has a high density back
		/// buffer of 3840x2160 pixels, it would have a pixel density of 2.0.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @exception window_error If getting the pixel density of the window failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Pixel density factor of the window.
		[[nodiscard]] float pixel_density() const;

		/// @brief Returns the display mode of the window.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @return Display mode of the window.
		[[nodiscard]] tr::display_mode display_mode() const noexcept;

		/// @brief Checks whether the window is maximized.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @return `true` if the window is maximized, `false` otherwise.
		[[nodiscard]] bool maximized() const noexcept;

		/// @brief Checks whether the window is minimized.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @return `true` if the window is minimized, `false` otherwise.
		[[nodiscard]] bool minimized() const noexcept;

		/// @brief Checks whether the window has input focus.
		/// @par Thread safety
		/// This function should only be called on the main thread.
		/// @return `true` if the window has input focus, `false` otherwise.
		[[nodiscard]] bool has_focus() const noexcept;

		/// @cond sdl_interop
		/// @brief Returns a pointer to the underlying SDL window.
		/// @return Pointer to the underlying SDL window.
		[[nodiscard]] SDL_Window* pointer() const noexcept;
		/// @endcond

		/// @}

	  private:
		/// @brief Underlying SDL window pointer.
		SDL_Window* m_ptr;
	};
} // namespace tr

//

/// @brief Vertical synchronisation configuration formatter.
template <>
struct std::formatter<tr::vsync>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid vertical synchronisation configuration specification."};
		}
		return context.begin();
	}

	/// @brief Formats a vertical synchronisation configuration.
	/// @tparam FormatContext Formatting context type.
	/// @param vsync Vertical synchronisation configuration to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(tr::vsync vsync, FormatContext& context) const
	{
		switch (vsync) {
		case tr::vsync::adaptive:
			return std::format_to(context.out(), "adaptive");
		case tr::vsync::disabled:
			return std::format_to(context.out(), "disabled");
		case tr::vsync::enabled:
			return std::format_to(context.out(), "enabled");
		default:
			return std::format_to(context.out(), "<invalid vertical synchronization configuration>");
		}
	}
};

/// @brief Display mode formatter.
template <>
struct std::formatter<tr::display_mode>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid display mode specification."};
		}
		return context.begin();
	}

	/// @brief Formats a display mode.
	/// @tparam FormatContext Formatting context type.
	/// @param display_mode Display mode to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(tr::display_mode display_mode, FormatContext& context) const
	{
		switch (display_mode) {
		case tr::display_mode::windowed:
			return std::format_to(context.out(), "windowed");
		case tr::display_mode::fullscreen:
			return std::format_to(context.out(), "fullscreen");
		}
	}
};