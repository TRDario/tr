/// @file
/// @brief Provides a mouse cursor class and related functionality.

#pragma once
#include <tr/utility/common.hpp>

struct SDL_Cursor;
namespace tr
{
	class bitmap;
	class bitmap_view;
} // namespace tr

//

namespace tr
{
	/// System mouse cursor icons.
	enum class sys_cursor
	{
		/// Default arrow cursor.
		arrow,

		/// I-shaped cursor.
		ibeam,

		/// Waiting cursor.
		wait,

		/// Crosshair cursor.
		crosshair,

		/// Waiting arrow cursor.
		wait_arrow,

		/// Resizing (northwest-southeast) cursor.
		size_nwse,

		/// Resizing (northeast-southwest) cursor.
		size_nesw,

		/// Resizing (west-east) cursor.
		size_we,

		/// Resizing (north-south) cursor.
		size_ns,

		/// Resizing (all directions) cursor.
		size_all,

		/// Forbidden action cursor.
		no,

		/// Pointing hand cursor.
		hand
	};

	/// Mouse cursor image.
	/// @details
	/// Moved-from instances of `tr::cursor` are left in a special 'invalid' state. Invalid `tr::cursor` instances may not be interacted
	/// with besides moving a new value into them and checking for validity using `valid()`.
	class cursor
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates a default mouse cursor.
		[[nodiscard]] cursor();

		/// @cond sdl_interop

		/// Wraps an SDL_Cursor pointer.
		/// @param ptr Pointer to wrap.
		[[nodiscard]] explicit cursor(SDL_Cursor* ptr);

		/// @endcond

		/// Creates a system cursor.
		/// @param icon Icon to use.
		[[nodiscard]] cursor(sys_cursor icon);

		/// Creates a cursor from a bitmap.
		/// @param bitmap Cursor bitmap. The bitmap does not have to stay alive after this.
		/// @param focus Focus point on the bitmap.
		[[nodiscard]] cursor(const bitmap& bitmap, glm::ivec2 focus);

		/// Creates a cursor from a bitmap view.
		/// @param view Cursor bitmap view. The view does not have to stay alive after this.
		/// @param focus Focus point on the bitmap.
		[[nodiscard]] cursor(const bitmap_view& view, glm::ivec2 focus);

		/// Cursors are not copyable.
		cursor(const cursor&) = delete;

		/// Moves a cursor.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Cursor to move.
		[[nodiscard]] cursor(cursor&& rhs) noexcept;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Cursors are not copyable.
		cursor& operator=(const cursor&) = delete;

		/// Moves a cursor.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Cursor to move.
		/// @return Reference to `*this`.
		cursor& operator=(cursor&& rhs) noexcept = default;

		/// @}
		/// @name Validity
		/// @{

		/// Gets whether the cursor is in a valid state.
		/// @return `true` if the cursor is in a valid state, `false` otherwise.
		[[nodiscard]] bool valid() const noexcept;

		/// @}
		/// @cond sdl_interop
		/// @name SDL interoperability
		/// @{

		/// Unwraps the SDL cursor pointer.
		/// @note This does not release the pointer.
		/// @return Pointer to the SDL cursor.
		[[nodiscard]] SDL_Cursor* unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Cursor deleter.
		struct deleter
		{
			/// Deletes a cursor.
			/// @param ptr Pointer to the cursor.
			static void operator()(SDL_Cursor* ptr) noexcept;
		};

		//

		/// Handle to the SDL cursor.
		std::unique_ptr<SDL_Cursor, deleter> m_ptr;
	};

	/// @name Cursor
	/// @{

	/// Shows the cursor.
	/// @exception cursor_error If showing the cursor failed.
	void show_cursor();

	/// Hides the cursor.
	/// @exception cursor_error If hiding the cursor failed.
	void hide_cursor();

	/// Sets the mouse cursor.
	/// @param cursor Cursor to set. The cursor does not have to stay alive after this.
	/// @exception cursor_error If setting the cursor failed.
	void set_cursor(const cursor& cursor);

	/// @}
} // namespace tr