///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements ttfont.hpp.                                                                                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/ttfont.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include <SDL3_ttf/SDL_ttf.h>

////////////////////////////////////////////////////////////////// ERRORS /////////////////////////////////////////////////////////////////

tr::ttfont_load_error::ttfont_load_error(std::string_view path, std::string&& details)
	: m_description{TR_FMT::format("Failed to load bitmap from '{}'", path)}
	, m_details{std::move(details)}
{
}

std::string_view tr::ttfont_load_error::name() const
{
	return "TrueType font loading error";
}

std::string_view tr::ttfont_load_error::description() const
{
	return m_description;
}

std::string_view tr::ttfont_load_error::details() const
{
	return m_details;
}

//

tr::ttfont_render_error::ttfont_render_error(std::string_view description)
	: m_description{description}
{
}

std::string_view tr::ttfont_render_error::name() const
{
	return "TrueType font rendering error";
}

std::string_view tr::ttfont_render_error::description() const
{
	return m_description;
}

std::string_view tr::ttfont_render_error::details() const
{
	return {};
}

//

tr::ttfont_error::ttfont_error(std::string&& description)
	: m_description{std::move(description)}
	, m_details{SDL_GetError()}
{
}

std::string_view tr::ttfont_error::name() const
{
	return "TrueType font error";
}

std::string_view tr::ttfont_error::description() const
{
	return m_description;
}

std::string_view tr::ttfont_error::details() const
{
	return m_details;
}

/////////////////////////////////////////////////////////// MISC. FONT FUNCTIONS //////////////////////////////////////////////////////////

tr::ttfont::ttfont(TTF_Font* font)
	: m_ptr{font}
{
}

void tr::ttfont::deleter::operator()(TTF_Font* font) const
{
	TTF_CloseFont(font);
}

int tr::ttfont::ascent() const
{
	return TTF_GetFontAscent(m_ptr.get());
}

int tr::ttfont::descent() const
{
	return TTF_GetFontDescent(m_ptr.get());
}

int tr::ttfont::height() const
{
	return TTF_GetFontHeight(m_ptr.get());
}

int tr::ttfont::line_skip() const
{
	return TTF_GetFontLineSkip(m_ptr.get());
}

bool tr::ttfont::contains(u32 glyph) const
{
	return TTF_FontHasGlyph(m_ptr.get(), glyph);
}

void tr::ttfont::resize(float size)
{
	TR_ASSERT(size > 0, "Requested invalid font size {}.", size);

	if (!TTF_SetFontSize(m_ptr.get(), size)) {
		throw ttfont_error{"Failed to resize font to size {:.0f}.", size};
	}
}

void tr::ttfont::set_style(ttf_style style)
{
	TTF_SetFontStyle(m_ptr.get(), to_underlying(style));
}

void tr::ttfont::set_outline(int outline)
{
	if (!TTF_SetFontOutline(m_ptr.get(), outline)) {
		throw ttfont_error{"Failed to set font outline to {}.", outline};
	}
}

tr::glyph_metrics tr::ttfont::metrics(u32 glyph)
{
	glyph_metrics metrics{};
	if (!TTF_GetGlyphMetrics(m_ptr.get(), glyph, &metrics.min.x, &metrics.max.x, &metrics.min.y, &metrics.max.y, &metrics.advance)) {
		throw ttfont_error{"Failed to get glyph metrics."};
	}
	return metrics;
}

int tr::ttfont::kerning(u32 prev_glyph, u32 next_glyph)
{
	int kerning{};
	if (!TTF_GetGlyphKerning(m_ptr.get(), prev_glyph, next_glyph, &kerning)) {
		throw ttfont_error{"Failed to get glyph kerning."};
	}
	return kerning;
}

tr::ttf_measure_result tr::ttfont::measure_text(std::string_view text, int max_w) const
{
	ttf_measure_result result{};
	usize length{};
	if (!TTF_MeasureString(m_ptr.get(), text.data(), text.size(), max_w, &result.size, &length)) {
		throw ttfont_error{"Failed to measure text '{}'.", text};
	}
	result.text = {text.begin(), text.begin() + length};
	return result;
}

glm::ivec2 tr::ttfont::text_size(std::string_view text, int max_w) const
{
	glm::ivec2 size{};
	if (!TTF_GetStringSizeWrapped(m_ptr.get(), text.data(), text.size(), max_w, &size.x, &size.y)) {
		throw ttfont_error{"Failed to get size of text '{}'.", text};
	}
	return size;
}

