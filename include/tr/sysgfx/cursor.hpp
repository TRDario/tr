///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a mouse cursor class and related functionality.                                                                              //
//                                                                                                                                       //
// A cursor can be created with a stock system graphic, or a bitmap:                                                                     //
//     - tr::sys::cursor{} -> default system cursor                                                                                      //
//     - tr::sys::cursor{tr::sys::sys_cursor::hand} -> pointing hand system cursor                                                       //
//     - tr::sys::cursor{bmp, {5, 5}} -> cursor with the graphics of bmp, the actual position of the cursor is at pixel (5, 5) of bmp    //
//                                                                                                                                       //
// The cursor can be shown or hidden, and its graphic can be changed:                                                                    //
//     - tr::sys::show_cursor() -> shows the cursor                                                                                      //
//     - tr::sys::hide_cursor() -> hides the cursor                                                                                      //
//     - tr::sys::cursor hand{tr::sys::sys_cursor::hand}; tr::sys::set_cursor(hand) -> sets the cursor to the pointing hand              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/common.hpp"

struct SDL_Cursor;
namespace tr {
	class bitmap;
	class bitmap_view;
} // namespace tr

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::sys {
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
	void show_cursor();
	// Hides the cursor.
	void hide_cursor();
	// Sets the mouse cursor.
	void set_cursor(const cursor& cursor);
} // namespace tr::sys