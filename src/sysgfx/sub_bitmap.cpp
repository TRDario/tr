/// @file
/// @brief Implements sub_bitmap.hpp.

#include <SDL3/SDL.h>
#include <tr/sysgfx/bitmap.hpp>
#include <tr/sysgfx/bitmap_view.hpp>
#include <tr/sysgfx/pixel_iterator.hpp>
#include <tr/sysgfx/sub_bitmap.hpp>

//

tr::sub_bitmap::sub_bitmap(const bitmap& bitmap, rectangle<int> region) noexcept
	: m_ptr{bitmap.unwrap()}
	, m_region{region}
{
}

tr::sub_bitmap::sub_bitmap(const bitmap_view& view, rectangle<int> region) noexcept
	: m_ptr{view.unwrap()}
	, m_region{region}
{
}

tr::sub_bitmap::sub_bitmap(sub_bitmap sub, rectangle<int> region) noexcept
	: m_ptr{sub.m_ptr}
	, m_region{region}
{
}

//

tr::sub_bitmap tr::sub_bitmap::sub(rectangle<int> region) noexcept
{
	TR_ASSERT(m_region.contains(region.tl + region.size),
			  "Tried to create out-of-bounds sub-bitmap from ({}, {}) to ({}, {}) in a sub-bitmap of size {}x{}.", region.tl.x, region.tl.y,
			  region.tl.x + region.size.x, region.tl.y + region.size.y, m_region.size.x, m_region.size.y);

	return sub_bitmap{*this, {region.tl + region.tl, region.size}};
}

//

tr::rectangle<int> tr::sub_bitmap::region() const noexcept
{
	return m_region;
}

glm::ivec2 tr::sub_bitmap::size() const noexcept
{
	return m_region.size;
}

tr::pixel_format tr::sub_bitmap::format() const noexcept
{
	return static_cast<pixel_format>(m_ptr->format);
}

int tr::sub_bitmap::pitch() const noexcept
{
	return m_ptr->pitch;
}

//

tr::sub_bitmap::reference tr::sub_bitmap::operator[](int x, int y) const noexcept
{
	return *(begin() + glm::ivec2{x, y});
}

tr::sub_bitmap::reference tr::sub_bitmap::operator[](glm::ivec2 pos) const noexcept
{
	return *(begin() + pos);
}

const std::byte* tr::sub_bitmap::data() const noexcept
{
	return static_cast<const std::byte*>(m_ptr->pixels) + pitch() * m_region.tl.y + pixel_bytes(format()) * m_region.tl.x;
}

//

tr::sub_bitmap::iterator tr::sub_bitmap::begin() const noexcept
{
	return iterator{*this, {}};
}

tr::sub_bitmap::iterator tr::sub_bitmap::cbegin() const noexcept
{
	return begin();
}

tr::sub_bitmap::iterator tr::sub_bitmap::end() const noexcept
{
	return iterator{*this, {0, size().y}};
}

tr::sub_bitmap::iterator tr::sub_bitmap::cend() const noexcept
{
	return end();
}

//

SDL_Surface* tr::sub_bitmap::unwrap() const noexcept
{
	return m_ptr;
}