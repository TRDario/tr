#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/bitmap_iterators.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace tr {
	// Checks that a pointer is not null.
	template <class T> T _check_not_null(T ptr) noexcept;
	// Base bitmap saving function.
	void _save_bitmap(SDL_Surface* bitmap, const std::filesystem::path& path);
} // namespace tr

template <class T> T tr::_check_not_null(T ptr) noexcept
{
	if (ptr == nullptr) {
		terminate("Out of memory", "Exception occurred while allocating a bitmap.");
	}
	return ptr;
}

void tr::_save_bitmap(SDL_Surface* bitmap, const std::filesystem::path& path)
{
	TR_ASSERT(bitmap != nullptr, "Tried to save a moved-from bitmap.");

#ifdef _WIN32
	if (!IMG_SavePNG(bitmap, path.string().c_str())) {
#else
	if (!IMG_SavePNG(bitmap, path.c_str())) {
#endif
		throw bitmap_save_error{path.string(), SDL_GetError()};
	}
}

int tr::pixel_bytes(pixel_format format) noexcept
{
	return SDL_BYTESPERPIXEL(static_cast<SDL_PixelFormat>(format));
}

////////////////////////////////////////////////////////// AUDIO FILE OPEN ERROR //////////////////////////////////////////////////////////

tr::bitmap_load_error::bitmap_load_error(std::string_view path, std::string&& details) noexcept
	: _description{std::format("Failed to load bitmap from '{}'", path)}, _details{std::move(details)}
{
}

std::string_view tr::bitmap_load_error::name() const noexcept
{
	return "Bitmap loading error";
}

std::string_view tr::bitmap_load_error::description() const noexcept
{
	return _description;
}

std::string_view tr::bitmap_load_error::details() const noexcept
{
	return _details;
}

tr::bitmap_save_error::bitmap_save_error(std::string_view path, std::string&& details) noexcept
	: _description{std::format("Failed to save bitmap to '{}'", path)}, _details{std::move(details)}
{
}

std::string_view tr::bitmap_save_error::name() const noexcept
{
	return "Bitmap saving error";
}

std::string_view tr::bitmap_save_error::description() const noexcept
{
	return _description;
}

std::string_view tr::bitmap_save_error::details() const noexcept
{
	return _details;
}

tr::sub_bitmap::sub_bitmap(const bitmap& bitmap, const irect2& rect) noexcept
	: _bitmap{bitmap._impl.get()}, _rect{rect}
{
}

tr::sub_bitmap::sub_bitmap(const bitmap_view& bitmap, const irect2& rect) noexcept
	: _bitmap{bitmap._impl.get()}, _rect{rect}
{
}

glm::ivec2 tr::sub_bitmap::size() const noexcept
{
	return _rect.size;
}

tr::sub_bitmap tr::sub_bitmap::sub(const irect2& rect) noexcept
{
	TR_ASSERT(_rect.contains(rect.tl + rect.size),
			  "Tried to create out-of-bounds sub-bitmap from ({}, {}) to ({}, {}) in a sub-bitmap of size {}x{}.", rect.tl.x, rect.tl.y,
			  rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, _rect.size.x, _rect.size.y);
	return {_bitmap, {_rect.tl + rect.tl, rect.size}};
}

tr::sub_bitmap::pixel_ref tr::sub_bitmap::operator[](glm::ivec2 pos) const noexcept
{
	return *(begin() + pos);
}

tr::sub_bitmap::iterator tr::sub_bitmap::begin() const noexcept
{
	return {*this, {}};
}

tr::sub_bitmap::iterator tr::sub_bitmap::cbegin() const noexcept
{
	return begin();
}

tr::sub_bitmap::iterator tr::sub_bitmap::end() const noexcept
{
	return {*this, {0, size().y}};
}

tr::sub_bitmap::iterator tr::sub_bitmap::cend() const noexcept
{
	return end();
}

const std::byte* tr::sub_bitmap::data() const noexcept
{
	return reinterpret_cast<const std::byte*>(_bitmap->pixels) + pitch() * _rect.tl.y + pixel_bytes(format()) * _rect.tl.x;
}

tr::pixel_format tr::sub_bitmap::format() const noexcept
{
	return static_cast<pixel_format>(_bitmap->format);
}

int tr::sub_bitmap::pitch() const noexcept
{
	return _bitmap->pitch;
}

int tr::operator-(const tr::sub_bitmap::iterator& l, const tr::sub_bitmap::iterator& r) noexcept
{
	return (l._pos.y * l._bitmap_size.x + l._pos.x) - (r._pos.y * r._bitmap_size.x + r._pos.x);
}

tr::bitmap_view::bitmap_view(std::span<const std::byte> raw_data, glm::ivec2 size, pixel_format format) noexcept
	: bitmap_view(raw_data.data(), size.x * pixel_bytes(format), size, format)
{
	TR_ASSERT(raw_data.size() == size.x * size.y * static_cast<std::size_t>(pixel_bytes(format)),
			  "Tried to create a bitmap view from data with unexpected (expected {} bytes vs. actual {} bytes).",
			  size.x * size.y * static_cast<std::size_t>(pixel_bytes(format)), raw_data.size());
}

tr::bitmap_view::bitmap_view(const std::byte* raw_data_start, int pitch, glm::ivec2 size, pixel_format format) noexcept
	: _impl{_check_not_null(
		  SDL_CreateSurfaceFrom(size.x, size.y, static_cast<SDL_PixelFormat>(format), const_cast<std::byte*>(raw_data_start), pitch))}
{
}

void tr::bitmap_view::deleter::operator()(SDL_Surface* ptr) const noexcept
{
	SDL_DestroySurface(ptr);
}

glm::ivec2 tr::bitmap_view::size() const noexcept
{
	return {_impl->w, _impl->h};
}

tr::bitmap_view::pixel_ref tr::bitmap_view::operator[](glm::ivec2 pos) const noexcept
{
	return *(begin() + pos);
}

tr::bitmap_view::iterator tr::bitmap_view::begin() const noexcept
{
	return cbegin();
}

tr::bitmap_view::iterator tr::bitmap_view::cbegin() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get an iterator to the beginning of a moved-from bitmap view.");

	return sub_bitmap{*this}.begin();
}

