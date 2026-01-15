#pragma once
#include "../utility/common.hpp"

struct SDL_Cursor;

namespace tr {
	class bitmap;
	class bitmap_view;
	class cursor;
} // namespace tr

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
		// Creates a simple black-and-white cursor from color and mask bitfields.
		cursor(std::span<const std::byte> color, std::span<const std::byte> mask, glm::ivec2 size, glm::ivec2 focus);
		// Creates a simple black-and-white cursor from color and mask bitfields.
		template <std::ranges::contiguous_range R1, std::ranges::contiguous_range R2>
		cursor(R1&& color, R2&& mask, glm::ivec2 size, glm::ivec2 focus);

	  private:
		struct deleter {
			void operator()(SDL_Cursor* ptr) const;
		};

		// Handle to the SDL cursor.
		std::unique_ptr<SDL_Cursor, deleter> m_ptr;

		friend void set_cursor(const cursor& cursor);
	};
} // namespace tr::sys

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range R1, std::ranges::contiguous_range R2>
tr::sys::cursor::cursor(R1&& color, R2&& mask, glm::ivec2 size, glm::ivec2 focus)
	: cursor{range_bytes(color), range_bytes(mask), size, focus}
{
}