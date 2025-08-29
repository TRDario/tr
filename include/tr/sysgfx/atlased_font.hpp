#include "atlas.hpp"
#include "blending.hpp"
#include "renderer_2d.hpp"
#include "ttfont.hpp"

namespace tr::gfx {
	struct text_style {
		float font_size;
		system::ttf_style style{system::ttf_style::NORMAL};
		float scale{1.0f};
		rgba8 tint{255, 255, 255, 255};
	};
	struct outlined_text_style {
		float font_size;
		system::ttf_style style{system::ttf_style::NORMAL};
		float outline_thickness;
		float scale{1.0f};
		rgba8 text_tint{255, 255, 255, 255};
		rgba8 outline_tint{255, 255, 255, 255};
	};
	struct text_box {
		glm::vec2 pos;
		glm::vec2 size;
		tr::align alignment;
		tr::align text_alignment;
	};
	// Sentinel to signal that a box has no max size for text.
	constexpr glm::vec2 UNSIZED{};

	// Font utilising a texture atlas to cache glyphs.
	class atlased_font {
	  public:
		// Creates an atlased font by wrapping a regular font.
		atlased_font(system::ttfont&& base);

		// Gets a reference to the font's atlas texture.
		operator texture_ref() const;
		// Gets a reference to the base font.
		system::ttfont& ttfont() const;

		// Clears the font atlas.
		void clear_cache();

		// Gets the size of text.
		glm::vec2 text_size(std::string_view text, const text_style& style, float box_width = system::UNLIMITED_WIDTH) const;
		// Gets the size of outlined text.
		glm::vec2 text_size(std::string_view text, const outlined_text_style& style, float box_width = system::UNLIMITED_WIDTH) const;

		// Adds text without an outline to the 2D renderer.
		void add_to_2d_renderer(std::string_view text, const text_style& style, const text_box& box, int layer) const;
		// Adds text without an outline to the 2D renderer.
		void add_to_2d_renderer(std::string_view text, const text_style& style, const text_box& box, int layer, const glm::mat4& mat,
								const blend_mode& blend_mode) const;
		// Adds outlined text to the 2D renderer.
		void add_to_2d_renderer(std::string_view text, const outlined_text_style& style, const text_box& box, int text_layer,
								int outline_layer) const;
		// Adds outlined text to the 2D renderer.
		void add_to_2d_renderer(std::string_view text, const outlined_text_style& style, const text_box& box, int text_layer,
								int outline_layer, const glm::mat4& mat, const blend_mode& text_blend_mode,
								const blend_mode& outline_blend_mode) const;

	  private:
		struct glyph_key {
			codepoint cp;
			float size;
			float outline;
			system::ttf_style style;

			constexpr bool operator==(const glyph_key& r) const = default;
		};
		struct glyph_hash {
			u64 operator()(const glyph_key& k) const;
		};

		mutable system::ttfont m_font;
		mutable dyn_atlas<glyph_key, glyph_hash> m_atlas;

		bool is_whitespace(codepoint cp) const;
		usize count_glyphs_and_add_to_cache(std::string_view str, const text_style& style) const;
		usize count_glyphs_and_add_to_cache(std::string_view str, const outlined_text_style& style) const;
		void fill_mesh(std::string_view text, const text_style& style, const text_box& box, textured_mesh_ref mesh) const;
		void fill_mesh_text(std::string_view text, const outlined_text_style& style, const text_box& box, textured_mesh_ref mesh) const;
		void fill_mesh_outline(std::string_view text, const outlined_text_style& style, const text_box& box, textured_mesh_ref mesh) const;
	};
} // namespace tr::gfx