//////////////////////////////////////////////////////////////// RENDERING ////////////////////////////////////////////////////////////////

// Fixes alpha artifacts in transparent text.
static tr::bitmap fix_alpha_artifacts(tr::bitmap&& bitmap, tr::u8 max_alpha)
{
	// We know the bitmap is ARGB_8888.
	tr::u8* row_it{reinterpret_cast<tr::u8*>(bitmap.data())};
	for (int y = 0; y < bitmap.size().y; ++y) {
		for (int x = 0; x < bitmap.size().x; ++x) {
			row_it[x * 4 + 3] = std::min(row_it[x * 4 + 3], max_alpha);
		}
		row_it += bitmap.pitch();
	}
	return std::move(bitmap);
}

tr::bitmap tr::ttfont::render(u32 glyph, rgba8 color) const
{
	SDL_Surface* const ptr{TTF_RenderGlyph_Blended(m_ptr.get(), glyph, std::bit_cast<SDL_Color>(color))};
	return ptr != nullptr ? bitmap{ptr} : throw ttfont_render_error{SDL_GetError()};
}

tr::bitmap tr::ttfont::render(std::string_view text, int max_w, halign align, rgba8 color) const
{
	TTF_SetFontWrapAlignment(m_ptr.get(), static_cast<TTF_HorizontalAlignment>(align));
	SDL_Surface* const ptr{TTF_RenderText_Blended_Wrapped(m_ptr.get(), text.data(), text.size(), std::bit_cast<SDL_Color>(color), max_w)};
	return ptr != nullptr ? fix_alpha_artifacts(bitmap{ptr}, color.a) : throw ttfont_render_error{SDL_GetError()};
}

//////////////////////////////////////////////////////////// LOADING FUNCTIONS ////////////////////////////////////////////////////////////

tr::ttfont tr::load_embedded_ttfont(std::span<const std::byte> data, float size)
{
	TTF_Font* const ptr{TTF_OpenFontIO(SDL_IOFromConstMem(data.data(), data.size()), true, size)};
	return ptr != nullptr ? ttfont{ptr} : throw ttfont_load_error{"(Embedded)", SDL_GetError()};
}

tr::ttfont tr::load_ttfont_file(const std::filesystem::path& path, float size)
{
	if (!exists(path)) {
		throw ttfont_load_error{path.string(), "File not found."};
	}

	TTF_Font* const ptr{TTF_OpenFont(TR_PATH_CSTR(path), size)};
	return ptr != nullptr ? ttfont{ptr} : throw ttfont_load_error{path.string(), SDL_GetError()};
}

////////////////////////////////////////////////////////////// LINE SPLITTING /////////////////////////////////////////////////////////////

std::vector<std::string_view> tr::split_into_lines(std::string_view str)
{
	std::vector<std::string_view> lines;
	std::string_view::iterator start{str.begin()};
	std::string_view::iterator end{std::find(start, str.end(), '\n')};
	while (end != str.end()) {
		lines.push_back({start, end});
		start = end + 1;
		end = std::find(start, str.end(), '\n');
	}
	lines.push_back({start, end});
	return lines;
}

std::vector<std::string_view> tr::break_overlong_lines(std::vector<std::string_view>&& lines, const ttfont& font, int max_w)
{
	for (std::vector<std::string_view>::iterator line_it = lines.begin(); line_it != lines.end(); ++line_it) {
		if (line_it->empty()) {
			continue;
		}

		const tr::ttf_measure_result measure{font.measure_text(*line_it, max_w)};
		if (measure.text != std::string_view{*line_it}) {
			usize last_ws{std::string_view{line_it->begin(), line_it->begin() + measure.text.size() + 1}.find_last_of(" \t")};
			if (last_ws != std::string_view::npos) {
				line_it = std::prev(lines.emplace(std::next(line_it), line_it->begin() + last_ws + 1, line_it->end()));
				*line_it = line_it->substr(0, last_ws);
			}
			else {
				line_it = std::prev(lines.emplace(std::next(line_it), line_it->begin() + measure.text.size(), line_it->end()));
				*line_it = line_it->substr(0, measure.text.size());
			}
		}
	}
	return std::move(lines);
}

std::vector<std::string_view> tr::split_into_lines(std::string_view str, const ttfont& font, int max_w)
{
	return break_overlong_lines(split_into_lines(str), font, max_w);
}