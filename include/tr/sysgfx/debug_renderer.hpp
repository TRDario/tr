#pragma once
#include <tr/utility.hpp>

namespace tr {
	// Debug text renderer.
	struct debug_renderer {
		// Initializes the debug renderer.
		static void initialize(float scale = 1.0f, std::uint8_t column_limit = 255) noexcept;
		// Gets whether the debug renderer is active.
		static bool active() noexcept;
		// Shuts the debug renderer down.
		static void shut_down() noexcept;

		// Sets the text's drawing scale.
		static void set_scale(float scale) noexcept;
		// Sets the text's column limit.
		static void set_column_limit(std::uint8_t columns) noexcept;

		// Writes a line of formatted text to the left side of the screen.
		static void write_left(std::string_view text, rgba8 text_color = {255, 255, 255, 255}, rgba8 bg_color = {0, 0, 0, 255},
							   std::span<rgba8> extra_colors = {}) noexcept;
		// Writes a line of formatted text to the right side of the screen.
		static void write_right(std::string_view text, rgba8 text_color = {255, 255, 255, 255}, rgba8 bg_color = {0, 0, 0, 255},
								std::span<rgba8> extra_colors = {}) noexcept;
		// Writes benchmark data to the right side of the screen.
		static void write_right(const benchmark& benchmark, std::string_view name, duration limit) noexcept;
		// Writes benchmark data to the right side of the screen.
		template <class R, class P>
		static void write_right(const benchmark& benchmark, std::string_view name, std::chrono::duration<R, P> limit) noexcept;

		// Writes a newline on the left side of the screen.
		static void newline_left() noexcept;
		// Writes a newline on the right side of the screen.
		static void newline_right() noexcept;

		// Draws all written text to the screen and clears it.
		static void draw() noexcept;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class R, class P>
void tr::debug_renderer::write_right(const benchmark& benchmark, std::string_view name, std::chrono::duration<R, P> limit) noexcept
{
	write_right(benchmark, name, duration_cast<duration>(limit));
}