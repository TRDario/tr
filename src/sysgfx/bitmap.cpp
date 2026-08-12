/// @file
/// @brief Implements the non-templated parts of bitmap.hpp.

#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/bitmap_iterators.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

//

int tr::pixel_bytes(pixel_format format)
{
	return SDL_BYTESPERPIXEL(std::to_underlying(format));
}

//

tr::bitmap_load_error::bitmap_load_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to load bitmap from '{}'", path)}
	, m_details{std::move(details)}
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

//

tr::bitmap_save_error::bitmap_save_error(std::string_view path, std::string&& details)
	: m_description{std::format("Failed to save bitmap to '{}'", path)}
	, m_details{std::move(details)}
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

//

tr::bitmap::bitmap(SDL_Surface* ptr)
	: m_ptr{ptr}
{
	if (ptr == nullptr) {
		throw out_of_memory{"bitmap allocation"};
	}

	const SDL_PixelFormat format{ptr->format};
	if (SDL_ISPIXELFORMAT_INDEXED(format) && SDL_PIXELTYPE(format) != SDL_PIXELTYPE_INDEX8) {
		*this = bitmap{*this, pixel_format::r8};
	}
	else if (SDL_ISPIXELFORMAT_FOURCC(format) || SDL_ISPIXELFORMAT_FLOAT(format) ||
			 (SDL_ISPIXELFORMAT_ARRAY(format) && SDL_PIXELTYPE(format) >= SDL_PIXELTYPE_ARRAYU16) || SDL_ISPIXELFORMAT_10BIT(format)) {
		*this = bitmap{*this, pixel_format::rgba32};
	}
}

tr::bitmap::bitmap(glm::ivec2 size, pixel_format format)
	: bitmap{SDL_CreateSurface(size.x, size.y, static_cast<SDL_PixelFormat>(format))}
{
}

tr::bitmap::bitmap(const bitmap& bitmap, pixel_format format)
	: tr::bitmap{SDL_ConvertSurface(bitmap.unwrap(), static_cast<SDL_PixelFormat>(format))}
{
}

tr::bitmap::bitmap(const bitmap_view& view, pixel_format format)
	: bitmap{SDL_ConvertSurface(view.unwrap(), static_cast<SDL_PixelFormat>(format))}
{
}

tr::bitmap::bitmap(sub_bitmap source, pixel_format format)
	: bitmap{source.size(), format}
{
	blit({}, source);
}

void tr::bitmap::deleter::operator()(SDL_Surface* ptr)
{
	SDL_DestroySurface(ptr);
}

//

tr::bitmap::operator tr::sub_bitmap() const
{
	return sub({{}, size()});
}

tr::sub_bitmap tr::bitmap::sub(rectangle<int> region) const
{
	return sub_bitmap{*this, region};
}

//

glm::ivec2 tr::bitmap::size() const
{
	return {m_ptr->w, m_ptr->h};
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

//

tr::bitmap::reference tr::bitmap::operator[](int x, int y)
{
	return *(begin() + glm::ivec2{x, y});
}

tr::bitmap::reference tr::bitmap::operator[](glm::ivec2 pos)
{
	return *(begin() + pos);
}

tr::bitmap::const_reference tr::bitmap::operator[](int x, int y) const
{
	return *(begin() + glm::ivec2{x, y});
}

tr::bitmap::const_reference tr::bitmap::operator[](glm::ivec2 pos) const
{
	return *(begin() + pos);
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

//

tr::bitmap::iterator tr::bitmap::begin()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the beginning of a moved-from bitmap.");

	return {*this, {}};
}

tr::bitmap::const_iterator tr::bitmap::begin() const
{
	return cbegin();
}

tr::bitmap::const_iterator tr::bitmap::cbegin() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the beginning of a moved-from bitmap.");

	return sub_bitmap{*this}.begin();
}

tr::bitmap::iterator tr::bitmap::end()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return {*this, {0, size().y}};
}

tr::bitmap::const_iterator tr::bitmap::end() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return cend();
}

tr::bitmap::const_iterator tr::bitmap::cend() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get an iterator to the end of a moved-from bitmap.");

	return sub_bitmap(*this).end();
}

//

void tr::bitmap::blit(glm::ivec2 tl, sub_bitmap source)
{
	TR_ASSERT(m_ptr != nullptr, "Tried to blit to a moved-from bitmap.");
	TR_ASSERT(rectangle<int>{size()}.contains(tl + source.size()),
			  "Tried to blit to out-of-bounds region from ({}, {}) to ({}, {}) in a bitmap of size {}x{}.", tl.x, tl.y,
			  tl.x + source.size().x, tl.y + source.size().y, size().x, size().y);

	const rectangle<int> source_region{source.region()};
	const SDL_Rect sdl_source{source_region.tl.x, source_region.tl.y, source.size().x, source.size().y};
	const SDL_Rect sdl_destination{tl.x, tl.y, source.size().x, source.size().y};
	SDL_BlitSurface(source.unwrap(), &sdl_source, m_ptr.get(), &sdl_destination);
}

void tr::bitmap::fill(rectangle<int> region, rgba8 color)
{
	TR_ASSERT(rectangle<int>{size()}.contains(region.tl + region.size),
			  "Tried to fill out-of-bounds region from ({}, {}) to ({}, {}) in a bitmap of size {}x{}.", region.tl.x, region.tl.y,
			  region.tl.x + region.size.x, region.tl.y + region.size.y, size().x, size().y);

	const SDL_Rect sdl_rect{region.tl.x, region.tl.y, region.size.x, region.size.y};
	const SDL_PixelFormatDetails* sdl_details{SDL_GetPixelFormatDetails(m_ptr->format)};
	const SDL_Palette* sdl_palette{SDL_GetSurfacePalette(m_ptr.get())};
	const u32 sdl_color{SDL_MapRGBA(sdl_details, sdl_palette, color.r, color.g, color.b, color.a)};
	SDL_FillSurfaceRect(m_ptr.get(), &sdl_rect, sdl_color);
}

//

void tr::bitmap::save(const std::filesystem::path& path) const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to save a moved-from bitmap.");

	if (!IMG_SavePNG(m_ptr.get(), TR_PATH_CSTR(path))) {
		throw bitmap_save_error{path.string(), SDL_GetError()};
	}
}

//

SDL_Surface* tr::bitmap::unwrap() const
{
	return m_ptr.get();
}

//

tr::bitmap tr::create_checkerboard(glm::ivec2 size)
{
	constexpr rgba8 black{0, 0, 0, 255};
	constexpr rgba8 magenta{255, 0, 255, 255};
	const glm::ivec2 half_size{size / 2};

	bitmap bitmap{size, pixel_format::rgb24};
	bitmap.fill({{0, 0}, half_size}, black);
	bitmap.fill({{half_size.x, 0}, half_size}, magenta);
	bitmap.fill({{0, half_size.x}, half_size}, magenta);
	bitmap.fill({half_size, half_size}, black);
	return bitmap;
}

tr::bitmap tr::load_embedded_bitmap(std::span<const std::byte> data)
{
	return bitmap{IMG_Load_IO(SDL_IOFromConstMem(data.data(), data.size()), true)};
}

tr::bitmap tr::load_bitmap_file(const std::filesystem::path& path)
{
	if (!is_regular_file(path)) {
		throw bitmap_load_error{path.string(), "File not found."};
	}

	SDL_Surface* ptr{IMG_Load(TR_PATH_CSTR(path))};
	if (ptr == nullptr) {
		throw bitmap_load_error{path.string(), SDL_GetError()};
	}
	return bitmap{ptr};
}