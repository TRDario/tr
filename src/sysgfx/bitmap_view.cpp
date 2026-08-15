/// @file
/// @brief Implements the non-templated parts of bitmap_view.hpp.

#include "../../include/tr/sysgfx/bitmap_view.hpp"
#include "../../include/tr/sysgfx/bitmap.hpp"
#include "../../include/tr/sysgfx/bitmap_iterators.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

//

tr::bitmap_view::bitmap_view(std::span<const std::byte> raw_data, glm::ivec2 size, pixel_format format)
	: bitmap_view(raw_data.data(), size.x * pixel_bytes(format), size, format)
{
	TR_ASSERT(raw_data.size() == size.x * size.y * static_cast<usize>(pixel_bytes(format)),
			  "Tried to create a bitmap view from data with unexpected (expected {} bytes vs. actual {} bytes).",
			  size.x * size.y * static_cast<usize>(pixel_bytes(format)), raw_data.size());
}

tr::bitmap_view::bitmap_view(const std::byte* raw_data_start, int pitch, glm::ivec2 size, pixel_format format)
	: m_ptr{SDL_CreateSurfaceFrom(size.x, size.y, static_cast<SDL_PixelFormat>(format), const_cast<std::byte*>(raw_data_start), pitch)}
{
	if (m_ptr == nullptr) {
		throw out_of_memory{"bitmap view allocation"};
	}
}

void tr::bitmap_view::deleter::operator()(SDL_Surface* ptr)
{
	SDL_DestroySurface(ptr);
}

//

tr::bitmap_view::operator tr::sub_bitmap() const
{
	return sub({{}, size()});
}

tr::sub_bitmap tr::bitmap_view::sub(rectangle<int> region) const
{
	return sub_bitmap{*this, region};
}

//

glm::ivec2 tr::bitmap_view::size() const
{
	return {m_ptr->w, m_ptr->h};
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

//

tr::bitmap_view::reference tr::bitmap_view::operator[](int x, int y) const
{
	return *(begin() + glm::ivec2{x, y});
}

tr::bitmap_view::reference tr::bitmap_view::operator[](glm::ivec2 pos) const
{
	return *(begin() + pos);
}

const std::byte* tr::bitmap_view::data() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to get the data of a moved-from bitmap view.");

	return static_cast<const std::byte*>(m_ptr->pixels);
}

//

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

//

void tr::bitmap_view::save(const std::filesystem::path& path) const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to save a moved-from bitmap.");

	if (!IMG_SavePNG(m_ptr.get(), TR_PATH_CSTR(path))) {
		throw bitmap_save_error{path.string(), SDL_GetError()};
	}
}

//

SDL_Surface* tr::bitmap_view::unwrap() const
{
	return m_ptr.get();
}