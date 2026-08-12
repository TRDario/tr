/// @file
/// @brief Provides a renderer for easy writing of debug information on the screen.

#pragma once
#include "../utility/chrono.hpp"
#include "shader_pipeline.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"
#include "vertex_format.hpp"

namespace tr
{
	enum class renderer_id : u32;
}

//

namespace tr
{
	/// Benchmark interface compatible with debug_renderer.
	template <typename T>
	concept debug_writable_benchmark = requires(const T& benchmark) {
		std::chrono::duration_cast<duration>(benchmark.min());
		std::chrono::duration_cast<duration>(benchmark.avg());
		std::chrono::duration_cast<duration>(benchmark.max());
	};

	//

	/// Debug text renderer.
	class debug_renderer
	{
	  public:
		/// Debug text style.
		struct style
		{
			/// Color of the text.
			rgba8 text_color{255, 255, 255, 255};

			/// Color of the background.
			rgba8 background_color{0, 0, 0, 255};

			/// Any extra colors that may be accessed using escape sequences in the debug text.
			std::span<rgba8> extra_colors{};
		};

		//

		/// Default text style: white text on black background.
		static constexpr style default_style{{255, 255, 255, 255}, {0, 0, 0, 255}, {}};

		/// @name Constructors
		/// @{

		/// Creates a debug text renderer.
		/// @param context Graphics context to create the renderer on.
		/// @param scale Initial text drawing scale.
		/// @param column_limit Initial text column limit.
		debug_renderer(graphics_context& context, float scale = 1.0f, u8 column_limit = 255);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the renderer is on.
		/// @return Reference to the graphics context the renderer is on
		graphics_context& context() const;

		/// @}
		/// @name Properties
		/// @{

		/// Sets the text's drawing scale.
		/// @param scale Text drawing scale.
		void set_scale(float scale);

		/// Sets the text's column limit.
		/// @param columns Maximum number of text columns.
		void set_column_limit(u8 columns);

		/// @}
		/// @name Writing
		/// @{

		/// Writes a line of formatted text to the left side of the screen.
		/// @param text Text to write.
		/// @param style Text style.
		void write_left(std::string_view text, const style& style = default_style);

		/// Writes a line of text formatted as if by std::format to the left side of the screen in the default style.
		/// @tparam Args Formatting argument types.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void write_left_format(std::format_string<Args...> fmt, Args&&... args);

		/// Writes a line of text formatted as if by std::format to the left side of the screen.
		/// @tparam Args Formatting argument types.
		/// @param style Text style.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void write_left_format(const style& style, std::format_string<Args...> fmt, Args&&... args);

		/// Writes a line of formatted text to the right side of the screen.
		/// @param text Text to write.
		/// @param style Text style.
		void write_right(std::string_view text, const style& style = default_style);

		/// Writes a line of text formatted as if by std::format to the right side of the screen in the default style.
		/// @tparam Args Formatting argument types.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void write_right_format(std::format_string<Args...> fmt, Args&&... args);

		/// Writes a line of text formatted as if by std::format to the right side of the screen.
		/// @tparam Args Formatting argument types.
		/// @param style Text style.
		/// @param fmt Format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		void write_right_format(const style& style, std::format_string<Args...> fmt, Args&&... args);

		/// Writes benchmark data to the right side of the screen.
		/// @tparam Benchmark Writable benchmark type.
		/// @tparam Rep Duration representation type.
		/// @tparam Period Duration period.
		/// @param benchmark Benchmark to write.
		/// @param name Name of the benchmark.
		/// @param limit Maximum allowed duration before it's written in red.
		template <debug_writable_benchmark Benchmark, typename Rep, typename Period>
		void write_benchmark(const Benchmark& benchmark, std::string_view name, std::chrono::duration<Rep, Period> limit);

		/// Writes a newline on the left side of the screen.
		void newline_left();

		/// Writes a newline on the right side of the screen.
		void newline_right();

		/// @}
		/// @name Drawing
		/// @{

		/// Draws all written text to the screen and clears it.
		void draw();

		/// @}

	  private:
		/// Glyph information sent to the vertex shader.
		struct glyph
		{
			/// Position on the glyph grid.
			glm::u8vec2 pos;

