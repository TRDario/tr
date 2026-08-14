/// @file
/// @brief Provides a TrueType font class and related functions.

#pragma once
#include "../utility/alignment.hpp"
#include "../utility/color.hpp"
#include "../utility/enum.hpp"
#include "../utility/exception.hpp"

struct TTF_Font;
namespace tr
{
	class bitmap;
}

//

namespace tr
{
	/// Error thrown when font loading fails.
	class ttfont_load_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param path Path to the file that failed to load.
		/// @param details Details of the error.
		ttfont_load_error(std::string_view path, std::string&& details);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"TrueType font loading error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Details of the error.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string m_details;
	};

	/// Error thrown when font bitmap rendering fails.
	class ttfont_render_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an exception.
		/// @param description Description of the error.
		ttfont_render_error(std::string_view description);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"TrueType font rendering error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Always empty.
		std::string_view details() const override;

		/// @}

	  private:
		// Description of the error.
		std::string_view m_description;
	};

	/// Error thrown when font manipulation fails.
	class ttfont_error : public exception
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a window error.
		/// @param description Description of the error.
		ttfont_error(std::string&& description);

		/// Constructs a font error.
		/// @tparam Args Types of the formatting arguments.
		/// @param description_fmt Error description format string.
		/// @param args Formatting arguments.
		template <typename... Args>
		ttfont_error(std::format_string<Args...> description_fmt, Args&&... args);

		/// @}
		/// @name Information
		/// @{

		/// Gets the name of the error.
		/// @return `"TrueType font error"`.
		std::string_view name() const override;

		/// Gets the description of the error.
		/// @return Description of the error.
		std::string_view description() const override;

		/// Gets further details about the error.
		/// @return Details of the error.
		std::string_view details() const override;

		/// @}

	  private:
		/// Description of the error.
		std::string m_description;

		/// Details of the error.
		std::string_view m_details;
	};

	//

	/// Font style types (may be ORed together).
	enum class ttf_style : u8
	{
		/// Normal font.
		normal = 0,
		/// Bold font.
		bold = 1,
		/// Italic font.
		italic = 2,
		/// Underlined font.
		underline = 4,
		/// Striked font.
		strikethrough = 8
	};

	/// @cond __hidden
	TR_DEFINE_ENUM_BITMASK_OPERATORS(ttf_style);
	/// @endcond

	/// Font glyph metrics.
	struct glyph_metrics
	{
		/// Minimum coordinates of the glyph offset from the bottom-left of its bounding box.
		glm::ivec2 min;

		/// Maximum coordinates of the glyph offset from the bottom-left of its bounding box.
		glm::ivec2 max;

		/// Distance between the left edge of the glyph and the next glyph.
		int advance;
	};

	/// TTF font string measurement result.
	struct ttf_measure_result
	{
		/// String that will fit.
		std::string_view text;

		/// Size of the fitting string.
		int size;
	};

	/// Sentinel value signifying that a string can be of unlimited length (instead of being wrapped).
	inline constexpr int unlimited_width{0};

	//

	// TrueType font.
	class ttfont
	{
	  public:
		/// @cond sdl_interop
		/// @name Constructors
		/// @{

		/// Wraps an SDL TrueType font.
		/// @param ptr Pointer to wrap.
		explicit ttfont(TTF_Font* ptr);

		/// @}
		/// @endcond
		/// @name Font information
		/// @{

		/// Gets the ascent of the font.
		/// @return Ascent of the font.
		int ascent() const;

		/// Gets the descent of the font.
		/// @return Descent of the font.
		int descent() const;

		/// Gets the height of the font.
		/// @return Height of the font.
		int height() const;

		/// Gets the distance between two lines.
		/// @return Distance between two lines.
		int line_skip() const;

		/// Gets whether the font contains a glyph.
		/// @param glyph Glyph to check for.
		/// @return `true` if the font contains `glyph`, `false` otherwise.
		bool contains(u32 glyph) const;

		/// @}
		/// @name Manipulation
		/// @{

		/// Resizes the font.
		/// @param size Point size of the font.
		void resize(float size);

		/// Sets the font style.
		/// @param style Style of the font.
		void set_style(ttf_style style);

		/// Sets the font outline.
		/// @param outline Outline width of the font.
		void set_outline(int outline);

		/// @}
		/// @name Measurements
		/// @{

		/// Gets the metrics of a glyph given the current size, style, and outline.
		/// @param glyph Glyph to get the metrics of.
		/// @return Metrics of `glyph`.
		glyph_metrics metrics(u32 glyph);

		/// Gets the kerning between two glyphs given the current size, style, and outline.
		/// @param prev_glyph Previous glyph.
		/// @param next_glyph Next glyph.
		/// @return Kerning between `prev_glyph` and `next_glyph`.
		int kerning(u32 prev_glyph, u32 next_glyph);

		/// Measures the amount of text that will fit in a given width given the current size, style, and outline.
		/// @param text Text to measure.
		/// @param max_w Maximum width of a line of text.
		/// @return Measurement of the drawn text.
		ttf_measure_result measure_text(std::string_view text, int max_w = unlimited_width) const;

		/// Gets the drawn size of a string of text given the current size, style, and outline.
		/// @param text Text to measure.
		/// @param max_w Maximum width of a line of text.
		/// @return Size of the drawn text.
		glm::ivec2 text_size(std::string_view text, int max_w = unlimited_width) const;

		/// @}
		/// @name Rendering
		/// @{

		/// Renders a glyph.
		/// @param glyph Glyph to render.
		/// @param color Glyph color.
		/// @exception ttfont_render_error If rendering the glyph bitmap failed.
		bitmap render(u32 glyph, rgba8 color) const;

		/// Renders a string of text.
		/// @param text Text to render.
		/// @param max_w Maximum width of a line of text.
		/// @param align Alignment of the lines of text.
		/// @param color Text color.
		/// @exception ttfont_render_error If rendering the string bitmap failed.
		bitmap render(std::string_view text, int max_w, halign align, rgba8 color) const;

		/// @}
		/// @cond sdl_interop
		/// @name SDL interoperability
		/// @{

		/// Unwraps the SDL font pointer.
		/// @note This does not release the pointer.
		/// @return Pointer to the SDL font.
		TTF_Font* unwrap() const;

		/// @}
		/// @endcond

	  private:
		/// Font closer.
		struct deleter
		{
			/// Closes a font.
			/// @param ptr Pointer to the SDL TrueType font.
			static void operator()(TTF_Font* ptr);
		};

		/// Handle to the SDL TrueType font.
		std::unique_ptr<TTF_Font, deleter> m_ptr;
	};

	/// @name TrueType font factories
	/// @{

	/// Loads an embedded font file.
	/// @param data Font file data.
	/// @param size Initial font size.
	/// @exception ttf_load_error If loading the font failed.
	/// @return Loaded TrueType font.
	ttfont load_embedded_ttfont(std::span<const std::byte> data, float size = 16);

	/// Loads an embedded font file.
	/// @param range Font file data range.
	/// @param size Initial font size.
	/// @exception ttf_load_error If loading the font failed.
	/// @return Loaded TrueType font.
	template <std::ranges::contiguous_range Range>
	ttfont load_embedded_ttfont(Range&& range, float size = 16);

	/// Loads a font from file.
	/// @param path Path to the font file.
	/// @param size Initial font size.
	/// @exception ttf_load_error If loading the font failed.
	/// @return Loaded TrueType font.
	ttfont load_ttfont_file(const std::filesystem::path& path, float size = 16);

	/// @}
	/// @name Text splitting
	/// @{

	/// Splits a string view into a list of lines.
	/// @param str String to split.
	/// @return Vector of views to the lines of the string.
	std::vector<std::string_view> split_into_lines(std::string_view str);

	/// Splits a vector of lines, breaking overlong lines according to the font's current size, style, and outline.
	/// @param lines Vector of views to the lines of the string.
	/// @param font Font according which to split.
	/// @param max_w Maximum width of a line of text.
	/// @return Vector of views to the split lines of the string.
	std::vector<std::string_view> break_overlong_lines(std::vector<std::string_view>&& lines, const ttfont& font, int max_w);

	/// Splits a string view into a list of lines, breaking overlong lines according to the font's current size, style, and outline.
	/// @param str String to split.
	/// @param font Font according which to split.
	/// @param max_w Maximum width of a line of text.
	/// @return Vector of views to the split lines of the string.
	std::vector<std::string_view> split_into_lines(std::string_view str, const ttfont& font, int max_w);

	/// @}
} // namespace tr

#include "impl/ttfont.hpp" // IWYU pragma: export