#pragma once
#include "../utility/common.hpp"

struct SDL_Cursor;

namespace tr {
	class bitmap;
	class bitmap_view;
	class cursor;
	namespace mouse {
		void set_cursor(const cursor& cursor);
	}

	// System mouse cursor icons.
	enum class sys_cursor {
		ARROW,      // Default arrow cursor.
		IBEAM,      // I-shaped cursor.
		WAIT,       // Waiting cursor.
		CROSSHAIR,  // Crosshair cursor.
		WAIT_ARROW, // Waiting arrow cursor.
		SIZE_NWSE,  // Resizing (northwest-southeast) cursor.
		SIZE_NESW,  // Resizing (northeast-southwest) cursor.
		SIZE_WE,    // Resizing (west-east) cursor.
		SIZE_NS,    // Resizing (north-south) cursor.
		SIZE_ALL,   // Resizing (all directions) cursor.
		NO,         // Forbidden action cursor.
		HAND        // Pointing hand cursor.
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
		std::unique_ptr<SDL_Cursor, deleter> ptr;

		friend void mouse::set_cursor(const cursor& cursor);
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range R1, std::ranges::contiguous_range R2>
tr::cursor::cursor(R1&& color, R2&& mask, glm::ivec2 size, glm::ivec2 focus)
	: cursor{range_bytes(color), range_bytes(mask), size, focus}
{
}