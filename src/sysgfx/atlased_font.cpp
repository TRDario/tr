#include "../../include/tr/sysgfx/atlased_font.hpp"

//

tr::u64 tr::gfx::atlased_font::glyph_hash::operator()(const glyph_key& k) const
{
	// Designed to be perfect with fonts up to size 512, outline 255 (within the float granularities below).
	const u64 size{u16(k.size * 128)};
	const u64 outline{u16(k.outline * 16) & 0xFFFU};
	return k.cp | size << 32 | outline < 48 | u64(k.style) << 60;
}

//

tr::gfx::atlased_font::atlased_font(system::ttfont&& base)
	: m_font{std::move(base)}
{
	m_atlas.set_filtering(min_filter::LINEAR, mag_filter::LINEAR);
}

//

tr::gfx::atlased_font::operator texture_ref() const
{
	return m_atlas;
}

tr::system::ttfont& tr::gfx::atlased_font::ttfont() const
{
	return m_font;
}

//

void tr::gfx::atlased_font::clear_cache()
{
	m_atlas.clear();
}

//

void tr::gfx::atlased_font::add_to_2d_renderer(std::string_view text, const text_style& style, const text_box& box, int layer) const
{
	usize count{count_glyphs_and_add_to_cache(text, style)};
	if (count > 0) {
		fill_mesh(text, style, box, renderer_2d::new_textured_mesh(layer, count * 4, poly_idx(4) * count, m_atlas));
	}
}

void tr::gfx::atlased_font::add_to_2d_renderer(std::string_view text, const text_style& style, const text_box& box, int layer,
											   const glm::mat4& mat, const blend_mode& blend_mode) const
{
	usize count{count_glyphs_and_add_to_cache(text, style)};
	if (count > 0) {
		fill_mesh(text, style, box, renderer_2d::new_textured_mesh(layer, count * 4, poly_idx(4) * count, m_atlas, mat, blend_mode));
	}
}

void tr::gfx::atlased_font::add_to_2d_renderer(std::string_view text, const outlined_text_style& style, const text_box& box, int text_layer,
											   int outline_layer) const
{
	usize count{count_glyphs_and_add_to_cache(text, style)};
	if (count > 0) {
		fill_mesh_text(text, style, box, renderer_2d::new_textured_mesh(text_layer, count * 4, poly_idx(4) * count, m_atlas));
		fill_mesh_outline(text, style, box, renderer_2d::new_textured_mesh(outline_layer, count * 4, poly_idx(4) * count, m_atlas));
	}
}

void tr::gfx::atlased_font::add_to_2d_renderer(std::string_view text, const outlined_text_style& style, const text_box& box, int text_layer,
											   int outline_layer, const glm::mat4& mat, const blend_mode& text_blend_mode,
											   const blend_mode& outline_blend_mode) const
{
	usize count{count_glyphs_and_add_to_cache(text, style)};
	if (count > 0) {
		fill_mesh_text(text, style, box,
					   renderer_2d::new_textured_mesh(text_layer, count * 4, poly_idx(4) * count, m_atlas, mat, text_blend_mode));
		fill_mesh_outline(text, style, box,
						  renderer_2d::new_textured_mesh(outline_layer, count * 4, poly_idx(4) * count, m_atlas, mat, outline_blend_mode));
	}
}

//

bool tr::gfx::atlased_font::is_whitespace(codepoint cp) const
{
	const system::glyph_metrics metrics{m_font.metrics(cp)};
	return metrics.max - metrics.min == glm::ivec2{0, 0};
}

tr::usize tr::gfx::atlased_font::count_glyphs_and_add_to_cache(std::string_view str, const text_style& style) const
{
	usize count{0};
	m_font.resize(style.font_size);
	m_font.set_style(style.style);
	m_font.set_outline(0);
	for (codepoint cp : utf8::range(str)) {
		const glyph_key key{cp, style.font_size, 0, style.style};
		if (!is_whitespace(cp)) {
			++count;
			if (!m_atlas.contains(key)) {
				m_atlas.add(key, m_font.render(cp, {255, 255, 255, 255}));
			}
		}
	}
	return count;
}

tr::usize tr::gfx::atlased_font::count_glyphs_and_add_to_cache(std::string_view str, const outlined_text_style& style) const
{
	usize count{count_glyphs_and_add_to_cache(str, {style.font_size, style.style, style.scale, style.text_tint})};
	m_font.set_outline(style.outline_thickness);
	for (codepoint cp : utf8::range(str)) {
		const glyph_key key{cp, style.font_size, style.outline_thickness, style.style};
		if (!m_atlas.contains(key) && !is_whitespace(cp)) {
			m_atlas.add(key, m_font.render(cp, {255, 255, 255, 255}));
		}
	}
	return count;
}