tr::bitmap_view::iterator tr::bitmap_view::end() const noexcept
{
	return cend();
}

tr::bitmap_view::iterator tr::bitmap_view::cend() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get an iterator to the end of a moved-from bitmap view.");

	return sub_bitmap{*this}.end();
}

tr::bitmap_view::operator tr::sub_bitmap() const noexcept
{
	return sub({{}, size()});
}

tr::sub_bitmap tr::bitmap_view::sub(const irect2& rect) const noexcept
{
	return sub_bitmap{*this, rect};
}

const std::byte* tr::bitmap_view::data() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get the data of a moved-from bitmap view.");

	return static_cast<const std::byte*>(_impl->pixels);
}

tr::pixel_format tr::bitmap_view::format() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get the format of a moved-from bitmap view.");

	return static_cast<pixel_format>(_impl->format);
}

int tr::bitmap_view::pitch() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get the pitch of a moved-from bitmap view.");

	return _impl->pitch;
}

void tr::bitmap_view::save(const std::filesystem::path& path) const
{
	_save_bitmap(_impl.get(), path);
}

tr::bitmap::bitmap(SDL_Surface* ptr)
	: _impl{_check_not_null(ptr)}
{
	if (SDL_ISPIXELFORMAT_INDEXED(_impl->format) && format() != pixel_format::R8) {
		*this = bitmap{*this, pixel_format::R8};
	}
	if (SDL_ISPIXELFORMAT_FOURCC(_impl->format) || SDL_ISPIXELFORMAT_FLOAT(_impl->format) ||
		(SDL_ISPIXELFORMAT_ARRAY(_impl->format) &&
		 (SDL_PIXELTYPE(_impl->format) == SDL_PIXELTYPE_ARRAYU16 || SDL_PIXELTYPE(_impl->format) == SDL_PIXELTYPE_ARRAYU32)) ||
		SDL_ISPIXELFORMAT_10BIT(_impl->format)) {
		*this = bitmap{*this, pixel_format::RGBA32};
	}
}

tr::bitmap::bitmap(glm::ivec2 size, pixel_format format) noexcept
	: _impl{_check_not_null(SDL_CreateSurface(size.x, size.y, static_cast<SDL_PixelFormat>(format)))}
{
}

tr::bitmap::bitmap(const bitmap& bitmap, pixel_format format) noexcept
	: _impl{_check_not_null(SDL_ConvertSurface(bitmap._impl.get(), static_cast<SDL_PixelFormat>(format)))}
{
}

tr::bitmap::bitmap(const bitmap_view& view, pixel_format format) noexcept
	: _impl{_check_not_null(SDL_ConvertSurface(view._impl.get(), static_cast<SDL_PixelFormat>(format)))}
{
}

tr::bitmap::bitmap(const sub_bitmap& source, pixel_format format) noexcept
	: bitmap{source.size(), format}
{
	blit({}, source);
}

glm::ivec2 tr::bitmap::size() const noexcept
{
	return {_impl->w, _impl->h};
}

tr::bitmap::pixel_ref tr::bitmap::operator[](glm::ivec2 pos) noexcept
{
	return *(begin() + pos);
}

tr::bitmap::pixel_cref tr::bitmap::operator[](glm::ivec2 pos) const noexcept
{
	return *(begin() + pos);
}

