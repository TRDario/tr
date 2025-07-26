#pragma once
#include "../utility/color.hpp"
#include "../utility/exception.hpp"
#include "../utility/geometry.hpp"

struct TTF_Font;

namespace tr {
	class bitmap;
}

namespace tr {
	// Error thrown when font loading fails.
	class ttfont_load_error : public exception {
	  public:
		// Constructs an exception.
		ttfont_load_error(std::string_view path, std::string&& details) noexcept;

		// Gets the name of the error.
		std::string_view name() const noexcept override;
		// Gets the description of the error.
		std::string_view description() const noexcept override;
		// Gets further details about the error.
		std::string_view details() const noexcept override;

	  private:
		// The description of the error.
		std::string _description;
		// The details of the error.
		std::string _details;
	};
	// Error thrown when font bitmap rendering fails.
	class ttfont_render_error : public exception {
	  public:
		// Constructs an exception.
		ttfont_render_error(std::string_view description) noexcept;

		// Gets the name of the error.
		std::string_view name() const noexcept override;
		// Gets the description of the error.
		std::string_view description() const noexcept override;
		// Gets further details about the error.
		std::string_view details() const noexcept override;

	  private:
		// The details of the error.
		std::string_view _description;
	};

	// Font style types. May be ORed together.
	enum class ttf_style : std::uint8_t {
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
		int ascent() const noexcept;
		// Gets the descent of the font.
		int descent() const noexcept;
		// Gets the height of the font.
		int height() const noexcept;
		// Gets the distance between two lines.
		int line_skip() const noexcept;
		// Gets whether the font contains a glyph.
		bool contains(std::uint32_t glyph) const noexcept;

		// Resizes the font.
		void resize(float size) noexcept;
		// Sets the font style.
		void set_style(ttf_style style) noexcept;
		// Sets the font outline.
		void set_outline(int outline) noexcept;

		// Gets the metrics of a glyph given the current size, style, and outline.
		glyph_metrics metrics(std::uint32_t glyph);
		// Gets the kerning between two glyphs given the current size, style, and outline.
		int kerning(std::uint32_t prev_glyph, std::uint32_t next_glyph) noexcept;
		// Measures the amount of text that will fit in a given width given the current size, style, and outline.
		ttf_measure_result measure_text(std::string_view text, int max_w) const noexcept;
		// Gets the drawn size of a string of text given the current size, style, and outline.
		glm::ivec2 text_size(std::string_view text, int max_w = UNLIMITED_WIDTH) const noexcept;

		// Renders a glyph.
		// May throw: ttfont_render_error.
		bitmap render(std::uint32_t glyph, rgba8 color) const;
		// Renders a string of text.
		// May throw: ttfont_render_error.
		bitmap render(std::string_view text, int max_w, halign align, rgba8 color) const;

	  private:
		struct deleter {
			void operator()(TTF_Font* ptr) const noexcept;
		};

		// Handle to the SDL TTF font.
		std::unique_ptr<TTF_Font, deleter> _impl;

		ttfont(TTF_Font* impl) noexcept;

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
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <std::ranges::contiguous_range R> tr::ttfont tr::load_embedded_ttfont(R&& range, float size)
{
	return load_embedded_ttfont(std::span<const std::byte>{range_bytes(range)}, size);
};