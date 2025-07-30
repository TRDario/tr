#pragma once
#include "../utility/chrono.hpp"
#include "../utility/color.hpp"

namespace tr {
	class benchmark;

	// Debug text renderer functionality.
	namespace debug_renderer {
		// Initializes the debug renderer.
		void initialize(float scale = 1.0f, std::uint8_t column_limit = 255);
		// Gets whether the debug renderer is active.
		bool active();
		// Shuts the debug renderer down.
		void shut_down();

		// Sets the text's drawing scale.
		void set_scale(float scale);
		// Sets the text's column limit.
		void set_column_limit(std::uint8_t columns);

		// Writes a line of formatted text to the left side of the screen.
		void write_left(std::string_view text, rgba8 text_color = {255, 255, 255, 255}, rgba8 bg_color = {0, 0, 0, 255},
						std::span<rgba8> extra_colors = {});
		// Writes a line of formatted text to the right side of the screen.
		void write_right(std::string_view text, rgba8 text_color = {255, 255, 255, 255}, rgba8 bg_color = {0, 0, 0, 255},
						 std::span<rgba8> extra_colors = {});
		// Writes benchmark data to the right side of the screen.
		void write_right(const benchmark& benchmark, std::string_view name, duration limit);
		// Writes benchmark data to the right side of the screen.
		template <class R, class P> void write_right(const benchmark& benchmark, std::string_view name, std::chrono::duration<R, P> limit);

		// Writes a newline on the left side of the screen.
		void newline_left();
		// Writes a newline on the right side of the screen.
		void newline_right();

		// Draws all written text to the screen and clears it.
		void draw();
	}; // namespace debug_renderer
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class R, class P>
void tr::debug_renderer::write_right(const benchmark& benchmark, std::string_view name, std::chrono::duration<R, P> limit)
{
	write_right(benchmark, name, duration_cast<duration>(limit));
}