			/// Whether the glyph should be right-aligned.
			bool align_right;

			/// ASCII value of the glyph.
			char chr;

			/// Color of the text.
			rgba8 text_color;

			/// Color of the background.
			rgba8 bg_color;

			//

			/// Provided for `tr::as_vertex_attribute_list`.
			static constexpr auto as_vertex_attribute_list{tr::as_vertex_attribute_list<glm::u8vec2, u8, u8, rgba8, rgba8>};
		};

		/// Class that the debug renderer delegates writing of glyph information to.
		class writer
		{
		  public:
			/// Creates a writer.
			/// @param style Text style.
			/// @param glyphs Glyph vector to output to.
			/// @param line Reference to the used line position.
			/// @param right_aligned Whether the written text is right-aligned.
			/// @param column_limit Column limit of the written text.
			/// @param offset
			writer(const style& style, std::vector<glyph>& glyphs, u8& line, bool right_aligned, u8 column_limit, usize offset);

			//

			/// Writes a string of text.
			/// @param text Text to write.
			void write(std::string_view text);

		  private:
			/// Used text style.
			const style& m_style;

			/// Output glyph vector.
			std::vector<glyph>& m_glyphs;

			/// Current line number.
			u8& m_line;

			/// Whether the text is right-aligned.
			bool m_right_aligned;

			/// Maximum allowed number of glyphs per line.
			u8 m_column_limit;

			/// Text color.
			rgba8 m_text_color;

			/// Background color.
			rgba8 m_background_color;

			/// Length of the current line.
			u8 m_line_length;

			/// Offset to the start of the text written by this writer.
			usize m_current_text_start;

			/// Offset to the start of the current line.
			usize m_current_line_start;

			/// Offset to the start of the current word.
			usize m_current_word_start;

			//

			/// Right-aligns the characters of the current line up to the specified end.
			/// @param line_end Index to the line end.
			void right_align_current_line_up_to(usize line_end);

			/// Trims trailing whitespace in a line.
			void trim_whitespace_before_current_word();

			/// Moves the current word to the next line.
			void move_current_word_to_next_line();

			/// Breaks the current line before the current word.
			void break_before_current_word();

			/// Breaks the current line in the middle of the current word.
			void break_current_word();

			/// Breaks the current line.
			void break_current_line();

			/// Handles a newline.
			void handle_newline();

			/// Writes a character.
			/// @param chr Character to write.
			void write_character(char chr);

			/// Handles a control sequence.
			/// @param control_it Iterator to the control sequence.
			/// @param end Iterator to the end of the string.
			void handle_control_sequence(std::string_view::iterator& control_it, std::string_view::iterator end);
		};

		//

		/// Bindings of the debug renderer vertex format.
		static constexpr std::array vertex_format_bindings{make_vertex_binding<glm::u8vec2>(), make_vertex_binding<glyph>(1)};

		//

		/// Pipeline and shaders used by the renderer.
		owning_shader_pipeline m_pipeline;

		/// Debug vertex format.
		vertex_format m_format;

		/// Font texture.
		texture m_font;

		/// Vertex buffer holding a glyph mesh.
		static_vertex_buffer<glm::u8vec2> m_mesh;

		/// Vertex buffer holding the glyph information.
		dyn_vertex_buffer<glyph> m_glyph_buffer;

		/// List of glyphs to draw.
		std::vector<glyph> m_glyphs;

		/// ID of the renderer.
		renderer_id m_id;

		/// Maximum allowed number of glyphs per line.
		u8 m_column_limit;

		/// Current left line position.
		u8 m_left_line;

		/// Current right line position.
		u8 m_right_line;

		//

		/// Writes benchmark data to the right side of the screen.
		/// @param min Minimum duration reported by the benchmark.
		/// @param avg Average duration reported by the benchmark.
		/// @param max Maximum duration reported by the benchmark.
		/// @param name Name of the benchmark.
		/// @param limit Maximum allowed duration before it's written in red.
		void write_benchmark(duration min, duration avg, duration max, std::string_view name, duration limit);
	};
}; // namespace tr

#include "impl/debug_renderer.hpp" // IWYU pragma: export