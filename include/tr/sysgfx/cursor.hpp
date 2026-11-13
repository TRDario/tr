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
	/// @brief System mouse cursor icons.
	enum class sys_cursor
	{
		/// @brief Default arrow cursor.
		arrow,

		/// @brief I-shaped cursor.
		ibeam,

		/// @brief Waiting cursor.
		wait,

		/// @brief Crosshair cursor.
		crosshair,

		/// @brief Waiting arrow cursor.
		wait_arrow,

		/// @brief Resizing (northwest-southeast) cursor.
		size_nwse,

		/// @brief Resizing (northeast-southwest) cursor.
		size_nesw,

		/// @brief Resizing (west-east) cursor.
		size_we,

		/// @brief Resizing (north-south) cursor.
		size_ns,

		/// @brief Resizing (all directions) cursor.
		size_all,

		/// @brief Forbidden action cursor.
		no,

		/// @brief Pointing hand cursor.
		hand
	};

	/// @brief Mouse cursor image.
	/// @details
	/// Moved-from instances of `tr::cursor` are left in a special 'invalid' state. Invalid `tr::cursor` instances may not be interacted
	/// with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	class cursor
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a default cursor.
		[[nodiscard]] cursor();

		/// @cond sdl_interop

		/// @brief Constructs a cursor by taking ownership over an SDL_Cursor pointer.
		/// @param ptr Pointer to take ownership over.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] explicit cursor(SDL_Cursor* ptr);

		/// @endcond

		/// @brief Constructs a system cursor.
		/// @param icon Icon to use.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] cursor(sys_cursor icon);

		/// @brief Constructs a cursor from a bitmap.
		/// @param bitmap Cursor bitmap. The bitmap does not have to stay alive after this.
		/// @param focus Focus point on the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] cursor(const bitmap& bitmap, glm::ivec2 focus);

		/// @brief Constructs a cursor from a bitmap view.
		/// @param view Cursor bitmap view. The view does not have to stay alive after this.
		/// @param focus Focus point on the bitmap.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] cursor(const bitmap_view& view, glm::ivec2 focus);

		/// @brief Cursors are not copyable.
		cursor(const cursor&) = delete;

		/// @brief Cursors are not copyable.
		cursor& operator=(const cursor&) = delete;

		/// @brief Constructs a cursor by transferring ownership from another cursor.
		/// @param rhs Cursor to move from.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] cursor(cursor&& rhs) noexcept;

		/// @brief Destroys the current cursor and replaces it with one transferred from another cursor.
		/// @param rhs Cursor to move from.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		cursor& operator=(cursor&& rhs) noexcept = default;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the cursor is in a valid state.
		/// @return `true` if the cursor is in a valid state, `false` otherwise.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the cursor is in a valid state.
		/// @return `true` if the cursor is in a valid state, `false` otherwise.
		[[nodiscard]] bool valid() const noexcept;

		/// @cond sdl_interop
		/// @brief Returns a pointer to the underlying SDL cursor.
		/// @return Pointer to the underyling SDL cursor pointer.
		[[nodiscard]] SDL_Cursor* pointer() const noexcept;
		/// @endcond

		/// @}

	  private:
		/// @brief SDL cursor deleter.
		struct sdl_cursor_deleter
		{
			/// @brief Deletes an SDL cursor.
			/// @param ptr Pointer to the cursor.
			static void operator()(SDL_Cursor* ptr) noexcept;
		};

		//

		/// @brief Managed pointer to an SDL cursor.
		std::unique_ptr<SDL_Cursor, sdl_cursor_deleter> m_ptr;
	};

	//

	/// @brief Shows the cursor.
	/// @exception cursor_error If showing the cursor failed.
	void show_cursor();

	/// @brief Hides the cursor.
	/// @exception cursor_error If hiding the cursor failed.
	void hide_cursor();

	/// @brief Sets the mouse cursor.
	/// @param cursor Cursor to set. The cursor does not have to stay alive after this.
	/// @exception cursor_error If setting the cursor failed.
	void set_cursor(const cursor& cursor);
} // namespace tr