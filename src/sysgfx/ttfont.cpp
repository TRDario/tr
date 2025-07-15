#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/ttfont.hpp"
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace tr {
	// Initializes SDL TTF if needed.
	void _init_ttf_if_needed() noexcept;
	// atexit callback for SDL TTF.
	void _atexit_ttf() noexcept;
	// Fixes certain edge artifacts when rendering partially transparent text.
	void _fix_alpha_artifacts(bitmap& bitmap, std::uint8_t max_alpha) noexcept;

} // namespace tr

void tr::_init_ttf_if_needed() noexcept
{
	if (!TTF_WasInit()) {
		if (!TTF_Init()) {
			TR_TERMINATE("SDL3_ttf initialization failed", SDL_GetError());
		}
		TR_LOG(log, severity::INFO, "Initialized SDL3_ttf.");
		atexit(_atexit_ttf);
	}
}

void tr::_atexit_ttf() noexcept
{
	TTF_Quit();
}

void tr::_fix_alpha_artifacts(bitmap& bitmap, std::uint8_t max_alpha) noexcept
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

tr::ttfont_load_error::ttfont_load_error(std::string_view location, std::string_view path, std::string&& details) noexcept
	: exception{location}, _description{std::format("Failed to load bitmap from '{}'", path)}, _details{std::move(details)}
{
}

std::string_view tr::ttfont_load_error::name() const noexcept
{
	return "TrueType font loading error";
}

std::string_view tr::ttfont_load_error::description() const noexcept
{
	return _description;
}

std::string_view tr::ttfont_load_error::details() const noexcept
{
	return _details;
}

tr::ttfont_render_error::ttfont_render_error(std::string_view location, std::string_view description) noexcept
	: exception{location}, _description{description}
{
}

std::string_view tr::ttfont_render_error::name() const noexcept
{
	return "TrueType font rendering error";
}

std::string_view tr::ttfont_render_error::description() const noexcept
{
	return _description;
}

std::string_view tr::ttfont_render_error::details() const noexcept
{
	return {};
}

tr::ttfont::ttfont(TTF_Font* font) noexcept
	: _impl{font}
{
}

void tr::ttfont::deleter::operator()(TTF_Font* font) const noexcept
{
	TTF_CloseFont(font);
}

int tr::ttfont::ascent() const noexcept
{
	return TTF_GetFontAscent(_impl.get());
}

int tr::ttfont::descent() const noexcept
{
	return TTF_GetFontDescent(_impl.get());
}

int tr::ttfont::height() const noexcept
{
	return TTF_GetFontHeight(_impl.get());
}

int tr::ttfont::line_skip() const noexcept
{
	return TTF_GetFontLineSkip(_impl.get());
}

bool tr::ttfont::contains(std::uint32_t glyph) const noexcept
{
	return TTF_FontHasGlyph(_impl.get(), glyph);
}

void tr::ttfont::resize(float size) noexcept
{
	TR_ASSERT(size > 0, "Requested invalid font size {}.", size);

	TTF_SetFontSize(_impl.get(), size);
}

void tr::ttfont::set_style(ttf_style style) noexcept
{
	TTF_SetFontStyle(_impl.get(), static_cast<TTF_FontStyleFlags>(style));
}

void tr::ttfont::set_outline(int outline) noexcept
{
	TTF_SetFontOutline(_impl.get(), outline);
}

tr::ttf_measure_result tr::ttfont::measure_text(std::string_view text, int max_w) const noexcept
{
	ttf_measure_result result;
	std::size_t length;
	if (!TTF_MeasureString(_impl.get(), text.data(), text.size(), max_w, &result.size, &length)) {
		TR_TERMINATE("Out of memory", "Exception occurred while measuring text.");
	}
	result.text = {text.begin(), text.begin() + length};
	return result;
}

glm::ivec2 tr::ttfont::text_size(std::string_view text, int max_w) const noexcept
{
	glm::ivec2 size;
	if (!TTF_GetStringSizeWrapped(_impl.get(), text.data(), text.size(), max_w, &size.x, &size.y)) {
		TR_TERMINATE("Out of memory", "Exception occurred while getting the size of text.");
	}
	return size;
}

tr::bitmap tr::ttfont::render(std::string_view text, int max_w, halign align, rgba8 color) const
{
	TTF_SetFontWrapAlignment(_impl.get(), static_cast<TTF_HorizontalAlignment>(align));

	const SDL_Color sdl_color{color.r, color.g, color.b, color.a};
	SDL_Surface* surface{TTF_RenderText_Blended_Wrapped(_impl.get(), text.data(), text.size(), sdl_color, max_w)};
	if (surface == nullptr) {
		TR_THROW(ttfont_render_error, SDL_GetError());
	}
	bitmap output{surface};
	_fix_alpha_artifacts(output, color.a);
	return output;
}

tr::ttfont tr::load_embedded_ttfont(std::span<const std::byte> data, float size)
{
	_init_ttf_if_needed();
	TTF_Font* font{TTF_OpenFontIO(SDL_IOFromConstMem(data.data(), data.size()), true, size)};
	if (font == nullptr) {
		TR_THROW(ttfont_load_error, "(Embedded)", SDL_GetError());
	}
	return ttfont{font};
}

tr::ttfont tr::load_ttfont_file(const std::filesystem::path& path, float size)
{
	if (!exists(path)) {
		TR_THROW(ttfont_load_error, path.string(), "File not found.");
	}

	_init_ttf_if_needed();
#ifdef _WIN32
	TTF_Font* font{TTF_OpenFont(path.string().c_str(), size)};
#else
	TTF_Font* font{TTF_OpenFont(path.c_str(), size)};
#endif
	if (font == nullptr) {
		TR_THROW(ttfont_load_error, path.string(), SDL_GetError());
	}
	return ttfont{font};
}