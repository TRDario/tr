#pragma once
#include "shader_pipeline.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"
#include "vertex_format.hpp"

namespace tr {
	class benchmark;
}

namespace tr::gfx {
	class debug_renderer {
	  public:
		debug_renderer(float scale = 1.0f, u8 column_limit = 255);

		// Sets the text's drawing scale.
		void set_scale(float scale);
		// Sets the text's column limit.
		void set_column_limit(u8 columns);

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

	  private:
		struct glyph {
			// Position on the glyph grid.
			glm::u8vec2 pos;
			// Whether the glyph should be right-aligned.
			bool align_right;
			// The ASCII value of the glyph.
			char chr;
			// The color of the text.
			rgba8 text_color;
			// The color of the background.
			rgba8 bg_color;
		};
		struct context;

		// The pipeline and shaders used by the renderer.
		owning_shader_pipeline m_pipeline;
		// The debug vertex format.
		vertex_format m_format;
		// The font texture.
		texture m_font;
		// The vertex buffer holding a glyph mesh.
		static_vertex_buffer<glm::u8vec2> m_mesh;
		// The vertex buffer holding the glyph information
		dyn_vertex_buffer<glyph> m_glyph_buffer;
		// List of glyphs to draw.
		std::vector<glyph> m_glyphs;
		// Maximum allowed number of glyphs per line.
		u8 m_column_limit;
		// The current left line position.
		u8 m_left_line;
		// The current right line position.
		u8 m_right_line;

		// Right-aligns a line.
		void right_align_line(usize begin, usize end);
		// Trims trailing whitespace in a line.
		void trim_trailing_whitespace(context& context);
		// Moves a word to the next line.
		void move_word_to_next_line(context& context);
		// Breaks a line at the last whitespace.
		void break_at_last_whitespace(context& context);
		// Breaks an overlong word.
		void break_overlong_word(context& context);
		// Handles a line reaching the column limit.
		void handle_column_limit(context& context);
		// Writes a character.
		void write_character(char chr, context& context);
		// Handles a newline.
		void handle_newline(context& context);
		// Handles a control sequence.
		void handle_control_sequence(std::string_view::iterator& it, std::string_view::iterator end, context& context, rgba8 text_color,
									 rgba8 bg_color, std::span<rgba8> extra_colors);
		// Writes a line of formatted text.
		void write(bool right, std::string_view text, rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors);
	};
}; // namespace tr::gfx

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class R, class P>
void tr::gfx::debug_renderer::write_right(const benchmark& benchmark, std::string_view name, std::chrono::duration<R, P> limit)
{
	write_right(benchmark, name, duration_cast<duration>(limit));
}