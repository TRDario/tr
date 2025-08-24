#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/bitmap_iterators.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace tr {
	// Checks that a pointer is not null.
	SDL_Surface* check_not_null(SDL_Surface* ptr);
	// Base bitmap saving function.
	void save_bitmap(SDL_Surface* bitmap, const std::filesystem::path& path);
} // namespace tr

SDL_Surface* tr::check_not_null(SDL_Surface* ptr)
{
	if (ptr == nullptr) {
		throw out_of_memory{"bitmap allocation"};
	}
	return ptr;
}

void tr::save_bitmap(SDL_Surface* bitmap, const std::filesystem::path& path)
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

int tr::pixel_bytes(pixel_format format)
{
	return SDL_BYTESPERPIXEL(static_cast<SDL_PixelFormat>(format));
}

////////////////////////////////////////////////////////// AUDIO FILE OPEN ERROR //////////////////////////////////////////////////////////

tr::bitmap_load_error::bitmap_load_error(std::string_view path, std::string&& details)
	: m_description{TR_FMT::format("Failed to load bitmap from '{}'", path)}, m_details{std::move(details)}
{
}

std::string_view tr::bitmap_load_error::name() const
{
	return "Bitmap loading error";
}

std::string_view tr::bitmap_load_error::description() const
{
	return m_description;
}

std::string_view tr::bitmap_load_error::details() const
{
	return m_details;
}

tr::bitmap_save_error::bitmap_save_error(std::string_view path, std::string&& details)
	: m_description{TR_FMT::format("Failed to save bitmap to '{}'", path)}, m_details{std::move(details)}
{
}

std::string_view tr::bitmap_save_error::name() const
{
	return "Bitmap saving error";
}

std::string_view tr::bitmap_save_error::description() const
{
	return m_description;
}

std::string_view tr::bitmap_save_error::details() const
{
	return m_details;
}

tr::sub_bitmap::sub_bitmap(const bitmap& bitmap, const irect2& rect)
	: m_ptr{bitmap.m_ptr.get()}, m_rect{rect}
{
}

tr::sub_bitmap::sub_bitmap(const bitmap_view& bitmap, const irect2& rect)
	: m_ptr{bitmap.m_ptr.get()}, m_rect{rect}
{
}

glm::ivec2 tr::sub_bitmap::size() const
{
	return m_rect.size;
}

tr::sub_bitmap tr::sub_bitmap::sub(const irect2& rect)
{
	TR_ASSERT(rect.contains(rect.tl + rect.size),
			  "Tried to create out-of-bounds sub-bitmap from ({}, {}) to ({}, {}) in a sub-bitmap of size {}x{}.", rect.tl.x, rect.tl.y,
			  rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, rect.size.x, rect.size.y);
	return {m_ptr, {rect.tl + rect.tl, rect.size}};
}

tr::sub_bitmap::pixel_ref tr::sub_bitmap::operator[](glm::ivec2 pos) const
{
	return *(begin() + pos);
}

tr::sub_bitmap::iterator tr::sub_bitmap::begin() const
{
	return {*this, {}};
}

tr::sub_bitmap::iterator tr::sub_bitmap::cbegin() const
{
	return begin();
}

tr::sub_bitmap::iterator tr::sub_bitmap::end() const
{
	return {*this, {0, size().y}};
}

tr::sub_bitmap::iterator tr::sub_bitmap::cend() const
{
	return end();
}

const std::byte* tr::sub_bitmap::data() const
{
	return reinterpret_cast<const std::byte*>(m_ptr->pixels) + pitch() * m_rect.tl.y + pixel_bytes(format()) * m_rect.tl.x;
}

tr::pixel_format tr::sub_bitmap::format() const
{
	return static_cast<pixel_format>(m_ptr->format);
}

int tr::sub_bitmap::pitch() const
{
	return m_ptr->pitch;
}

int tr::operator-(const tr::sub_bitmap::iterator& l, const tr::sub_bitmap::iterator& r)
{
	return (l.m_bitmap_pos.y * l.m_bitmap_size.x + l.m_bitmap_pos.x) - (r.m_bitmap_pos.y * r.m_bitmap_size.x + r.m_bitmap_pos.x);
}

