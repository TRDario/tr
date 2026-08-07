/// @file
/// @brief Provides a mouse cursor class and related functionality.

#pragma once
#include "../utility/exception.hpp"

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

	/// Cursor error.
	class cursor_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a cursor error.
		/// @param description Description of the error.
		cursor_error(std::string_view description);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"Cursor error"`.
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

		/// Details of the error.
		std::string_view m_details;
	};

	/// Mouse cursor graphic.
	class cursor
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates a default mouse cursor.
		cursor();

		/// Creates a system cursor.
		/// @param icon Icon to use.
		cursor(sys_cursor icon);

		/// Creates a cursor from a bitmap.
		/// @param bitmap Cursor bitmap. The bitmap does not have to stay alive after this.
		/// @param focus Focus point on the bitmap.
		cursor(const bitmap& bitmap, glm::ivec2 focus);

		/// Creates a cursor from a bitmap view.
		/// @param view Cursor bitmap view. The view does not have to stay alive after this.
		/// @param focus Focus point on the bitmap.
		cursor(const bitmap_view& view, glm::ivec2 focus);

		/// @}

	  private:
		/// Cursor deleter.
		struct deleter
		{
			/// Deletes a cursor.
			/// @param ptr Pointer to the cursor.
			void operator()(SDL_Cursor* ptr) const;
		};

		//

		/// Handle to the SDL cursor.
		std::unique_ptr<SDL_Cursor, deleter> m_ptr;

		//

		/// Wraps an SDL_Cursor pointer.
		/// @param ptr Pointer to wrap.
		cursor(SDL_Cursor* ptr);

		//

		// Uses the private cursor.
		friend void set_cursor(const cursor& cursor);
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
	/// @param cursor Cursor to set.
	/// @exception cursor_error If setting the cursor failed.
	void set_cursor(const cursor& cursor);

	/// @}
} // namespace tr