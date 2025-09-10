#pragma once
#include "../utility/color.hpp"
#include "../utility/exception.hpp"
#include "../utility/geometry.hpp"

struct TTF_Font;

namespace tr {
	class bitmap;
}

namespace tr::sys {
	// Error thrown when font loading fails.
	class ttfont_load_error : public exception {
	  public:
		// Constructs an exception.
		ttfont_load_error(std::string_view path, std::string&& details);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string m_description;
		// The details of the error.
		std::string m_details;
	};
	// Error thrown when font bitmap rendering fails.
	class ttfont_render_error : public exception {
	  public:
		// Constructs an exception.
		ttfont_render_error(std::string_view description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The details of the error.
		std::string_view m_description;
	};

	// Font style types. May be ORed together.
	enum class ttf_style : u8 {
		NORMAL = 0,       // Normal font.
		BOLD = 1,         // Bold font.
		ITALIC = 2,       // Italic font.
		UNDERLINE = 4,    // Underlined font.
		STRIKETHROUGH = 8 // Striked font.
	};
	DEFINE_BITMASK_OPERATORS(ttf_style);

	// Font glyph metrics.
	struct glyph_metrics {
		// Minimum coordinates of the glyph offset from the bottom-left of its bounding box.
		glm::ivec2 min;
		// Maximum coordinates of the glyph offset from the bottom-left of its bounding box.
		glm::ivec2 max;
		// Distance between the left edge of the glyph and the next glyph.
		int advance;
	};

	// TTF font string measurement result.
	struct ttf_measure_result {
		// The string that will fit.
		std::string_view text;
		// The size of the fitting string.
		int size;
	};

	// Sentinel value signifying that a string can be of unlimited length (instead of being wrapped).
	inline constexpr int UNLIMITED_WIDTH{0};

	// TrueType font.
	class ttfont {
	  public:
		// Gets the ascent of the font.
		int ascent() const;
		// Gets the descent of the font.
		int descent() const;
		// Gets the height of the font.
		int height() const;
		// Gets the distance between two lines.
		int line_skip() const;
		// Gets whether the font contains a glyph.
		bool contains(u32 glyph) const;

		// Resizes the font.
		void resize(float size);
		// Sets the font style.
		void set_style(ttf_style style);
		// Sets the font outline.
		void set_outline(int outline);

		// Gets the metrics of a glyph given the current size, style, and outline.
		glyph_metrics metrics(u32 glyph);
		// Gets the kerning between two glyphs given the current size, style, and outline.
		int kerning(u32 prev_glyph, u32 next_glyph);
		// Measures the amount of text that will fit in a given width given the current size, style, and outline.
		ttf_measure_result measure_text(std::string_view text, int max_w) const;
		// Gets the drawn size of a string of text given the current size, style, and outline.
		glm::ivec2 text_size(std::string_view text, int max_w = UNLIMITED_WIDTH) const;

		// Renders a glyph.
		// May throw: ttfont_render_error.
		bitmap render(u32 glyph, rgba8 color) const;
		// Renders a string of text.
		// May throw: ttfont_render_error.
		bitmap render(std::string_view text, int max_w, halign align, rgba8 color) const;

	  private:
		struct deleter {
			void operator()(TTF_Font* ptr) const;
		};

		// Handle to the SDL TTF font.
		std::unique_ptr<TTF_Font, deleter> m_ptr;

		ttfont(TTF_Font* ptr);

		friend ttfont load_embedded_ttfont(std::span<const std::byte> data, float size);
		friend ttfont load_ttfont_file(const std::filesystem::path& path, float size);
	};
	// Loads an embedded font file.
	// May throw: ttf_load_error.
	ttfont load_embedded_ttfont(std::span<const std::byte> data, float size = 16);
	// Loads an embedded font file.
	// May throw: ttf_load_error.
	template <std::ranges::contiguous_range R> ttfont load_embedded_ttfont(R&& range, float size = 16);
	// Loads a font from file.
	// May throw: file_not_found, ttfont_file_load_error.
	ttfont load_ttfont_file(const std::filesystem::path& path, float size = 16);

	// Splits a string view into a list of lines.
	std::vector<std::string_view> split_into_lines(std::string_view str);
	// Splits a vector of lines, breaking overlong lines according to the font's current size, style, and outline.
	std::vector<std::string_view> break_overlong_lines(std::vector<std::string_view>&& lines, const ttfont& font, int max_w);
	// Splits a string view into a list of lines, breaking overlong lines according to the font's current size, style, and outline.
	std::vector<std::string_view> split_into_lines(std::string_view str, const ttfont& font, int max_w);
} // namespace tr::sys

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range R> tr::sys::ttfont tr::sys::load_embedded_ttfont(R&& range, float size)
{
	return load_embedded_ttfont(std::span<const std::byte>{range_bytes(range)}, size);
};