tr::bitmap_view::bitmap_view(std::span<const std::byte> raw_data, glm::ivec2 size, pixel_format format)
	: bitmap_view(raw_data.data(), size.x * pixel_bytes(format), size, format)
{
	TR_ASSERT(raw_data.size() == size.x * size.y * static_cast<std::size_t>(pixel_bytes(format)),
			  "Tried to create a bitmap view from data with unexpected (expected {} bytes vs. actual {} bytes).",
			  size.x * size.y * static_cast<std::size_t>(pixel_bytes(format)), raw_data.size());
}

tr::bitmap_view::bitmap_view(const std::byte* raw_data_start, int pitch, glm::ivec2 size, pixel_format format)
	: m_ptr{check_not_null(
		  SDL_CreateSurfaceFrom(size.x, size.y, static_cast<SDL_PixelFormat>(format), const_cast<std::byte*>(raw_data_start), pitch))}
{
}

void tr::bitmap_view::deleter::operator()(SDL_Surface* ptr) const
{
	SDL_DestroySurface(ptr);
}

glm::ivec2 tr::bitmap_view::size() const
{
	return {m_ptr->w, m_ptr->h};
}

tr::bitmap_view::pixel_ref tr::bitmap_view::operator[](glm::ivec2 pos) const
{
	return *(begin() + pos);
}

tr::bitmap_view::iterator tr::bitmap_view::begin() const
{
	return cbegin();
}

tr::bitmap_view::iterator tr::bitmap_view::cbegin() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the beginning of a moved-from bitmap view.");

	return sub_bitmap{*this}.begin();
}

tr::bitmap_view::iterator tr::bitmap_view::end() const
{
	return cend();
}

tr::bitmap_view::iterator tr::bitmap_view::cend() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the end of a moved-from bitmap view.");

	return sub_bitmap{*this}.end();
}

tr::bitmap_view::operator tr::sub_bitmap() const
{
	return sub({{}, size()});
}

tr::sub_bitmap tr::bitmap_view::sub(const irect2& rect) const
{
	return sub_bitmap{*this, rect};
}

const std::byte* tr::bitmap_view::data() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get the data of a moved-from bitmap view.");

	return static_cast<const std::byte*>(m_ptr->pixels);
}

tr::pixel_format tr::bitmap_view::format() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get the format of a moved-from bitmap view.");

	return static_cast<pixel_format>(m_ptr->format);
}

int tr::bitmap_view::pitch() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get the pitch of a moved-from bitmap view.");

	return m_ptr->pitch;
}

void tr::bitmap_view::save(const std::filesystem::path& path) const
{
	save_bitmap(m_ptr.get(), path);
}

tr::bitmap::bitmap(SDL_Surface* ptr)
	: m_ptr{check_not_null(ptr)}
{
	if (SDL_ISPIXELFORMAT_INDEXED(ptr->format) && format() != pixel_format::R8) {
		*this = bitmap{*this, pixel_format::R8};
	}
	if (SDL_ISPIXELFORMAT_FOURCC(ptr->format) || SDL_ISPIXELFORMAT_FLOAT(ptr->format) ||
		(SDL_ISPIXELFORMAT_ARRAY(ptr->format) &&
		 (SDL_PIXELTYPE(ptr->format) == SDL_PIXELTYPE_ARRAYU16 || SDL_PIXELTYPE(ptr->format) == SDL_PIXELTYPE_ARRAYU32)) ||
		SDL_ISPIXELFORMAT_10BIT(ptr->format)) {
		*this = bitmap{*this, pixel_format::RGBA32};
	}
}

tr::bitmap::bitmap(glm::ivec2 size, pixel_format format)
	: m_ptr{check_not_null(SDL_CreateSurface(size.x, size.y, static_cast<SDL_PixelFormat>(format)))}
{
}

tr::bitmap::bitmap(const bitmap& bitmap, pixel_format format)
	: m_ptr{check_not_null(SDL_ConvertSurface(bitmap.m_ptr.get(), static_cast<SDL_PixelFormat>(format)))}
{
}

tr::bitmap::bitmap(const bitmap_view& view, pixel_format format)
	: m_ptr{check_not_null(SDL_ConvertSurface(view.m_ptr.get(), static_cast<SDL_PixelFormat>(format)))}
{
}

