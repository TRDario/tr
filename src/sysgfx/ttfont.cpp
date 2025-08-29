#include "../../include/tr/sysgfx/ttfont.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/initialization.hpp"
#include <SDL3_ttf/SDL_ttf.h>

namespace tr::system {
	// Initializes SDL TTF if needed.
	void initialize_sdl_ttf_if_needed();
	// atexit callback for SDL TTF.
	void atexit_sdl_ttf();
	// Fixes certain edge artifacts when rendering partially transparent text.
	void fix_alpha_artifacts(bitmap& bitmap, u8 max_alpha);

} // namespace tr::system

void tr::system::initialize_sdl_ttf_if_needed()
{
	if (!TTF_WasInit()) {
		if (!TTF_Init()) {
			throw init_error{"Failed to initialize SDL_ttf."};
		}
		TR_LOG(log, severity::INFO, "Initialized SDL3_ttf.");
		atexit(atexit_sdl_ttf);
	}
}

void tr::system::atexit_sdl_ttf()
{
	TTF_Quit();
}

void tr::system::fix_alpha_artifacts(bitmap& bitmap, u8 max_alpha)
{
	// We know the bitmap is ARGB_8888.
	u8* it{(u8*)bitmap.data()};
	for (int y = 0; y < bitmap.size().y; ++y) {
		for (int x = 0; x < bitmap.size().x; ++x) {
			it[x * 4 + 3] = std::min(it[x * 4 + 3], max_alpha);
		}
		it += bitmap.pitch();
	}
}

tr::system::ttfont_load_error::ttfont_load_error(std::string_view path, std::string&& details)
	: m_description{TR_FMT::format("Failed to load bitmap from '{}'", path)}, m_details{std::move(details)}
{
}

std::string_view tr::system::ttfont_load_error::name() const
{
	return "TrueType font loading error";
}

std::string_view tr::system::ttfont_load_error::description() const
{
	return m_description;
}

std::string_view tr::system::ttfont_load_error::details() const
{
	return m_details;
}

tr::system::ttfont_render_error::ttfont_render_error(std::string_view description)
	: m_description{description}
{
}

std::string_view tr::system::ttfont_render_error::name() const
{
	return "TrueType font rendering error";
}

std::string_view tr::system::ttfont_render_error::description() const
{
	return m_description;
}

std::string_view tr::system::ttfont_render_error::details() const
{
	return {};
}

tr::system::ttfont::ttfont(TTF_Font* font)
	: m_ptr{font}
{
}

void tr::system::ttfont::deleter::operator()(TTF_Font* font) const
{
	TTF_CloseFont(font);
}

int tr::system::ttfont::ascent() const
{
	return TTF_GetFontAscent(m_ptr.get());
}

int tr::system::ttfont::descent() const
{
	return TTF_GetFontDescent(m_ptr.get());
}

int tr::system::ttfont::height() const
{
	return TTF_GetFontHeight(m_ptr.get());
}

int tr::system::ttfont::line_skip() const
{
	return TTF_GetFontLineSkip(m_ptr.get());
}

bool tr::system::ttfont::contains(u32 glyph) const
{
	return TTF_FontHasGlyph(m_ptr.get(), glyph);
}