tr::bitmap::mut_it tr::bitmap::begin() noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get an iterator to the beginning of a moved-from bitmap.");

	return {*this, {}};
}

tr::bitmap::const_it tr::bitmap::begin() const noexcept
{
	return cbegin();
}

tr::bitmap::const_it tr::bitmap::cbegin() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get an iterator to the beginning of a moved-from bitmap.");

	return sub_bitmap{*this}.begin();
}

tr::bitmap::mut_it tr::bitmap::end() noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return {*this, {0, size().y}};
}

tr::bitmap::const_it tr::bitmap::end() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return cend();
}

tr::bitmap::const_it tr::bitmap::cend() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return sub_bitmap(*this).end();
}

void tr::bitmap::blit(glm::ivec2 tl, const sub_bitmap& source) noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to blit to a moved-from bitmap.");
	TR_ASSERT(irect2{size()}.contains(tl + source.size()),
			  "Tried to blit to out-of-bounds region from ({}, {}) to ({}, {}) in a bitmap of size {}x{}.", tl.x, tl.y,
			  tl.x + source.size().x, tl.y + source.size().y, size().x, size().y);

	SDL_Rect sdl_src{source._rect.tl.x, source._rect.tl.y, source.size().x, source.size().y};
	SDL_Rect sdl_dest{tl.x, tl.y, source.size().x, source.size().y};
	SDL_BlitSurface(source._bitmap, &sdl_src, _impl.get(), &sdl_dest);
}

void tr::bitmap::fill(const irect2& rect, rgba8 color) noexcept
{
	TR_ASSERT(irect2{size()}.contains(rect.tl + rect.size),
			  "Tried to fill out-of-bounds region from ({}, {}) to ({}, {}) in a bitmap of size {}x{}.", rect.tl.x, rect.tl.y,
			  rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, size().x, size().y);

	const SDL_Rect sdlRect{rect.tl.x, rect.tl.y, rect.size.x, rect.size.y};
	const std::uint32_t sdl_color{
		SDL_MapRGBA(SDL_GetPixelFormatDetails(_impl->format), SDL_GetSurfacePalette(_impl.get()), color.r, color.g, color.b, color.a),
	};
	SDL_FillSurfaceRect(_impl.get(), &sdlRect, sdl_color);
}

tr::bitmap::operator tr::sub_bitmap() const noexcept
{
	return sub({{}, size()});
}

tr::sub_bitmap tr::bitmap::sub(const irect2& rect) const noexcept
{
	return sub_bitmap{*this, rect};
}

std::byte* tr::bitmap::data() noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get the data of a moved-from bitmap.");

	return static_cast<std::byte*>(_impl->pixels);
}

const std::byte* tr::bitmap::data() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get the data of a moved-from bitmap.");

	return static_cast<const std::byte*>(_impl->pixels);
}

tr::pixel_format tr::bitmap::format() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get the format of a moved-from bitmap.");

	return static_cast<pixel_format>(_impl->format);
}

int tr::bitmap::pitch() const noexcept
{
	TR_ASSERT(_impl != nullptr, "Tried to get the pitch of a moved-from bitmap.");

	return _impl->pitch;
}

void tr::bitmap::save(const std::filesystem::path& path) const
{
	_save_bitmap(_impl.get(), path);
}

tr::bitmap tr::create_checkerboard(glm::ivec2 size) noexcept
{
	constexpr rgba8 BLACK{0, 0, 0, 255};
	constexpr rgba8 MAGENTA{255, 0, 255, 255};
	const glm::ivec2 half_size{size / 2};

	bitmap bitmap{size, pixel_format::RGB24};
	bitmap.fill({{0, 0}, half_size}, BLACK);
	bitmap.fill({{half_size.x, 0}, half_size}, MAGENTA);
	bitmap.fill({{0, half_size.x}, half_size}, MAGENTA);
	bitmap.fill({half_size, half_size}, BLACK);
	return bitmap;
}

tr::bitmap tr::load_embedded_bitmap(std::span<const std::byte> data) noexcept
{
	return bitmap{_check_not_null(IMG_Load_IO(SDL_IOFromConstMem(data.data(), data.size()), true))};
}

tr::bitmap tr::load_bitmap_file(const std::filesystem::path& path)
{
	if (!is_regular_file(path)) {
		throw bitmap_load_error{path.string(), "File not found."};
	}

#ifdef _WIN32
	SDL_Surface* ptr{IMG_Load(path.string().c_str())};
#else
	SDL_Surface* ptr{IMG_Load(path.c_str())};
#endif
	if (ptr == nullptr) {
		throw bitmap_load_error{path.string(), SDL_GetError()};
	}
	return bitmap{ptr};
}