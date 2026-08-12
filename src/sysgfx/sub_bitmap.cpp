/// @file
/// @brief Implements sub_bitmap.hpp.

#include "../../include/tr/sysgfx/sub_bitmap.hpp"
#include "../../include/tr/sysgfx/bitmap_iterators.hpp"
#include <SDL3/SDL.h>

//

tr::sub_bitmap::sub_bitmap(const bitmap& bitmap, rectangle<int> region)
	: m_ptr{bitmap.unwrap()}
	, m_region{region}
{
}

tr::sub_bitmap::sub_bitmap(const bitmap_view& view, rectangle<int> region)
	: m_ptr{view.unwrap()}
	, m_region{region}
{
}

tr::sub_bitmap::sub_bitmap(sub_bitmap sub, rectangle<int> region)
	: m_ptr{sub.m_ptr}
	, m_region{region}
{
}

//

tr::sub_bitmap tr::sub_bitmap::sub(rectangle<int> region)
{
	TR_ASSERT(m_region.contains(region.tl + region.size),
			  "Tried to create out-of-bounds sub-bitmap from ({}, {}) to ({}, {}) in a sub-bitmap of size {}x{}.", region.tl.x, region.tl.y,
			  region.tl.x + region.size.x, region.tl.y + region.size.y, m_region.size.x, m_region.size.y);

	return sub_bitmap{*this, {region.tl + region.tl, region.size}};
}

//

tr::rectangle<int> tr::sub_bitmap::region() const
{
	return m_region;
}

glm::ivec2 tr::sub_bitmap::size() const
{
	return m_region.size;
}

tr::pixel_format tr::sub_bitmap::format() const
{
	return static_cast<pixel_format>(m_ptr->format);
}

int tr::sub_bitmap::pitch() const
{
	return m_ptr->pitch;
}

//

tr::sub_bitmap::reference tr::sub_bitmap::operator[](int x, int y) const
{
	return *(begin() + glm::ivec2{x, y});
}

tr::sub_bitmap::reference tr::sub_bitmap::operator[](glm::ivec2 pos) const
{
	return *(begin() + pos);
}

const std::byte* tr::sub_bitmap::data() const
{
	return static_cast<const std::byte*>(m_ptr->pixels) + pitch() * m_region.tl.y + pixel_bytes(format()) * m_region.tl.x;
}

//

tr::sub_bitmap::iterator tr::sub_bitmap::begin() const
{
	return iterator{*this, {}};
}

tr::sub_bitmap::iterator tr::sub_bitmap::cbegin() const
{
	return begin();
}

tr::sub_bitmap::iterator tr::sub_bitmap::end() const
{
	return iterator{*this, {0, size().y}};
}

tr::sub_bitmap::iterator tr::sub_bitmap::cend() const
{
	return end();
}

//

SDL_Surface* tr::sub_bitmap::unwrap() const
{
	return m_ptr;
}