void tr::system::ttfont::resize(float size)
{
	TR_ASSERT(size > 0, "Requested invalid font size {}.", size);

	if (!TTF_SetFontSize(m_ptr.get(), size)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to resize font to size {:.0f}.", size);
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::system::ttfont::set_style(ttf_style style)
{
	TTF_SetFontStyle(m_ptr.get(), TTF_FontStyleFlags(style));
}

void tr::system::ttfont::set_outline(int outline)
{
	if (!TTF_SetFontOutline(m_ptr.get(), outline)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set font outline to {}.", outline);
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

tr::system::glyph_metrics tr::system::ttfont::metrics(u32 glyph)
{
	system::glyph_metrics metrics{};
	if (!TTF_GetGlyphMetrics(m_ptr.get(), glyph, &metrics.min.x, &metrics.max.x, &metrics.min.y, &metrics.max.y, &metrics.advance)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to get glyph metrics.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	return metrics;
}

int tr::system::ttfont::kerning(u32 prev_glyph, u32 next_glyph)
{
	int kerning{};
	if (!TTF_GetGlyphKerning(m_ptr.get(), prev_glyph, next_glyph, &kerning)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to get glyph kerning.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	return kerning;
}

tr::system::ttf_measure_result tr::system::ttfont::measure_text(std::string_view text, int max_w) const
{
	ttf_measure_result result{};
	usize length{};
	if (!TTF_MeasureString(m_ptr.get(), text.data(), text.size(), max_w, &result.size, &length)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to measure text.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	result.text = {text.begin(), text.begin() + length};
	return result;
}

glm::ivec2 tr::system::ttfont::text_size(std::string_view text, int max_w) const
{
	glm::ivec2 size{};
	if (!TTF_GetStringSizeWrapped(m_ptr.get(), text.data(), text.size(), max_w, &size.x, &size.y)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to get text size.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	return size;
}

tr::bitmap tr::system::ttfont::render(u32 glyph, rgba8 color) const
{
	const SDL_Color sdl_color{color.r, color.g, color.b, color.a};
	SDL_Surface* surface{TTF_RenderGlyph_Blended(m_ptr.get(), glyph, sdl_color)};
	if (surface == nullptr) {
		throw ttfont_render_error{SDL_GetError()};
	}
	return surface;
}

tr::bitmap tr::system::ttfont::render(std::string_view text, int max_w, halign align, rgba8 color) const
{
	TTF_SetFontWrapAlignment(m_ptr.get(), TTF_HorizontalAlignment(align));

	const SDL_Color sdl_color{color.r, color.g, color.b, color.a};
	SDL_Surface* surface{TTF_RenderText_Blended_Wrapped(m_ptr.get(), text.data(), text.size(), sdl_color, max_w)};
	if (surface == nullptr) {
		throw ttfont_render_error{SDL_GetError()};
	}
	bitmap output{surface};
	fix_alpha_artifacts(output, color.a);
	return output;
}

tr::system::ttfont tr::system::load_embedded_ttfont(std::span<const std::byte> data, float size)
{
	initialize_sdl_ttf_if_needed();
	TTF_Font* font{TTF_OpenFontIO(SDL_IOFromConstMem(data.data(), data.size()), true, size)};
	if (font == nullptr) {
		throw ttfont_load_error{"(Embedded)", SDL_GetError()};
	}
	return ttfont{font};
}

tr::system::ttfont tr::system::load_ttfont_file(const std::filesystem::path& path, float size)
{
	if (!exists(path)) {
		throw ttfont_load_error{path.string(), "File not found."};
	}

	initialize_sdl_ttf_if_needed();
#ifdef _WIN32
	TTF_Font* font{TTF_OpenFont(path.string().c_str(), size)};
#else
	TTF_Font* font{TTF_OpenFont(path.c_str(), size)};
#endif
	if (font == nullptr) {
		throw ttfont_load_error{path.string(), SDL_GetError()};
	}
	return ttfont{font};
}

//

std::vector<std::string_view> tr::system::split_into_lines(std::string_view str)
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

std::vector<std::string_view> tr::system::break_overlong_lines(std::vector<std::string_view>&& lines, const ttfont& font, int max_w)
{
	for (std::vector<std::string_view>::iterator it = lines.begin(); it != lines.end(); ++it) {
		if (it->empty()) {
			continue;
		}

		const tr::system::ttf_measure_result measure{font.measure_text(*it, int(max_w))};
		if (measure.text != std::string_view{*it}) {
			usize last_ws{measure.text.find_last_of(" \t")};
			if (last_ws != std::string_view::npos) {
				it = std::prev(lines.emplace(std::next(it), it->begin() + last_ws + 1, it->end()));
				*it = it->substr(0, last_ws);
			}
			else {
				it = std::prev(lines.emplace(std::next(it), it->begin() + measure.text.size(), it->end()));
				*it = it->substr(0, measure.text.size());
			}
		}
	}
	return std::move(lines);
}

std::vector<std::string_view> tr::system::split_into_lines(std::string_view str, const ttfont& font, int max_w)
{
	return break_overlong_lines(split_into_lines(str), font, max_w);
}