void tr::gfx::atlased_font::fill_mesh(std::string_view text, const text_style& style, const text_box& box, textured_mesh_ref mesh) const
{
	m_font.set_outline(0);

	u16 i{0};
	for (auto it = mesh.indices.begin(); it != mesh.indices.end();) {
		it = fill_poly_idx(it, 4, mesh.base_index + i);
		i += 4;
	}

	glm::vec2 start;
	std::vector<std::string_view> lines;
	if (box.size == UNSIZED) {
		start = tl(box.pos, glm::vec2{m_font.text_size(text)}, box.alignment);
		lines = system::split_into_lines(text);
	}
	else {
		start = tl(tl(box.pos, box.size, box.alignment), glm::vec2{m_font.text_size(text) * style.scale}, box.text_alignment);
		lines = system::split_into_lines(text, m_font, box.size.x / style.scale);
	}

	glm::vec2 tl{start};
	auto pos_it{mesh.positions.begin()};
	auto uv_it{mesh.uvs.begin()};
	auto tints_it{mesh.tints.begin()};
	for (std::string_view line : lines) {
		codepoint prev{UINT32_MAX};
		for (codepoint cp : utf8::range(line)) {
			const glyph_key key{cp, style.font_size, 0, style.style};
			const system::glyph_metrics metrics{m_font.metrics(cp)};

			if (prev != UINT32_MAX) {
				tl.x += m_font.kerning(prev, cp) * style.scale;
			}
			if (!is_whitespace(cp)) {
				pos_it = fill_rect_vtx(pos_it, {tl * style.scale, m_atlas.unnormalized(key).size * style.scale});
				uv_it = fill_rect_vtx(uv_it, m_atlas[key]);
				tints_it = std::fill_n(tints_it, 4, style.tint);
			}
			tl.x += metrics.advance * style.scale;
			prev = cp;
		}
		tl = {start.x, tl.y + m_font.line_skip() * style.scale};
	}
}

void tr::gfx::atlased_font::fill_mesh_text(std::string_view text, const outlined_text_style& style, const text_box& box,
										   textured_mesh_ref mesh) const
{
	m_font.set_outline(0);

	u16 i{0};
	for (auto it = mesh.indices.begin(); it != mesh.indices.end();) {
		it = fill_poly_idx(it, 4, mesh.base_index + i);
		i += 4;
	}

	glm::vec2 start;
	std::vector<std::string_view> lines;
	const glm::vec2 text_size{(glm::vec2{m_font.text_size(text)} + 2 * style.outline_thickness) * style.scale};
	if (box.size == UNSIZED) {
		start = tl(box.pos, text_size, box.alignment);
		lines = system::split_into_lines(text);
	}
	else {
		start = tl(tl(box.pos, box.size, box.alignment), text_size, box.text_alignment);
		lines = system::split_into_lines(text, m_font, (box.size.x - 2 * style.outline_thickness) / style.scale);
	}

	glm::vec2 tl{start};
	auto pos_it{mesh.positions.begin()};
	auto uv_it{mesh.uvs.begin()};
	auto tints_it{mesh.tints.begin()};
	for (std::string_view line : lines) {
		codepoint prev{UINT32_MAX};
		for (codepoint cp : utf8::range(line)) {
			const system::glyph_metrics metrics{m_font.metrics(cp)};

			if (prev != UINT32_MAX) {
				tl.x += m_font.kerning(prev, cp) * style.scale;
			}
			if (!is_whitespace(cp)) {
				const glyph_key key{cp, style.font_size, 0, style.style};

				pos_it = fill_rect_vtx(pos_it, {tl + style.outline_thickness * style.scale, m_atlas.unnormalized(key).size * style.scale});
				uv_it = fill_rect_vtx(uv_it, m_atlas[key]);
				tints_it = std::fill_n(tints_it, 4, style.text_tint);
			}
			tl.x += metrics.advance * style.scale;
			prev = cp;
		}
		tl = {start.x, tl.y + m_font.line_skip() * style.scale};
	}
}

void tr::gfx::atlased_font::fill_mesh_outline(std::string_view text, const outlined_text_style& style, const text_box& box,
											  textured_mesh_ref mesh) const
{
	m_font.set_outline(0);

	u16 i{0};
	for (auto it = mesh.indices.begin(); it != mesh.indices.end();) {
		it = fill_poly_idx(it, 4, mesh.base_index + i);
		i += 4;
	}

	glm::vec2 start;
	std::vector<std::string_view> lines;
	const glm::vec2 text_size{(glm::vec2{m_font.text_size(text)} + 2 * style.outline_thickness) * style.scale};
	if (box.size == UNSIZED) {
		start = tl(box.pos, text_size, box.alignment);
		lines = system::split_into_lines(text);
	}
	else {
		start = tl(tl(box.pos, box.size, box.alignment), text_size, box.text_alignment);
		lines = system::split_into_lines(text, m_font, (box.size.x - 2 * style.outline_thickness) / style.scale);
	}

	glm::vec2 tl{start};
	auto pos_it{mesh.positions.begin()};
	auto uv_it{mesh.uvs.begin()};
	auto tints_it{mesh.tints.begin()};
	for (std::string_view line : lines) {
		codepoint prev{UINT32_MAX};
		for (codepoint cp : utf8::range(line)) {
			const system::glyph_metrics metrics{m_font.metrics(cp)};

			if (prev != UINT32_MAX) {
				tl.x += m_font.kerning(prev, cp) * style.scale;
			}
			if (!is_whitespace(cp)) {
				const glyph_key key{cp, style.font_size, style.outline_thickness, style.style};

				pos_it = fill_rect_vtx(pos_it, {tl, m_atlas.unnormalized(key).size * style.scale});
				uv_it = fill_rect_vtx(uv_it, m_atlas[key]);
				tints_it = std::fill_n(tints_it, 4, style.outline_tint);
			}
			tl.x += metrics.advance * style.scale;
			prev = cp;
		}
		tl = {start.x, tl.y + m_font.line_skip() * style.scale};
	}
}