tr::bitmap::bitmap(const sub_bitmap& source, pixel_format format)
	: bitmap{source.size(), format}
{
	blit({}, source);
}

glm::ivec2 tr::bitmap::size() const
{
	return {m_ptr->w, m_ptr->h};
}

tr::bitmap::pixel_ref tr::bitmap::operator[](glm::ivec2 pos)
{
	return *(begin() + pos);
}

tr::bitmap::pixel_cref tr::bitmap::operator[](glm::ivec2 pos) const
{
	return *(begin() + pos);
}

tr::bitmap::mut_it tr::bitmap::begin()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the beginning of a moved-from bitmap.");

	return {*this, {}};
}

tr::bitmap::const_it tr::bitmap::begin() const
{
	return cbegin();
}

tr::bitmap::const_it tr::bitmap::cbegin() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the beginning of a moved-from bitmap.");

	return sub_bitmap{*this}.begin();
}

tr::bitmap::mut_it tr::bitmap::end()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return {*this, {0, size().y}};
}

tr::bitmap::const_it tr::bitmap::end() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return cend();
}

tr::bitmap::const_it tr::bitmap::cend() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return sub_bitmap(*this).end();
}

void tr::bitmap::blit(glm::ivec2 tl, const sub_bitmap& source)
{
	TR_ASSERT(m_ptr != nullptr, "Tried to blit to a moved-from bitmap.");
	TR_ASSERT(irect2{size()}.contains(tl + source.size()),
			  "Tried to blit to out-of-bounds region from ({}, {}) to ({}, {}) in a bitmap of size {}x{}.", tl.x, tl.y,
			  tl.x + source.size().x, tl.y + source.size().y, size().x, size().y);

	SDL_Rect sdl_src{source.m_rect.tl.x, source.m_rect.tl.y, source.size().x, source.size().y};
	SDL_Rect sdl_dest{tl.x, tl.y, source.size().x, source.size().y};
	SDL_BlitSurface(source.m_ptr, &sdl_src, m_ptr.get(), &sdl_dest);
}

void tr::bitmap::fill(const irect2& rect, rgba8 color)
{
	TR_ASSERT(irect2{size()}.contains(rect.tl + rect.size),
			  "Tried to fill out-of-bounds region from ({}, {}) to ({}, {}) in a bitmap of size {}x{}.", rect.tl.x, rect.tl.y,
			  rect.tl.x + rect.size.x, rect.tl.y + rect.size.y, size().x, size().y);

	const SDL_Rect sdlRect{rect.tl.x, rect.tl.y, rect.size.x, rect.size.y};
	const std::uint32_t sdl_color{
		SDL_MapRGBA(SDL_GetPixelFormatDetails(m_ptr->format), SDL_GetSurfacePalette(m_ptr.get()), color.r, color.g, color.b, color.a),
	};
	SDL_FillSurfaceRect(m_ptr.get(), &sdlRect, sdl_color);
}

tr::bitmap::operator tr::sub_bitmap() const
{
	return sub({{}, size()});
}

tr::sub_bitmap tr::bitmap::sub(const irect2& rect) const
{
	return sub_bitmap{*this, rect};
}

std::byte* tr::bitmap::data()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get the data of a moved-from bitmap.");

	return static_cast<std::byte*>(m_ptr->pixels);
}

const std::byte* tr::bitmap::data() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get the data of a moved-from bitmap.");

	return static_cast<const std::byte*>(m_ptr->pixels);
}

tr::pixel_format tr::bitmap::format() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get the format of a moved-from bitmap.");

	return static_cast<pixel_format>(m_ptr->format);
}

int tr::bitmap::pitch() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get the pitch of a moved-from bitmap.");

	return m_ptr->pitch;
}

void tr::bitmap::save(const std::filesystem::path& path) const
{
	save_bitmap(m_ptr.get(), path);
}

tr::bitmap tr::create_checkerboard(glm::ivec2 size)
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

tr::bitmap tr::load_embedded_bitmap(std::span<const std::byte> data)
{
	return bitmap{check_not_null(IMG_Load_IO(SDL_IOFromConstMem(data.data(), data.size()), true))};
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