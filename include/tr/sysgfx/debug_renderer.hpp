///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a renderer for easy writing of debug information on the screen.                                                              //
//                                                                                                                                       //
// The renderer is capable of displaying ASCII text. The scale of the text, as well as the maximum length of a line of text can be set   //
// in the constructor of the renderer, or afterwards using the appropriate methods:                                                      //
//     - tr::debug_renderer debug{2, 128} -> equivalent to tr::debug_renderer debug; debug.set_scale(2); debug.set_column_limit(128);    //
//                                                                                                                                       //
// The debug renderer is written-to line-by-line until a call to the .draw() method, which clears all written text after rendering.      //
// The renderer keeps track of the left and right sides of the screen separately. A normal string, a format string, or a benchmark that  //
// implements the debug_writable_benchmark interface (has .min(), .avg(), and .max() that return durations) can all be written this way. //
// Both of the string types can use a custom style (text color, background color, extra colors). A lone newline can also be written:     //
//     - debug.write_left("Example") -> writes Example on the left side of the screen in the default style                               //
//     - debug.write_left("Example", {.text_color{255, 0, 0, 255}}) -> writes Example on the left side of the screen in red text         //
//     - debug.write_right_format("Total: {}", total) -> writes formatted text on the right side of the screen in the default style      //
//     - debug.write_right_format({.background_color{64, 64, 64, 255}}, "Total: {}", total)                                              //
//       -> writes formatted text on the right side of the screen with a gray background                                                 //
//     - debug.write_benchmark(tick_benchmark, "Tick: ", 1.0s / 240)                                                                     //
//       -> writes benchmark data with the label "Tick", if any of the measurements is above 1/240s, it's marked in red                  //
//     - debug.newline_left(); debug.newline_right() -> skips a line on both of the sides of the screen                                  //
//     - debug.draw() -> draws any text previously written to the backbuffer and resets the canvas to empty                              //
//                                                                                                                                       //
// Both string types may contain escape sequences which allow for some control over the writing style:                                   //
//   '$bX' sets the background color to the color at index X of the extra colors span of the style                                       //
//   '$B' resets the background color to the default style color                                                                         //
//   '$cX' sets the text color to the color at index X of the extra colors span of the style                                             //
//   '$C' resets the text color to the default style color                                                                               //
//   '$n' begins a new line of text                                                                                                      //
//   '$$' writes the character '$'                                                                                                       //
// '$b' and '$c' both only look at one digit in front of them, so only 10 extra colors may ever be accessed. The renderer ignores        //
// invalid indices.                                                                                                                      //
//     - "Example $c0escaped$C $b1string$B"                                                                                              //
//       -> "escaped" is colored with style.extra_colors[0], while the background behind "string" is colored with style.extra_colors[1]  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "shader_pipeline.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"
#include "vertex_format.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	enum class renderer_id : u32;

	// Defines a benchmark interface compatible with debug_renderer.
	template <class T>
	concept debug_writable_benchmark = requires(const T& benchmark) {
		std::chrono::duration_cast<duration>(benchmark.min());
		std::chrono::duration_cast<duration>(benchmark.avg());
		std::chrono::duration_cast<duration>(benchmark.max());
	};

	// Debug text renderer.
	class debug_renderer {
	  public:
		// Debug text style.
		struct style {
			// The color of the text.
			rgba8 text_color{255, 255, 255, 255};
			// The color of the background.
			rgba8 background_color{0, 0, 0, 255};
			// Any extra colors that may be accessed using escape sequences in the debug text.
			std::span<rgba8> extra_colors{};
		};
		// The default text style: white text on black background.
		static constexpr style default_style{{255, 255, 255, 255}, {0, 0, 0, 255}, {}};

		// Creates a debug text renderer.
		debug_renderer(float scale = 1.0f, u8 column_limit = 255);

		// Sets the text's drawing scale.
		void set_scale(float scale);
		// Sets the text's column limit.
		void set_column_limit(u8 columns);

		// Writes a line of formatted text to the left side of the screen.
		void write_left(std::string_view text, const style& style = default_style);
		// Writes a line of text formatted as if by std::format to the left side of the screen in the default style.
		template <class... Args> void write_left_format(TR_FORMAT_STRING<Args...> fmt, Args&&... args);
		// Writes a line of text formatted as if by std::format to the left side of the screen.
		template <class... Args> void write_left_format(const style& style, TR_FORMAT_STRING<Args...> fmt, Args&&... args);
		// Writes a line of formatted text to the right side of the screen.
		void write_right(std::string_view text, const style& style = default_style);
		// Writes a line of text formatted as if by std::format to the right side of the screen in the default style.
		template <class... Args> void write_right_format(TR_FORMAT_STRING<Args...> fmt, Args&&... args);
		// Writes a line of text formatted as if by std::format to the right side of the screen.
		template <class... Args> void write_right_format(const style& style, TR_FORMAT_STRING<Args...> fmt, Args&&... args);
		// Writes benchmark data to the right side of the screen.
		template <debug_writable_benchmark Benchmark, class R, class P>
		void write_benchmark(const Benchmark& benchmark, std::string_view name, std::chrono::duration<R, P> limit);

		// Writes a newline on the left side of the screen.
		void newline_left();
		// Writes a newline on the right side of the screen.
		void newline_right();

		// Draws all written text to the screen and clears it.
		void draw();

	  private:
		// Glyph information sent to the vertex shader.
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

		// Class that the debug renderer delegates writing of glyph information to.
		class writer {
		  public:
			// Creates a writer.
			writer(const style& style, std::vector<glyph>& glyphs, u8& line, bool right_aligned, u8 column_limit, usize offset);

			// Writes a string of text.
			void write(std::string_view text);

		  private:
			// The text style.
			const style& m_style;
			// The output glyph vector.
			std::vector<glyph>& m_glyphs;
			// The current line number.
			u8& m_line;
			// Whether the text is right-aligned.
			bool m_right_aligned;
			// Maximum allowed number of glyphs per line.
			u8 m_column_limit;
			// The text color.
			rgba8 m_text_color;
			// The background color.
			rgba8 m_background_color;
			// The length of the current line.
			u8 m_line_length;
			// The offset to the start of the text written by this writer.
			usize m_current_text_start;
			// The offset to the start of the current line.
			usize m_current_line_start;
			// The offset to the start of the current word.
			usize m_current_word_start;

			// Right-aligns the characters of the current line up to the specified end.
			void right_align_current_line_up_to(usize line_end);
			// Trims trailing whitespace in a line.
			void trim_whitespace_before_current_word();
			// Moves the current word to the next line.
			void move_current_word_to_next_line();
			// Breaks the current line before the current word.
			void break_before_current_word();
			// Breaks the current line in the middle of the current word.
			void break_current_word();
			// Breaks the current line.
			void break_current_line();
			// Handles a newline.
			void handle_newline();
			// Writes a character.
			void write_character(char chr);
			// Handles a control sequence.
			void handle_control_sequence(std::string_view::iterator& it, std::string_view::iterator end);
		};

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
		// The ID of the renderer.
		renderer_id m_id;
		// Maximum allowed number of glyphs per line.
		u8 m_column_limit;
		// The current left line position.
		u8 m_left_line;
		// The current right line position.
		u8 m_right_line;

		// Writes benchmark data to the right side of the screen.
		void write_benchmark(duration min, duration avg, duration max, std::string_view name, duration limit);
	};
}; // namespace tr::gfx

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class... Args> void tr::gfx::debug_renderer::write_left_format(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	write_left(std::format(fmt, std::forward<Args>(args)...), default_style);
}

template <class... Args> void tr::gfx::debug_renderer::write_left_format(const style& style, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	write_left(std::format(fmt, std::forward<Args>(args)...), style);
}

template <class... Args> void tr::gfx::debug_renderer::write_right_format(TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	write_left(std::format(fmt, std::forward<Args>(args)...), default_style);
}

template <class... Args> void tr::gfx::debug_renderer::write_right_format(const style& style, TR_FORMAT_STRING<Args...> fmt, Args&&... args)
{
	write_right(std::format(fmt, std::forward<Args>(args)...), style);
}

template <tr::gfx::debug_writable_benchmark Benchmark, class R, class P>
void tr::gfx::debug_renderer::write_benchmark(const Benchmark& benchmark, std::string_view name, std::chrono::duration<R, P> limit)
{
	write_benchmark(std::chrono::duration_cast<duration>(benchmark.min()), std::chrono::duration_cast<duration>(benchmark.avg()),
					std::chrono::duration_cast<duration>(benchmark.max()), name, std::chrono::duration_cast<duration>(limit));
}