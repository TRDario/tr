#include "../../include/tr/sysgfx/ttfont.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/initialization.hpp"
#include <SDL3_ttf/SDL_ttf.h>

namespace tr {
	// Initializes SDL TTF if needed.
	void initialize_sdl_ttf_if_needed();
	// atexit callback for SDL TTF.
	void atexit_sdl_ttf();
	// Fixes certain edge artifacts when rendering partially transparent text.
	void fix_alpha_artifacts(bitmap& bitmap, std::uint8_t max_alpha);

} // namespace tr

void tr::initialize_sdl_ttf_if_needed()
{
	if (!TTF_WasInit()) {
		if (!TTF_Init()) {
			throw system_initialization_error{"Failed to initialize SDL_ttf."};
		}
		TR_LOG(log, severity::INFO, "Initialized SDL3_ttf.");
		atexit(atexit_sdl_ttf);
	}
}

void tr::atexit_sdl_ttf()
{
	TTF_Quit();
}

void tr::fix_alpha_artifacts(bitmap& bitmap, std::uint8_t max_alpha)
{
	// We know the bitmap is ARGB_8888.
	std::uint8_t* it{reinterpret_cast<std::uint8_t*>(bitmap.data())};
	for (int y = 0; y < bitmap.size().y; ++y) {
		for (int x = 0; x < bitmap.size().x; ++x) {
			it[x * 4 + 3] = std::min(it[x * 4 + 3], max_alpha);
		}
		it += bitmap.pitch();
	}
}

tr::ttfont_load_error::ttfont_load_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to load bitmap from '{}'", path)}, m_details{std::move(details)}
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

tr::ttfont::ttfont(TTF_Font* font)
	: ptr{font}
{
}

void tr::ttfont::deleter::operator()(TTF_Font* font) const
{
	TTF_CloseFont(font);
}

int tr::ttfont::ascent() const
{
	return TTF_GetFontAscent(ptr.get());
}

int tr::ttfont::descent() const
{
	return TTF_GetFontDescent(ptr.get());
}

int tr::ttfont::height() const
{
	return TTF_GetFontHeight(ptr.get());
}

int tr::ttfont::line_skip() const
{
	return TTF_GetFontLineSkip(ptr.get());
}

bool tr::ttfont::contains(std::uint32_t glyph) const
{
	return TTF_FontHasGlyph(ptr.get(), glyph);
}

void tr::ttfont::resize(float size)
{
	TR_ASSERT(size > 0, "Requested invalid font size {}.", size);

	if (!TTF_SetFontSize(ptr.get(), size)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to resize font to size {:.0f}.", size);
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

void tr::ttfont::set_style(ttf_style style)
{
	TTF_SetFontStyle(ptr.get(), static_cast<TTF_FontStyleFlags>(style));
}

void tr::ttfont::set_outline(int outline)
{
	if (!TTF_SetFontOutline(ptr.get(), outline)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to set font outline to {}.", outline);
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
}

tr::glyph_metrics tr::ttfont::metrics(std::uint32_t glyph)
{
	tr::glyph_metrics metrics{};
	if (!TTF_GetGlyphMetrics(ptr.get(), glyph, &metrics.min.x, &metrics.max.x, &metrics.min.y, &metrics.max.y, &metrics.advance)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to get glyph metrics.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	return metrics;
}

int tr::ttfont::kerning(std::uint32_t prev_glyph, std::uint32_t next_glyph)
{
	int kerning{};
	if (!TTF_GetGlyphKerning(ptr.get(), prev_glyph, next_glyph, &kerning)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to get glyph kerning.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	return kerning;
}

tr::ttf_measure_result tr::ttfont::measure_text(std::string_view text, int max_w) const
{
	ttf_measure_result result{};
	std::size_t length{};
	if (!TTF_MeasureString(ptr.get(), text.data(), text.size(), max_w, &result.size, &length)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to measure text.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	result.text = {text.begin(), text.begin() + length};
	return result;
}

glm::ivec2 tr::ttfont::text_size(std::string_view text, int max_w) const
{
	glm::ivec2 size{};
	if (!TTF_GetStringSizeWrapped(ptr.get(), text.data(), text.size(), max_w, &size.x, &size.y)) {
		TR_LOG(log, tr::severity::ERROR, "Failed to get text size.");
		TR_LOG_CONTINUE(log, "{}", SDL_GetError());
	}
	return size;
}

tr::bitmap tr::ttfont::render(std::uint32_t glyph, rgba8 color) const
{
	const SDL_Color sdl_color{color.r, color.g, color.b, color.a};
	SDL_Surface* surface{TTF_RenderGlyph_Blended(ptr.get(), glyph, sdl_color)};
	if (surface == nullptr) {
		throw ttfont_render_error{SDL_GetError()};
	}
	return surface;
}

tr::bitmap tr::ttfont::render(std::string_view text, int max_w, halign align, rgba8 color) const
{
	TTF_SetFontWrapAlignment(ptr.get(), static_cast<TTF_HorizontalAlignment>(align));

	const SDL_Color sdl_color{color.r, color.g, color.b, color.a};
	SDL_Surface* surface{TTF_RenderText_Blended_Wrapped(ptr.get(), text.data(), text.size(), sdl_color, max_w)};
	if (surface == nullptr) {
		throw ttfont_render_error{SDL_GetError()};
	}
	bitmap output{surface};
	fix_alpha_artifacts(output, color.a);
	return output;
}

tr::ttfont tr::load_embedded_ttfont(std::span<const std::byte> data, float size)
{
	initialize_sdl_ttf_if_needed();
	TTF_Font* font{TTF_OpenFontIO(SDL_IOFromConstMem(data.data(), data.size()), true, size)};
	if (font == nullptr) {
		throw ttfont_load_error{"(Embedded)", SDL_GetError()};
	}
	return ttfont{font};
}

tr::ttfont tr::load_ttfont_file(const std::filesystem::path& path, float size)
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