///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a mouse cursor class and related functionality.                                                                              //
//                                                                                                                                       //
// A cursor can be created with a stock system graphic, or a bitmap:                                                                     //
//     - tr::cursor{} -> default system cursor                                                                                           //
//     - tr::cursor{tr::sys_cursor::hand} -> pointing hand system cursor                                                                 //
//     - tr::cursor{bmp, {5, 5}} -> cursor with the graphics of bmp, the actual position of the cursor is at pixel (5, 5) of bmp         //
//                                                                                                                                       //
// The cursor can be shown or hidden, and its graphic can be changed:                                                                    //
//     - tr::show_cursor() -> shows the cursor                                                                                           //
//     - tr::hide_cursor() -> hides the cursor                                                                                           //
//     - tr::cursor hand{tr::sys_cursor::hand}; tr::set_cursor(hand) -> sets the cursor to the pointing hand                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/exception.hpp"

struct SDL_Cursor;
namespace tr {
	class bitmap;
	class bitmap_view;
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// System mouse cursor icons.
	enum class sys_cursor {
		arrow,      // Default arrow cursor.
		ibeam,      // I-shaped cursor.
		wait,       // Waiting cursor.
		crosshair,  // Crosshair cursor.
		wait_arrow, // Waiting arrow cursor.
		size_nwse,  // Resizing (northwest-southeast) cursor.
		size_nesw,  // Resizing (northeast-southwest) cursor.
		size_we,    // Resizing (west-east) cursor.
		size_ns,    // Resizing (north-south) cursor.
		size_all,   // Resizing (all directions) cursor.
		no,         // Forbidden action cursor.
		hand        // Pointing hand cursor.
	};

	// Cursor error.
	class cursor_error : public exception {
	  public:
		// Constructs a cursor error.
		cursor_error(std::string_view description);

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
	};

	// Mouse cursor graphic.
	class cursor {
	  public:
		// Creates the default mouse cursor.
		cursor();
		// Creates a system cursor.
		cursor(sys_cursor icon);
		// Creates a cursor from a bitmap.
		cursor(const bitmap& bitmap, glm::ivec2 focus);
		// Creates a cursor from a bitmap view.
		cursor(const bitmap_view& view, glm::ivec2 focus);

	  private:
		struct deleter {
			void operator()(SDL_Cursor* ptr) const;
		};

		// Handle to the SDL cursor.
		std::unique_ptr<SDL_Cursor, deleter> m_ptr;

		// Wraps an SDL_Cursor.
		cursor(SDL_Cursor* ptr);

		friend void set_cursor(const cursor& cursor);
	};

	// Shows the cursor.
	// May throw: cursor_error.
	void show_cursor();
	// Hides the cursor.
	// May throw: cursor_error.
	void hide_cursor();
	// Sets the mouse cursor.
	// May throw: cursor_error.
	void set_cursor(const cursor& cursor);
} // namespace tr